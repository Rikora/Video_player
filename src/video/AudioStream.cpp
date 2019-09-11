#include "AudioStream.hpp"
#include <stdexcept>

namespace vp
{
	AudioStream::AudioStream() :
	m_pFormatCtx(NULL),
	m_pAudioStream(NULL),
	m_pAudioCodecCtx(NULL),
	m_pSamples(NULL),
	m_pData(NULL),
	m_dataSize(0),
	m_dataSizeMax(0)
	{
	}

	AudioStream::~AudioStream()
	{
		avcodec_close(m_pAudioCodecCtx);
		avformat_close_input(&m_pFormatCtx);
	}

	bool AudioStream::loadFromFile(const std::string& filename)
	{
		if (avformat_open_input(&m_pFormatCtx, filename.c_str(), NULL, NULL) != 0)
		{
			throw std::runtime_error("Demuxer::loadFromFile - Failed to load: " + filename);
		}

		if (avformat_find_stream_info(m_pFormatCtx, NULL) < 0)
		{
			throw std::runtime_error("Demuxer::loadFromFile - Failed to locate stream information");
		}

		// Print info
		av_dump_format(m_pFormatCtx, 0, filename.c_str(), 0);

		// Retrieve audio stream
		for (unsigned i = 0; i < m_pFormatCtx->nb_streams; ++i)
		{
			if (m_pFormatCtx->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_AUDIO)
			{
				m_pAudioStream = m_pFormatCtx->streams[i];
				createDecoderAndContext(&m_pAudioStream, &m_pAudioCodecCtx);
				initialize(m_pAudioStream->codecpar->channels, m_pAudioStream->codecpar->sample_rate);
			}
		}

		if (!m_pAudioStream)
		{
			throw std::runtime_error("Demuxer::loadFromFile - Failed to locate an existing audio stream");
		}

		return true;
	}

	void AudioStream::update(sf::Time dt)
	{
	}

	bool AudioStream::onGetData(Chunk& data)
	{
		return false;
	}

	void AudioStream::onSeek(sf::Time timeOffset)
	{
	}

	void AudioStream::createDecoderAndContext(AVStream** ppStream, AVCodecContext** ppCodecCtx)
	{
		auto pCodec = avcodec_find_decoder(ppStream[0]->codecpar->codec_id);
		*ppCodecCtx = avcodec_alloc_context3(pCodec);
		avcodec_parameters_to_context(*ppCodecCtx, ppStream[0]->codecpar);

		if (!pCodec)
		{
			throw std::runtime_error("Demuxer::createDecoderAndContext - Unsupported codec");
		}

		if (avcodec_open2(*ppCodecCtx, pCodec, NULL) < 0)
		{
			throw std::runtime_error("Demuxer::createDecoderAndContext - Failed to open codex context");
		}
	}
}