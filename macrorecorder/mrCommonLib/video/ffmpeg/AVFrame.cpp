#include "pch.h"
#include "FFmpeg.h"
#include "AVFrame.h"

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