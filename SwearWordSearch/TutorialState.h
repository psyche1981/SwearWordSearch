#pragma once
#include <SFML/Graphics.hpp>

#include "MainState.h"
#include "Resources.h"

class TutorialState : public MainState
{
public:
	TutorialState();
	virtual ~TutorialState();

	void Update(float dt) override;
	void Draw(sf::RenderWindow* wnd) override;
	void Input(sf::Event event) override;

private:
	sf::Text _titleText;
};
