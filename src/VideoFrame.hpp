#pragma once

#include <ffmpeg/ffmpegcpp.h>

using namespace ffmpegcpp;

namespace vp
{
	class VideoFrame final : public VideoFrameSink, public FrameWriter
	{
	public:
		virtual void WriteFrame(int streamIndex, AVFrame* frame, StreamData* streamData) override;
		virtual void Close(int streamIndex) override;
		virtual bool IsPrimed() override;
	};
}

//#include <ffmpeg/ffmpegcpp.h>
//
//using namespace std;
//using namespace ffmpegcpp;
//
//class PGMFileSink : public VideoFrameSink, public FrameWriter
//{
//public:
//
//	FrameSinkStream* CreateStream()
//	{
//		stream = new FrameSinkStream(this, 0);
//		return stream;
//	}
//
//	virtual void WriteFrame(int streamIndex, AVFrame* frame, StreamData* streamData)
//	{
//		++frameNumber;
//		//printf("saving frame %3d\n", frameNumber);
//		//fflush(stdout);
//
//		// write the first channel's color data to a PGM file.
//		// This raw image file can be opened with most image editing programs.
//		
//
//		/*pgm_save(frame->data[0], frame->linesize[0],
//			frame->width, frame->height, fileNameBuffer);*/
//	}
//
//	/*void pgm_save(unsigned char* buf, int wrap, int xsize, int ysize,
//		char* filename)
//	{
//		FILE* f;
//		int i;
//
//		f = fopen(filename, "w");
//		fprintf(f, "P5\n%d %d\n%d\n", xsize, ysize, 255);
//		for (i = 0; i < ysize; i++)
//			fwrite(buf + i * wrap, 1, xsize, f);
//		fclose(f);
//	}*/
//
//	virtual void Close(int streamIndex)
//	{
//		delete stream;
//	}
//
//	virtual bool IsPrimed()
//	{
//		// Return whether we have all information we need to start writing out data.
//		// Since we don't really need any data in this use case, we are always ready.
//		// A container might only be primed once it received at least one frame from each source
//		// it will be muxing together (see Muxer.cpp for how this would work then).
//		return true;
//	}
//
//private:
//	char fileNameBuffer[1024];
//	int frameNumber = 0;
//	FrameSinkStream* stream;
//
//};
