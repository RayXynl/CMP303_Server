#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <cstdlib> 
#include <ctime> 

#include "Networking.h"
#include "Player.h"
#include "Zombie.h"
#include "Bullet.h"

using namespace std;

class Game
{
public:
	Game(sf::RenderWindow& window, sf::Clock& clock);
	~Game();

	// Handle input
	void handleInput(float dt);
	// Update 
	void update(float dt);
	// Render
	void render();
private:
	// Reference to window
	sf::RenderWindow& window;
	// Networking Class
	Networking*       network;
	// Zombies object vector
	vector<Zombie*>   zombies;
	// Player object
	Player*			  player;
	// Bullet object vector
	vector<Bullet*>    bullets;
	// Ground Sprite
	sf::Sprite		  groundSprite;
	// Ground Texture
	sf::Texture		  groundTexture;

private:
	// Set tick rate to 60 ticks
	const float		  tickRate = 1.0f / 60.f;
	// Accumulation of delta time
	float			  totalTime;
	// Variable to check if player is connected
	bool			  playerConnected;
	
};

