#pragma once

#include <SFML/Graphics.hpp>
#include "VideoFrame.hpp"

#define WIDTH 640U
#define HEIGHT 360U

namespace vp
{
	class Application
	{
	public:
		Application();
		~Application();

		void run();

	private:
		void loadVideo();
		void render();
		void pollEvents();
		void update(sf::Time dt);

		sf::RenderWindow m_window;
		sf::Texture m_texture;
		sf::Sprite m_sprite;
		std::unique_ptr<Demuxer> m_demuxer;
		std::unique_ptr<VideoFrame> m_fileSink;
		SwsContext* m_img_convert_ctx;
	};
}