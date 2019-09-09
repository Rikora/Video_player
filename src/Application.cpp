#include "Application.hpp"

namespace vp
{
	Application::Application() : 
	m_pDemuxer(std::make_unique<video::Demuxer>())
	{
		m_pDemuxer->loadFromFile("samples/big_buck_bunny.mp4");

		m_window.create(sf::VideoMode(m_pDemuxer->getWidth(), m_pDemuxer->getHeight()), "Video Player", sf::Style::Close, sf::ContextSettings(0, 0, 8));
		m_window.setVerticalSyncEnabled(true);

		m_sprite.setTexture(m_pDemuxer->getTexture());
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
		m_pDemuxer->update(dt);
	}

	void Application::render()
	{
		m_window.clear();

		m_window.draw(m_sprite);

		m_window.display();
	}
}