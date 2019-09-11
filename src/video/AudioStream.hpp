#pragma once

extern "C"
{
	#include <ffmpeg/ffmpeg.h> // TODO: change this to only required headers
}

#include <SFML/Audio/SoundStream.hpp>
#include <string>

namespace vp
{
	class AudioStream final : public sf::SoundStream
	{
	public:
		AudioStream();
		~AudioStream();

		bool loadFromFile(const std::string& filename);
		void update(sf::Time dt);

	private:
		bool onGetData(Chunk& data) override;
		void onSeek(sf::Time timeOffset) override;
		void step(void* data, int& size);
		int getAVAudioData(void* data, int length);
		void createDecoderAndContext(AVStream** ppStream, AVCodecContext** ppCodecCtx);

		AVFormatContext* m_pFormatCtx;
		AVCodecContext* m_pAudioCodecCtx;
		AVStream* m_pAudioStream;
		AVFrame* m_pFrame;
		char* m_pSamples;
		char* m_pData;
		size_t m_dataSize;
		size_t m_dataSizeMax;
	};
}