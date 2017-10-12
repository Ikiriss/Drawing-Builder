#pragma once
#include <SFML\Graphics.hpp>
#include <memory>
#include <stack>  
#include <StatesNames.h>



class State
{
public:

	State(std::stack<States::stateName>& stack, sf::RenderWindow& window);

	virtual void		draw() = 0;
	virtual void		handleEvent(sf::Event& event) = 0;


protected:
	void				requestStackPush(States::stateName state); //Ask for a pushState
	void				requestStackPop(); //Ask for a popState


private:
	std::stack<States::stateName>*		mStack;
	sf::RenderWindow*					mWindow;
};