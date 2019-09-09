#pragma once

extern "C"
{
	#include <ffmpeg/ffmpeg.h>
}

#include <SFML/Config.hpp>
#include <string>

namespace vp
{
	class Demuxer
	{
	public:
		Demuxer();
		~Demuxer();

		bool loadFromFile(const std::string& file);
		float getFrameRate();
		int getWidth() const;
		int getHeight() const;
		sf::Uint8* getBuffer() const;

	private:
		void createDecoderAndContext();
		void createSwsContext();
		void createBuffer();

		AVFormatContext* m_pFormatCtx;
		AVCodecContext* m_pCodecCtx;
		AVCodec* m_pCodec;
		AVStream* m_pVideoStream;
		AVFrame* m_pPrevFrame;
		AVFrame* m_pCurrentFrame;
		SwsContext* m_pSwsContext;
		sf::Uint8* m_pBuffer;
	};
}