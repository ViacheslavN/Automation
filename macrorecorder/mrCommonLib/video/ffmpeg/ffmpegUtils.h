#pragma once

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
			};
		}
	}
}