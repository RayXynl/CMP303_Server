#pragma once
#include <SFML/Network.hpp>
#include <vector>
#include <unordered_map>

#include "PlayerMessage.h"
#include "Player.h"
#include "ZombieMessage.h"
#include "Zombie.h"
#include "BulletMessage.h"

using namespace std;

class Networking
{
public:
	Networking(sf::Clock& clock);
	~Networking();

	// GENERAL
	// Send start game to new player
	void sendStartGame(Player* player, vector<Zombie*> zombies, float startTime);
	// Send server time to client for synchronisation
	void sendServerTime(float serverTime);
	// Clear old network data
	bool clearOldData(bool playerConnected);

	// PLAYER
	// Accept new player connections 
	bool acceptPlayerConnections();
	// Recieve player updates
	void recievePlayerUpdate(bool playerConnected);
	// Send player updates
	void sendPlayerUpdate(Player* player);

	// ZOMBIE
	// Send zombie updates to player clients
	void sendZombieUpdate(Zombie* zombie);

	// BULLET
	// Recieve bullet update
	void recieveBulletUpdate();
private:
	// Vector to store recieved player messages
	vector<PlayerMsg> playerMsgs;
	// Vector to store recieved bullet information
	unordered_map<int, vector<BulletMsg>> bulletMsgs;
public:
	// Get player message vector
	vector<PlayerMsg> getPlayerMsgs() { return playerMsgs; };
	// Get bullet message vector
	vector<BulletMsg> getBulletMsgs(int name) { return bulletMsgs[name]; };
private:
	// Player Connections
	sf::SocketSelector connectionSelector;	   // Socket selector for new player connections
	sf::TcpListener	   playerListener;         // Listener socket for new player connections
	sf::TcpSocket	   playerConnectionSocket; // Socket for sending initial game data to player

	// Player Updates
	sf::UdpSocket	   playerUpdateSocket;	   // Socket for sending and recieving player updates
	sf::IpAddress	   playerIpAddress;		   // Variable to store connected player's IP address
	unsigned short     playerPort = 5400;	   // Port used to send and recieve player updates
	int				   updateMissedCounter;    // Counter used to to stop sending and recieving updates if player client times out
	PlayerMsg		   pSendMsg;			   // Struct containing all data variables to send to player client
	PlayerMsg		   pRecvMsg;			   // Struct containing all data variables to recieve from player client

	// Zombie Updates
	sf::UdpSocket	   zombieUpdateSocket;     // Socket for sending and zombie updates
	unsigned short     zombiePort = 5401;	   // Port used to send zombie updates
	ZombieMsg		   zSendMsg;			   // Struct containing all data to be sent to player client

	// Bullet Updates
	BulletMsg		   bRecvMsg;

	sf::Clock&		   clock;				   // Reference to games clock 

};

