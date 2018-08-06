#include "Cell.h"

Cell::Cell(int index, sf::Vector2f pos)
	:
	_index(index),
	_position(pos)
{
	sf::Vector2f size(Constants::CELLSIZE, Constants::CELLSIZE);
	_boundingBox = sf::Rect<float>(pos, size);
	_rectShape = sf::RectangleShape(size);
	_rectShape.setFillColor(sf::Color::Red);
	_rectShape.setPosition(pos);

	//temp initialise to -
	_letter = '-';
	CreateLetter();
}

Cell::~Cell()
{

}

void Cell::SetLetter(char letter)
{
	_letter = letter;
	_letterText.setString(_letter);
}

void Cell::Update(float dt)
{
	if (_found && !_selected)
	{
		_rectShape.setFillColor(sf::Color(0, 255, 0, 126));
	}
	else if ((_found && _selected) || (_selected && !_found))
	{
		_rectShape.setFillColor(sf::Color(0, 0, 255, 200));
	}
	else
	{
		_rectShape.setFillColor(sf::Color::White);
	}
}

void Cell::Draw(sf::RenderWindow * wnd)
{
	wnd->draw(_rectShape);
	wnd->draw(_letterText);
}

void Cell::Reset()
{
	_selected = false;
	_found = false;
	SetLetter('-');
}

void Cell::CreateLetter()
{
	_letterText = sf::Text(_letter, Resources::GetFont("CNB"), 20);
	float newX = _position.x + _letterText.getGlobalBounds().width;
	float newY = _position.y + _letterText.getGlobalBounds().height / 2;
	sf::Vector2f newPos(newX, newY);
	_letterText.setPosition(newPos);
	_letterText.setFillColor(sf::Color::Black);
}