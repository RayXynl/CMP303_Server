#pragma once
#include <SFML/Graphics.hpp>

#include "Player.h"

class Zombie : public sf::Sprite
{
public:
	Zombie();
	~Zombie();

	// Update zombies
	void update(float dt);

	// Track player movements
	void trackPlayer(Player* player);
public:
	// Set and Get zombie name
	void setName(int name) { zombieName = name; };
	int  getName()		   { return zombieName; };
private:
	// Position for zombie to move to
	sf::Vector2f moveTo;
	// Texture of zombie object
	sf::Texture  zombieTexture;
	// Velocity at which zombie can move
	const float	 velocity = 15.0f;
	// Zombie's name for indification in network sending
	int		     zombieName;
};

