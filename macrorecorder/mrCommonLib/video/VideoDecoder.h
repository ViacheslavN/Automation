#pragma once
#include "VideoEncodingIds.h"
#include "../desktop/Frame.h"
#include "VideoPackage.h"

namespace mrCommonLib
{
	namespace video
	{
		typedef std::shared_ptr<class IVideoDecoder> IVideoDecoderPtr;

		class IVideoDecoder
		{
		public:
			IVideoDecoder() {}
			virtual ~IVideoDecoder() {}

			virtual void Decode(desktop::IFrame* pFrame, CVideoPackage *pVideoPackage, bool &isSkip) = 0;
			virtual EVideoEncoderId GetEncoderId() const = 0;

			static 		IVideoDecoderPtr CreateVideoDecoder(EVideoEncoderId id);
		};


	}
}