#include "GameState.h"

GameState::GameState(Difficulty diff)
	:
	_difficulty(diff),
	_levelNumber(1)
{
	_hintText = sf::Text("Hints", Resources::GetFont("CNB"), 30);
	_hintText.setPosition(610.0f, 80.0f);
	_hintText.setColor(sf::Color::Black);
	//create the grid with indiviual cells
	for (int i = 0; i < Constants::NUMCELLS; i++)
	{
		int row = i / 16;
		int col = i % 16;
		sf::Vector2f pos(_firstCellX + Constants::CELLSIZE * col, _firstCellY + Constants::CELLSIZE * row);
		_grid.emplace_back(std::make_unique<Cell>(i, pos));
	}

	CreateInstructionText();
	CreateScoreText();
	CreateQuitText();
	
	SetUpGridOutline();
	CreateLevel(_levelNumber);

}

GameState::~GameState()
{
	std::cout << "Game Destroyed" << std::endl;
}

void GameState::Update(float dt)
{		
	if (!_gameOver)
	{
		_remainingTime = _levelTime - (int)_timer.GetElapsed() + _bonusTime;
		_instructionString = "Find " + std::to_string(_numWordsToFind) + " words in " + std::to_string(_remainingTime) + " seconds";
		_instructionText.setString(_instructionString);
		_scoreString = "Score: " + std::to_string(_score);
		_scoreText.setString(_scoreString);

		if (_remainingTime <= 0)
		{
			_gameOver = true;
			_update = false;
			_timer.End();
			_bonusTime = 0;
			_remainingTime = 0;
		}
		if (_update)
		{
			for (auto& c : _grid)
			{
				c->Update(dt);
			}
		}
		if (_levelCompleted)
		{
			//temp need beeter way to show level completed
			_instructionString = "Level Completed. You Scored " + std::to_string(_score);
			_instructionText.setString(_instructionString);
			
			_update = false;
			_bonusTime = 0;
			_timer.End();		
		}
	}	
	else
	{
		_update = false;
		
		_nextState = MainStates::GAME_OVER;
		NotifyObservers();
	}
}

void GameState::Draw(sf::RenderWindow * wnd)
{	
	wnd->draw(_instructionText);
	DrawGridOutline(wnd);
	for (auto& c : _grid)
	{
		c->Draw(wnd);
	}
	wnd->draw(_hintText);
	for (auto& w : _wordSfTexts)
	{
		wnd->draw(w);
	}
	wnd->draw(_scoreText);
	wnd->draw(_quitText);
}

void GameState::Input(sf::Event event)
{
	int x;
	int y;
	if (event.type == sf::Event::MouseButtonPressed)
	{
		x = event.mouseButton.x;
		y = event.mouseButton.y;
		if (!_levelCompleted)
		{
			for (auto& c : _grid)
			{
				if (c->GetBox().contains((float)x, (float)y))
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
								//check the word indices against the selection indices
								for (size_t i = 0; i < _wordIndices.size(); i++)
								{
									if ((_selection.first == _wordIndices[i].first && _selection.second == _wordIndices[i].second)
										|| (_selection.first == _wordIndices[i].second && _selection.second == _wordIndices[i].first))
									{//found a word
										for (auto& c : _grid)
										{
											if (c->IsSelected())
											{
												c->Found();
												c->Deselect();
											}
										}
										_numCellsSelected = 0;
										_prevSelectedCellIndex = -1;
										_score += _level->GetWordValue();
										_bonusTime += _level->GetBonusTime();
										_numWordsToFind--;
										if (_numWordsToFind == 0)
										{											
											_levelCompleted = true;
											ApplyTimeRemaining();
											_levelNumber++;
										}
									}
								}
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
			if (_quitText.getGlobalBounds().contains(x, y))
			{
				_nextState = MainStates::MENU;
				NotifyObservers();
			}
		}
		else
		{
			//temp - Go to next level on mouse click
			CreateLevel(_levelNumber);
		}
		
	}
	else if (event.type == sf::Event::KeyPressed)
	{
		if (event.key.code == sf::Keyboard::Escape)
		{
			_nextState = MainStates::GAME_OVER;
			NotifyObservers();
		}
	}
}

void GameState::PopulateGrid(int numWords)
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
bool GameState::AddWordToGrid(const std::string& word)
{
	int numTries = 0;
	int startIndex;
	WordDirection wordDir;
	std::string theWord = RemoveHashes(word);
	while (numTries < 5)
	{
		startIndex = Random::GetRandomInt(0, Constants::NUMCELLS - 1);
		//only try empty starting cells
		if (_grid[startIndex]->GetLetter() == '-')
		{
			wordDir = Random::GetRandomWordDirection();
			std::vector<int> cellIndices = GetCellIndices(wordDir, startIndex, theWord.length());
			if (cellIndices.size() == theWord.length())//we have a place to put the word
			{
				if (IndicesAreValid(cellIndices, theWord))
				{
					for (size_t i = 0; i < theWord.length(); i++)
					{
						_grid[cellIndices[i]]->SetLetter(theWord[i]);
					}
					//add first and last index to the list of word indices
					_wordIndices.push_back(std::make_pair(cellIndices.front(), cellIndices.back()));
					std::string newWord = ReplaceHashWithSpace(word);
					//add the word into the word vector with spaces if necessary
					_words.push_back(newWord);
					return true;
				}
			}
			numTries++;
		}	
	}	
	return false;
}

//return the indices of the cells that the word will span, includes the start index
std::vector<int> GameState::GetCellIndices(WordDirection wordDir, int startIndex, size_t wordLength)
{
	std::vector<int> cellIndices;
	int startRow = startIndex / 16;
	int startCol = startIndex % 16;
	switch (wordDir)
	{
	case WordDirection::DOWN:
		if (startRow + wordLength <= 16)
		{
			for (int i = startIndex; i <= startIndex + ((int)wordLength - 1) * 16; i += 16)
			{
				cellIndices.push_back(i);
			}
		}
		break;
	case WordDirection::UP:
		if (startRow + 1 >= (int)wordLength)
		{
			int j = startIndex - ((int)wordLength - 1) * 16;
			for (int i = startIndex; i >= j; i -= 16)
			{
				cellIndices.push_back(i);
			}
		}
		break;
	case WordDirection::LEFT:
		if (startCol + 1 >= (int)wordLength)
		{
			int j = startIndex;
			for (size_t i = 0; i < wordLength; i++)
			{
				cellIndices.push_back(j--);
			}
		}
		break;
	case WordDirection::RIGHT:
		if ((int)wordLength <= 16 - startCol)
		{
			for (size_t i = startIndex; i < startIndex + wordLength;)
			{
				cellIndices.push_back(i++);
			}
		}
		break;
	case WordDirection::NE:
		if ((int)wordLength <= 16 - startCol && startRow + 1 >= (int)wordLength)
		{
			int j = startIndex - (wordLength - 1) * 15;
			for (int i = startIndex; i >= j; i -= 15)
			{
				cellIndices.push_back(i);
			}
		}
		break;
	case WordDirection::NW:
		if (startCol + 1 >= (int)wordLength && startRow + 1 >= (int)wordLength)
		{
			int j = startIndex - (wordLength - 1) * 17;
			for (int i = startIndex; i >= j; i -= 17)
			{
				cellIndices.push_back(i);
			}
		}
		break;
	case WordDirection::SW:
		if (startCol + 1 >= (int)wordLength && startRow + (int)wordLength <= 16)
		{
			int j = startIndex + (wordLength - 1) * 15;
			for (int i = startIndex; i <= j; i += 15)
			{
				cellIndices.push_back(i);
			}
		}
		break;
	case WordDirection::SE:
		if ((int)wordLength <= 16 - startCol && startRow + wordLength <= 16)
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

bool GameState::IndicesAreValid(const std::vector<int>& cellIndices, const std::string& word)
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

//removes hash characters from word
std::string GameState::RemoveHashes(const std::string& word)
{
	std::string theWord = word;
	size_t pos = theWord.find('#');
	if (pos != std::string::npos)
	{
		theWord.erase(pos, 1);
	}
	return theWord;
}

std::string GameState::ReplaceHashWithSpace(const std::string& word)
{
	std::string newWord = word;
	std::replace(newWord.begin(), newWord.end(), '#', ' ');
	return newWord;
}

void GameState::ApplyTimeRemaining()
{
	_score += _remainingTime * _level->GetTimeBonusFactor();
}

void GameState::ResetLevel()
{
	ResetGrid();
	_numCellsSelected = 0;
	_prevSelectedCellIndex = -1;
	_wordIndices.clear();
	_words.clear();
	_wordSfTexts.clear();
}

void GameState::ResetGrid()
{
	for (auto& c : _grid)
	{
		c->Reset();
	}
}

void GameState::CreateLevel(int levelNumber)
{	
	if (levelNumber != 1)
	{
		ResetLevel();
	}
	
	_level = std::make_unique<Level>(GameMode::AGAINST_THE_CLOCK, _difficulty, levelNumber);
	PopulateGrid(_level->GetNumWordsInGrid());	
	_numWordsToFind = _level->GetNumWordsToFind();
	_levelTime = _level->GetLevelTime();
	_remainingTime = _levelTime;
	_update = true;
	_levelCompleted = false;
	_timer.Begin();
	ShowHints();
}

void GameState::CreateInstructionText()
{
	_instructionString = "Find " + std::to_string(_numWordsToFind) + " words in " + std::to_string(_levelTime) + " seconds";
	_instructionText = sf::Text(_instructionString, Resources::GetFont("CNB"), 30);
	_instructionText.setPosition(30.0f, 5.0f);
	_instructionText.setColor(sf::Color::Green);
}

void GameState::CreateScoreText()
{
	_scoreString = "Score: " + std::to_string(_score);
	_scoreText = sf::Text(_scoreString, Resources::GetFont("CNB"), 30);
	_scoreText.setPosition(590.0f, 500.0f);
	_scoreText.setColor(sf::Color::Black);
}

void GameState::CreateQuitText()
{
	_quitText = sf::Text("QUIT", Resources::GetFont("CNB"), 30);
	_quitText.setPosition(690.0f, 550.0f);
	_quitText.setColor(sf::Color::Black);
}

void GameState::ShowHints()
{	
	if (_difficulty == Difficulty::EASY)
	{		
		//show all words in grid
		for (size_t i = 0; i < _words.size() / 2; i++)
		{
			_wordSfTexts.emplace_back(_words[i], Resources::GetFont("CNB"), 20);
			_wordSfTexts[i].setPosition(600.0f, 120.0f + i * 20);
			_wordSfTexts[i].setColor(sf::Color::Black);
		}
	}
	else if (_difficulty == Difficulty::INTERMEDIATE)
	{
		//small amount of words shown		
		for (int i = 0; i < _words.size() / 5; i++)
		{
			_wordSfTexts.emplace_back(_words[i], Resources::GetFont("CNB"), 20);
			_wordSfTexts[i].setPosition(600.0f, 110.0f + i * 20);
			_wordSfTexts[i].setColor(sf::Color::Black);
		}
	}
	else
	{
		//no words shown on hard
		_wordSfTexts.emplace_back("No hints on \nhard mode", Resources::GetFont("CNB"), 20);
		_wordSfTexts[0].setPosition(600.0f, 120.0f);
		_wordSfTexts[0].setColor(sf::Color::Black);
	}
}

void GameState::DrawGridOutline(sf::RenderWindow* wnd)
{
	for (auto& o : _outlineSides)
	{
		wnd->draw(o);
	}
}

void GameState::SetUpGridOutline()
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

bool GameState::GuessCandidate(int index1, int index2)
{
	int row1 = index1 / 16;
	int row2 = index2 / 16;
	int col1 = index1 % 16;
	int col2 = index2 % 16;

	return (std::abs(row1 - row2) == std::abs(col1 - col2)) || (row1 == row2) || (col1 == col2);
}

void GameState::InterpolateAndSelect(int index1, int index2)
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


