#include "Application.hpp"

#include <iostream>
#include <ffmpeg/ffmpegcpp.h>

using namespace ffmpegcpp;

namespace vp
{
	Application::Application() : 
	m_window(sf::VideoMode(WIDTH, HEIGHT), "Video Player", sf::Style::Close, sf::ContextSettings(0, 0, 8)),
	m_img_convert_ctx(nullptr)
	{
		// Frame limit should be the same as the video frame limit!
		m_window.setVerticalSyncEnabled(true);

		loadVideo();

		// Apply texture
		m_sprite.setTexture(m_texture);
	}

	Application::~Application()
	{
		sws_freeContext(m_img_convert_ctx);
	}

	void Application::loadVideo()
	{
		// Load video
		m_demuxer = std::make_unique<Demuxer>("samples/big_buck_bunny.mp4");
		m_fileSink = std::make_unique<VideoFrame>();

		m_demuxer->DecodeBestVideoStream(m_fileSink.get());

		// Push a small amount of frames through the pipeline
		m_demuxer->PreparePipeline();

		// Create texture from info
		VideoStreamInfo stream = m_demuxer->GetInfo().videoStreams[0];
		m_texture.create(stream.width, stream.height);

		// Create sws context
		m_img_convert_ctx = sws_getContext(stream.width, stream.height, stream.format,
			stream.width, stream.height, AV_PIX_FMT_RGBA, SWS_FAST_BILINEAR, nullptr, nullptr, nullptr);
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

			if ((event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Right))
			{
				// TODO: check if we have not reached the last frame!

				// Go to the next frame
				m_demuxer->Step();
				// TODO: call avcodec_decode_video2? and sws_scale
			}
		}
	}

	void Application::update(sf::Time dt)
	{	
		m_texture.update(m_fileSink->getPixels().data());
	}

	void Application::render()
	{
		m_window.clear();

		m_window.draw(m_sprite);

		m_window.display();
	}
}