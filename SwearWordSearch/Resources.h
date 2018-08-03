#pragma once
#include <map>
#include <iostream>
#include <SFML/Graphics.hpp>

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
	static std::map<std::string, sf::Font> _fonts;
	static std::map<std::string, sf::Texture> _textures;

	static void LoadImages();
	static void LoadFonts();

	

	static const sf::Font& GetFont(const std::string& name);
	static const sf::Texture& GetTexture(const std::string& name);

private:
	Resources() {}
	~Resources() {}

	static sf::Texture LoadTexture(const std::string& filename);
	static sf::Font LoadFont(const std::string& filename);
};
