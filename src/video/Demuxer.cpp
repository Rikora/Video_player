#include "Demuxer.hpp"
#include <iostream>

namespace vp
{
	Demuxer::Demuxer() :
	m_pFormatCtx(NULL),
	m_pVideoStream(NULL),
	m_pCodec(NULL),
	m_pCodecCtx(NULL),
	m_pSwsContext(NULL)
	{
	}

	Demuxer::~Demuxer()
	{
		avcodec_close(m_pCodecCtx);
		avformat_close_input(&m_pFormatCtx);
		sws_freeContext(m_pSwsContext);
	}

	bool Demuxer::loadFromFile(const std::string& file)
	{
		// TODO: replace if statements with exception handling instead?
		if (avformat_open_input(&m_pFormatCtx, file.c_str(), NULL, NULL) != 0)
		{
			std::cout << "ERROR: Unexisting file!" << std::endl;;
			return false;
		}

		if (avformat_find_stream_info(m_pFormatCtx, NULL) < 0)
		{
			std::cout << "ERROR: Couldn't find stream information!" << std::endl;
			return false;
		}

		// Print info
		av_dump_format(m_pFormatCtx, 0, file.c_str(), 0);

		// Retrieve video stream
		for (unsigned i = 0; i < m_pFormatCtx->nb_streams; ++i)
		{
			if (m_pFormatCtx->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_VIDEO)
			{
				m_pVideoStream = m_pFormatCtx->streams[i];
				break;
			}
		}

		if (m_pVideoStream == NULL)
		{
			return false;
		}

		createDecoderAndContext();

		if (m_pCodec == NULL)
		{
			std::cout << "ERROR: Unsupported codec!" << std::endl;
			return false;
		}

		if (avcodec_open2(m_pCodecCtx, m_pCodec, NULL) < 0)
		{
			std::cout << "ERROR: Could not open codec context!" << std::endl;
			return false;
		}

		createSwsContext();

		if (m_pSwsContext == NULL)
		{
			std::cout << "ERROR: Could not create sws context!" << std::endl;
			return false;
		}

		return true;
	}

	float Demuxer::getFrameRate()
	{
		return 0.f;
	}

	int Demuxer::getWidth() const
	{
		return m_pCodecCtx->width;
	}

	int Demuxer::getHeight() const
	{
		return m_pCodecCtx->height;
	}

	void Demuxer::createDecoderAndContext()
	{
		m_pCodec = avcodec_find_decoder(m_pVideoStream->codecpar->codec_id);
		m_pCodecCtx = avcodec_alloc_context3(m_pCodec);
		avcodec_parameters_to_context(m_pCodecCtx, m_pVideoStream->codecpar);
	}

	void Demuxer::createSwsContext()
	{
		m_pSwsContext = sws_getContext(
				m_pCodecCtx->width, 
				m_pCodecCtx->height,
				m_pCodecCtx->pix_fmt,
				m_pCodecCtx->width, 
				m_pCodecCtx->height,
				AV_PIX_FMT_RGBA,
				SWS_FAST_BILINEAR,
				NULL, 
				NULL, 
				NULL);
	}
}