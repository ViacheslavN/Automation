#pragma once
#include "../desktop/Frame.h"

namespace mrCommonLib
{
	class IVideoEncode
	{
		virtual void Encode(IFrame* pFrame, CommonLib::IWriteStream* pStream) = 0;
	};
}