#include "Resources.h"

std::map<std::string, sf::Font> Resources::_fonts;
std::map<std::string, sf::Texture> Resources::_textures;
std::vector<std::string> Resources::_words;

sf::Texture Resources::LoadTexture(const std::string& filename)
{
	sf::Texture tex;
	if (!tex.loadFromFile(filename))
	{
		std::cout << "Texture not loaded: " << filename << std::endl;
	}
	return tex;
}

void Resources::LoadImages()
{
	

}

void Resources::LoadFonts()
{
	_fonts.insert(std::make_pair("CNB", LoadFont("Resources/Fonts/CourierNewBold.ttf")));
}

void Resources::LoadWords()
{
	std::ifstream f("Resources/words.txt");
	if (!f)
	{
		std::cout << "Failed to load the list of words" << std::endl;
	}
	else
	{
		std::string s;
		while (std::getline(f, s))
		{
			_words.push_back(s);
		}
	}
}

const sf::Font & Resources::GetFont(const std::string & name)
{
	return _fonts.at(name);
}

const sf::Texture& Resources::GetTexture(const std::string & name)
{
	return _textures.at(name);
}

const std::string & Resources::GetWord(int index)
{
	return _words[index];
}

const size_t Resources::GetNumWords()
{
	return _words.size();
}

sf::Font Resources::LoadFont(const std::string& filename)
{
	sf::Font f;
	if (!f.loadFromFile(filename))
	{
		std::cout << "Font not loaded: " << filename << std::endl;
	}
	return f;
}