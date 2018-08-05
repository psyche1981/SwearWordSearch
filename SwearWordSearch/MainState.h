#pragma once

#include "Observer.h"
#include "Resources.h"


class MainState : public Observable
{
public:
	MainState() {}
	virtual ~MainState() {}

	virtual void Update(float dt) = 0;
	virtual void Draw(sf::RenderWindow* wnd) = 0;
	virtual void Input(sf::Event event) = 0;

	MainStates GetNextState() { return _nextState; }

protected:
	MainStates _nextState = MainStates::MAINSTATES_DEFAULT;
};
