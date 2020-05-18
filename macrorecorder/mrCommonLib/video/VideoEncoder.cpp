#include "pch.h"
#include "VideoEncoder.h"
#include "ffmpeg/FFmpeg.h"
#include "ffmpeg/FFmpegVideoEncoder.h"
#include "vpx/VideoEncoderVpx.h"
#include "x265/VideoEncoderX265.h"

namespace mrCommonLib
{
	namespace video
	{
		//ffmpeg -codecs >codecs.txt
		IVideoEncoderPtr IVideoEncoder::CreateVideoEncoder(EVideoEncoderId id)
		{

			switch (id)
			{
			case mrCommonLib::video::VIDEO_ENCODING_MPEG4:
				return IVideoEncoderPtr((IVideoEncoder*)new ffmpeglib::CFFmpegVideoEncoder(VIDEO_ENCODING_MPEG4));
				break;
			case mrCommonLib::video::VIDEO_ENCODING_VP8:
				return vpxlib::CVideoEncoderVPX::createVP8();
				break;
			case mrCommonLib::video::VIDEO_ENCODING_VP9:
				return vpxlib::CVideoEncoderVPX::createVP9();
				break;
			case mrCommonLib::video::VIDEO_ENCODING_X265:
				return IVideoEncoderPtr((IVideoEncoder*)new x265lib::CVideoEncoderX265());
				break;
			default:
				break;
			}

			throw CommonLib::CExcBase("Unknown encoder id %1", id);
		}
	}
}