#pragma once
#include <map>
#include <random>
#include <ctime>
#include <iostream>
#include <SFML/Graphics.hpp>
#include <fstream>
#include <string>


namespace Constants
{
	static constexpr int NUMCELLS = 16 * 16;
	static constexpr int SCREENWIDTH = 800;
	static constexpr int SCREENHEIGHT = 600;
	static constexpr float CELLSIZE = 30.0f;
}

enum States
{
	SPLASH,
	TITLE,
	GAME,
	GAMEOVER,
	PAUSE,
	MENU
};

class Resources
{
public:
	static void LoadImages();
	static void LoadFonts();
	static void LoadWords();

	

	static const sf::Font& GetFont(const std::string& name);
	static const sf::Texture& GetTexture(const std::string& name);
	static const std::string& GetWord(int index);
	static const size_t GetNumWords();
	static const std::vector<std::string>& GetWords(int n = 0);


private:
	Resources() {}
	~Resources() {}

	static sf::Texture LoadTexture(const std::string& filename);
	static sf::Font LoadFont(const std::string& filename);
	static std::vector<std::string> _words;
	static std::map<std::string, sf::Font> _fonts;
	static std::map<std::string, sf::Texture> _textures;
};

class Random
{
public:
	static void Seed();

	static int GetRandomInt(int lowerBound, int upperBound);
	static float GetRandomFloat(float lowerBound, float upperBound);

private:
	static std::mt19937 _randomEngine;
};
