#include "Application.hpp"

int main()
{
	vp::Application app;

	app.run();

	return 0;
}

//#include <iostream>
//#include "VideoFrame.hpp"
//
//using namespace std;
//
//int main()
//{
//	// This example will decode a video stream from a container and output it as raw image data, one image per frame.
//	try
//	{
//		// Load this container file so we can extract video from it.
//		Demuxer* demuxer = new Demuxer("samples/big_buck_bunny.mp4");
//
//		// Create a file sink that will just output the raw frame data in one PGM file per frame.
//		vp::VideoFrame* fileSink = new vp::VideoFrame();
//
//		// tie the file sink to the best video stream in the input container.
//		demuxer->DecodeBestVideoStream(fileSink);
//
//		// Prepare the output pipeline. This will push a small amount of frames to the file sink until it IsPrimed returns true.
//		demuxer->PreparePipeline();
//
//		// Push all the remaining frames through.
//		while (!demuxer->IsDone())
//		{
//			demuxer->Step();
//		}
//
//		// done
//		delete demuxer;
//		delete fileSink;
//	}
//	catch (FFmpegException e)
//	{
//		cerr << "Exception caught!" << endl;
//		cerr << e.what() << endl;
//		throw e;
//	}
//
//	cout << "Decoding complete!" << endl;
//
//	system("pause");
//	return 0;
//}