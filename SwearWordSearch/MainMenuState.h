#pragma once
#include <SFML/Graphics.hpp>
#include <vector>

#include "MainState.h"
#include "Resources.h"

class MainMenuState : public MainState
{
public:
	MainMenuState();
	virtual ~MainMenuState();

	void Update(float dt) override;
	void Draw(sf::RenderWindow* wnd) override;
	void Input(sf::Event event) override;

private:
	std::vector<sf::Text> _menuTexts;
	sf::Text _titleText;
};

