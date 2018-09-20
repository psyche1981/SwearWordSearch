#pragma once
#include <map>
#include <random>
#include <ctime>
#include <iostream>
#include <sstream>
#include <SFML/Graphics.hpp>
#include <fstream>
#include <string>


namespace Constants
{
	static constexpr int NUMCELLS = 16 * 16;
	static constexpr int SCREENWIDTH = 800;
	static constexpr int SCREENHEIGHT = 600;
	static constexpr float CELLSIZE = 30.0f;
	static constexpr int BONUSTIME = 2;
}

enum MainStates
{
	INITIAL_SPLASH,
	MENU,
	TUTORIAL,
	GAME,
	GAME_OVER,
	PAUSE,

	MAINSTATES_DEFAULT
};

enum GameMode
{
	AGAINST_THE_CLOCK
};


enum Difficulty
{
	EASY,
	INTERMEDIATE,
	HARD
};

enum WordDirection
{
	LEFT,
	RIGHT,
	UP,
	DOWN,
	NE,
	NW, 
	SE,
	SW
};

class Resources
{
public:
	static void LoadImages();
	static void LoadFonts();
	static void LoadWords();
	static void LoadLevelConfigs();

	

	static const sf::Font& GetFont(const std::string& name);
	static const sf::Texture& GetTexture(const std::string& name);
	static const std::string& GetWord(int index);
	static const size_t GetNumWords();
	static const std::vector<std::string>& GetWords(int n = 0);
	static const std::vector<std::vector<int>>& GetATCConfig();
	static char keys[26]; 


private:
	Resources() {}
	~Resources() {}

	static sf::Texture LoadTexture(const std::string& filename);
	static sf::Font LoadFont(const std::string& filename);
	static bool PreviouslySelected(std::vector<int>& prev, int n);

	static std::vector<std::string> _words;
	static std::map<std::string, sf::Font> _fonts;
	static std::map<std::string, sf::Texture> _textures;
	//TODO: add the storage container for the level configs for each mode
	static std::vector<std::vector<int>> _atcConfig;
};

class Random
{
public:
	static void Seed();

	static int GetRandomInt(int lowerBound, int upperBound);
	static float GetRandomFloat(float lowerBound, float upperBound);
	static WordDirection GetRandomWordDirection();

private:
	static std::mt19937 _randomEngine;
};
