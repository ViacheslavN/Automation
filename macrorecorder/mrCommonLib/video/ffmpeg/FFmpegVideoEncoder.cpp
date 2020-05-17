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
			CFFmpegVideoEncoder::CFFmpegVideoEncoder(const char* codecName) : m_codec(codecName, true)
			{

			}

			CFFmpegVideoEncoder::CFFmpegVideoEncoder(AVCodecID codecId) : m_codec(codecId, true)
			{

			}

			CFFmpegVideoEncoder::~CFFmpegVideoEncoder()
			{

			}

			void CFFmpegVideoEncoder::Encode(desktop::IFrame* pFrame, CVideoPackage *pVideoPackage)
			{

			}
		}
	}
}