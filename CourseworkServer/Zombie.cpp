#include "Zombie.h"

Zombie::Zombie()
{
	// Load zombies texture from file and assign it to the sprite
	zombieTexture.loadFromFile("Assets/Zombie/zombie.png");
	// Set zombie texture
	setTexture(zombieTexture);
	
	// Set origin
	setOrigin(getTextureRect().width / 2, getTextureRect().height / 2);

	// Scale zombie appropiately
	setScale(sf::Vector2f(0.25f, 0.25f)); 
}

Zombie::~Zombie()
{

}

void Zombie::update(float dt)
{
	// Move to player position
	move(moveTo.x * dt * velocity, moveTo.y * dt * velocity);
}

void Zombie::trackPlayer(Player* player)
{
	// Calculate the vector between the player and the zombie
	moveTo = sf::Vector2f(player->getPosition().x - getPosition().x, player->getPosition().y - getPosition().y);

	// Calculate magnitude of the vector towards the player
	float magnitude = sqrt(moveTo.x * moveTo.x + moveTo.y * moveTo.y);

	// Set the move to vector 
	moveTo = sf::Vector2f(moveTo.x / magnitude, moveTo.y / magnitude);

	// Calculate the angle to face the player
	float angleToPlayer = -atan2(player->getPosition().x - getPosition().x, player->getPosition().y - getPosition().y) * 180 / 3.1415 + 90;

	// Set zombie rotation
	setRotation(angleToPlayer);
}