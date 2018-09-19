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
	std::string _hsNameStr;

	sf::Text _goText;
	sf::Text _instText;
	sf::Text _hsNameText;
};
