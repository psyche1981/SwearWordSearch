#pragma once
#include <SFML/Graphics.hpp>
#include <memory>

#include "Resources.h"
#include "MainState.h"
#include "GameState.h"
#include "MainMenuState.h"
#include "GameOverState.h"
#include "Observer.h"

class MainStateManager : public Observer
{
public:
	MainStateManager();
	~MainStateManager();

	void Draw(sf::RenderWindow* wnd);
	void Input(sf::Event e);
	void Update(float dt);


	void Notify(Observable* _obs);

private:
	std::unique_ptr<MainState> _currentState;
	void ChangeState(MainStates nextState);
};