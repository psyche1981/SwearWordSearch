#include "GameOverState.h"

GameOverState::GameOverState(int score)
	:
	_score(score)
{
	std::string s = "GAME OVER\nYou scored " + std::to_string(_score);
	_goText = sf::Text(s, Resources::GetFont("CNB"), 50);
	_goText.setColor(sf::Color::Red);
	float x = (Constants::SCREENWIDTH / 2) - (_goText.getGlobalBounds().width / 2);
	float y = (Constants::SCREENHEIGHT / 2) - (_goText.getGlobalBounds().height / 2);
	_goText.setPosition(x, y);
}

GameOverState::~GameOverState()
{
}

void GameOverState::Update(float dt)
{
}

void GameOverState::Draw(sf::RenderWindow * wnd)
{
	wnd->draw(_goText);
}

void GameOverState::Input(sf::Event event)
{
	if (event.type == sf::Event::KeyPressed)
	{
		if (event.key.code == sf::Keyboard::Space)
		{
			_nextState = MainStates::MENU;
			NotifyObservers();
		}
	}
}
