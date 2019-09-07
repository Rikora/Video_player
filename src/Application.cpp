#include "Application.hpp"

#include <iostream>
#include <ffmpeg/ffmpegcpp.h>

using namespace ffmpegcpp;

namespace vp
{
	Application::Application() : 
	m_window(sf::VideoMode(WIDTH, HEIGHT), "Video Player", sf::Style::Close, sf::ContextSettings(0, 0, 8))
	{
		m_window.setVerticalSyncEnabled(true);

		// Create texture and fill with red color
		m_texture.create(100, 100);
		m_pixels.resize(100 * 100 * 4); // Must be RGBA (*4)

		for (size_t i = 0; i < m_pixels.size(); i += 4)
		{
			m_pixels[i] = 255;
			m_pixels[i+1] = 0;
			m_pixels[i+2] = 0;
			m_pixels[i+3] = 255;
		}

		m_sprite.setTexture(m_texture);
		m_sprite.setPosition(sf::Vector2f(200.f, 100.f));

		//loadVideo();
	}

	void Application::loadVideo()
	{
		// TODO: pick the best video stream
		// TODO: create sf::texture with width and height from stream
		// Render sprite, add texture to sprite and update texture

		// Load video
		auto demuxer = Demuxer("samples/big_buck_bunny.mp4");

		ContainerInfo info = demuxer.GetInfo();
		VideoStreamInfo stream = info.videoStreams[0];
		
		// Prepare info
		/*m_pFrameRGB = av_frame_alloc();
		int numBytes = av_image_get_buffer_size(AV_PIX_FMT_RGBA, stream.width, stream.height, 1);
		m_pBuffer = (sf::Uint8*)av_malloc(numBytes * sizeof(sf::Uint8));

		avpicture_fill((AVPicture*)m_pFrameRGB, m_pBuffer, AV_PIX_FMT_RGBA, stream.width, stream.height);*/

		// 1. Create the texture
		m_texture.create(stream.width, stream.height);

		// 2. Update the image texture with content

		// Need to decode the video to get info for the pixel data?

		// Next frame...
		/*demuxer.PreparePipeline();
		demuxer.Step();*/
	
		// Print the data similar to ffmpeg.exe.
		/*std::cout << "Input " << info.format->name << " from '" << demuxer.GetFileName() << "'" << std::endl;

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
		}*/
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
		m_texture.update(m_pixels.data());
	}

	void Application::render()
	{
		m_window.clear();

		m_window.draw(m_sprite);

		m_window.display();
	}
}