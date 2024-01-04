#include "Game.h"


Game::Game(sf::RenderWindow& wndow, sf::Clock& clock) :
	window(wndow), playerConnected(false)
{
	// Load ground texture
	groundTexture.loadFromFile("Assets/groundTexture.png");

	// Apply ground texture to ground sprite
	groundSprite.setTexture(groundTexture);

	// Initialse networking interface
	network = new Networking(clock);

	// Initialise zombie objects
	for (int i = 0; i < 6; i++)
	{
		Zombie* zombie = new Zombie();
		zombie->setName(i);								  // Set zombie name
		zombie->setPosition(rand() % 1280, rand() % 720); // Set zombie position
		zombies.push_back(zombie);						  // Push zombie to zombies vector
	}
}

Game::~Game()
{
	if (player)
	{
		player = 0;
		delete player;
	}

	if (network)
	{
		network = 0;
		delete network;
	}

	zombies.clear();
}

void Game::handleInput(float dt)
{

}

void Game::update(float dt)
{
	totalTime += dt; // Accumulate delta Time
	// If player is not connected
	if (!playerConnected)
	{
		// Accept incoming player connection
		if (network->acceptPlayerConnections())
		{
			// Create new player
			player = new Player();
			// Set player connected to true
			playerConnected = true;
			// Send Initial game data to player client
			network->sendStartGame(player, zombies, totalTime);
		}
		else
		{
			// Subtract time waiting for player connection
			totalTime -= 1.0f;
		}
	
	}

	// If player is connected
	if (playerConnected)
	{
		while (totalTime >= tickRate)
		{
			// Recieve player update from client
			network->recievePlayerUpdate(playerConnected);

			// Send server time to client for sychronisation 
			network->sendServerTime(totalTime);

			// Predict player movement
			player->predictMovement(network->getPlayerMsgs());

			// Send player update to client
			network->sendPlayerUpdate(player);

			// Loop through zombie vector
			for (int i = 0; i < zombies.size(); i++)
			{
				// For every zombie object send the data to the player client
				network->sendZombieUpdate(zombies[i]);
			}

			// Clear old network messages
			if (network->clearOldData(playerConnected))
			{
				playerConnected = false;
				delete player;
			}

			// Subtract tickrate from total time
			totalTime -= tickRate;
		}
		for (int i = 0; i < zombies.size(); i++)
		{
			zombies[i]->trackPlayer(player);	   // track the player 
			zombies[i]->update(dt);				   // Update zombie position and rotation
		}
	}
}

void Game::render()
{
	window.clear(sf::Color::Yellow);

	window.draw(groundSprite);

	// If player is connected render the player and the zombies
	if (playerConnected)
	{
		window.draw(*player);
		for (int i = 0; i < zombies.size(); i++)
		{
			window.draw(*zombies[i]);
		}
	}

	window.display();
}