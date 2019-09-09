#pragma once

extern "C"
{
	#include <ffmpeg/ffmpeg.h>
}

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

	private:
		void createDecoderAndContext();
		void createSwsContext();

		AVFormatContext* m_pFormatCtx;
		AVCodecContext* m_pCodecCtx;
		AVCodec* m_pCodec;
		AVStream* m_pVideoStream;
		SwsContext* m_pSwsContext;
	};
}