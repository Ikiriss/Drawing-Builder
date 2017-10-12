#include "State.h"
#include <stack>  



State::State(std::stack<States::stateName>& stack, sf::RenderWindow& window)
	: mWindow(&window)
	, mStack(&stack)
{

}


void State::requestStackPush(States::stateName state)
{
	mStack->push(state);
}

void State::requestStackPop()
{
	mStack->pop();
}

