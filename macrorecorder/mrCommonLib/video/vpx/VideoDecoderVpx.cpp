#include "pch.h"
#include "VideoDecoderVpx.h"
#include "VPXException.h"
#include "../FormatConvertor.h"

namespace mrCommonLib
{
	namespace video
	{
		namespace vpxlib
		{
			namespace
			{

				void ConvertImage(const CVideoPackage *pVideoPackage, vpx_image_t* image, desktop::IFrame* frame)
				{
					try
					{
						if (image->fmt != VPX_IMG_FMT_I420)
							throw CommonLib::CExcBase("fmt != VPX_IMG_FMT_I420");

						desktop::CRect frame_rect = desktop::CRect::MakeSize(frame->Size());

						uint8_t* y_data = image->planes[0];
						uint8_t* u_data = image->planes[1];
						uint8_t* v_data = image->planes[2];

						int y_stride = image->stride[0];
						int uv_stride = image->stride[1];

						for (int i = 0; i < pVideoPackage->GetDirtyRectCount(); ++i)
						{
							desktop::CRect rect = pVideoPackage->GetDirtyRect(i);

							if (!frame_rect.ContainsRect(rect))
							{
								throw CommonLib::CExcBase("The rectangle is outside the screen area");
							}

							int y_offset = y_stride * rect.Y() + rect.X();
							int uv_offset = uv_stride * rect.Y() / 2 + rect.X() / 2;

							CConvertPixels::ConvertI420ToARGB(y_data + y_offset, u_data + uv_offset, v_data + uv_offset, y_stride, uv_stride, frame->DataAtPos(rect.TopLeft()), rect.Width(), rect.Height(), frame->Stride());
						}
					}
					catch (std::exception& excSrc)
					{
						CommonLib::CExcBase::RegenExc("Failed to convert image", excSrc);
						throw;
					}
				}
			}


			IVideoDecoderPtr CVideoDecoderVPX::CreateVP8()
			{
				return IVideoDecoderPtr(new CVideoDecoderVPX(VIDEO_ENCODING_VP8));
			}

			IVideoDecoderPtr CVideoDecoderVPX::CreateVP9()
			{
				return IVideoDecoderPtr(new CVideoDecoderVPX(VIDEO_ENCODING_VP9));
			}

			CVideoDecoderVPX::CVideoDecoderVPX(EVideoEncoderId encoding) : m_encodingId(encoding)
			{
				m_codec.reset(new vpx_codec_ctx_t());

				vpx_codec_dec_cfg_t config;

				config.w = 0;
				config.h = 0;
				config.threads = 2;

				vpx_codec_iface_t* algo;

				switch (encoding)
				{
				case VIDEO_ENCODING_VP8:
					algo = vpx_codec_vp8_dx();
					break;

				case VIDEO_ENCODING_VP9:
					algo = vpx_codec_vp9_dx();
					break;

				default:
					throw CommonLib::CExcBase("Unsupported video encoding %1", encoding);
				}

				vpx_codec_err_t ret = vpx_codec_dec_init(m_codec.get(), algo, &config, 0);
				if (ret != VPX_CODEC_OK)
					throw CVPXException(ret, "filed to init codec");

			}

			EVideoEncoderId CVideoDecoderVPX::GetEncoderId() const
			{
				return m_encodingId;
			}

			void CVideoDecoderVPX::Decode(desktop::IFrame* pFrame, const CVideoPackage *pVideoPackage, bool& isSkip)
			{
				try
				{
					isSkip = false;

					vpx_codec_err_t ret =
						vpx_codec_decode(m_codec.get(), pVideoPackage->GetEncodeData(), static_cast<unsigned int>(pVideoPackage->GetEncodeDataSize()),
							nullptr,
							0);
					if (ret != VPX_CODEC_OK)
					{
						const char* error = vpx_codec_error(m_codec.get());
						const char* error_detail = vpx_codec_error_detail(m_codec.get());

						throw CommonLib::CExcBase("Decoding failed %1, Details: %2", error ? error : "(NULL)", error_detail ? error_detail : "(NULL)");
					}

					vpx_codec_iter_t iter = nullptr;

					// Gets the decoded data.
					vpx_image_t* image = vpx_codec_get_frame(m_codec.get(), &iter);
					if (!image)
					{
						throw CommonLib::CExcBase("No video frame decoded");
					}

					if (desktop::CSize(image->d_w, image->d_h) != pFrame->Size())
					{
						throw CommonLib::CExcBase("Size of the encoded frame doesn't match size in the header");
					}

					ConvertImage(pVideoPackage, image, pFrame);
				}
				catch (std::exception& excSrc)
				{
					CommonLib::CExcBase::RegenExc("Failed to convert image", excSrc);
					throw;
				}
			}
		}
		
	}
}
