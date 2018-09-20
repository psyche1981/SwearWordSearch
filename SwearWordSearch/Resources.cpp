#include "Resources.h"

std::map<std::string, sf::Font> Resources::_fonts;
std::map<std::string, sf::Texture> Resources::_textures;
std::vector<std::string> Resources::_words;
std::mt19937 Random::_randomEngine;
std::vector<std::vector<int>> Resources::_atcConfig;
std::vector<std::vector<HighScore>> Resources::_atcHighScores;
char Resources::keys[26] = { 'a','b','c','d','e','f','g','h','i','j','k','l','m','n','o','p','q','r','s','t','u','v','w','x','y','z' };

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
			if (s.length() >= 3 && s.length() <= 16)
			{
				_words.push_back(s);
			}			
		}
		f.close();
	}
}

void Resources::LoadLevelConfigs()
{
	std::ifstream f("Resources/level_config.txt");
	if (!f)
	{
		std::cout << "Failed to load the level configurations" << std::endl;
	}
	else
	{
		std::string s;
		std::stringstream ss;
		int time;
		int numWords;
		std::vector<int> tempVec;
		while (!f.eof())
		{			
			std::getline(f, s);
			if (s == "## against_the_clock ##")
			{	
				while (s != "## end ##")
				{
					std::getline(f, s);
					if (s != "## end ##")
					{
						ss.str(s);
						ss >> time;
						ss >> numWords;
						tempVec.push_back(time);
						tempVec.push_back(numWords);
						_atcConfig.push_back(tempVec);
						tempVec.clear();
						ss.clear();
					}					
				}
			}
		}
		f.close();
	}
}

void Resources::LoadHighScores()
{
	std::ifstream f("Resources/high_scores.hsc");
	if (!f)
	{
		std::cout << "Failed to load the high scores file" << std::endl;
	}
	else
	{
		std::string s;
		std::stringstream ss;
		int mode;
		int diff;
		std::string name;
		int score;
		std::vector<HighScore> easyATC;
		std::vector<HighScore> interATC;
		std::vector<HighScore> hardATC;
		while (std::getline(f, s))
		{
			ss.str(s);
			ss >> mode;
			ss >> diff;
			ss >> name;
			ss >> score;
			HighScore hs(mode, diff, name, score);
			if (mode == 0)
			{
				switch (diff)
				{
				case 0:
					easyATC.push_back(hs);
					break;
				case 1:
					interATC.push_back(hs);
					break;
				case 2:
					hardATC.push_back(hs);
					break;
				}
			}
			ss.clear();
		}
		SortHighScores(easyATC);
		SortHighScores(interATC);
		SortHighScores(hardATC);
		_atcHighScores.push_back(easyATC);
		_atcHighScores.push_back(interATC);
		_atcHighScores.push_back(hardATC);
		f.close();
	}
}

const sf::Font& Resources::GetFont(const std::string & name)
{
	return _fonts.at(name);
}

const sf::Texture& Resources::GetTexture(const std::string & name)
{
	return _textures.at(name);
}

const std::string& Resources::GetWord(int index)
{
	return _words[index];
}

const size_t Resources::GetNumWords()
{
	return _words.size();
}

const std::vector<std::string>& Resources::GetWords(int n)
{
	static std::vector<std::string> words;
	words.clear();
	int num;
	std::vector<int> prevNums;
	if (n == 0)
	{
		return _words;
	}
	else
	{
		while ((int)words.size() < n)
		{
			num = Random::GetRandomInt(0, Resources::GetNumWords() - 1);
			if (!PreviouslySelected(prevNums, num))
			{
				prevNums.push_back(num);
				words.push_back(_words[num]);
			}			
		}
		return words;
	}
}

const std::vector<std::vector<int>>& Resources::GetATCConfig()
{
	return _atcConfig;
}

const std::vector<std::vector<HighScore>>& Resources::GetATCHighScores()
{
	return _atcHighScores;
}

void Resources::AddATCHighScore(HighScore hs)
{
	int d = hs.difficulty;
	_atcHighScores[d].push_back(hs);
	SortHighScores(_atcHighScores[d]);
}

void Resources::SaveHighScores()
{
	std::ofstream file;
	file.open("Resources/high_scores.hsc");
	for (size_t i = 0; i < _atcHighScores.size(); i++)
	{
		for (auto it = _atcHighScores[i].begin(); it != _atcHighScores[i].end(); ++it)
		{
			file << it->mode << " " << it->difficulty << " " << it->name << " " << it->score << "\n";
		}
	}
	file.close();
}

void Resources::SortHighScores(std::vector<HighScore>& hsVec)
{
	std::sort(hsVec.begin(), hsVec.end(),
		[](HighScore a, HighScore b) { return a.score > b.score; });
	while (hsVec.size() > 10)
	{
		hsVec.pop_back();
	}
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

bool Resources::PreviouslySelected(std::vector<int>& prev, int n)
{
	for (auto& p : prev)
	{
		if (n == p)
		{
			return true;
		}
	}
	return false;
}

void Random::Seed()
{
	_randomEngine = std::mt19937((unsigned int)time(nullptr));
}

int Random::GetRandomInt(int lowerBound, int upperBound)
{
	std::uniform_int_distribution<int> randInt(lowerBound, upperBound);

	return randInt(_randomEngine);
}

float Random::GetRandomFloat(float lowerBound, float upperBound)
{
	std::uniform_real_distribution<float> randFloat(lowerBound, upperBound);

	return randFloat(_randomEngine);
}

WordDirection Random::GetRandomWordDirection()
{
	return (WordDirection) GetRandomInt(0, 7);
}
