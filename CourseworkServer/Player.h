#pragma once
#include <SFML/Graphics.hpp>
#include <vector>

#include "PlayerMessage.h"

using namespace std;

class Player : public sf::Sprite
{
public:
	Player();
	~Player();

	// Predict the player client's movement
	void predictMovement(vector<PlayerMsg> playerMsgs);
public:
	// Set player name
	void setName(int name) { playerName = name; };
	// Get player name
	int  getName()		   { return playerName; };
	// Get player velocity 
	float getVelocity()	   { return velocity; };
private:
	sf::Texture  playerTexture;		// Player Texture
	sf::Vector2f predictedPos;		// Player client's predicted position
	int			 playerName;		// Player's name
	const float  velocity = 100.f;  // Player's constant movement velocity

};

