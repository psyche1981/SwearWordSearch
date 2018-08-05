#include "MainMenuState.h"

MainMenuState::MainMenuState()
	:
	_subState(SubState::MAIN),
	_selectedDifficulty(Difficulty::EASY)
{
	std::cout << "MEnu Created" << std::endl;
	CreateMainSubstate();	
	CreateDifficultySubstate();
}

MainMenuState::~MainMenuState()
{
	std::cout << "Menu Destroyed" << std::endl;
}

void MainMenuState::Update(float dt)
{
}

void MainMenuState::Draw(sf::RenderWindow* wnd)
{
	wnd->draw(_titleText);
	if (_subState == SubState::MAIN)
	{		
		for (auto& t : _mainTexts)
		{
			wnd->draw(t);
		}
	}
	else if (_subState == SubState::DIFFICULTY_SELECT)
	{
		for (auto& t : _diffTexts)
		{
			wnd->draw(t);
		}

	}
	else if (_subState == SubState::MODE_SELECT)
	{
		//TODO:
	}
}

void MainMenuState::Input(sf::Event event)
{
	if (event.type == sf::Event::MouseButtonPressed)
	{
		float x = event.mouseButton.x;
		float y = event.mouseButton.y;
		if (_subState == SubState::MAIN)
		{
			if (_mainTexts[0].getGlobalBounds().contains(x, y))
			{
				ChangeSubstate(SubState::DIFFICULTY_SELECT);
			}
			else if (_mainTexts[1].getGlobalBounds().contains(x, y))
			{				
				_nextState = MainStates::TUTORIAL;
				NotifyObservers();
			}
		}
		else if (_subState == SubState::DIFFICULTY_SELECT)
		{
			if (_diffTexts[0].getGlobalBounds().contains(x, y))
			{
				std::cout << "Hello" << std::endl;
				_nextState = MainStates::GAME;
				_selectedDifficulty = Difficulty::EASY;
				NotifyObservers();
			}
			else if(_diffTexts[1].getGlobalBounds().contains(x, y))
			{
				_nextState = MainStates::GAME;
				_selectedDifficulty = Difficulty::INTERMEDIATE;
				NotifyObservers();
			}
			else if (_diffTexts[2].getGlobalBounds().contains(x, y))
			{
				_nextState = MainStates::GAME;
				_selectedDifficulty = Difficulty::HARD;
				NotifyObservers();
			}
		}
		else if (_subState == SubState::MODE_SELECT)
		{
			//TODO:
		}
		
	}
}

void MainMenuState::ChangeSubstate(SubState nextSubstate)
{
	_subState = nextSubstate;
}

void MainMenuState::CreateMainSubstate()
{
	_titleText = sf::Text("(SWEAR)WORDSEARCH", Resources::GetFont("CNB"), 60);
	_titleText.setColor(sf::Color::Black);
	_titleText.setPosition(100.0f, 100.0f);

	//TODO: menu item factory function instead of hard coded
	_mainTexts.emplace_back("PLAY", Resources::GetFont("CNB"), 30);
	_mainTexts.emplace_back("TUTORIAL", Resources::GetFont("CNB"), 30);

	_mainTexts[0].setPosition(350.0f, 300.0f);
	_mainTexts[0].setColor(sf::Color::Blue);
	_mainTexts[1].setPosition(350.0f, 350.0f);
	_mainTexts[1].setColor(sf::Color::Blue);
}

void MainMenuState::CreateDifficultySubstate()
{
	_diffTexts.emplace_back("EASY", Resources::GetFont("CNB"), 30);
	_diffTexts.emplace_back("INTERMEDIATE", Resources::GetFont("CNB"), 30);
	_diffTexts.emplace_back("HARD", Resources::GetFont("CNB"), 30);
	for (size_t i = 0; i < _diffTexts.size(); i++)
	{
		_diffTexts[i].setColor(sf::Color::Black);
		_diffTexts[i].setPosition(350.0f, 300.0f + i * 50.0f);
	}
}

void MainMenuState::CreateModeSubstate()
{
}
