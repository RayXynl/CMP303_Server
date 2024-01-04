#pragma once
#include <SFML/Graphics.hpp>

#include "Player.h"

class Bullet : public sf::Sprite
{
public:
	Bullet();
	~Bullet();

	void update(float dt);
	void shoot(Player* player);
private:
	sf::Texture bulletTexture;
	sf::Vector2f moveTo;
	float magnitude;
	const float velocity = 300.f;
	float distanceTravelled;
	float range;
	int name;
	bool newShot;
public:
	float getDistanceTravelled() { return distanceTravelled; };

	void setName(int newName) { name = newName; };
	int getName() { return name; };

	void setNewShot(bool newState) { newShot = newState; };
	bool getNewShot() { return newShot; };
};

