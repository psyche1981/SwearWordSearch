#pragma once
#include <SFML/Graphics.hpp>
#include <vector>

#include "MainState.h"
#include "Resources.h"

class MainMenuState : public MainState
{
public:
	enum SubState
	{
		MAIN,
		DIFFICULTY_SELECT,
		MODE_SELECT
	};


public:
	MainMenuState();
	virtual ~MainMenuState();

	void Update(float dt) override;
	void Draw(sf::RenderWindow* wnd) override;
	void Input(sf::Event event) override;

	const Difficulty GetSelectedDifficulty() const { return _selectedDifficulty; }

private:
	//main substate stuff
	std::vector<sf::Text> _mainTexts;	

	//difficulty substate stuff
	std::vector<sf::Text> _diffTexts;
	sf::Text _instructionText;

	//mode substate stuff
	

	//common in all substates
	sf::Text _titleText;

	void CreateMainSubstate();
	void CreateDifficultySubstate();
	void CreateModeSubstate();

	void ChangeSubstate(SubState nextSubstate);
	SubState _subState;

	Difficulty _selectedDifficulty;
};

