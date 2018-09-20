#pragma once
#include <SFML/Graphics.hpp>

#include "MainState.h"
#include "Resources.h"

class HighScoreState : public MainState
{
public:
	HighScoreState();
	virtual ~HighScoreState();

	void Update(float dt) override;
	void Draw(sf::RenderWindow* wnd) override;
	void Input(sf::Event event) override;

private:
	sf::Text _titleText;
	sf::Text _backText;
	std::vector<sf::Text> _diffHeaderTexts;
	std::vector<sf::Text> _easyTexts;
	std::vector<sf::Text> _interTexts;
	std::vector<sf::Text> _hardTexts;
	std::vector<std::vector<HighScore>> _atcHighScores;

	void CreateDiffHeaders();
	void PopulateHSTexts();
};