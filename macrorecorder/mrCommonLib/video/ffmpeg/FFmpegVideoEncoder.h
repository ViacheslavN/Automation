#pragma once

#include "../BaseVideoEncoder.h"
#include "BaseCodec.h"

namespace mrCommonLib
{
	namespace video
	{
		namespace ffmpeglib
		{
			class CAVPacket;
			class CAVFrame;
			class CBaseCodec;

			class CFFmpegVideoEncoder : IBaseVideoEncoder
			{
			public:
				CFFmpegVideoEncoder(EVideoEncoderId encodeId);
				virtual ~CFFmpegVideoEncoder();

				virtual void Encode(desktop::IFrame* pFrame, CVideoPackage *pVideoPackage, bool &isSkip);

			private:
				std::unique_ptr<CBaseCodec> m_codec;
				EVideoEncoderId m_encodeId;
				std::unique_ptr<CAVFrame>  m_frame;
				std::unique_ptr<CAVPacket>  m_packet;
				uint32_t m_frameCount = 0;
			};
		}
	}
}