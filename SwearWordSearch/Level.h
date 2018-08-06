#pragma once
#include <iostream>

#include "Resources.h"


class Level
{
public:
	Level(GameMode mode, Difficulty diff, int levelNumber);
	~Level();

	void Update(float dt);
	int GetLevelTime() { return _levelTime; }
	int GetNumWordsToFind() { return _numWordsToFind; }
	int GetBonusTime() { return _bonusTime; }
	int GetWordValue() { return _wordValue; }

private:
	int _levelNumber;
	int _levelTime;
	int _numWordsToFind;
	int _bonusTime;
	int _wordValue;

	GameMode _mode;
	Difficulty _difficulty;
};