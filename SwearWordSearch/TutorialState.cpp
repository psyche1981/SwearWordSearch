#include "TutorialState.h"

TutorialState::TutorialState()
{
	_titleText = sf::Text("Tutorials \n\nUnder Construction\nPress Escape to return to menu", Resources::GetFont("CNB"), 30);
	_titleText.setColor(sf::Color::Black);
	_titleText.setPosition(100.0f, 100.0f);
}

TutorialState::~TutorialState()
{
}

void TutorialState::Update(float dt)
{
}

void TutorialState::Draw(sf::RenderWindow * wnd)
{
	wnd->draw(_titleText);
}

void TutorialState::Input(sf::Event event)
{
	if (event.type == sf::Event::KeyPressed)
	{
		if (event.key.code == sf::Keyboard::Escape)
		{
			_nextState = MainStates::MENU;
			NotifyObservers();
		}
	}
}
