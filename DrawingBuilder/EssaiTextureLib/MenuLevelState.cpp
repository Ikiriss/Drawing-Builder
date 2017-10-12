#include "MenuLevelState.h"



MenuLevelState::MenuLevelState(sf::RenderWindow& window, std::stack<States::stateName>& stack)
	: State(stack, window)
	, mWindow(&window)
	, mStack(&stack)
{
	if (!font.loadFromFile("arial.ttf"))
	{
		std::cout << "erreur de chargement de arial.ttf";
	}

	menu[0].setFont(font);
	menu[0].setColor(sf::Color::Red);
	menu[0].setString("Level 1");
	menu[0].setPosition(sf::Vector2f((mWindow->getSize().x - 100) / (maxLevel + 1) * 1, mWindow->getSize().y / (maxLevel + 1) * 1));

	menu[1].setFont(font);
	menu[1].setColor(sf::Color::White);
	menu[1].setString("Level 2");
	menu[1].setPosition(sf::Vector2f((mWindow->getSize().x - 100) / (maxLevel + 1) * 5, mWindow->getSize().y / (maxLevel + 1) * 1));

	menu[2].setFont(font);
	menu[2].setColor(sf::Color::White);
	menu[2].setString("Level 3");
	menu[2].setPosition(sf::Vector2f((mWindow->getSize().x - 100) / (maxLevel + 1) * 9, mWindow->getSize().y / (maxLevel + 1) * 1));

	menu[3].setFont(font);
	menu[3].setColor(sf::Color::White);
	menu[3].setString("Level 4");
	menu[3].setPosition(sf::Vector2f((mWindow->getSize().x - 100) / (maxLevel + 1) * 1, mWindow->getSize().y / (maxLevel + 1) * 5));

	menu[4].setFont(font);
	menu[4].setColor(sf::Color::White);
	menu[4].setString("Level 5");
	menu[4].setPosition(sf::Vector2f((mWindow->getSize().x - 100) / (maxLevel + 1) * 5, mWindow->getSize().y / (maxLevel + 1) * 5));

	menu[5].setFont(font);
	menu[5].setColor(sf::Color::White);
	menu[5].setString("Level 6");
	menu[5].setPosition(sf::Vector2f((mWindow->getSize().x - 100) / (maxLevel + 1) * 9, mWindow->getSize().y / (maxLevel + 1) * 5));

	menu[6].setFont(font);
	menu[6].setColor(sf::Color::White);
	menu[6].setString("Level 7");
	menu[6].setPosition(sf::Vector2f((mWindow->getSize().x - 100) / (maxLevel + 1) * 1, mWindow->getSize().y / (maxLevel + 1) * 9));

	menu[7].setFont(font);
	menu[7].setColor(sf::Color::White);
	menu[7].setString("Level 8");
	menu[7].setPosition(sf::Vector2f((mWindow->getSize().x - 100) / (maxLevel + 1) * 5, mWindow->getSize().y / (maxLevel + 1) * 9));

	menu[8].setFont(font);
	menu[8].setColor(sf::Color::White);
	menu[8].setString("Add Level");
	menu[8].setPosition(sf::Vector2f((mWindow->getSize().x - 100) / (maxLevel + 1) * 9, mWindow->getSize().y / (maxLevel + 1) * 9));

	selectedItemIndex = 0;
}


void MenuLevelState::draw()
{
	for (int i = 0; i < maxLevel; i++)
	{
		mWindow->draw(menu[i]);
	}
}

void MenuLevelState::moveUp()
{
	if (selectedItemIndex - 3 >= 0)
	{
		menu[selectedItemIndex].setColor(sf::Color::White);
		selectedItemIndex-=3;
		menu[selectedItemIndex].setColor(sf::Color::Red);
	}
}

void MenuLevelState::moveDown()
{
	if (selectedItemIndex + 3 < maxLevel)
	{
		menu[selectedItemIndex].setColor(sf::Color::White);
		selectedItemIndex+=3;
		menu[selectedItemIndex].setColor(sf::Color::Red);
	}
}

void MenuLevelState::moveRight()
{
	if (selectedItemIndex + 1 < maxLevel)
	{
		menu[selectedItemIndex].setColor(sf::Color::White);
		selectedItemIndex++;
		menu[selectedItemIndex].setColor(sf::Color::Red);
	}
	else
	{
		menu[selectedItemIndex].setColor(sf::Color::White);
		selectedItemIndex = 0;
		menu[selectedItemIndex].setColor(sf::Color::Red);
	}
}

void MenuLevelState::moveLeft()
{
	if (selectedItemIndex - 1 >= 0)
	{
		menu[selectedItemIndex].setColor(sf::Color::White);
		selectedItemIndex--;
		menu[selectedItemIndex].setColor(sf::Color::Red);
	}
	else
	{
		menu[selectedItemIndex].setColor(sf::Color::White);
		selectedItemIndex = maxLevel - 1;
		menu[selectedItemIndex].setColor(sf::Color::Red);
	}
}

void MenuLevelState::handleEvent(sf::Event& event)
{
	switch (event.type)
	{
	case sf::Event::KeyReleased:
		switch (event.key.code)
		{
		case sf::Keyboard::Up:
			moveUp();
			break;

		case sf::Keyboard::Down:
			moveDown();
			break;

		case sf::Keyboard::Right:
			moveRight();
			break;

		case sf::Keyboard::Left:
			moveLeft();
			break;

		case sf::Keyboard::Return:
			switch (getPressedItem())
			{
			case 0:
				std::cout << "You have selected the level 1 \n" << std::endl;
				State::requestStackPush(States::stateName::Game);
				break;
			case 8:
				std::cout << "You have selected the level constructor \n" << std::endl;
				State::requestStackPush(States::stateName::LevelDesignState);
				break;
			default:
				std::cout << "The level you have selected does not exist please try later \n" << std::endl;
				break;
			}
			break;

		case sf::Keyboard::Escape:
			State::requestStackPop();
			break;
		}
	}
}