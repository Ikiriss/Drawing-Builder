#pragma once
#include <SFML\Graphics.hpp>
#include "State.h"
#include "World.h"
#include "PugiXML\pugixml.hpp"
#include <iostream>



class GameState : public State
{
public:

	GameState(sf::RenderWindow& window, std::stack<States::stateName>& stack);

	virtual void		draw();
	virtual void		handleEvent(sf::Event& event);

	bool				isWin();

	void loadLevelInfo();

	void deleteLevelInfo();

	int getLevel() { return mLevel; };

	void setLevel(int level) { mLevel = level; };

	World* getWorld() { return &mWorld; };

	bool getIsLevelAlreadyLoaded() { return isLevelAlreadyLoaded; };
	void setIsLevelAlreadyLoaded(bool value) { isLevelAlreadyLoaded = value; };

	

private:
	std::stack<States::stateName>*		mStack;
	sf::RenderWindow*					mWindow;
	World								mWorld;
	int									mLevel;
	pugi::xml_document					mDoc;
	bool								isLevelAlreadyLoaded;

	sf::View							topView;
	sf::View							buttomView;

	sf::CircleShape						circleGenerator;
	sf::RectangleShape					rectangleGenerator;
	sf::CircleShape						triangleGenerator;
};