#pragma once


extern "C"
{
	typedef struct AVCodecContext AVCodecContext;
	typedef struct AVCodec	AVCodec;
	enum AVCodecID;
	enum AVPixelFormat;
}


namespace mrCommonLib
{
	namespace video
	{
		namespace ffmpeglib
		{
			class CAVPacket;
			class CAVFrame;

			class CBaseCodec
			{
			public:
				CBaseCodec(const char* codecName, bool encode);
				CBaseCodec(AVCodecID codecId, bool encode);
				virtual ~CBaseCodec();

				void SetOption(const char* name, const char* value);
				void SetOption(const char* name, int value);
				void SetOption(const char* name, double value);
				void SetGenericOption(const char* name, const char* value);

				AVCodecContext* GetContext();

				int OpenCodec();
				int OpenCodec(uint32_t wight, uint32_t height, AVPixelFormat pxfmt);
				int SendPacket(CAVPacket* packet);
				int ReceiveFrame(CAVFrame* frame);

			protected:
				AVCodecContext* AllocContext(AVCodec* codec);
				AVCodecContext* GetContextExc();
				void Clean();
			protected:
				AVCodecContext* m_codecContext = nullptr;
			};
		}
	}
}