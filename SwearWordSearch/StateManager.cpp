#include "StateManager.h"



StateManager::StateManager()
{
	_states.emplace(std::make_unique<Game>(this, Difficulty::EASY));
}

StateManager::~StateManager()
{
}

void StateManager::Draw(sf::RenderWindow * wnd)
{
	_states.top()->Draw(wnd);
}

void StateManager::Input(sf::Event e)
{
	_states.top()->Input(e);
}

void StateManager::Update(float dt)
{
	_states.top()->Update(dt);
}

void StateManager::ChangeState(States nextState)
{
	_states.pop();
	_states.emplace(std::make_unique<Temp>(this));
}

State::State(StateManager * sm)
	:
	_sm(sm)
{
}

State::~State()
{
}
