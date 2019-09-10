#include "Demuxer.hpp"
#include <iostream>

namespace vp::video
{
	Demuxer::Demuxer() :
	m_pFormatCtx(NULL),
	m_pVideoStream(NULL),
	m_pCodec(NULL),
	m_pCodecCtx(NULL),
	m_pSwsContext(NULL),
	m_pBuffer(NULL),
	m_pPrevFrame(NULL),
	m_pCurrentFrame(NULL),
	m_frameRate(0.f),
	m_frameFinished(false),
	m_videoFinished(false),
	m_updateTimer(sf::Time::Zero)
	{
		m_pPrevFrame = av_frame_alloc();
		m_pCurrentFrame = av_frame_alloc();
	}

	Demuxer::~Demuxer()
	{
		av_free(m_pBuffer);
		av_free(m_pCurrentFrame);
		av_free(m_pPrevFrame);
		avcodec_close(m_pCodecCtx);
		avformat_close_input(&m_pFormatCtx);
		sws_freeContext(m_pSwsContext);
	}

	bool Demuxer::loadFromFile(const std::string& file)
	{
		if (avformat_open_input(&m_pFormatCtx, file.c_str(), NULL, NULL) != 0)
		{
			throw std::runtime_error("Demuxer::loadFromFile - Failed to load: " + file);
		}

		if (avformat_find_stream_info(m_pFormatCtx, NULL) < 0)
		{
			throw std::runtime_error("Demuxer::loadFromFile - Failed to locate stream information");
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
			throw std::runtime_error("Demuxer::loadFromFile - Failed to locate an existing video stream");
		}

		createDecoderAndContext();

		if (m_pCodec == NULL)
		{
			throw std::runtime_error("Demuxer::loadFromFile - Unsupported codec");
		}

		if (avcodec_open2(m_pCodecCtx, m_pCodec, NULL) < 0)
		{
			throw std::runtime_error("Demuxer::loadFromFile - Failed to open codex context");
		}

		calculateFrameRate();
		createSwsContext();

		if (m_pSwsContext == NULL)
		{
			throw std::runtime_error("Demuxer::loadFromFile - Failed to create sws context");
		}

		createBuffer();
		m_texture.create(m_pCodecCtx->width, m_pCodecCtx->height);

		// Note: push a few frames through from the beginning?

		return true;
	}

	void Demuxer::update(sf::Time dt)
	{
		m_updateTimer += dt;

		if (m_frameFinished)
		{
			updateTexture();
		}

		if (m_updateTimer.asMilliseconds() > (1000.f / m_frameRate))
		{
			m_updateTimer = sf::Time::Zero;
			step();
		}
	}

	bool Demuxer::isFrameFinished() const
	{
		return m_frameFinished;
	}

	bool Demuxer::isVideoFinished() const
	{
		return m_videoFinished;
	}

	float Demuxer::getFrameRate() const
	{
		return m_frameRate;
	}

	int Demuxer::getWidth() const
	{
		return m_pCodecCtx->width;
	}

	int Demuxer::getHeight() const
	{
		return m_pCodecCtx->height;
	}

	sf::Uint8* Demuxer::getBuffer() const
	{
		return m_pBuffer;
	}

	sf::Texture& Demuxer::getTexture()
	{
		return m_texture;
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

	void Demuxer::createBuffer()
	{
		auto size = av_image_get_buffer_size(AV_PIX_FMT_RGBA, m_pCodecCtx->width, m_pCodecCtx->height, 32);
		m_pBuffer = (sf::Uint8*)av_malloc(size * sizeof(sf::Uint8));

		av_image_alloc(m_pCurrentFrame->data, m_pCurrentFrame->linesize, m_pCodecCtx->width, m_pCodecCtx->height, AV_PIX_FMT_RGBA, 32);
		av_image_fill_arrays(&m_pBuffer, m_pCurrentFrame->linesize, *m_pCurrentFrame->data, AV_PIX_FMT_RGBA, m_pCodecCtx->width, m_pCodecCtx->height, 32);
	}

	void Demuxer::calculateFrameRate()
	{
		AVRational r = m_pVideoStream->avg_frame_rate;

		if (!r.num || !r.den)
		{
			std::cout << "WARNING: Unable to retrieve video frame rate. Frame rate set to 25 FPS" << std::endl;
			m_frameRate = 25.f;
		}
		else
		{
			if (r.num && r.den)
			{
				m_frameRate = static_cast<float>(r.num / r.den);
			}
		}
	}

	void Demuxer::step()
	{
		do
		{
			av_packet_unref(&m_pVideoStream->attached_pic);

			if (av_read_frame(m_pFormatCtx, &m_pVideoStream->attached_pic) < 0)
			{
				m_videoFinished = true;
				return;
			}

		} while (m_pVideoStream->attached_pic.stream_index != m_pVideoStream->index);

		avcodec_send_packet(m_pCodecCtx, &m_pVideoStream->attached_pic);

		if (avcodec_receive_frame(m_pCodecCtx, m_pPrevFrame) == 0)
		{
			sws_scale(m_pSwsContext, m_pPrevFrame->data, m_pPrevFrame->linesize, 0,
				m_pCodecCtx->height, m_pCurrentFrame->data, m_pCurrentFrame->linesize);
			m_frameFinished = true;
		}
	}

	void Demuxer::updateTexture()
	{
		m_texture.update(m_pBuffer);
		m_frameFinished = false;
	}
}