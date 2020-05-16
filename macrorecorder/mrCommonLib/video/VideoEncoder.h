#pragma once
#include "../desktop/Frame.h"

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

			virtual void Encode(desktop::IFrame* pFrame, CommonLib::IWriteStream* pStream) = 0;
		};

		enum EVideoEncoderId
		{
			VIDEO_ENCODING_FFMPEG_MPEG4,
			VIDEO_ENCODING_VP8,
			VIDEO_ENCODING_VP9,
			VIDEO_ENCODING_X265
		};

		IVideoEncoderPtr CreateVideoEncoder(EVideoEncoderId id);
	}
}