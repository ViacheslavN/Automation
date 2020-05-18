#pragma once
#include "../desktop/Frame.h"
#include "VideoEncodingIds.h"
#include "VideoPackage.h"

namespace mrCommonLib
{
	namespace video
	{
		typedef std::shared_ptr<class IVideoEncoder> IVideoEncoderPtr;
		typedef std::shared_ptr<class IVideoFileEncoder> IVideoFileEncoderPtr;

		class IVideoEncoder
		{
		public:
			IVideoEncoder() {}
			virtual ~IVideoEncoder() {}

			virtual void Encode(desktop::IFrame* pFrame, CVideoPackage *pVideoPackage, bool& bSkip) = 0;

			static 	IVideoEncoderPtr CreateVideoEncoder(EVideoEncoderId id);
		};

		class IVideoFileEncoder
		{
		public:
			IVideoFileEncoder() {}
			virtual ~IVideoFileEncoder() {}

			virtual void OpenFile(const char* pszFileName, uint32_t width, uint32_t height, const desktop::CPixelFormat& format) = 0;
			virtual void Encode(desktop::IFrame* pFrame) = 0;
			virtual void Close() = 0;


			static 	IVideoFileEncoderPtr CreateVideoEncoder(EVideoEncoderId id);
		};
		
	
	}
}