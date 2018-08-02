#include "Game.h"

Game::Game(StateManager* sm)
	:
	State(sm)
{
	_numWordsToFind = 4;
	_countdownTimer = 30;
	for (int i = 0; i < Constants::NUMCELLS; i++)
	{
		int row = i / 16;
		int col = i % 16;
		sf::Vector2f pos(_firstCellX + Constants::CELLSIZE * col, _firstCellY + Constants::CELLSIZE * row);
		_grid.emplace_back(std::make_unique<Cell>(i, pos));
	}
	
	SetUpGridOutline();
	

}

Game::~Game()
{
	std::cout << "Game Destroyed" << std::endl;
}

void Game::Update(float dt)
{
	_instructionText = "Find " + std::to_string(_numWordsToFind) + " words in " + std::to_string(_countdownTimer) + " seconds";
	for (auto& c : _grid)
	{
		c->Update(dt);
	}
}

void Game::Draw(sf::RenderWindow * wnd)
{
	sf::Text inst(_instructionText, Resources::GetFont("CNB"), 30);
	inst.setPosition(30.0f, 5.0f);
	inst.setColor(sf::Color::Green);
	wnd->draw(inst);
	DrawGridOutline(wnd);
	for (auto& c : _grid)
	{
		c->Draw(wnd);
	}
	
}

void Game::Input(sf::Event event)
{
	float x;
	float y;
	if (event.type == sf::Event::MouseButtonPressed)
	{
		x = event.mouseButton.x;
		y = event.mouseButton.y;
		for (auto& c : _grid)
		{
			if (c->GetBox().contains(x, y))
			{
				c->Click();
			}
		}		
	}
	
}

void Game::DrawGridOutline(sf::RenderWindow* wnd)
{
	for (auto& o : _outlineSides)
	{
		wnd->draw(o);
	}
}

void Game::SetUpGridOutline()
{
	float length = 16 * Constants::CELLSIZE + 10;
	float x = _firstCellX - 5;
	float y = _firstCellY - 5;
	sf::Vector2f size(length, 5);
	_outlineSides.emplace_back(size);
	_outlineSides.emplace_back(size);
	size = sf::Vector2f(5, length);
	_outlineSides.emplace_back(size);
	_outlineSides.emplace_back(size);
	_outlineSides[0].setPosition(x, y);
	_outlineSides[1].setPosition(x, _firstCellY + length - 10);
	_outlineSides[2].setPosition(x, y);
	_outlineSides[3].setPosition(_firstCellX + length - 10, y);
	for (auto& s : _outlineSides)
	{
		s.setFillColor(sf::Color::Black);
	}
}

Cell::Cell(int index, sf::Vector2f pos)
	:
	_index(index)
{
	sf::Vector2f size(Constants::CELLSIZE, Constants::CELLSIZE);
	_boundingBox = sf::Rect<float>(pos, size);
	_rectShape = sf::RectangleShape(size);
	_rectShape.setFillColor(sf::Color::Red);
	_rectShape.setPosition(pos);
}

Cell::~Cell()
{
}

void Cell::Update(float dt)
{
	if (_selected)
	{
		_rectShape.setFillColor(sf::Color::Blue);
	}
	else
	{
		_rectShape.setFillColor(sf::Color::White);
	}
}

void Cell::Draw(sf::RenderWindow * wnd)
{
	wnd->draw(_rectShape);
}
