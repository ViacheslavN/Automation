#pragma once

#include "../VideoDecoder.h"
#include "vpx.h"
#include "vpx_codec.h"

namespace mrCommonLib
{
	namespace video
	{
		namespace vpxlib
		{

			class CVideoDecoderVPX : public IVideoDecoder
			{
			public:
				~CVideoDecoderVPX() = default;

				static IVideoDecoderPtr CreateVP8();
				static IVideoDecoderPtr CreateVP9();

				virtual void Decode(desktop::IFrame* pFrame, const CVideoPackage *pVideoPackage, bool &isSkip);
				virtual EVideoEncoderId GetEncoderId() const;

			private:
				explicit CVideoDecoderVPX(EVideoEncoderId encoding);

				VpxCodecPtr m_codec;

				EVideoEncoderId m_encodingId;
			};
		}
	}
}
