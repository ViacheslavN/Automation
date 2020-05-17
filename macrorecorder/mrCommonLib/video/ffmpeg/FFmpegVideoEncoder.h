#pragma once

#include "../VideoEncoder.h"
#include "BaseCodec.h"

namespace mrCommonLib
{
	namespace video
	{
		namespace ffmpeglib
		{
			class CFFmpegVideoEncoder : IVideoEncoder
			{
			public:
				CFFmpegVideoEncoder(const char* codecName);
				CFFmpegVideoEncoder(AVCodecID codecId);
				virtual ~CFFmpegVideoEncoder();

				virtual void Encode(desktop::IFrame* pFrame, CVideoPackage *pVideoPackage);

			private:
				AVFormatContext m_formatContext;
				CBaseCodec m_codec;
			};
		}
	}
}