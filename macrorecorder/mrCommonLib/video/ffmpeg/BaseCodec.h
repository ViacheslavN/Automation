#pragma once
namespace mrCommonLib
{
	namespace video
	{
		namespace ffmpeglib
		{
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