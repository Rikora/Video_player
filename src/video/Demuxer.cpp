#include "Demuxer.hpp"
#include <iostream>

namespace vp::video
{
	Demuxer::Demuxer() :
	m_pFormatCtx(NULL),
	m_pVideoStream(NULL),
	m_pAudioStream(NULL),
	m_pVideoCodecCtx(NULL),
	m_pAudioCodecCtx(NULL),
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
		avcodec_close(m_pVideoCodecCtx);
		avcodec_close(m_pAudioCodecCtx);
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

		// Retrieve video and audio stream
		for (unsigned i = 0; i < m_pFormatCtx->nb_streams; ++i)
		{
			if (m_pFormatCtx->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_VIDEO)
			{
				m_pVideoStream = m_pFormatCtx->streams[i];
				createDecoderAndContext(&m_pVideoStream, &m_pVideoCodecCtx);
			}
			else if (m_pFormatCtx->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_AUDIO)
			{
				m_pAudioStream = m_pFormatCtx->streams[i];
				createDecoderAndContext(&m_pAudioStream, &m_pAudioCodecCtx);
			}
		}

		if (m_pVideoStream == NULL)
		{
			throw std::runtime_error("Demuxer::loadFromFile - Failed to locate an existing video stream");
		}

		calculateFrameRate();
		createSwsContext();
		createBuffer();
		m_texture.create(m_pVideoCodecCtx->width, m_pVideoCodecCtx->height);

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
		return m_pVideoCodecCtx->width;
	}

	int Demuxer::getHeight() const
	{
		return m_pVideoCodecCtx->height;
	}

	sf::Uint8* Demuxer::getBuffer() const
	{
		return m_pBuffer;
	}

	sf::Texture& Demuxer::getTexture()
	{
		return m_texture;
	}

	void Demuxer::createDecoderAndContext(AVStream** stream, AVCodecContext** codecCtx)
	{
		auto pCodec = avcodec_find_decoder(stream[0]->codecpar->codec_id);
		*codecCtx = avcodec_alloc_context3(pCodec);
		avcodec_parameters_to_context(*codecCtx, stream[0]->codecpar);

		if (pCodec == NULL)
		{
			throw std::runtime_error("Demuxer::createDecoderAndContext - Unsupported codec");
		}

		if (avcodec_open2(*codecCtx, pCodec, NULL) < 0)
		{
			throw std::runtime_error("Demuxer::createDecoderAndContext - Failed to open codex context");
		}
	}

	void Demuxer::createSwsContext()
	{
		m_pSwsContext = sws_getContext(
				m_pVideoCodecCtx->width, 
				m_pVideoCodecCtx->height,
				m_pVideoCodecCtx->pix_fmt,
				m_pVideoCodecCtx->width, 
				m_pVideoCodecCtx->height,
				AV_PIX_FMT_RGBA,
				SWS_FAST_BILINEAR,
				NULL, 
				NULL, 
				NULL);

		if (m_pSwsContext == NULL)
		{
			throw std::runtime_error("Demuxer::createSwsContext - Failed to create sws context");
		}
	}

	void Demuxer::createBuffer()
	{
		auto size = av_image_get_buffer_size(AV_PIX_FMT_RGBA, m_pVideoCodecCtx->width, m_pVideoCodecCtx->height, 32);
		m_pBuffer = (sf::Uint8*)av_malloc(size * sizeof(sf::Uint8));

		av_image_alloc(m_pCurrentFrame->data, m_pCurrentFrame->linesize, m_pVideoCodecCtx->width, 
			m_pVideoCodecCtx->height, AV_PIX_FMT_RGBA, 32);
		av_image_fill_arrays(&m_pBuffer, m_pCurrentFrame->linesize, *m_pCurrentFrame->data, 
			AV_PIX_FMT_RGBA, m_pVideoCodecCtx->width, m_pVideoCodecCtx->height, 32);
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

		avcodec_send_packet(m_pVideoCodecCtx, &m_pVideoStream->attached_pic);

		if (avcodec_receive_frame(m_pVideoCodecCtx, m_pPrevFrame) == 0)
		{
			sws_scale(m_pSwsContext, m_pPrevFrame->data, m_pPrevFrame->linesize, 0,
				m_pVideoCodecCtx->height, m_pCurrentFrame->data, m_pCurrentFrame->linesize);
			m_frameFinished = true;
		}
	}

	void Demuxer::updateTexture()
	{
		m_texture.update(m_pBuffer);
		m_frameFinished = false;
	}
}