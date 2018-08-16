#include "Level.h"

Level::Level(GameMode mode, Difficulty diff, int levelNumber)
	:
	_mode(mode),
	_difficulty(diff),
	_levelNumber(levelNumber)
{
	//DEBUG:
	std::cout << "Level " << _levelNumber << " created" << std::endl;
	
	
	//levelconfig: index 0 - time, index 1 - words
	std::vector<int> levelConfig = Resources::GetATCConfig()[_levelNumber - 1];

	switch (_difficulty)
	{
	case Difficulty::EASY:
		_bonusTime = 3;
		_wordValue = 1;
		_levelTime = levelConfig[0];
		_numWordsToFind = levelConfig[1];
		_numWordsInGrid = 20;
		_timeBonusFactor = 0.2f;
		break;
	case Difficulty::INTERMEDIATE:
		_bonusTime = 2;
		_wordValue = 5;		
		_levelTime = levelConfig[0];
		_numWordsToFind = levelConfig[1];
		_numWordsInGrid = _numWordsToFind + 10;
		_timeBonusFactor = 0.6f;
		break;
	case Difficulty::HARD:
		_bonusTime = 1;
		_wordValue = 15;
		_levelTime = levelConfig[0];
		_numWordsToFind = levelConfig[1];
		_numWordsInGrid = _numWordsToFind + 2;
		_timeBonusFactor = 1.0f;
		break;
	default:
		_bonusTime = 3;
		_wordValue = 5;
	}
}

Level::~Level()
{
	std::cout << "Level " << _levelNumber << " destroyed" << std::endl;
}

void Level::Update(float dt)
{
	//TODO: move some of the logic from gamestate into here
}
