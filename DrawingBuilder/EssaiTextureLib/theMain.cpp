#include <SFML\Graphics.hpp>
#include <Box2D\Box2D.h>
#include <fstream>
#include <iostream>
#include <iomanip>
#include <string>
#include <stdlib.h>
#include <math.h>
#include <chrono>
#include <thread>
#include <vector>
#include "theMain.h"
#include "MenuMainState.h"
#include "MenuLevelState.h"
#include "State.h"
#include "GameState.h"
#include "LevelDesignState.h"
#include "StatesNames.h"

#pragma region VARIABLES
static int fps = 60; // Frame Per Second
#pragma endregion VARIABLES

int	theMain()
{

	/** Prepare the window */
	sf::RenderWindow window(sf::VideoMode(1200, 1000, 32), "DrawingBuilder");
	window.setFramerateLimit(fps);

	// Create the state stack and initialise with the menuState
	std::stack<States::stateName> stack;
	const States::stateName menuState = States::stateName::Menu;
	stack.push(menuState);

	MenuMainState mainMenu(window, stack);
	MenuLevelState levelMenu(window, stack);
	GameState game(window, stack);
	LevelDesignState levelConstructor(window, stack);

	while (window.isOpen())
	{
		sf::Event event;

		while (window.pollEvent(event))
		{
			// Close event
			if (event.type == sf::Event::Closed)
				window.close();

			// Event handler
			switch (stack.top())
			{
			case States::stateName::Menu :
				mainMenu.handleEvent(event);
				break;
			case States::stateName::LevelMenu :
				levelMenu.handleEvent(event);
				break;
			case States::stateName::Game :
				if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
				{
					float mouseX = (float)sf::Mouse::getPosition(window).x;
					float mouseY = (float)sf::Mouse::getPosition(window).y;
					game.getWorld()->changeBodyPosition(mouseX, mouseY);
				}
				game.handleEvent(event);
				break;
			case States::stateName::LevelDesignState :
				if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
				{
					float mouseX = (float)sf::Mouse::getPosition(window).x;
					float mouseY = (float)sf::Mouse::getPosition(window).y;
					levelConstructor.getWorld()->changeBodyPosition(mouseX, mouseY);
				}
				levelConstructor.handleEvent(event);
				break;
			}
		}

		// Drawing handler
		switch (stack.top())
		{
		case States::stateName::Menu:
			window.clear();
			window.setView(window.getDefaultView());
			mainMenu.draw();
			window.display();
			break;
		case States::stateName::LevelMenu:
			window.clear();
			window.setView(window.getDefaultView());
			levelMenu.draw();
			window.display();
			break;
		case States::stateName::Game:
			game.setLevel(levelMenu.getPressedItem() + 1);
			/*
			if (!game.getIsLevelAlreadyLoaded())
			{ 
				game.setIsLevelAlreadyLoaded(true);
				game.loadLevelInfo();
			}
			*/
			window.clear(sf::Color::White);
			window.setView(window.getDefaultView());
			game.draw();
			window.display();
			break;
		case States::stateName::LevelDesignState:
			window.clear(sf::Color::White);
			window.setView(window.getDefaultView());
			levelConstructor.draw();
			window.display();
			break;
		}


	}

}