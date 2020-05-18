#include "pch.h"
#include "VideoDecoder.h"
#include "vpx/VideoDecoderVpx.h"
#include "ffmpeg/FFmpegVideoDecoder.h"

namespace mrCommonLib
{
	namespace video
	{

		IVideoDecoderPtr IVideoDecoder::CreateVideoDecoder(EVideoEncoderId id)
		{
			switch (id)
			{
			case mrCommonLib::video::VIDEO_ENCODING_MPEG4:
				return IVideoDecoderPtr((IVideoDecoder*)new ffmpeglib::CFFmpegVideoDecoder(VIDEO_ENCODING_MPEG4));
				break;
			case mrCommonLib::video::VIDEO_ENCODING_VP8:
				return vpxlib::CVideoDecoderVPX::CreateVP8();
				break;
			case mrCommonLib::video::VIDEO_ENCODING_VP9:
				return vpxlib::CVideoDecoderVPX::CreateVP9();
				break;
			case mrCommonLib::video::VIDEO_ENCODING_X265:
				return IVideoDecoderPtr((IVideoDecoder*)new ffmpeglib::CFFmpegVideoDecoder(VIDEO_ENCODING_X265));
				break;
			default:
				break;
			}

			throw CommonLib::CExcBase("Unknown encoder id %1", id);
		}

	}
}