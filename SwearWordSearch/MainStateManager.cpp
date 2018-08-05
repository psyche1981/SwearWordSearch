#include "MainStateManager.h"

MainStateManager::MainStateManager()
{
	_currentState = std::make_unique<Game>(Difficulty::EASY);
	_currentState->AddObserver(this);
}

MainStateManager::~MainStateManager()
{
}

void MainStateManager::Draw(sf::RenderWindow * wnd)
{
	_currentState->Draw(wnd);
}

void MainStateManager::Input(sf::Event e)
{
	_currentState->Input(e);
}

void MainStateManager::Update(float dt)
{
	_currentState->Update(dt);
}

void MainStateManager::Notify(Observable* _obs)
{	
	ChangeState(_currentState->GetNextState());	
}

void MainStateManager::ChangeState(MainStates nextState)
{
	switch (nextState)
	{
	case MainStates::GAME_OVER:
		Game* g = (Game*)_currentState.get();
		int score = g->GetScore();
		_currentState = std::make_unique<GameOverState>(score);
		_currentState->AddObserver(this);
		break;
	}
}


