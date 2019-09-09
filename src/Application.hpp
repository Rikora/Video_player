#pragma once

//#include <SFML/Graphics.hpp>
#include "video/Demuxer.hpp"

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
		std::unique_ptr<video::Demuxer> m_pDemuxer;
	};
}