#include "Application.hpp"

namespace vp
{
	Application::Application() : 
	m_window(sf::VideoMode(WIDTH, HEIGHT), "Video Player", sf::Style::Close, sf::ContextSettings(0, 0, 8)),
	m_video("samples/big_buck_bunny.mp4")
	{
		// Frame limit should be the same as the video frame limit!
		m_window.setVerticalSyncEnabled(true);

		// Apply texture
		m_sprite.setTexture(m_video);
	}

	void Application::run()
	{
		sf::Clock clock;

		while (m_window.isOpen())
		{
			pollEvents();
			update(clock.restart());
			render();
		}
	}

	void Application::pollEvents()
	{
		sf::Event event;

		while (m_window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed || (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape))
			{
				m_window.close();
			}
		}
	}

	void Application::update(sf::Time dt)
	{	
		m_video.Update(dt.asSeconds());
	}

	void Application::render()
	{
		m_window.clear();

		m_window.draw(m_sprite);

		m_window.display();
	}
}