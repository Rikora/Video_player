#include "Application.hpp"

namespace vp
{
	Application::Application() : 
	m_window(sf::VideoMode(WIDTH, HEIGHT), "Video Player", sf::Style::Close, sf::ContextSettings(0, 0, 8)),
	m_shape(100.f)
	{
		m_shape.setFillColor(sf::Color::Green);

		m_window.setVerticalSyncEnabled(true);
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
		// Empty by default
	}

	void Application::render()
	{
		m_window.clear();

		m_window.draw(m_shape);

		m_window.display();
	}	
}