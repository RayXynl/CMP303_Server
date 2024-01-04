#include <iostream>
#include <SFML/Graphics.hpp>
#include "Game.h"

int main()
{
	sf::RenderWindow window(sf::VideoMode(1280, 720), "Coursework Server"); // Intialise render window
	window.setFramerateLimit(60); // Set framerate limit of window to 60 fps
	window.requestFocus();
	
	float deltaTime; // Intialise deltaTime
	sf::Clock clock; // Intialise clock to apply deltaTime

	Game game(window, clock); // Intialise game engine

	sf::Event event;

	// While window is open run server
	while (window.isOpen())
	{
		window.pollEvent(event);
		deltaTime = clock.restart().asSeconds(); // Get the time since last clock restart and set delta time to it
		game.handleInput(deltaTime); // Game engine handle input
		game.update(deltaTime); // Game engine update
		game.render(); // Game engine render
	}

	return 0;
}