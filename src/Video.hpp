#pragma once

#include <SFML/Graphics.hpp>
#include <iostream>
#include <string>

using namespace std;

typedef unsigned int nuint;

#define __STDC_CONSTANT_MACROS

extern "C"
{
	#include <ffmpeg/ffmpeg.h>
}

class Video
{
public:
     Video(const string& filename = "");
	~Video();

    bool LoadFromFile(const string& filename);
    void Update(float time);

    sf::Vector2i Size() const { return sf::Vector2i(GetWidth(), GetHeight()); }
    nuint GetWidth() const { return m_pCodecCtx->width; }
    nuint GetHeight() const { return m_pCodecCtx->height; }
    float GetFrameRate() const { return 1 / m_fSecondsPerFrame; }

    operator const sf::Texture&() const { return m_Texture; }

    void CloseVideo();

private:
	void UpdateImage();
	void LoadNextFrame();

	// Variables
	sf::Texture m_Texture;
	bool m_bVideoLoaded;
	bool m_bImageNeedsUpdate;
	AVFormatContext* m_pFormatCtx;
	sf::Uint8 m_iVideoStream;
	sf::Uint32 m_iFrameSize;
	AVCodecContext* m_pCodecCtx;
	AVFrame* m_pFrame;
	AVFrame* m_pFrameRGB;
	sf::Uint8* m_pBuffer;
	AVPacket m_Packet;
	AVCodec* m_pCodec;
	SwsContext* img_convert_ctx;
	string m_sFilename;
	float m_fSecondsPerFrame;
	float m_fTimePassedSinceLastFrameUpdate;
};