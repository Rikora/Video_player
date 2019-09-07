#pragma once

#include <SFML/Graphics.hpp>

#define WIDTH 800U
#define HEIGHT 600U

namespace vp
{
	class Application
	{
	public:
		Application();

		void run();

	private:
		void loadVideo();
		void render();
		void pollEvents();
		void update(sf::Time dt);

		sf::RenderWindow m_window;
		sf::CircleShape m_shape;
	};
}