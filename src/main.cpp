#include "Application.hpp"

int main()
{
	vp::Application app;

	app.run();

	return 0;
}

//#include <ffmpeg/ffmpegcpp.h>
//#include <memory>
//
//using namespace ffmpegcpp;
//
//int main()
//{
//	// Create a muxer that will output the video as MP4.
//	auto muxer = std::make_unique<Muxer>("filtered_video.mp4");
//
//	// Create a MPEG2 codec that will encode the raw data.
//	auto codec = std::make_unique<VideoCodec>(AV_CODEC_ID_MPEG2VIDEO);
//
//	// Create an encoder that will encode the raw audio data using the codec specified above.
//	// Tie it to the muxer so it will be written to file.
//	auto encoder = std::make_unique<VideoEncoder>(codec.get(), muxer.get());
//
//	// Create a video filter and do some funny stuff with the video data.
//	auto filter = std::make_unique<Filter>("scale=640:150,transpose=cclock,vignette", encoder.get());
//
//	// Load a container. Pick the best video stream container in the container
//	// And send it to the filter.
//	auto demuxer = Demuxer("samples/big_buck_bunny.mp4");
//	demuxer.DecodeBestVideoStream(filter.get());
//
//	// Prepare the output pipeline.
//	// This will decode a small amount of frames so the pipeline can configure itself.
//	demuxer.PreparePipeline();
//
//	// Push all the remaining frames through.
//	while (!demuxer.IsDone())
//	{
//		demuxer.Step();
//	}
//
//	// Save everything to disk by closing the muxer.
//	muxer->Close();
//}