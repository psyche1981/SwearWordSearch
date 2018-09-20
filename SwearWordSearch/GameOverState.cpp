#include "GameOverState.h"

GameOverState::GameOverState(Difficulty diff, GameMode mode, int score)
	:
	_difficulty(diff),
	_mode(mode),
	_score(score)
{
	std::string s = "GAME OVER\nYou scored " + std::to_string(_score);
	_goText = sf::Text(s, Resources::GetFont("CNB"), 50);
	_goText.setColor(sf::Color::Red);
	float x = (Constants::SCREENWIDTH / 2) - (_goText.getGlobalBounds().width / 2);
	float y = (Constants::SCREENHEIGHT / 2) - (_goText.getGlobalBounds().height / 2) - 100;
	s = "Press SPACE to return to menu";
	_instText = sf::Text(s, Resources::GetFont("CNB"), 30);
	_instText.setColor(sf::Color::Black);
	_goText.setPosition(x, y);
	_instText.setPosition(x - 20, y + 150);
	_hsNameStr = "";	
	_hsNameText = sf::Text(_hsNameStr, Resources::GetFont("CNB"), 30);
	_hsNameText.setColor(sf::Color::Black);
	_hsNameText.setPosition(x, y + 220);
}

GameOverState::~GameOverState()
{
}

void GameOverState::Update(float dt)
{
	_hsNameText.setString(_hsNameStr);
}

void GameOverState::Draw(sf::RenderWindow * wnd)
{
	wnd->draw(_goText);
	wnd->draw(_instText);
	wnd->draw(_hsNameText);
}

void GameOverState::Input(sf::Event event)
{
	if (event.type == sf::Event::KeyPressed)
	{
		int code = event.key.code;
		if (code == sf::Keyboard::Space)
		{
			_nextState = MainStates::MENU;
			NotifyObservers();
		}
		else if (code >= 0 && code <= 25)
		{
			if (_hsNameStr.length() <= 8)
			{
				_hsNameStr += Resources::keys[code];
			}			
		}
		else if (code == sf::Keyboard::BackSpace)
		{
			int last = _hsNameStr.length() - 1;
			if (last >= 0)
			{
				_hsNameStr.erase(last);
			}			
		}
		else if (code == sf::Keyboard::Return)
		{
			if (!_hsNameStr.empty())
			{
				int m = (int)_mode;
				int d = (int)_difficulty;
				HighScore hs(m,d,_hsNameStr,_score);
				if (m == 0)//ATC
				{
					Resources::AddATCHighScore(hs);
				}
			}
		}
		
	}
}
