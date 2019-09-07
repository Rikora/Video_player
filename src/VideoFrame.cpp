#include "VideoFrame.hpp"

namespace vp
{
	void VideoFrame::WriteFrame(int streamIndex, AVFrame* frame, StreamData* streamData)
	{	
	}

	void VideoFrame::Close(int streamIndex)
	{
	}

	bool VideoFrame::IsPrimed()
	{
		return false;
	}
}