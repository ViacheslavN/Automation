#include "pch.h"
#include "VideoEncoder.h"
#include "ffmpeg/FFmpeg.h"
#include "ffmpeg/FFmpegVideoEncoder.h"
#include "vpx/VideoEncoderVpx.h"

namespace mrCommonLib
{
	namespace video
	{
		//ffmpeg -codecs >codecs.txt
		IVideoEncoderPtr CreateVideoEncoder(EVideoEncoderId id)
		{

			switch (id)
			{
			case mrCommonLib::video::VIDEO_ENCODING_FFMPEG_MPEG4:
				return IVideoEncoderPtr((IVideoEncoder*)new ffmpeglib::CFFmpegVideoEncoder(AV_CODEC_ID_H265));
				break;
			case mrCommonLib::video::VIDEO_ENCODING_VP8:
				return vpxlib::CVideoEncoderVPX::createVP8();
				break;
			case mrCommonLib::video::VIDEO_ENCODING_VP9:
				return vpxlib::CVideoEncoderVPX::createVP9();
				break;
			case mrCommonLib::video::VIDEO_ENCODING_X265:
				break;
			default:
				break;
			}

			throw CommonLib::CExcBase("Unknown encoder id %1", id);
		}
	}
}