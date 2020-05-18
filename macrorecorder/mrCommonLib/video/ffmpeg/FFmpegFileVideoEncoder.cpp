#include "pch.h"
#include "FFmpeg.h"
#include "BaseCodec.h"
#include "AVFrame.h"
#include "AVPacket.h"
#include "FFmpegFileVideoEncoder.h"
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
			CFFmpegFileVideoEncoder::CFFmpegFileVideoEncoder(EVideoEncoderId encodeId) : m_encodeId(encodeId)
			{

			}

			CFFmpegFileVideoEncoder::~CFFmpegFileVideoEncoder()
			{

			}

			void CFFmpegFileVideoEncoder::OpenFile(const char* pszFileName, uint32_t width, uint32_t height, const desktop::CPixelFormat& format)
			{
				try
				{
					if (m_codec.get() != nullptr)
						throw CommonLib::CExcBase("file is open");

					m_file = CommonLib::file::CFileCreator::OpenFileA(pszFileName, CommonLib::file::ofmCreateAlways, CommonLib::file::arWrite, CommonLib::file::smNoMode, CommonLib::file::oftBinary);
					m_format = format;
					AVCodecID av_codec = CFFmpegUtil::ConvertEncodeID2AVCodeID(m_encodeId);

					m_codec.reset(new CBaseCodec(av_codec, true));
					int ret = m_codec->OpenCodec(width, height, AV_PIX_FMT_YUV420P);
					if (ret < 0)
						throw CFFmpegException(ret, "Failed to open codec");

					m_packet.reset(new CAVPacket());
					m_frame.reset(new CAVFrame(m_codec.get()));
					m_frameCount = 0;				

				}
				catch (std::exception& excSrc)
				{
					CommonLib::CExcBase::RegenExc("Failed to open file %1", pszFileName, excSrc);
					throw;
				}
			}

			void CFFmpegFileVideoEncoder::Close()
			{
				try
				{
					int ret = m_codec->SendFrame(nullptr);
					if (ret != 0)
						throw CFFmpegException(ret, "Failed to send frame");					

					while (ret >= 0)
					{
						ret = m_codec->ReceivePacket(m_packet.get());
						if (ret == AVERROR(EAGAIN) || ret == AVERROR_EOF)
							break;

						else if (ret < 0)
							throw CFFmpegException(ret, "Error during encoding");

						m_file->Write(m_packet->GetPacket()->data, m_packet->GetPacket()->size);
					}

					m_file->CloseFile();
					m_codec.reset();
				}
				catch (std::exception& excSrc)
				{
					CommonLib::CExcBase::RegenExc("Failed to close file %1", excSrc);
					throw;
				}

			}

			void CFFmpegFileVideoEncoder::Encode(desktop::IFrame* pFrame)
			{
				try
				{

					auto convert_to_i420 = libyuv::ARGBToI420;

					if (pFrame->Format().BitsPerPixel() == 16)
						convert_to_i420 = libyuv::RGB565ToI420;


					const desktop::CRect rect = desktop::CRect::MakeSize(pFrame->Size());


					const int y_stride = m_frame->GetFrame()->linesize[0];
					const int uv_stride = m_frame->GetFrame()->linesize[1];
					uint8_t* y_data = (uint8_t*)m_frame->GetFrame()->data[0];
					uint8_t* u_data = (uint8_t*)m_frame->GetFrame()->data[1];
					uint8_t* v_data = (uint8_t*)m_frame->GetFrame()->data[2];


					const int y_offset = y_stride * rect.Y() + rect.X();
					const int uv_offset = uv_stride * rect.Y() / 2 + rect.X() / 2;



					convert_to_i420(pFrame->DataAtPos(rect.TopLeft()),
						pFrame->Stride(),
						y_data + y_offset, y_stride,
						u_data + uv_offset, uv_stride,
						v_data + uv_offset, uv_stride,
						rect.Width(),
						rect.Height());

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

						m_file->Write(m_packet->GetPacket()->data, m_packet->GetPacket()->size);
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