#pragma once
#include "../desktop/geometry.h"
#include "../desktop/pixelformat.h"

namespace mrCommonLib
{
	namespace video
	{
		class CVideoUtil
		{
		public:
			static desktop::CRect ReadRect(CommonLib::IReadStream *pStream);
			static void WriteRect(const desktop::CRect& retc, CommonLib::IWriteStream *pStream);
			static desktop::CPixelFormat ReadPixelFormat(CommonLib::IReadStream *pStream);
			static void WritePixelFormat(const desktop::CPixelFormat& pixel, CommonLib::IWriteStream *pStream);
		};
	}
}