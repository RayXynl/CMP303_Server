#include "Player.h"

Player::Player()
{
	// Load player texture from file
	playerTexture.loadFromFile("Assets/Player/player.png");
	// Set player texture
	setTexture(playerTexture);
	
	// Set sprite origiin
	setOrigin(getTextureRect().width / 2, getTextureRect().height / 2);
	// Set sprite position
	setPosition(620, 360);
	// Set sprite scale
	setScale(sf::Vector2f(0.25f, 0.25f));
}

Player::~Player()
{

}

void Player::predictMovement(vector<PlayerMsg> playerMsgs)
{
	// If message vector has enough messages to predict player movement
	if (playerMsgs.size() > 1)
	{
		const PlayerMsg& msg0 = playerMsgs[playerMsgs.size() - 1]; // Retrieve most recent player message 
		const PlayerMsg& msg1 = playerMsgs[playerMsgs.size() - 2]; // Retrieve second most recent player message

		// Calculate time between last two messages
		double timeBetween = msg0.timeStamp - msg1.timeStamp;

		// If time between messages equals 0 do not predict position
		if (timeBetween == 0)
		{
			return;
		}

		// Calculate speed between last two messages
		sf::Vector2f speed((msg0.playerXpos - msg1.playerXpos) / timeBetween, (msg0.playerYpos - msg1.playerYpos) / timeBetween);
		// Calculate displace between last two messages
		sf::Vector2f displacement(speed.x * timeBetween, speed.y * timeBetween);        
		// Calculate predicted posiition based of the last two messages
		predictedPos = sf::Vector2f(msg0.playerXpos + displacement.x, msg0.playerYpos + displacement.y);
		// Set player's new position the calculated predicted position
		setPosition(predictedPos);
		setRotation(msg0.playerRotation);
	}
	else if (playerMsgs.size() > 0) // Not enough messages to predicted - just set position
	{
		const PlayerMsg& msg0 = playerMsgs[playerMsgs.size() - 1];
		setPosition(sf::Vector2f(msg0.playerXpos, msg0.playerYpos));
		setRotation(msg0.playerRotation);
	}
}
