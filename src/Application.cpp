#include "Application.hpp"

#include <iostream>
#include <ffmpeg/ffmpegcpp.h>

using namespace ffmpegcpp;

namespace vp
{
	Application::Application() : 
	m_window(sf::VideoMode(WIDTH, HEIGHT), "Video Player", sf::Style::Close, sf::ContextSettings(0, 0, 8)),
	m_shape(100.f)
	{
		m_shape.setFillColor(sf::Color::Green);

		m_window.setVerticalSyncEnabled(true);
		loadVideo();
	}

	void Application::loadVideo()
	{
		// Load video
		auto demuxer = Demuxer("samples/big_buck_bunny.mp4");

		// TODO: pick the best video stream
		// TODO: create sf::texture with width and height from stream
		// Render sprite, add texture to sprite and update texture

		ContainerInfo info = demuxer.GetInfo();
	
		// Print the data similar to ffmpeg.exe.
		std::cout << "Input " << info.format->name << " from '" << demuxer.GetFileName() << "'" << std::endl;

		std::cout << "Video streams:" << std::endl;
		for (int i = 0; i < info.videoStreams.size(); ++i)
		{
			VideoStreamInfo stream = info.videoStreams[i];
			std::cout << "Stream #" << (i + 1)
				<< ": codec " << stream.codec->name
				<< ", pixel format " << stream.formatName
				<< ", resolution " << stream.width << "x" << stream.height
				<< ", bit rate " << stream.bitRate << "kb/s"
				<< ", fps " << ((float)stream.frameRate.num / (float)stream.frameRate.den)
				<< ", time base " << stream.timeBase.num << "/" << stream.timeBase.den
				<< ", " << demuxer.GetFrameCount(stream.id) << " frames"
				<< std::endl;
		}
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