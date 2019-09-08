#pragma once

#include <SFML/Config.hpp>
#include <ffmpeg/ffmpegcpp.h>
#include <memory>

using namespace ffmpegcpp;

namespace vp
{
	class VideoFrame final : public VideoFrameSink, public FrameWriter
	{
	public:
		virtual FrameSinkStream* CreateStream() override;
		virtual void WriteFrame(int streamIndex, AVFrame* frame, StreamData* streamData) override;
		virtual void Close(int streamIndex) override;
		virtual bool IsPrimed() override;

		std::vector<sf::Uint8> getPixels();

	private:
		std::unique_ptr<FrameSinkStream> m_stream;
		std::vector<sf::Uint8> m_pixels;
		//SwsContext* m_img_convert_ctx;
	};
}
