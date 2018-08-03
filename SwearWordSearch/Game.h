#pragma once
#include <SFML\Graphics.hpp>
#include <vector>
#include <memory>

#include "State.h"
#include "Resources.h"



class Cell
{
public:
	Cell(int index, sf::Vector2f pos, char letter);
	~Cell();

	const int GetIndex() const { return _index; }
	const sf::Rect<float>& GetBox() { return _boundingBox; }
	void Click() { _selected = !_selected; }
	void Update(float dt);
	void Draw(sf::RenderWindow* wnd);

	//temp
	void CreateLetter();

private:
	sf::RectangleShape _rectShape;
	sf::Rect<float> _boundingBox;
	sf::Text _letterText;
	sf::Vector2f _position;
	
	bool _selected = false;
	int _index;
	char _letter;
};

class Game : public State
{
public:
	Game(StateManager* sm);
	virtual ~Game();

	void Update(float dt);
	void Draw(sf::RenderWindow* wnd);
	void Input(sf::Event event);

	void PopulateGrid();

private:
	std::vector<std::unique_ptr<Cell>> _grid;
	float _firstCellX = 50.0f;
	float _firstCellY = 80.0f;
	void DrawGridOutline(sf::RenderWindow* wnd);
	void SetUpGridOutline();
	std::vector<sf::RectangleShape> _outlineSides;
	int _numWordsToFind;
	int _countdownTimer;
	std::string _instructionText;
};
