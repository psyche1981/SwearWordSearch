#include "GameOverState.h"

GameOverState::GameOverState(Difficulty diff, GameMode mode, int score)
	:
	_difficulty(diff),
	_mode(mode),
	_score(score)
{
	int m = (int)_mode;
	int d = (int)_difficulty;
	if (m == 0)
	{
		_highScores = Resources::GetATCHighScores()[d];
	}
	if (_highScores.size() == 10)
	{
		_saveScore = _score >= _highScores.back().score;
	}
	else
	{
		_saveScore = true;
	}
	
	std::string s = "GAME OVER\nYou scored " + std::to_string(_score);
	_goText = sf::Text(s, Resources::GetFont("CNB"), 50);
	_goText.setColor(sf::Color::Red);
	float x = (Constants::SCREENWIDTH / 2) - (_goText.getGlobalBounds().width / 2);
	float y = (Constants::SCREENHEIGHT / 2) - (_goText.getGlobalBounds().height / 2) - 200;
	if (_saveScore)
	{
		s = "   New High Score!\nEnter your name and click Save";
	}
	else
	{
		s = "Click Back to return to main menu";
	}
	
	_instText = sf::Text(s, Resources::GetFont("CNB"), 30);
	_instText.setColor(sf::Color::Black);
	_goText.setPosition(x, y);
	_instText.setPosition(x - 80, y + 150);
	_hsNameStr = "";	
	_hsNameText = sf::Text(_hsNameStr, Resources::GetFont("CNB"), 35);
	_hsNameText.setColor(sf::Color::Black);
	_hsNameText.setPosition(x, y + 280);

	_saveText = sf::Text("SAVE", Resources::GetFont("CNB"), 30);
	_saveText.setColor(sf::Color::Blue);
	_saveText.setPosition(320.0f, 550.0f);

	_backText = sf::Text("BACK", Resources::GetFont("CNB"), 30);
	_backText.setColor(sf::Color::Blue);
	_backText.setPosition(690.0f, 550.0f);

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
	wnd->draw(_backText);
	if (_saveScore)
	{
		wnd->draw(_saveText);
	}
}

void GameOverState::Input(sf::Event event)
{
	if (event.type == sf::Event::KeyPressed)
	{
		int code = event.key.code;
		if (code >= 0 && code <= 25)
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
	}

	if (event.type == sf::Event::MouseButtonPressed)
	{
		int x = event.mouseButton.x;
		int y = event.mouseButton.y;
		if (_backText.getGlobalBounds().contains(x, y))
		{
			_nextState = MainStates::MENU;
			NotifyObservers();
		}
		else if (_saveText.getGlobalBounds().contains(x, y))
		{
			if (!_hsNameStr.empty())
			{
				int m = (int)_mode;
				int d = (int)_difficulty;
				HighScore hs(m, d, _hsNameStr, _score);
				if (m == 0)//ATC
				{
					Resources::AddATCHighScore(hs);
				}
				_nextState = MainStates::MENU;
				NotifyObservers();
			}
		}
		
	}
}
