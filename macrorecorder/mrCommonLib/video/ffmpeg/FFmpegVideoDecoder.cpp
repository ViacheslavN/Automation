#include "pch.h"
#include "FFmpeg.h"
#include "BaseCodec.h"
#include "AVFrame.h"
#include "AVPacket.h"
#include "FFmpegVideoDecoder.h"
#include "ffmpegUtils.h"
#include "FFmpegException.h"
#include "../FormatConvertor.h"

namespace mrCommonLib
{
	namespace video
	{
		namespace ffmpeglib
		{
			CFFmpegVideoDecoder::CFFmpegVideoDecoder(EVideoEncoderId encodeId) : m_encodeId(encodeId)
			{

			}
			
			CFFmpegVideoDecoder::~CFFmpegVideoDecoder()
			{

			}



			void ConvertImage(CAVFrame* avFrame, desktop::IFrame* frame)
			{
				try
				{
					AVPicture* picture = (AVPicture*)avFrame->GetFrame()->data;
				//	if (avFrame->buf != VPX_IMG_FMT_I420)
				//		throw CommonLib::CExcBase("fmt != VPX_IMG_FMT_I420");

					desktop::CRect frame_rect = desktop::CRect::MakeSize(frame->Size());

					uint8_t* y_data = picture->data[0];
					uint8_t* u_data = picture->data[1];
					uint8_t* v_data = picture->data[2];

					int y_stride = picture->linesize[0];
					int uv_stride = picture->linesize[1];

					CConvertPixels::ConvertI420ToARGB(y_data, u_data, v_data, y_stride, uv_stride, frame);
				}
				catch (std::exception& excSrc)
				{
					CommonLib::CExcBase::RegenExc("Failed to convert image", excSrc);
					throw;
				}
			}
		

			void CFFmpegVideoDecoder::Decode(desktop::IFrame* pFrame, const CVideoPackage *pVideoPackage, bool& isSkip)
			{
				try
				{
					if (m_codec.get() == nullptr || pVideoPackage->IsChange())
					{
						AVCodecID av_codec = CFFmpegUtil::ConvertEncodeID2AVCodeID(m_encodeId);

						m_codec.reset(new CBaseCodec(av_codec, false));
						m_packet.reset(new CAVPacket());
						m_frame.reset(new CAVFrame());
						//	AVCodecContext* ctx = m_codec->GetContext();
						//	ctx->extradata = NULL;
						//	ctx->width = pFrame->Size().Width(); 
						//	ctx->height = pFrame->Size().Height();
						//	ctx->pix_fmt = AV_PIX_FMT_YUV420P;

						int ret = m_codec->OpenCodec();
						if (ret < 0)
							throw CFFmpegException(ret, "Failed to open codec");
					}
					isSkip = false;

					if (pVideoPackage->GetEncodeDataSize() == 0)
					{
						isSkip = true;
						return;
					}

					m_packet->AttachData(pVideoPackage->GetEncodeData(), pVideoPackage->GetEncodeDataSize());

				//	while (true)
					{
						int ret = m_codec->SendPacket(m_packet.get());
						if (ret != 0)
							throw CFFmpegException(ret, "Failed to send packet");

						ret = m_codec->ReceiveFrame(m_frame.get());
						if (ret == AVERROR(EAGAIN) || ret == AVERROR_EOF)
						{
							isSkip = true;
							return;
						}

						if(ret != 0 )
							throw CFFmpegException(ret, "Failed to recive frame");
					}

					ConvertImage(m_frame.get(), pFrame);					 
					
				}			
				catch (std::exception& excSrc)
				{
					CommonLib::CExcBase::RegenExc("Failed to decode", excSrc);
					throw;
				}
			}

			EVideoEncoderId CFFmpegVideoDecoder::GetEncoderId() const
			{
				return m_encodeId;
			}
		}
	}
}