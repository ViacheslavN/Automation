#include "pch.h"
#include "VideoEncoder.h"
#include "ffmpeg/FFmpeg.h"
#include "ffmpeg/FFmpegVideoEncoder.h"
#include "ffmpeg/FFmpegFileVideoEncoder.h"
#include "vpx/VideoEncoderVpx.h"
#include "x265/VideoEncoderX265.h"
#include "x265/FileVideoEncodeX265.h"

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


		IVideoFileEncoderPtr IVideoFileEncoder::CreateVideoEncoder(EVideoEncoderId id)
		{
			switch (id)
			{
			case mrCommonLib::video::VIDEO_ENCODING_NODE:
				break;
			case mrCommonLib::video::VIDEO_ENCODING_MPEG4:
				return IVideoFileEncoderPtr((IVideoFileEncoder*)new ffmpeglib::CFFmpegFileVideoEncoder(VIDEO_ENCODING_MPEG4));
				break;
			case mrCommonLib::video::VIDEO_ENCODING_VP8:
				break;
			case mrCommonLib::video::VIDEO_ENCODING_VP9:
				break;
			case mrCommonLib::video::VIDEO_ENCODING_X265:
				return IVideoFileEncoderPtr((IVideoFileEncoder*)new x265lib::CFileVideoEncoderX265());
				break;
			default:
				break;
			}


			throw CommonLib::CExcBase("Unknown encoder id %1", id);
		}

	}
}