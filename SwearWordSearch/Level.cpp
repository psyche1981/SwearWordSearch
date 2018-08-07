#include "Level.h"

Level::Level(GameMode mode, Difficulty diff, int levelNumber)
	:
	_mode(mode),
	_difficulty(diff),
	_levelNumber(levelNumber)
{
	//DEBUG:
	std::cout << "Level " << _levelNumber << " created" << std::endl;
	
	//TODO: get level config from resources
	//levelconfig: index 0 - time, index 1 - words
	std::vector<int> levelConfig{ 30, 1 };//temp initialisation

	switch (_difficulty)
	{
	case Difficulty::EASY:
		_bonusTime = 3;
		_wordValue = 10;
		//temp until config file loaded properly
		/*_levelTime = levelConfig[0];
		_numWordsToFind = levelConfig[1];*/
		break;
	case Difficulty::INTERMEDIATE:
		_bonusTime = 1;
		_wordValue = 12;
		//temp until config file loaded properly
		/*_levelTime = levelConfig[0];
		_numWordsToFind = levelConfig[1];*/
		break;
	case Difficulty::HARD:
		_bonusTime = 0;
		_wordValue = 15;
		//temp until config file loaded properly
		/*_levelTime = levelConfig[0];
		_numWordsToFind = levelConfig[1];*/
		break;
	default:
		_bonusTime = 3;
		_wordValue = 10;
	}


	//temp until config file loaded properly
	//_levelTime = levelConfig[0];
	//_numWordsToFind = levelConfig[1];
	_levelTime = 30;
	_numWordsToFind = _levelNumber;
	if (_numWordsToFind > 30)
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
