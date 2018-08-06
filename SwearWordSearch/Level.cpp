#include "Level.h"

Level::Level(GameMode mode, Difficulty diff, int levelNumber)
	:
	_mode(mode),
	_difficulty(diff),
	_levelNumber(levelNumber)
{
	std::cout << "Level " << _levelNumber << " created" << std::endl;
	switch (_difficulty)
	{
	case Difficulty::EASY:
		_bonusTime = 3;
		_wordValue = 10;
		break;
	case Difficulty::INTERMEDIATE:
		_bonusTime = 1;
		_wordValue = 12;
		break;
	case Difficulty::HARD:
		_bonusTime = 0;
		_wordValue = 15;
		break;
	default:
		_bonusTime = 3;
		_wordValue = 10;
	}
	//TODO: get level config from resources

	//temp to get it working
	_levelTime = 30;
	if (_levelNumber <= 30)
	{
		_numWordsToFind = _levelNumber;
	}
	else
	{
		_numWordsToFind = 30;
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
