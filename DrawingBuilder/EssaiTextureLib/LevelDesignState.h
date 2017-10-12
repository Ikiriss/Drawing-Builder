#pragma once
#include <SFML\Graphics.hpp>
#include "State.h"
#include "World.h"
#include "PugiXML\pugixml.hpp"
#include <iostream>



class LevelDesignState : public State
{
public:

	LevelDesignState(sf::RenderWindow& window, std::stack<States::stateName>& stack);

	virtual void		draw();
	virtual void		handleEvent(sf::Event& event);

	void saveConstructionInfo();

	World* getWorld() { return &mWorld; };




private:
	std::stack<States::stateName>*		mStack;
	sf::RenderWindow*					mWindow;
	World								mWorld;

	sf::View							topView;
	sf::View							buttomView;

	sf::CircleShape						circleGenerator;
	sf::RectangleShape					rectangleGenerator;
	sf::CircleShape						triangleGenerator;
};