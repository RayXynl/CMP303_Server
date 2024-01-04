#include "Bullet.h"

Bullet::Bullet()
{
    // Load bullet texture
    bulletTexture.loadFromFile("Assets/Player/bullet1.png");
    // Set bullet texture
    setTexture(bulletTexture);

    // Set scale
    setScale(sf::Vector2f(0.015f, 0.015f));

    // Set origin
    setOrigin(getTextureRect().width / 2, getTextureRect().height / 2);

    // Set bullet sprite position
    setPosition(sf::Vector2f(720, 360));
}

Bullet::~Bullet()
{

}

void Bullet::update(float dt)
{
    move(moveTo.x * dt * velocity, moveTo.y * dt * velocity);

    distanceTravelled += velocity * dt;
    printf("%f\n", distanceTravelled);
}

void Bullet::shoot(Player* player)
{
    // Set bullet position to player position
    setPosition(player->getPosition() + sf::Vector2f(10.f, 10.f));

    // Calculate movement vector based on player's rotation
    float angle = player->getRotation() * (3.1415 / 180); // Convert degrees to radians
    float moveX = cos(angle);
    float moveY = sin(angle);

    // Set the movement vector with the desired range
    sf::Vector2f movementVector(moveX * range, moveY * range);

    // Normalize the movement vector
    float magnitude = sqrt(movementVector.x * movementVector.x + movementVector.y * movementVector.y);

    moveTo = sf::Vector2f(movementVector.x / magnitude, movementVector.y / magnitude);
}