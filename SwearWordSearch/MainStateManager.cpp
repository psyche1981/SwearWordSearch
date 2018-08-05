#include "MainStateManager.h"

MainStateManager::MainStateManager()
{
	_currentState = std::make_unique<MainMenuState>();
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
	case MainStates::GAME:
		//TODO: pass in difficulty from menu - user selected
		_currentState = std::make_unique<GameState>(Difficulty::EASY);
		_currentState->AddObserver(this);
		break;
	case MainStates::GAME_OVER:
		{
			GameState* g = (GameState*)_currentState.get();
			int score = g->GetScore();
			_currentState = std::make_unique<GameOverState>(score);
			_currentState->AddObserver(this);
		}		
		break;
	case MainStates::MENU:
		_currentState = std::make_unique<MainMenuState>();
		_currentState->AddObserver(this);
		break;
	case MainStates::TUTORIAL:
		_currentState = std::make_unique<TutorialState>();
		_currentState->AddObserver(this);
	}
}


