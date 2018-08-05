#pragma once

#include "MainState.h"

class GameOverState : public MainState
{
public:
	GameOverState(int score);
	~GameOverState();

	void Update(float dt) override;
	void Draw(sf::RenderWindow* wnd) override;
	void Input(sf::Event event) override;

private:
	int _score;

	sf::Text _goText;
};
