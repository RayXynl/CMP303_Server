#include "Networking.h"

Networking::Networking(sf::Clock& clck) :
	updateMissedCounter(0), clock(clck)
{
	// Bind UDP port to player update socket
	if (playerUpdateSocket.bind(playerPort) == sf::Socket::Done)
	{
		printf("Player socket bound\n");
	}
	else
	{
		printf("Player socket could not bind\n");
	}

	// Bind UDP port to zombie update socket
	if (zombieUpdateSocket.bind(zombiePort) == sf::Socket::Done)
	{
		printf("Zombie socket bound\n");
	}
	else
	{
		printf("Zombie socket could not bind\n");
	}

	// Set all sockets to non-blocking
	playerUpdateSocket.setBlocking(false);
	zombieUpdateSocket.setBlocking(false);
	playerConnectionSocket.setBlocking(false);
	playerListener.setBlocking(false);
}

Networking::~Networking()
{

}

bool Networking::acceptPlayerConnections()
{
	// Clear connection socket selector
	connectionSelector.clear();
	// Add listener socket to the connection selector
	connectionSelector.add(playerListener);
	// Listen on the player listener socket
	playerListener.listen(playerPort);
	// Wait for potential new connection
	if (connectionSelector.wait(sf::milliseconds(1000.f)))
	{
		// Check if there is a pending connection to accept
		if (connectionSelector.isReady(playerListener))
		{
			// Accept new player connection
			if (playerListener.accept(playerConnectionSocket) == sf::Socket::Done)
			{
				// Store player's IP address
				playerIpAddress = playerConnectionSocket.getRemoteAddress();
				printf("Player connected\n");
				return true; // Return true - player connected to server
			}
			else
			{
				return false; // Return false - could not accept new player
			}
		}
		else
		{
			return false; // Return false - listener socket not ready 
		}
	}
	else
	{
		return false; // Return false - wait period expired
	}
}

// Send initial game data to player client
void Networking::sendStartGame(Player* player, vector<Zombie*> zombies, float startTime)
{
	// Initialise zombie start message
	ZombieMsg zStartMsg;

	for (int i = 0; i < zombies.size(); i++)
	{
		// Set each variable to send to player client for every zombie
		zStartMsg.zombieName = zombies[i]->getName();
		zStartMsg.zombieXpos = zombies[i]->getPosition().x;
		zStartMsg.zombieYpos = zombies[i]->getPosition().y;
		zStartMsg.rotation = zombies[i]->getRotation();
		zStartMsg.zombiePacket << zStartMsg.zombieName << zStartMsg.zombieXpos << zStartMsg.zombieYpos << zStartMsg.rotation;
		
		// Send zombie start data to player Client
		playerConnectionSocket.send(zStartMsg.zombiePacket); 
	
		// Clear data from packet for next iteration
		zStartMsg.zombiePacket.clear();						 
	}

	// Intialise player start message
	PlayerMsg pStartMsg;

	// Set initial player variables to send to the player client
	pStartMsg.playerName = player->getName();
	pStartMsg.playerXpos = player->getPosition().x;
	pStartMsg.playerYpos = player->getPosition().y;
	pStartMsg.playerRotation = player->getRotation();
	pStartMsg.playerVelocity = player->getVelocity();

	// Pack player packet with initial variables for game start
	pStartMsg.playerPacket << pStartMsg.playerName << pStartMsg.playerXpos << pStartMsg.playerYpos << pStartMsg.playerRotation << pStartMsg.playerVelocity;

	// Send initial player game data to player client
	playerConnectionSocket.send(pStartMsg.playerPacket);

	sf::Packet timePacket;
	timePacket << startTime;

	playerConnectionSocket.send(timePacket);
}

// Send server time to player client
void Networking::sendServerTime(float serverTime)
{
	// Create packet with the server's time
	sf::Packet timePacket;
	timePacket << serverTime;
	
	// Send the server time to the player client
	playerConnectionSocket.send(timePacket);
}

// UPDATES
// Recieve player update from player client
void Networking::recievePlayerUpdate(bool playerConnected)
{
	// Recieve update from player
	if (playerUpdateSocket.receive(pRecvMsg.playerPacket, playerIpAddress, playerPort) == sf::Socket::Done)
	{
		if (pRecvMsg.playerPacket >> pRecvMsg.identifier >> pRecvMsg.playerName >> pRecvMsg.playerXpos >> pRecvMsg.playerYpos >> pRecvMsg.playerRotation >> pRecvMsg.hasShot >> pRecvMsg.timeStamp)
		{
			// Push recieved player message
			playerMsgs.push_back(pRecvMsg); 

			// Set missed counter to 0
			updateMissedCounter = 0; 

			// Clear player recieve packet for next use
			pRecvMsg.playerPacket.clear();
		}
	}
	else
	{
		if (playerConnected)
		{
			// Increment misssed counter		
			updateMissedCounter++; 	
		}
	}
	
}

void Networking::sendPlayerUpdate(Player* player)
{
	// Set variables to be sent to player
	pSendMsg.identifier = true;
	pSendMsg.playerName = player->getName();
	pSendMsg.playerXpos = player->getPosition().x;
	pSendMsg.playerYpos = player->getPosition().y;
	pSendMsg.playerRotation = player->getRotation();
	pSendMsg.timeStamp = clock.getElapsedTime().asSeconds();

	// Pack player packet
	pSendMsg.playerPacket << pSendMsg.identifier << pSendMsg.playerName << pSendMsg.playerXpos << pSendMsg.playerYpos << pSendMsg.playerRotation << pSendMsg.timeStamp;

	// Send player update to player client
	playerUpdateSocket.send(pSendMsg.playerPacket, playerIpAddress, playerPort);

	// Clear player send packet for next use
	pSendMsg.playerPacket.clear();
}

void Networking::sendZombieUpdate(Zombie* zombie)
{
	// Set variables to be sent to player
	zSendMsg.identifier = false;
	zSendMsg.zombieName = zombie->getName();
	zSendMsg.zombieXpos = zombie->getPosition().x;
	zSendMsg.zombieYpos = zombie->getPosition().y;
	zSendMsg.rotation = zombie->getRotation();

	// Pack zombie packet
	zSendMsg.zombiePacket << zSendMsg.identifier << zSendMsg.zombieName << zSendMsg.zombieXpos << zSendMsg.zombieYpos << zSendMsg.rotation;

	// Send Zombie update to player client
	playerUpdateSocket.send(zSendMsg.zombiePacket, playerIpAddress, playerPort);

	// Clear zombie packet for next use
	zSendMsg.zombiePacket.clear();
}

// UNUSED - not fully implemented
void Networking::recieveBulletUpdate()
{
	if (playerUpdateSocket.receive(bRecvMsg.bulletPacket, playerIpAddress, playerPort) == sf::Socket::Done)
	{
		if (bRecvMsg.bulletPacket >> bRecvMsg.newShot >> bRecvMsg.bulletName >> bRecvMsg.startXpos >> bRecvMsg.startYpos >> bRecvMsg.angle)
		{
			bulletMsgs[bRecvMsg.bulletName].push_back(bRecvMsg);
		}
	}
}

bool Networking::clearOldData(bool playerConnected)
{
	// If player message vector size is greater than 2000 then clear the messages
	if (playerMsgs.size() > 2000)
	{
		playerMsgs.clear();
	}

	// If updates missed is about 1020 disconnect player and clear messages
	if (updateMissedCounter > 1020)
	{
		playerMsgs.clear();
		playerIpAddress = sf::IpAddress::None;
		playerConnectionSocket.disconnect();
		updateMissedCounter = 0;
		return true;
	}
	else
	{
		return false;
	}
}
