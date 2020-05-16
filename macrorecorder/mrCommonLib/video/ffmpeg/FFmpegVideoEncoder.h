#pragma once

#include "../VideoEncoder.h"
#include "BaseCodec.h"

namespace mrCommonLib
{
	namespace video
	{
		namespace ffmpeglib
		{
			class CFFmpegVideoEncoder : public  CBaseCodec,
												IVideoEncoder
			{
			public:
				CFFmpegVideoEncoder(const char* codecName);
				CFFmpegVideoEncoder(AVCodecID codecId);
				virtual ~CFFmpegVideoEncoder();

				virtual void Encode(desktop::IFrame* pFrame, CommonLib::IWriteStream* pStream);

			private:
				AVFormatContext m_formatContext;
			};
		}
	}
}