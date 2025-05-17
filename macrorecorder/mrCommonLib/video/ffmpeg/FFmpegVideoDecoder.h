#pragma once

#include "../VideoDecoder.h"


namespace mrCommonLib
{
	namespace video
	{
		namespace ffmpeglib
		{
			class CAVPacket;
			class CAVFrame;
			class CBaseCodec;

			class CFFmpegVideoDecoder: IVideoDecoder
			{
			public:
				CFFmpegVideoDecoder(EVideoEncoderId encodeId);
				virtual ~CFFmpegVideoDecoder();

				virtual void Decode(desktop::IFrame* pFrame, const CVideoPackage *pVideoPackage, bool &isSkip);
				virtual EVideoEncoderId GetEncoderId() const;

			private:
 
				std::unique_ptr<CBaseCodec> m_codec;
				EVideoEncoderId m_encodeId;
				std::unique_ptr<CAVFrame>  m_frame;
				std::unique_ptr<CAVPacket>  m_packet;
			};
		}
	}
}