#include "pch.h"
#include "VideoEncoderVpx.h"
#include "../../../../../ThirdParty/libyuv/include/libyuv/convert.h"
#include "../../../../../ThirdParty/libyuv/include/libyuv/convert_from_argb.h"
#include "VPXException.h"
#include "../VideoUtil.h"
#include <thread>

namespace mrCommonLib
{
	namespace video
	{
		namespace vpxlib
		{
			namespace {

				// Defines the dimension of a macro block. This is used to compute the active map for the encoder.
				const int kMacroBlockSize = 16;

				// Magic encoder profile numbers for I444 input formats.
				const int kVp9I420ProfileNumber = 0;

				// Magic encoder constant for adaptive quantization strategy.
				const int kVp9AqModeCyclicRefresh = 3;

				void setCommonCodecParameters(vpx_codec_enc_cfg_t* config, const desktop::CSize& size)
				{
					// Use millisecond granularity time base.
					config->g_timebase.num = 1;
					config->g_timebase.den = 1000;

					config->g_w = size.Width();
					config->g_h = size.Height();
					config->g_pass = VPX_RC_ONE_PASS;

					// Start emitting packets immediately.
					config->g_lag_in_frames = 0;

					// Since the transport layer is reliable, keyframes should not be necessary.
					// However, due to crbug.com/440223, decoding fails after 30,000 non-key
					// frames, so take the hit of an "unnecessary" key-frame every 10,000 frames.
					config->kf_min_dist = 10000;
					config->kf_max_dist = 10000;

					// Using 2 threads gives a great boost in performance for most systems with
					// adequate processing power. NB: Going to multiple threads on low end
					// windows systems can really hurt performance.
					// http://crbug.com/99179
					config->g_threads = (std::thread::hardware_concurrency() > 2) ? 2 : 1;
				}

				void CreateImage(const desktop::CSize& size,
					std::unique_ptr<vpx_image_t>* out_image,
					std::vector<byte_t>* out_image_buffer)
				{
					std::unique_ptr<vpx_image_t> image = std::make_unique<vpx_image_t>();

					memset(image.get(), 0, sizeof(vpx_image_t));

					image->d_w = image->w = size.Width();
					image->d_h = image->h = size.Height();

					image->fmt = VPX_IMG_FMT_YV12;
					image->x_chroma_shift = 1;
					image->y_chroma_shift = 1;

					// libyuv's fast-path requires 16-byte aligned pointers and strides, so pad the Y, U and V
					// planes' strides to multiples of 16 bytes.
					const int y_stride = ((image->w - 1) & ~15) + 16;
					const int uv_unaligned_stride = y_stride >> image->x_chroma_shift;
					const int uv_stride = ((uv_unaligned_stride - 1) & ~15) + 16;

					// libvpx accesses the source image in macro blocks, and will over-read if the image is not
					// padded out to the next macroblock: crbug.com/119633.
					// Pad the Y, U and V planes' height out to compensate.
					// Assuming macroblocks are 16x16, aligning the planes' strides above also macroblock aligned
					// them.
					const int y_rows = ((image->h - 1) & ~(kMacroBlockSize - 1)) + kMacroBlockSize;

					const int uv_rows = y_rows >> image->y_chroma_shift;

					std::vector<byte_t> image_buffer;

					// Allocate a YUV buffer large enough for the aligned data & padding.
					image_buffer.resize(y_stride * y_rows + (2 * uv_stride) * uv_rows);

					// Reset image value to 128 so we just need to fill in the y plane.
					memset(image_buffer.data(), 128, image_buffer.size());

					// Fill in the information.
					image->planes[0] = image_buffer.data();
					image->planes[1] = image->planes[0] + y_stride * y_rows;
					image->planes[2] = image->planes[1] + uv_stride * uv_rows;

					image->stride[0] = y_stride;
					image->stride[1] = image->stride[2] = uv_stride;

					*out_image = std::move(image);
					*out_image_buffer = std::move(image_buffer);
				}

				int RoundToTwosMultiple(int x)
				{
					return x & (~1);
				}

				desktop::CRect AlignRect(const desktop::CRect& rect)
				{
					int x = RoundToTwosMultiple(rect.Left());
					int y = RoundToTwosMultiple(rect.Top());
					int right = RoundToTwosMultiple(rect.Right() + 1);
					int bottom = RoundToTwosMultiple(rect.Bottom() + 1);

					return desktop::CRect::MakeLTRB(x, y, right, bottom);
				}

			} // namespace


			IVideoEncoderPtr CVideoEncoderVPX::createVP8()
			{
				return IVideoEncoderPtr(new CVideoEncoderVPX(VIDEO_ENCODING_VP8));
			}


			IVideoEncoderPtr CVideoEncoderVPX::createVP9()
			{
				return IVideoEncoderPtr(new CVideoEncoderVPX(VIDEO_ENCODING_VP9));
			}

			CVideoEncoderVPX::CVideoEncoderVPX(EVideoEncoderId encodingId)
				: m_encodingId(encodingId)
			{
			}

			void CVideoEncoderVPX::createActiveMap(const desktop::CSize& size)
			{
				m_activeMap.cols = (size.Width() + kMacroBlockSize - 1) / kMacroBlockSize;
				m_activeMap.rows = (size.Height() + kMacroBlockSize - 1) / kMacroBlockSize;

				m_activeMapBuffer.resize(m_activeMap.cols * m_activeMap.rows);
				memset(m_activeMapBuffer.data(), 0, m_activeMapBuffer.size());
				m_activeMap.active_map = m_activeMapBuffer.data();
			}

			void CVideoEncoderVPX::createVp8Codec(const desktop::CSize& size)
			{
				try
				{
					m_codec.reset(new vpx_codec_ctx_t());

					vpx_codec_enc_cfg_t config = { 0 };

					// Configure the encoder.
					vpx_codec_iface_t* algo = vpx_codec_vp8_cx();

					vpx_codec_err_t ret = vpx_codec_enc_config_default(algo, &config, 0);
					if (ret != VPX_CODEC_OK)
						throw CVPXException(ret);


					// Adjust default target bit-rate to account for actual desktop size.
					config.rc_target_bitrate = size.Width() * size.Height() *
						config.rc_target_bitrate / config.g_w / config.g_h;

					setCommonCodecParameters(&config, size);

					// Value of 2 means using the real time profile. This is basically a redundant option since we
					// explicitly select real time mode when doing encoding.
					config.g_profile = 2;

					// Clamping the quantizer constrains the worst-case quality and CPU usage.
					config.rc_min_quantizer = 20;
					config.rc_max_quantizer = 30;

					ret = vpx_codec_enc_init(m_codec.get(), algo, &config, 0);
					if (ret != VPX_CODEC_OK)
						throw CVPXException(ret, "failed enc init ");

					// Value of 16 will have the smallest CPU load. This turns off subpixel motion search.
					ret = vpx_codec_control(m_codec.get(), VP8E_SET_CPUUSED, 16);
					if (ret != VPX_CODEC_OK)
						Log.Warning("Failed vpx codec control  VP8E_SET_CPUUSED %1", CVPXException::GetVPXLibErrorDesc(ret));

					ret = vpx_codec_control(m_codec.get(), VP8E_SET_SCREEN_CONTENT_MODE, 1);
					if (ret != VPX_CODEC_OK)
						Log.Warning("Failed vpx codec control VP8E_SET_SCREEN_CONTENT_MODE %1", CVPXException::GetVPXLibErrorDesc(ret));

					// Use the lowest level of noise sensitivity so as to spend less time on motion estimation and
					// inter-prediction mode.
					ret = vpx_codec_control(m_codec.get(), VP8E_SET_NOISE_SENSITIVITY, 0);
					if (ret != VPX_CODEC_OK)
						Log.Warning("Failed vpx codec control VP8E_SET_NOISE_SENSITIVITY %1", CVPXException::GetVPXLibErrorDesc(ret));
				}
				catch (std::exception& excSrc)
				{
					CommonLib::CExcBase::RegenExc("Failed to create vp8", excSrc);
					throw;
				}
			}

			void CVideoEncoderVPX::createVp9Codec(const desktop::CSize& size)
			{
				try
				{
					m_codec.reset(new vpx_codec_ctx_t());

					vpx_codec_enc_cfg_t config = { 0 };

					// Configure the encoder.
					vpx_codec_iface_t* algo = vpx_codec_vp9_cx();

					vpx_codec_err_t ret = vpx_codec_enc_config_default(algo, &config, 0);
					if (ret != VPX_CODEC_OK)
						throw CVPXException(ret);

					setCommonCodecParameters(&config, size);

					// Configure VP9 for I420 source frames.
					config.g_profile = kVp9I420ProfileNumber;
					config.rc_min_quantizer = 20;
					config.rc_max_quantizer = 30;
					config.rc_end_usage = VPX_CBR;

					// In the absence of a good bandwidth estimator set the target bitrate to a
					// conservative default.
					config.rc_target_bitrate = 500;

					ret = vpx_codec_enc_init(m_codec.get(), algo, &config, 0);
					if (ret != VPX_CODEC_OK)
						throw CVPXException(ret, "failed enc init");

					// Request the lowest-CPU usage that VP9 supports, which depends on whether we are encoding
					// lossy or lossless.
					ret = vpx_codec_control(m_codec.get(), VP8E_SET_CPUUSED, 6);
					if (ret != VPX_CODEC_OK)
						Log.Warning("Failed vpx codec control  VP8E_SET_CPUUSED %1", CVPXException::GetVPXLibErrorDesc(ret));

					ret = vpx_codec_control(m_codec.get(), VP9E_SET_TUNE_CONTENT, VP9E_CONTENT_SCREEN);
					if (ret != VPX_CODEC_OK)
						Log.Warning("Failed vpx codec control  VP9E_SET_TUNE_CONTENT %1", CVPXException::GetVPXLibErrorDesc(ret));

					// Use the lowest level of noise sensitivity so as to spend less time on motion estimation and
					// inter-prediction mode.
					ret = vpx_codec_control(m_codec.get(), VP8E_SET_NOISE_SENSITIVITY, 0);
					if (ret != VPX_CODEC_OK)
						Log.Warning("Failed vpx codec control  VP8E_SET_NOISE_SENSITIVITY %1", CVPXException::GetVPXLibErrorDesc(ret));

					// Set cyclic refresh (aka "top-off") only for lossy encoding.
					ret = vpx_codec_control(m_codec.get(), VP9E_SET_AQ_MODE, kVp9AqModeCyclicRefresh);
					if (ret != VPX_CODEC_OK)
						Log.Warning("Failed vpx codec control  VP9E_SET_AQ_MODE %1", CVPXException::GetVPXLibErrorDesc(ret));
				}
				catch (std::exception& excSrc)
				{
					CommonLib::CExcBase::RegenExc("Failed to create vpx9", excSrc);
					throw;
				}
			}

			void CVideoEncoderVPX::setActiveMap(const desktop::CRect& rect)
			{
				int left = rect.Left() / kMacroBlockSize;
				int top = rect.Top() / kMacroBlockSize;
				int right = (rect.Right() - 1) / kMacroBlockSize;
				int bottom = (rect.Bottom() - 1) / kMacroBlockSize;

				uint8_t* map = m_activeMap.active_map + top * m_activeMap.cols;

				for (int y = top; y <= bottom; ++y)
				{
					for (int x = left; x <= right; ++x)
					{
						map[x] = 1;
					}

					map += m_activeMap.cols;
				}
			}

			void CVideoEncoderVPX::prepareImageAndActiveMap(
				const desktop::IFrame* frame, CVideoPackage *pVideoPackage)
			{
				const int padding = ((m_encodingId == VIDEO_ENCODING_VP9) ? 8 : 3);

				for (desktop::CRegion::Iterator it(frame->UpdatedRegionConst()); !it.IsAtEnd(); it.Advance())
				{
					const desktop::CRect& rect = it.Rect();

					// Pad each rectangle to avoid the block-artefact filters in libvpx from introducing
					// artefacts; VP9 includes up to 8px either side, and VP8 up to 3px, so unchanged pixels
					// up to that far out may still be affected by the changes in the updated region, and so
					// must be listed in the active map. After padding we align each rectangle to 16x16
					// active-map macroblocks. This implicitly ensures all rects have even top-left coords,
					// which is is required by ARGBToI420().
					m_updatedRegion.AddRect(
						AlignRect(desktop::CRect::MakeLTRB(rect.Left() - padding, rect.Top() - padding,
							rect.Right() + padding, rect.Bottom() + padding)));
				}

				// Clip back to the screen dimensions, in case they're not macroblock aligned. The conversion
				// routines don't require even width & height, so this is safe even if the source dimensions
				// are not even.
				m_updatedRegion.IntersectWith(desktop::CRect::MakeWH(m_image->w, m_image->h));

				memset(m_activeMapBuffer.data(), 0, m_activeMapBuffer.size());

				const int y_stride = m_image->stride[0];
				const int uv_stride = m_image->stride[1];
				uint8_t* y_data = m_image->planes[0];
				uint8_t* u_data = m_image->planes[1];
				uint8_t* v_data = m_image->planes[2];

				auto convert_to_i420 = libyuv::ARGBToI420;

				if (frame->Format().BitsPerPixel() == 16)
					convert_to_i420 = libyuv::RGB565ToI420;

				for (desktop::CRegion::Iterator it(m_updatedRegion); !it.IsAtEnd(); it.Advance())
				{
					const desktop::CRect& rect = it.Rect();

					const int y_offset = y_stride * rect.Y() + rect.X();
					const int uv_offset = uv_stride * rect.Y() / 2 + rect.X() / 2;

					convert_to_i420(frame->DataAtPos(rect.TopLeft()),
						frame->Stride(),
						y_data + y_offset, y_stride,
						u_data + uv_offset, uv_stride,
						v_data + uv_offset, uv_stride,
						rect.Width(),
						rect.Height());

					pVideoPackage->AddDirtyRect(rect);
					setActiveMap(rect);
				}
			}

			void CVideoEncoderVPX::Encode(desktop::IFrame* pFrame, CVideoPackage *pVideoPackage)
			{
				try
				{
					FillPacketInfo(m_encodingId, pFrame, pVideoPackage);

					if (m_codec.get() == 0 || pVideoPackage->IsChange())
					{
						const desktop::CSize& screen_size = pFrame->Size();

						CreateImage(screen_size, &m_image, &m_imageBuffer);
						createActiveMap(screen_size);

						if (m_encodingId == VIDEO_ENCODING_VP8)
						{
							createVp8Codec(screen_size);
						}
						else
						{
							createVp9Codec(screen_size);
						}

						m_updatedRegion = desktop::CRegion(desktop::CRect::MakeSize(screen_size));
					}
					else
					{
						m_updatedRegion.Clear();
					}

					// Convert the updated capture data ready for encode.
					// Update active map based on updated region.
					prepareImageAndActiveMap(pFrame, pVideoPackage);

					// Apply active map to the encoder.
					vpx_codec_err_t ret = vpx_codec_control(m_codec.get(), VP8E_SET_ACTIVEMAP, &m_activeMap);
					//DCHECK_EQ(ret, VPX_CODEC_OK);

					// Do the actual encoding.
					ret = vpx_codec_encode(m_codec.get(), m_image.get(), 0, 1, 0, VPX_DL_REALTIME);
					//DCHECK_EQ(ret, VPX_CODEC_OK);

					// Read the encoded data.
					vpx_codec_iter_t iter = nullptr;

					while (true)
					{
						const vpx_codec_cx_pkt_t* pkt = vpx_codec_get_cx_data(m_codec.get(), &iter);
						if (!pkt)
							break;

						if (pkt->kind == VPX_CODEC_CX_FRAME_PKT)
						{
							pVideoPackage->AddEncodeData((const byte_t*)pkt->data.frame.buf, pkt->data.frame.sz);
							break;
						}
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