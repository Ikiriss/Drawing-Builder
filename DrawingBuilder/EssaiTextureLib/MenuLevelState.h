#include "SFML/Graphics.hpp"
#include <stdlib.h>
#include <fstream>
#include <iostream>
#include <iomanip>
#include <string>
#include <State.h>
#include <stack>

const int maxLevel = 9;

class MenuLevelState : public State
{
public:
	MenuLevelState(sf::RenderWindow& window, std::stack<States::stateName>& stack);

	void draw();
	void moveUp();
	void moveDown();
	void moveRight();
	void moveLeft();
	int getPressedItem() { return selectedItemIndex; }
	void handleEvent(sf::Event& event);

private:
	int									selectedItemIndex;
	sf::Font							font;
	sf::Text							menu[maxLevel];
	std::stack<States::stateName>*		mStack;
	sf::RenderWindow*					mWindow;

};
