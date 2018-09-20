#pragma once

#include "MainState.h"

class GameOverState : public MainState
{
public:
	GameOverState(Difficulty diff, GameMode mode, int score);
	~GameOverState();

	void Update(float dt) override;
	void Draw(sf::RenderWindow* wnd) override;
	void Input(sf::Event event) override;

private:
	int _score;
	Difficulty _difficulty;
	GameMode _mode;
	std::string _hsNameStr;

	sf::Text _goText;
	sf::Text _instText;
	sf::Text _hsNameText;
};
