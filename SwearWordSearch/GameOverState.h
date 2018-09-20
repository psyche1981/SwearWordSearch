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
	std::vector<HighScore> _highScores;
	bool _saveScore = false;

	sf::Text _goText;
	sf::Text _instText;
	sf::Text _hsNameText;
	sf::Text _saveText;
	sf::Text _backText;
};
