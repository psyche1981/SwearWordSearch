#include <iostream>
#include <chrono>
#include <SFML\Graphics.hpp>

#include "StateManager.h"
#include "Resources.h"

class Window
{
public:
	Window(int width, int height, const std::string& title)
		:
		_window(sf::VideoMode(width, height), title),
		_sm()
	{
		Resources::LoadFonts();
		Resources::LoadImages();
	}

	virtual ~Window() {}

	void Loop()
	{
		auto tp = std::chrono::steady_clock::now();
		sf::Event event;

		while (_window.isOpen())
		{

			while (_window.pollEvent(event))
			{
				if (event.type == sf::Event::Closed)
					_window.close();

				_sm.Input(event);
			}

			float dt;
			{
				const auto new_tp = std::chrono::steady_clock::now();
				dt = std::chrono::duration<float>(new_tp - tp).count();
				tp = new_tp;
			}

			_window.clear(sf::Color::White);

			_sm.Update(dt);

			_sm.Draw(&_window);

			_window.display();
		}
	}

private:
	sf::RenderWindow _window;
	StateManager  _sm;
};


int main()
{
	Window wnd(Constants::SCREENWIDTH, Constants::SCREENHEIGHT, "Swear-WordSearch");
	wnd.Loop();
	//std::cin.get();
	return 0;
}