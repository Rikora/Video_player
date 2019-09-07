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
		sf::Texture m_texture;
		sf::Sprite m_sprite;
		std::vector<sf::Uint8> m_pixels;
	};
}