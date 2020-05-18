#pragma once
#include "../VideoEncodingIds.h"

namespace mrCommonLib
{
	namespace video
	{
		namespace ffmpeglib
		{
			class CFFmpegUtil
			{
			public:
				static void CheckAndThrowException(int err);
				static AVCodecID ConvertEncodeID2AVCodeID(EVideoEncoderId id);
			};
		}
	}
}