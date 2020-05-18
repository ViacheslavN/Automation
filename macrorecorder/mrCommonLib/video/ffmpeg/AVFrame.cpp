#include "pch.h"
#include "FFmpeg.h"
#include "AVFrame.h"
#include "BaseCodec.h"

namespace mrCommonLib
{
	namespace video
	{
		namespace ffmpeglib
		{
			CAVFrame::CAVFrame()
			{
				m_frame = av_frame_alloc();
				if (m_frame == nullptr)
					throw CommonLib::CExcBase("Failed to frame alloc");
			}

			CAVFrame::CAVFrame(CBaseCodec *pCodec)
			{
				m_frame = av_frame_alloc();
				if (m_frame == nullptr)
					throw CommonLib::CExcBase("Failed to frame alloc");

				AVCodecContext* ctx =pCodec->GetContextExc();

				m_frame->format = ctx->pix_fmt;
				m_frame->width = ctx->width;
				m_frame->height = ctx->height;

				int ret = av_frame_get_buffer(m_frame, 32);
				if (ret < 0)
					throw CommonLib::CExcBase("Failed to alloc the frame data");

				ret = av_frame_make_writable(m_frame);
				if (ret < 0)
					throw CommonLib::CExcBase("Failed to make frame writable");
			}

			CAVFrame::~CAVFrame()
			{
				if (m_frame != nullptr)
					av_frame_free(&m_frame);
			}

			AVFrame * CAVFrame::GetFrame()
			{
				return m_frame;
			}

		}
	}
}