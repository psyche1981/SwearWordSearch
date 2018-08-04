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
	PopulateGrid(30);		
}

Game::~Game()
{
	
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

void Game::PopulateGrid(int numWords)
{
	int numWordsAdded = 0;
	//increase 50 random words if necessary
	std::vector<std::string> words = Resources::GetWords(50);
	while (numWordsAdded < numWords)
	{		
		for (auto it = words.cbegin(); it != words.cend();)
		{			
			if (AddWordToGrid(*it))
			{
				it = words.erase(it);
				numWordsAdded++;
				if (numWordsAdded == numWords)
				{
					break;
				}
			}
			else
			{
				++it;
			}
		}
	}
	//populate empty cells with random letter
	for (auto& c : _grid)
	{
		if (c->GetLetter() == '-')
		{
			c->SetLetter(Random::GetRandomInt(97, 122));
		}
	}
}

//try the word starting in three different cells in three random directions
//if doesnt fit - return false
bool Game::AddWordToGrid(const std::string& word)
{
	int numTries = 0;
	int startIndex;
	WordDirection wordDir;
	while (numTries < 5)
	{
		startIndex = Random::GetRandomInt(0, Constants::NUMCELLS - 1);
		//only try empty starting cells
		if (_grid[startIndex]->GetLetter() == '-')
		{
			wordDir = Random::GetRandomWordDirection();
			std::vector<int> cellIndices = GetCellIndices(wordDir, startIndex, word.length());
			if (cellIndices.size() == word.length())//we have a place to put the word
			{
				if (IndicesAreValid(cellIndices, word))
				{
					for (size_t i = 0; i < word.length(); i++)
					{
						_grid[cellIndices[i]]->SetLetter(word[i]);
					}
					//add first and last index to the list of word indices
					_wordIndices.push_back(std::make_pair(cellIndices.front(), cellIndices.back()));
					_wordIndices.push_back(std::make_pair(cellIndices.back(), cellIndices.front()));
					return true;
				}
			}
			numTries++;
		}	
	}	
	return false;
}


//return the indices of the cells that the word will span, includes the start index
std::vector<int> Game::GetCellIndices(WordDirection wordDir, int startIndex, size_t wordLength)
{
	std::vector<int> cellIndices;
	int startRow = startIndex / 16;
	int startCol = startIndex % 16;
	switch (wordDir)
	{
	case WordDirection::DOWN:
		if (startRow + wordLength <= 16)
		{
			for (int i = startIndex; i <= startIndex + (wordLength - 1) * 16; i += 16)
			{
				cellIndices.push_back(i);
			}
		}
		break;
	case WordDirection::UP:
		if (startRow + 1 >= wordLength)
		{
			int j = startIndex - ((int)wordLength - 1) * 16;
			for (int i = startIndex; i >= j; i -= 16)
			{
				cellIndices.push_back(i);
			}
		}
		break;
	case WordDirection::LEFT:
		if (startCol + 1 >= wordLength)
		{
			int j = startIndex;
			for (size_t i = 0; i < wordLength; i++)
			{
				cellIndices.push_back(j--);
			}
		}
		break;
	case WordDirection::RIGHT:
		if (wordLength <= 16 - startCol)
		{
			for (size_t i = startIndex; i < startIndex + wordLength;)
			{
				cellIndices.push_back(i++);
			}
		}
		break;
	case WordDirection::NE:
		if (wordLength <= 16 - startCol && startRow + 1 >= wordLength)
		{
			int j = startIndex - (wordLength - 1) * 15;
			for (int i = startIndex; i >= j; i -= 15)
			{
				cellIndices.push_back(i);
			}
		}
		break;
	case WordDirection::NW:
		if (startCol + 1 >= wordLength && startRow + 1 >= wordLength)
		{
			int j = startIndex - (wordLength - 1) * 17;
			for (int i = startIndex; i >= j; i -= 17)
			{
				cellIndices.push_back(i);
			}
		}
		break;
	case WordDirection::SW:
		if (startCol + 1 >= wordLength && startRow + wordLength <= 16)
		{
			int j = startIndex + (wordLength - 1) * 15;
			for (int i = startIndex; i <= j; i += 15)
			{
				cellIndices.push_back(i);
			}
		}
		break;
	case WordDirection::SE:
		if (wordLength <= 16 - startCol && startRow + wordLength <= 16)
		{
			int j = startIndex + (wordLength - 1) * 17;
			for (int i = startIndex; i <= j; i += 17)
			{
				cellIndices.push_back(i);
			}
		}
		break;
	}

	return cellIndices;
}

bool Game::IndicesAreValid(const std::vector<int>& cellIndices, const std::string& word)
{
	auto wordIt = word.cbegin();
	for (auto it = cellIndices.cbegin(); it != cellIndices.cend(); ++it, ++wordIt)
	{
		char letter = _grid[*it]->GetLetter();
		if (letter == '-')//"empty cell"
		{
			continue;
		}
		else
		{
			if (letter != *wordIt)
			{
				return false;
			}
		}
	}
	return true;
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

void Cell::SetLetter(char letter)
{
	_letter = letter;
	_letterText.setString(_letter);
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
