#pragma once

class StateManager;

class State
{
public:
	State(StateManager* sm);
	virtual ~State();

	virtual void Update(float dt) = 0;
	virtual void Draw(sf::RenderWindow* wnd) = 0;
	virtual void Input(sf::Event event) = 0;

protected:
	StateManager* _sm;
};
