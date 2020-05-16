#pragma once
#include "../desktop/Frame.h"

namespace mrCommonLib
{
	namespace video
	{
		class IVideoDecoder
		{
		public:
			IVideoDecoder() {}
			virtual ~IVideoDecoder() {}

			virtual void Decode(IFrame* pFrame, CommonLib::IReadStream* pStream) = 0;
		};
	}
}