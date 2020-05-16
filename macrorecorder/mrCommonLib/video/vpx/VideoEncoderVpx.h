#pragma once

#include "../BaseVideoEncoder.h"
#include "vpx.h"
#include "vpx_codec.h"

namespace mrCommonLib
{
	namespace video
	{
		namespace vpxlib
		{
			class CVideoEncoderVPX : public IBaseVideoEncoder
			{
			public:
				~CVideoEncoderVPX() = default;

				static IVideoEncoderPtr createVP8();
				static IVideoEncoderPtr createVP9();
				virtual void Encode(desktop::IFrame* pFrame, CommonLib::IWriteStream* pStream);

			private:
				explicit CVideoEncoderVPX(EVideoEncoderId encodingId);

				void createActiveMap(const desktop::CSize& size);
				void createVp8Codec(const desktop::CSize& size);
				void createVp9Codec(const desktop::CSize& size);
				void prepareImageAndActiveMap(const desktop::IFrame* frame, CommonLib::IWriteStream* pStream);
				void setActiveMap(const desktop::CRect& rect);

				EVideoEncoderId m_encodingId;

				desktop::CRegion updated_region_;
				VpxCodecPtr m_codec;

				std::vector<uint8_t> active_map_buffer_;
				vpx_active_map_t active_map_;

				// VPX image and buffer to hold the actual YUV planes.
				std::unique_ptr<vpx_image_t> image_;
				std::vector<uint8_t> image_buffer_;

			};
		}
	}
}
