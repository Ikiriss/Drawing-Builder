#include "MenuMainState.h"



MenuMainState::MenuMainState(sf::RenderWindow& window, std::stack<States::stateName>& stack)
	: State(stack, window)
	, mWindow(&window)
	, mStack(&stack)
	, backGroundTexture()
	, backGround()
{
	if (!font.loadFromFile("arial.ttf"))
	{
		std::cout << "erreur de chargement de arial.ttf";
	}

	menu[0].setFont(font);
	menu[0].setColor(sf::Color::Red);
	menu[0].setString("Play");
	menu[0].setPosition(sf::Vector2f((float) window.getSize().x / 2, (float)window.getSize().y / (maxMenuItem + 1) * 1));

	menu[1].setFont(font);
	menu[1].setColor(sf::Color::Black);
	menu[1].setString("Exit");
	menu[1].setPosition(sf::Vector2f((float)window.getSize().x / 2, (float)window.getSize().y / (maxMenuItem + 1) * 2));

	backGroundTexture.loadFromFile("miniProjet.png");
	backGround.setOrigin(0.f, 0.f);
	backGround.setPosition(0.f, 0.f);
	backGround.setTexture(backGroundTexture);

	selectedItemIndex = 0;
}


void MenuMainState::draw()
{
	mWindow->draw(backGround);
	for (int i = 0; i < maxMenuItem; i++)
	{
		mWindow->draw(menu[i]);
	}
}

void MenuMainState::moveUp()
{
	if (selectedItemIndex - 1 >= 0)
	{
		menu[selectedItemIndex].setColor(sf::Color::Black);
		selectedItemIndex--;
		menu[selectedItemIndex].setColor(sf::Color::Red);
	}
	else
	{
		menu[selectedItemIndex].setColor(sf::Color::Black);
		selectedItemIndex = maxMenuItem - 1;
		menu[selectedItemIndex].setColor(sf::Color::Red);
	}
}

void MenuMainState::moveDown()
{
	if (selectedItemIndex + 1 < maxMenuItem)
	{
		menu[selectedItemIndex].setColor(sf::Color::Black);
		selectedItemIndex++;
		menu[selectedItemIndex].setColor(sf::Color::Red);
	}
	else
	{
		menu[selectedItemIndex].setColor(sf::Color::Black);
		selectedItemIndex=0;
		menu[selectedItemIndex].setColor(sf::Color::Red);
	}
}

void MenuMainState::handleEvent(sf::Event& event)
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

		case sf::Keyboard::Return:
			switch (getPressedItem())
			{
			case 0:
				State::requestStackPush(States::stateName::LevelMenu);
				break;
			case 1:
				mWindow->close();
				break;
			}
		}
	}

}