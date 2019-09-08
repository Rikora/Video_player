#pragma once

#include <SFML/Graphics.hpp>
#include "Video.hpp"

#define WIDTH 640U
#define HEIGHT 360U

namespace vp
{
	class Application
	{
	public:
		Application();

		void run();

	private:
		void render();
		void pollEvents();
		void update(sf::Time dt);

		sf::RenderWindow m_window;
		sf::Sprite m_sprite;
		Video m_video;
	};
}