#pragma once
#include "VideoEncoder.h"

namespace mrCommonLib
{
	namespace video
	{

		class IBaseVideoEncoder : public IVideoEncoder
		{
		public:
			IBaseVideoEncoder() {}
			virtual ~IBaseVideoEncoder() {}
		protected:

			void FillPacketInfo(EVideoEncoderId encodingID,	const desktop::IFrame* frame, CVideoPackage *pVideoPackage);


			desktop::CSize m_lastSize;
 
		};
	}
} 
