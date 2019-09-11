#pragma once

extern "C"
{
	#include <ffmpeg/ffmpeg.h> // TODO: change this to only required headers
}

#include <SFML/Graphics.hpp> // TODO: change this to only required headers
#include <SFML/Audio/SoundStream.hpp>

namespace vp
{
	// TODO: add pause, stop, resume, find functionality?
	class Demuxer : public sf::SoundStream
	{
	public:
		Demuxer();
		~Demuxer();

		// sf::SoundStream overloads
		bool onGetData(Chunk& data) override;
		inline void onSeek(sf::Time timeOffset) override {};

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
		void createDecoderAndContext(AVStream** ppStream, AVCodecContext** ppCodecCtx);
		void createSwsContext();
		void createBuffer();
		void calculateFrameRate();
		void updateTexture();
		void step();

		AVFormatContext* m_pFormatCtx;
		AVCodecContext* m_pVideoCodecCtx;
		AVCodecContext* m_pAudioCodecCtx;
		AVStream* m_pVideoStream;
		AVStream* m_pAudioStream;
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