#pragma once
#include <SFML/Graphics.hpp>

#include "Resources.h"


class Cell
{
public:
	Cell(int index, sf::Vector2f pos);
	~Cell();

	const int GetIndex() const { return _index; }
	const sf::Rect<float>& GetBox() { return _boundingBox; }
	void SetLetter(char letter);
	const char GetLetter() { return _letter; }
	void Click() { _selected = !_selected; }

	void Update(float dt);
	void Draw(sf::RenderWindow* wnd);

	void Deselect() { _selected = false; }
	void Select() { _selected = true; }
	void Reset();

	const bool IsSelected() { return _selected; }
	void Found() { _found = true; }

private:
	sf::RectangleShape _rectShape;
	sf::Rect<float> _boundingBox;
	sf::Text _letterText;
	sf::Vector2f _position;

	bool _selected = false;
	bool _found = false;
	int _index;
	char _letter;

	void CreateLetter();
};