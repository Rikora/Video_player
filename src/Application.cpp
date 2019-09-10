#include "Application.hpp"
#include <iostream>

namespace vp
{
	Application::Application() : 
	m_demuxer(video::Demuxer())
	{
		// TODO: need to overload sf::soundstream and play audio from there!
		try
		{
			m_demuxer.loadFromFile("samples/big_buck_bunny.mp4");
		}
		catch (std::exception& e)
		{
			std::cout << e.what() << std::endl;
			system("pause");
			exit(EXIT_FAILURE);
		}

		m_window.create(sf::VideoMode(m_demuxer.getWidth(), m_demuxer.getHeight()), 
			"Video Player", sf::Style::Close);
		m_window.setVerticalSyncEnabled(true);

		m_sprite.setTexture(m_demuxer.getTexture());
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
			if (event.type == sf::Event::Closed || 
			   (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape))
			{
				m_window.close();
			}
		}
	}

	void Application::update(sf::Time dt)
	{	
		m_demuxer.update(dt);
	}

	void Application::render()
	{
		m_window.clear();

		m_window.draw(m_sprite);

		m_window.display();
	}
}