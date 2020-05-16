#include "pch.h"
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

		}
	}
}