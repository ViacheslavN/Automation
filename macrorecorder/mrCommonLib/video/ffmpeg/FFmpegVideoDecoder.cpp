#include "pch.h"
#include "FFmpeg.h"
#include "BaseCodec.h"
#include "AVFrame.h"
#include "AVPacket.h"
#include "FFmpegVideoDecoder.h"
#include "ffmpegUtils.h"
#include "FFmpegException.h"
#include "../../../../../ThirdParty/libyuv/include/libyuv/convert.h"
#include "../../../../../ThirdParty/libyuv/include/libyuv/convert_from_argb.h"
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

//					for (int i = 0; i < pVideoPackage->GetDirtyRectCount(); ++i)
					{
						desktop::CRect rect = desktop::CRect::MakeSize(frame->Size());

					//	if (!frame_rect.ContainsRect(rect))
					//	{
							//LOG(LS_WARNING) << "The rectangle is outside the screen area";
							//return false;
					//		throw CommonLib::CExcBase("The rectangle is outside the screen area");
					//	}

						int y_offset = y_stride * rect.Y() + rect.X();
						int uv_offset = uv_stride * rect.Y() / 2 + rect.X() / 2;

						libyuv::I420ToARGB(y_data + y_offset, y_stride,
							u_data + uv_offset, uv_stride,
							v_data + uv_offset, uv_stride,
							frame->DataAtPos(rect.TopLeft()),
							frame->Stride(),
							rect.Width(),
							rect.Height());
					}
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