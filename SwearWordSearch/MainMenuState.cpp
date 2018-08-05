#include "MainMenuState.h"

MainMenuState::MainMenuState()
{
	std::cout << "MEnu Created" << std::endl;

	_titleText = sf::Text("(SWEAR)WORDSEARCH", Resources::GetFont("CNB"), 60);
	_titleText.setColor(sf::Color::Black);
	_titleText.setPosition(100.0f, 100.0f);

	//TODO: menu item factory function instead of hard coded
	_menuTexts.emplace_back("PLAY", Resources::GetFont("CNB"), 30);
	_menuTexts.emplace_back("TUTORIAL", Resources::GetFont("CNB"), 30);

	_menuTexts[0].setPosition(350.0f, 300.0f);
	_menuTexts[0].setColor(sf::Color::Blue);
	_menuTexts[1].setPosition(350.0f, 350.0f);
	_menuTexts[1].setColor(sf::Color::Blue);
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
	for (auto& t : _menuTexts)
	{
		wnd->draw(t);
	}
}

void MainMenuState::Input(sf::Event event)
{
	//TODO: do mouse collision detection
	if (event.type == sf::Event::MouseButtonPressed)
	{
		float x = event.mouseButton.x;
		float y = event.mouseButton.y;
		if (_menuTexts[0].getGlobalBounds().contains(x, y))
		{
			_nextState = MainStates::GAME;
			NotifyObservers();
		}
		else if (_menuTexts[1].getGlobalBounds().contains(x, y))
		{
			_nextState = MainStates::TUTORIAL;
			NotifyObservers();
		}
	}
}
