#include "SFML/Graphics.hpp"
#include <stdlib.h>
#include <fstream>
#include <iostream>
#include <iomanip>
#include <string>
#include <State.h>
#include <stack>

const int maxMenuItem = 2;

class MenuMainState : public State
{
public:
	MenuMainState(sf::RenderWindow &window, std::stack<States::stateName>& stack);

	void draw();
	void moveUp();
	void moveDown();
	int	getPressedItem() { return selectedItemIndex; }
	void handleEvent(sf::Event& event);

private:
	int selectedItemIndex;
	sf::Font							font;
	sf::Text							menu[maxMenuItem];
	std::stack<States::stateName>*		mStack;
	sf::RenderWindow*					mWindow;
	sf::Texture							backGroundTexture;
	sf::Sprite							backGround;

};
