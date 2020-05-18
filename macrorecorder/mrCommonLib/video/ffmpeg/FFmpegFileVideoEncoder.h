#pragma once

#include "../BaseVideoEncoder.h"
#include "BaseCodec.h"

namespace mrCommonLib
{
	namespace video
	{
		namespace ffmpeglib
		{
			class CAVPacket;
			class CAVFrame;
			class CBaseCodec;

			class CFFmpegFileVideoEncoder : IVideoFileEncoder
			{
			public:
				CFFmpegFileVideoEncoder(EVideoEncoderId encodeId);
				virtual ~CFFmpegFileVideoEncoder();

				virtual void OpenFile(const char* pszFileName, uint32_t width, uint32_t height, const desktop::CPixelFormat& format);
				virtual void Encode(desktop::IFrame* pFrame);
				virtual void Close();

			private:
				std::unique_ptr<CBaseCodec> m_codec;
				EVideoEncoderId m_encodeId;
				std::unique_ptr<CAVFrame>  m_frame;
				std::unique_ptr<CAVPacket>  m_packet;
				uint32_t m_frameCount = 0;
				CommonLib::file::TFilePtr m_file;
				desktop::CPixelFormat m_format;
			};
		}
	}
}