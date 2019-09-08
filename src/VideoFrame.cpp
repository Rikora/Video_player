#include "VideoFrame.hpp"

namespace vp
{
	FrameSinkStream* VideoFrame::CreateStream()
	{
		m_stream = std::make_unique<FrameSinkStream>(this, 0);
		return m_stream.get();
	}

	void VideoFrame::WriteFrame(int streamIndex, AVFrame* frame, StreamData* streamData)
	{
		// Write frame pixels to the pixel vector
		auto size = av_image_get_buffer_size(AV_PIX_FMT_RGBA, frame->width, frame->height, 32);
		m_pixels.resize(size); // Probs don't need to resize every frame since each frame has the same amount of bytes!

		av_image_copy_to_buffer(m_pixels.data(), size, frame->data, frame->linesize, AV_PIX_FMT_RGBA, frame->width, frame->height, 32);	

		// Need two frames!
		//sws_scale(m_img_convert_ctx, frame->data, frame->linesize, 0, frame->height, m_->data, m_pFrameRGB->linesize);	
	}

	void VideoFrame::Close(int streamIndex)
	{
		m_stream.reset();
	}

	bool VideoFrame::IsPrimed()
	{
		return true; // Change this later on...
	}

	std::vector<sf::Uint8> VideoFrame::getPixels()
	{
		return m_pixels;
	}
}