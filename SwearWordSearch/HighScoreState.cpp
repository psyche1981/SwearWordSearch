#include "HighScoreState.h"

HighScoreState::HighScoreState()
{
	_titleText = sf::Text("AGAINST THE CLOCK\n   HIGH SCORES", Resources::GetFont("CNB"), 40);
	_titleText.setColor(sf::Color::Black);
	_titleText.setPosition(220.0f, 50.0f);
	_backText = sf::Text("BACK", Resources::GetFont("CNB"), 30);
	_backText.setColor(sf::Color::Blue);
	_backText.setPosition(690.0f, 550.0f);
	_atcHighScores = Resources::GetATCHighScores();
	CreateDiffHeaders();
	PopulateHSTexts();
}

HighScoreState::~HighScoreState()
{
	std::cout << "HS destroyed" << std::endl;
}

void HighScoreState::Update(float dt)
{
}

void HighScoreState::Draw(sf::RenderWindow* wnd)
{
	wnd->draw(_titleText);
	wnd->draw(_backText);
	for (auto& t : _diffHeaderTexts)
	{
		wnd->draw(t);
	}
	for (auto& t : _easyTexts)
	{
		wnd->draw(t);
	}
	for (auto& t : _interTexts)
	{
		wnd->draw(t);
	}
	for (auto& t : _hardTexts)
	{
		wnd->draw(t);
	}
}

void HighScoreState::Input(sf::Event event)
{
	if (event.type == sf::Event::KeyPressed)
	{
		if (event.key.code == sf::Keyboard::Escape)
		{
			_nextState = MainStates::MENU;
			NotifyObservers();
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
	}
}

void HighScoreState::CreateDiffHeaders()
{
	_diffHeaderTexts.emplace_back("Easy", Resources::GetFont("CNB"), 30);
	_diffHeaderTexts.emplace_back("Intermediate", Resources::GetFont("CNB"), 30);
	_diffHeaderTexts.emplace_back("Hard", Resources::GetFont("CNB"), 30);

	_diffHeaderTexts[0].setColor(sf::Color::Black);
	_diffHeaderTexts[0].setPosition(50.0f, 200.0f);
	_diffHeaderTexts[1].setColor(sf::Color::Black);
	_diffHeaderTexts[1].setPosition(300.0f, 200.0f);
	_diffHeaderTexts[2].setColor(sf::Color::Black);
	_diffHeaderTexts[2].setPosition(650.0f, 200.0f);
}

void HighScoreState::PopulateHSTexts()
{
	std::string s;
	int counter = 0;
	for (auto it = _atcHighScores[0].begin(); it != _atcHighScores[0].end(); ++it)
	{		
		s = std::to_string(it->score) + " " + it->name;
		_easyTexts.emplace_back(s, Resources::GetFont("CNB"), 20);
		_easyTexts[counter].setColor(sf::Color::Black);
		_easyTexts[counter].setPosition(40.0f, 250.0f + 25.0f * counter);
		counter++;
	}
	counter = 0;
	for (auto it = _atcHighScores[1].begin(); it != _atcHighScores[1].end(); ++it)
	{
		s = std::to_string(it->score) + " " + it->name;
		_interTexts.emplace_back(s, Resources::GetFont("CNB"), 20);
		_interTexts[counter].setColor(sf::Color::Black);
		_interTexts[counter].setPosition(310.0f, 250.0f + 25.0f * counter);
		counter++;
	}
	counter = 0;
	for (auto it = _atcHighScores[2].begin(); it != _atcHighScores[2].end(); ++it)
	{
		s = std::to_string(it->score) + " " + it->name;
		_hardTexts.emplace_back(s, Resources::GetFont("CNB"), 20);
		_hardTexts[counter].setColor(sf::Color::Black);
		_hardTexts[counter].setPosition(620.0f, 250.0f + 25.0f * counter);
		counter++;
	}
}
