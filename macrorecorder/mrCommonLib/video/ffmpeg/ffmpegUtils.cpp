#include "pch.h"
#include "FFmpeg.h"
#include "ffmpegUtils.h"
#include "FFmpegException.h"

namespace mrCommonLib
{
	namespace video
	{
		namespace ffmpeglib
		{
			void CFFmpegUtil::CheckAndThrowException(int err)
			{
				if (err != 0)
					throw CFFmpegException(err);
			}

			AVCodecID CFFmpegUtil::ConvertEncodeID2AVCodeID(EVideoEncoderId id)
			{
				switch (id)
				{
				case mrCommonLib::video::VIDEO_ENCODING_NODE:
					return AV_CODEC_ID_NONE;
					break;
				case mrCommonLib::video::VIDEO_ENCODING_MPEG4:
					return AV_CODEC_ID_MPEG4;
					break;
				case mrCommonLib::video::VIDEO_ENCODING_VP8:
					return AV_CODEC_ID_VP8;
					break;
				case mrCommonLib::video::VIDEO_ENCODING_VP9:
					return AV_CODEC_ID_VP9;
					break;
				case mrCommonLib::video::VIDEO_ENCODING_X265:
					return AV_CODEC_ID_H265;
					break;
				default:
					return AV_CODEC_ID_NONE;
					break;
				}
			}
		}
	}
}