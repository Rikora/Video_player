#include "Video.hpp"

Video::Video(const string& filename) : 
	m_Texture(),
	m_bVideoLoaded(false),
	m_bImageNeedsUpdate(false),
	m_pFormatCtx(NULL),
	m_iVideoStream(0),
	m_iFrameSize(0),
	m_pCodecCtx(NULL),
	m_pFrame(NULL),
	m_pFrameRGB(NULL),
	m_pBuffer(NULL),
	m_Packet(),
	m_pCodec(NULL),
	img_convert_ctx(NULL),
	m_sFilename(""),
	m_fSecondsPerFrame(0),
	m_fTimePassedSinceLastFrameUpdate(0)
{
    if (!filename.empty())
    {
        if (!LoadFromFile(filename))
        {
            cout << "Could not load video!" << endl;
        }
    }
}

Video::~Video()
{
	CloseVideo();
}

bool Video::LoadFromFile(const string& filename)
{
    m_sFilename = filename;
    const char* file = m_sFilename.c_str();

    if(avformat_open_input(&m_pFormatCtx, file, NULL, NULL) != 0)
    {
        cout << "Unexisting file!" << endl;;
        return false;
    }

    if(avformat_find_stream_info(m_pFormatCtx, NULL) < 0)
    {
        cout << "Couldn't find stream information!" << endl;
        return false;
    }

	// Print info
    av_dump_format(m_pFormatCtx, 0, file, 0);

    m_iVideoStream = -1;
    for(nuint i = 0; i < m_pFormatCtx->nb_streams; i++)
    {
        if(m_pFormatCtx->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_VIDEO)
        {
            m_iVideoStream = i;
            break;
        }
    }

    if(m_iVideoStream == -1)
        return false;

	m_pCodec = avcodec_find_decoder(m_pFormatCtx->streams[m_iVideoStream]->codecpar->codec_id);

	m_pCodecCtx = avcodec_alloc_context3(m_pCodec);
	avcodec_parameters_to_context(m_pCodecCtx, m_pFormatCtx->streams[m_iVideoStream]->codecpar);

    AVRational r = m_pFormatCtx->streams[m_iVideoStream]->avg_frame_rate;

    if ((!r.num || !r.den))
    {
        std::cerr << "Video_video::Initialize() - unable to get the video frame rate. Using 25 fps." << std::endl;
        m_fSecondsPerFrame = 1.f / 25;
    }
    else
    {
		if (r.num && r.den)
		{
			m_fSecondsPerFrame = 1.f / ((float)r.num / r.den);
		}
    }

    if(m_pCodec == NULL)
    {
        cout << "Unsupported codec!" << endl;
        return false;
    }

    if(avcodec_open2(m_pCodecCtx, m_pCodec,NULL) < 0)
    {
        cout << "Could not open Codec Context" << endl;
        return false;
    }

    m_pFrame = av_frame_alloc();
    m_pFrameRGB = av_frame_alloc();

    if(m_pFrameRGB == NULL || m_pFrame == NULL)
    {
        cout << "Error allocating frame" << endl;
        return false;
    }

	// Allocate size for frame
	av_image_alloc(m_pFrameRGB->data, m_pFrameRGB->linesize, m_pCodecCtx->width, m_pCodecCtx->height, AV_PIX_FMT_RGBA, 32);

	// Allocate buffer size
    int numBytes = av_image_get_buffer_size(AV_PIX_FMT_RGBA, m_pCodecCtx->width, m_pCodecCtx->height, 32);
    m_pBuffer = (sf::Uint8 *)av_malloc(numBytes * sizeof(sf::Uint8));
    
	// Fill the buffer with pixel data from a frame
	av_image_fill_arrays(&m_pBuffer, m_pFrameRGB->linesize, *m_pFrameRGB->data, AV_PIX_FMT_RGBA, m_pCodecCtx->width, m_pCodecCtx->height, 32);

    img_convert_ctx = sws_getContext(m_pCodecCtx->width, m_pCodecCtx->height,
                                     m_pCodecCtx->pix_fmt,
                                     m_pCodecCtx->width, m_pCodecCtx->height,
                                     AV_PIX_FMT_RGBA, SWS_FAST_BILINEAR,
                                     NULL, NULL, NULL);
    m_bVideoLoaded = true;
    m_bImageNeedsUpdate = true;

    m_Texture.create(GetWidth(), GetHeight());

    Update(10000);

    return true;
}

void Video::Update(float time)
{
    if (m_bVideoLoaded)
    {
        m_fTimePassedSinceLastFrameUpdate += time;
        UpdateImage();

        if (m_fTimePassedSinceLastFrameUpdate > m_fSecondsPerFrame)
        {
            m_fTimePassedSinceLastFrameUpdate = 0;
            LoadNextFrame();
        }
    }
}

void Video::LoadNextFrame()
{
    do
    {
		av_packet_unref(&m_Packet);
        int result = av_read_frame(m_pFormatCtx, &m_Packet);

		// Result < 0 on error or if the movie has ended
        /*if (result < 0)
        {
            CloseVideo();
            LoadFromFile(m_sFilename);
            m_fTimePassedSinceLastFrameUpdate = 0;
            return;
        }*/
    } 
	while (m_Packet.stream_index != m_iVideoStream);

    int frameFinished = 0;

	avcodec_send_packet(m_pCodecCtx, &m_Packet);
	frameFinished = avcodec_receive_frame(m_pCodecCtx, m_pFrame);

    if (frameFinished == 0)
    {
        sws_scale(img_convert_ctx, m_pFrame->data, m_pFrame->linesize, 0, 
			m_pCodecCtx->height, m_pFrameRGB->data, m_pFrameRGB->linesize);
        m_bImageNeedsUpdate = true;
    }
}

void Video::UpdateImage()
{
    if (m_bImageNeedsUpdate)
    {
        m_Texture.update(m_pBuffer);
        m_bImageNeedsUpdate = false;
    }
}

void Video::CloseVideo()
{
    if (m_bVideoLoaded)
    {
		av_packet_unref(&m_Packet);
        av_free(m_pBuffer);
        av_free(m_pFrameRGB);
        av_free(m_pFrame);
        avcodec_close(m_pCodecCtx);
        avformat_close_input(&m_pFormatCtx);
        sws_freeContext(img_convert_ctx);

        m_bVideoLoaded = false;
        m_bImageNeedsUpdate = false;
    }
}