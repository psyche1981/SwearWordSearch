#include "Game.h"

Game::Game(StateManager* sm)
	:
	State(sm)
{
	_numWordsToFind = 4;
	_countdownTimer = 30;
	//create the grid with indiviual cells
	for (int i = 0; i < Constants::NUMCELLS; i++)
	{
		int row = i / 16;
		int col = i % 16;
		sf::Vector2f pos(_firstCellX + Constants::CELLSIZE * col, _firstCellY + Constants::CELLSIZE * row);
		_grid.emplace_back(std::make_unique<Cell>(i, pos));
	}
	
	SetUpGridOutline();
	//temp to put a word on the grid
	std::string word = Resources::GetWord(Random::GetRandomInt(0, Resources::GetNumWords() - 1));
	for (size_t i = 0; i < word.length(); i++)
	{
		_grid[i]->SetLetter(word[i]);
	}
	std::cout << word << std::endl << std::endl;
	//temp to try the get random words functionality
	/*std::vector<std::string> words = Resources::GetWords(5);
	for (auto& w : words)
	{
		std::cout << w << std::endl;
	}*/
}

Game::~Game()
{
	
}

void Game::Update(float dt)
{
	//should just do this once, not every frame, 
	//but need to do it after construction until the staemanager is implemented
	//PopulateGrid();
	
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
				_numCellsSelected++;
				if (_numCellsSelected == 1)
				{
					_prevSelectedCellIndex = c->GetIndex();
				}
				else if (_numCellsSelected == 2)
				{
					int i2 = c->GetIndex();
					int i1 = _prevSelectedCellIndex;
					if (i1 == i2)
					{
						_numCellsSelected = 0;
					}
					else
					{
						if (GuessCandidate(i1, i2))
						{
							InterpolateAndSelect(i1, i2);
							_selection = std::make_pair(i1, i2);
							//TODO check if selection is in list of solution pairs
							std::cout << _selection.first << " , " << _selection.second << std::endl;
						}
						else
						{
							_grid[i1]->Deselect();
							_grid[i2]->Deselect();
							_numCellsSelected = 0;
							_prevSelectedCellIndex = -1;
						}
					}					
				}
				else
				{
					for (auto& c : _grid)
					{
						c->Deselect();
					}
					_numCellsSelected = 0;
					_prevSelectedCellIndex = -1;
				}
			}
		}		
	}
	
}

void Game::PopulateGrid()
{
	
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

bool Game::GuessCandidate(int index1, int index2)
{
	int row1 = index1 / 16;
	int row2 = index2 / 16;
	int col1 = index1 % 16;
	int col2 = index2 % 16;

	return (std::abs(row1 - row2) == std::abs(col1 - col2)) || (row1 == row2) || (col1 == col2);
}

void Game::InterpolateAndSelect(int index1, int index2)
{
	int row1 = index1 / 16;
	int row2 = index2 / 16;
	int col1 = index1 % 16;
	int col2 = index2 % 16;

	if (row1 == row2)
	{
		if (index1 < index2)
		{			
			for (int num = index1 + 1; num < index2; num++)
			{
				_grid[num]->Select();
			}
		}
		else
		{
			for (int num = index2 + 1; num < index1; num++)
			{
				_grid[num]->Select();
			}
		}
	}
	else if (col1 == col2)
	{
		if (index1 < index2)
		{
			for (int num = index1 + 16; num < index2; num += 16)
			{
				_grid[num]->Select();
			}
		}
		else
		{
			for (int num = index2 + 16; num < index1; num += 16)
			{
				_grid[num]->Select();
			}
		}
	}
	else //diagonal
	{
		if (row1 < row2)
		{
			if (col1 < col2)
			{		
				for (int num = index1 + 17; num < index2; num += 17)
				{
					_grid[num]->Select();
				}				
			}
			else
			{
				for (int num = index1 + 15; num < index2; num += 15)
				{
					_grid[num]->Select();
				}
			}			
		}
		else
		{
			if (col1 < col2)
			{
				for (int num = index1 - 15; num > index2; num -= 15)
				{
					_grid[num]->Select();
				}
			}
			else
			{
				for (int num = index1 - 17; num > index2; num -= 17)
				{
					_grid[num]->Select();
				}
			}
		}
	}
}

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
	wnd->draw(_letterText);
}

void Cell::CreateLetter()
{	
	_letterText = sf::Text(_letter, Resources::GetFont("CNB"), 20);
	float newX = _position.x + _letterText.getGlobalBounds().width / 2;
	float newY = _position.y; +_letterText.getGlobalBounds().height / 2;
	sf::Vector2f newPos(newX, newY);
	_letterText.setPosition(newPos);
	_letterText.setFillColor(sf::Color::Black);
}
