#pragma once
#include "../desktop/Frame.h"
#include "VideoEncodingIds.h"
#include "VideoPackage.h"

namespace mrCommonLib
{
	namespace video
	{
		typedef std::shared_ptr<class IVideoEncoder> IVideoEncoderPtr;
		class IVideoEncoder
		{
		public:
			IVideoEncoder() {}
			virtual ~IVideoEncoder() {}

			virtual void Encode(desktop::IFrame* pFrame, CVideoPackage *pVideoPackage, bool& bSkip) = 0;

			static 	IVideoEncoderPtr CreateVideoEncoder(EVideoEncoderId id);
		};

		
	
	}
}