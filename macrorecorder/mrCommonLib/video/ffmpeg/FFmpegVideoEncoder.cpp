#include "pch.h"
#include "FFmpeg.h"
#include "BaseCodec.h"
#include "AVFrame.h"
#include "AVPacket.h"
#include "FFmpegVideoEncoder.h"
#include "ffmpegUtils.h"
#include "FFmpegException.h"
#include "../FormatConvertor.h"

namespace mrCommonLib
{
	namespace video
	{
		namespace ffmpeglib
		{
			CFFmpegVideoEncoder::CFFmpegVideoEncoder(EVideoEncoderId encodeId) : m_encodeId(encodeId)
			{

			}

			CFFmpegVideoEncoder::~CFFmpegVideoEncoder()
			{

			}

			void CFFmpegVideoEncoder::Encode(desktop::IFrame* pFrame, CVideoPackage *pVideoPackage, bool &isSkip)
			{
				try
				{
					FillPacketInfo(m_encodeId, pFrame, pVideoPackage);
					isSkip = true;

					if (m_codec.get() == nullptr || pVideoPackage->IsChange())
					{
						AVCodecID av_codec = CFFmpegUtil::ConvertEncodeID2AVCodeID(m_encodeId);

						m_codec.reset(new CBaseCodec(av_codec, true));
						int ret = m_codec->OpenCodec(pFrame->Size().Width(), pFrame->Size().Height(), AV_PIX_FMT_YUV420P);
						if (ret < 0)
							throw CFFmpegException(ret, "Failed to open codec");

						m_packet.reset(new CAVPacket());
						m_frame.reset(new CAVFrame(m_codec.get()));
						m_frameCount = 0;
					
					}

 


					const int y_stride = m_frame->GetFrame()->linesize[0];
					const int uv_stride = m_frame->GetFrame()->linesize[1];
					uint8_t* y_data = (uint8_t*)m_frame->GetFrame()->data[0];
					uint8_t* u_data = (uint8_t*)m_frame->GetFrame()->data[1];
					uint8_t* v_data = (uint8_t*)m_frame->GetFrame()->data[2];

					CConvertPixels::ConvertToI420(pFrame, y_data, u_data, v_data, y_stride, uv_stride);


					m_frame->GetFrame()->pts = m_frameCount;
					int ret = m_codec->SendFrame(m_frame.get());
					if (ret != 0)
						throw CFFmpegException(ret, "Failed to send frame");

					m_frameCount += 1;
					ret = 0;
					while (ret >= 0)
					{
						ret = m_codec->ReceivePacket(m_packet.get());
						if (ret == AVERROR(EAGAIN) || ret == AVERROR_EOF)
							return;

						else if (ret < 0)
							throw CFFmpegException(ret, "Error during encoding");

						pVideoPackage->AddEncodeData(m_packet->GetPacket()->data, m_packet->GetPacket()->size);
						isSkip = false;
					}
				}
				catch (std::exception& excSrc)
				{
					CommonLib::CExcBase::RegenExc("Failed to encode", excSrc);
					throw;
				}
			}
		}
	}
}