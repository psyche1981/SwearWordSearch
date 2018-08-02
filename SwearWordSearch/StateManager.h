#pragma once
#include <SFML/Graphics.hpp>
#include <memory>
#include <stack>

#include "Resources.h"
#include "Game.h"
#include "State.h"

class StateManager
{
public:
	StateManager();
	~StateManager();

	void Draw(sf::RenderWindow* wnd);
	void Input(sf::Event e);
	void Update(float dt);

	void ChangeState(States nextState);

private:
	std::stack<std::unique_ptr<State>> _states;
};


class Temp : public State
{
public:
	Temp(StateManager* sm) : State(sm) {}
	virtual ~Temp() {}

	void Update(float dt) {}
	void Draw(sf::RenderWindow* wnd) 
	{
		sf::Text hello("Temp state", Resources::GetFont("CNB"), 50);
		wnd->draw(hello);
	}
	void Input(sf::Event event) {}
};
