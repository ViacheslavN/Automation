#include "pch.h"
#include "FileVideoEncodeX265.h"
#include "../FormatConvertor.h"

namespace mrCommonLib
{
	namespace video
	{
		namespace x265lib
		{
			CFileVideoEncoderX265::CFileVideoEncoderX265()
			{

			}

			void CFileVideoEncoderX265::OpenFile(const char* pszFileName, uint32_t width, uint32_t height, const desktop::CPixelFormat& format)
			{
				try
				{
					if (m_encoder.get() != nullptr)
						throw CommonLib::CExcBase("file is open");

					m_file = CommonLib::file::CFileCreator::OpenFileA(pszFileName, CommonLib::file::ofmCreateAlways, CommonLib::file::arWrite, CommonLib::file::smNoMode, CommonLib::file::oftBinary);
					m_format = format;

					m_param.reset(new CX265Param());
					m_param->GetHandle()->internalCsp = X265_CSP_I420;
					m_param->GetHandle()->sourceWidth = width;
					m_param->GetHandle()->sourceHeight = height;
					m_param->GetHandle()->fpsNum = 30;
					m_param->GetHandle()->fpsDenom = 1;

					m_encoder.reset(new CX265Encoder(m_param));
					m_picture.reset(new CX265Picture(m_param));

					x265_nal *pNals = nullptr;
					uint32_t iNal = 0;

					int ret = m_encoder->EncodeHeader(&pNals, &iNal);
					if (ret < 0)
						throw CommonLib::CExcBase("Error encode header");


					for (int i = 0; i < iNal; ++i)
					{
						m_file->Write(pNals[i].payload, pNals[i].sizeBytes);
					}

				}
				catch (std::exception& excSrc)
				{
					CommonLib::CExcBase::RegenExc("Failed to open file %1", pszFileName, excSrc);
					throw;
				}
			}


			void CFileVideoEncoderX265::Encode(desktop::IFrame* pFrame)
			{
				try
				{
					if (m_format != pFrame->Format())
						throw CommonLib::CExcBase("invalid format");

					if (pFrame->Size().Width() != m_param->GetHandle()->sourceWidth || pFrame->Size().Height() != m_param->GetHandle()->sourceHeight)
						throw CommonLib::CExcBase("invalid size");


					const int y_stride = m_picture->GetHandle()->stride[0];
					const int uv_stride = m_picture->GetHandle()->stride[1];
					uint8_t* y_data = (uint8_t*)m_picture->GetHandle()->planes[0];
					uint8_t* u_data = (uint8_t*)m_picture->GetHandle()->planes[1];
					uint8_t* v_data = (uint8_t*)m_picture->GetHandle()->planes[2];

					CConvertPixels::ConvertToI420(pFrame, y_data, u_data, v_data, y_stride, uv_stride);

					x265_nal *pNals = nullptr;
					uint32_t iNal = 0;
					int ret = m_encoder->Encode(&pNals, &iNal, m_picture->GetHandle(), nullptr);
					if (ret < 0)
						throw CommonLib::CExcBase("Error encode");

					for (int i = 0; i < iNal; ++i)
					{
						m_file->Write(pNals[i].payload, pNals[i].sizeBytes);
					}
				}
				catch (std::exception& excSrc)
				{
					CommonLib::CExcBase::RegenExc("Failed to encode", excSrc);
					throw;
				}
			}

			void CFileVideoEncoderX265::Close()
			{
				try
				{
					x265_nal *pNals = nullptr;
					uint32_t iNal = 0;

					while (1)
					{
						int ret = m_encoder->Encode(&pNals, &iNal, NULL, NULL);
						if (ret == 0)
							break;

						if (ret < 0)
							throw CommonLib::CExcBase("Error encode flush");

						for (int i = 0; i < iNal; ++i)
						{
							m_file->Write(pNals[i].payload, pNals[i].sizeBytes);
						}
					}

					m_file->CloseFile();

					m_file.reset();
					m_encoder.reset();
					m_picture.reset();
					m_param.reset();


				}
				catch (std::exception& excSrc)
				{
					CommonLib::CExcBase::RegenExc("Failed to close", excSrc);
					throw;
				}
			}

		}
	}
}