#pragma once
#include <map>
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


private:
	Resources() {}
	~Resources() {}

	static sf::Texture LoadTexture(const std::string& filename);
	static sf::Font LoadFont(const std::string& filename);
	static std::vector<std::string> _words;
	static std::map<std::string, sf::Font> _fonts;
	static std::map<std::string, sf::Texture> _textures;
};
