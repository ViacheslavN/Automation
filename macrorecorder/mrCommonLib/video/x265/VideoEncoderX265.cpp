#include "pch.h"
#include "VideoEncoderX265.h"
#include "../../../../../ThirdParty/libyuv/include/libyuv/convert.h"
#include "../../../../../ThirdParty/libyuv/include/libyuv/convert_from_argb.h"

namespace mrCommonLib
{
	namespace video
	{
		namespace x265lib
		{
			CVideoEncoderX265::CVideoEncoderX265()
			{

			}

			void CVideoEncoderX265::Encode(desktop::IFrame* pFrame, CVideoPackage *pVideoPackage, bool& bSkip)
			{
				try
				{
					bSkip = true;
					FillPacketInfo(VIDEO_ENCODING_X265, pFrame, pVideoPackage);
					if (m_encoder.get() == nullptr || pVideoPackage->IsChange())
					{
						m_param.reset(new CX265Param());
						m_param->GetHandle()->internalCsp = X265_CSP_I420;
						m_param->GetHandle()->sourceWidth = pFrame->Size().Width();
						m_param->GetHandle()->sourceHeight = pFrame->Size().Height();
						m_param->GetHandle()->fpsNum = 30;
						m_param->GetHandle()->fpsDenom = 1; 

						m_encoder.reset(new CX265Encoder(m_param));
						m_picture.reset(new CX265Picture(m_param));

						x265_nal *pNals = nullptr;
						uint32_t iNal = 0;

						int ret = m_encoder->EncodeHeader(&pNals, &iNal);
						if (ret < 0)
							throw CommonLib::CExcBase("Error encode header");

						if(iNal != 0)
							bSkip= false;

						for (int i = 0; i < iNal; ++i)
						{
							pVideoPackage->AddEncodeData(pNals[i].payload, pNals[i].sizeBytes);
						}
					}

					auto convert_to_i420 = libyuv::ARGBToI420;

					if (pFrame->Format().BitsPerPixel() == 16)
						convert_to_i420 = libyuv::RGB565ToI420;


					const desktop::CRect rect = desktop::CRect::MakeSize(pFrame->Size());


					const int y_stride = m_picture->GetHandle()->stride[0];
					const int uv_stride = m_picture->GetHandle()->stride[1];
					uint8_t* y_data = (uint8_t*)m_picture->GetHandle()->planes[0];
					uint8_t* u_data = (uint8_t*)m_picture->GetHandle()->planes[1];
					uint8_t* v_data = (uint8_t*)m_picture->GetHandle()->planes[2];


					const int y_offset = y_stride * rect.Y() + rect.X();
					const int uv_offset = uv_stride * rect.Y() / 2 + rect.X() / 2;


					
					convert_to_i420(pFrame->DataAtPos(rect.TopLeft()),
						pFrame->Stride(),
						y_data + y_offset, y_stride,
						u_data + uv_offset, uv_stride,
						v_data + uv_offset, uv_stride,
						rect.Width(),
						rect.Height());

					x265_nal *pNals = nullptr;
					uint32_t iNal = 0;
					int ret;
				/*	int ret = m_encoder->EncodeHeader(&pNals, &iNal);
					if (ret < 0)
						throw CommonLib::CExcBase("Error encode header");
					
				
					for (int i = 0; i < iNal; ++i)
					{
						pVideoPackage->AddEncodeData(pNals[i].payload, pNals[i].sizeBytes);
					}*/

					ret = m_encoder->Encode(&pNals, &iNal, m_picture->GetHandle(), nullptr);
					if (ret < 0)
						throw CommonLib::CExcBase("Error encode");

					if (iNal != 0)
						bSkip = false;

					for (int i = 0; i < iNal; ++i)
					{
						pVideoPackage->AddEncodeData(pNals[i].payload, pNals[i].sizeBytes);
					}

					//flush encoder
				/*	while (1)
					{
						ret = m_encoder->Encode(&pNals, &iNal, NULL, NULL);
						if (ret == 0)
							break;

						if (ret < 0)
							throw CommonLib::CExcBase("Error encode flush");

						for (int i = 0; i < iNal; ++i)
						{
							pVideoPackage->AddEncodeData(pNals[i].payload, pNals[i].sizeBytes);
						}
					}*/
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