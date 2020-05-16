#include "pch.h"
#include "FFmpeg.h"
#include "BaseCodec.h"
#include "FFmpegVideoEncoder.h"

namespace mrCommonLib
{
	namespace video
	{
		namespace ffmpeglib
		{
			CFFmpegVideoEncoder::CFFmpegVideoEncoder(const char* codecName) : CBaseCodec(codecName, true)
			{

			}

			CFFmpegVideoEncoder::CFFmpegVideoEncoder(AVCodecID codecId) : CBaseCodec(codecId, true)
			{

			}

			CFFmpegVideoEncoder::~CFFmpegVideoEncoder()
			{

			}

			void CFFmpegVideoEncoder::Encode(desktop::IFrame* pFrame, CommonLib::IWriteStream* pStream)
			{

			}
		}
	}
}