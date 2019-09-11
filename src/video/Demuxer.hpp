#pragma once

extern "C"
{
	#include <ffmpeg/ffmpeg.h>
}

#include <SFML/Graphics.hpp>

namespace vp
{
	class Demuxer
	{
	public:
		Demuxer();
		~Demuxer();

		bool loadFromFile(const std::string& filename);
		void update(sf::Time dt);
		bool isFrameFinished() const;
		bool isVideoFinished() const;
		float getFrameRate() const;
		int getWidth() const;
		int getHeight() const;
		sf::Uint8* getBuffer() const;
		sf::Texture& getTexture();

	private:
		void createDecoderAndContext();
		void createSwsContext();
		void createBuffer();
		void calculateFrameRate();
		void updateTexture();
		void step();

		AVFormatContext* m_pFormatCtx;
		AVCodecContext* m_pVideoCodecCtx;
		AVStream* m_pVideoStream;
		AVFrame* m_pPrevFrame;
		AVFrame* m_pCurrentFrame;
		SwsContext* m_pSwsContext;
		sf::Uint8* m_pBuffer;
		sf::Texture m_texture;
		sf::Time m_updateTimer;
		float m_frameRate;
		bool m_frameFinished;
		bool m_videoFinished;
	};
}