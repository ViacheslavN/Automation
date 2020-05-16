#include "pch.h"
#include "BaseVideoEncoder.h"
#include "VideoUtil.h"

namespace mrCommonLib
{
	namespace video
	{

		void IBaseVideoEncoder::FillPacketInfo(EVideoEncoderId encodingID, const desktop::IFrame* frame, CommonLib::IWriteStream *pStream)
		{
			pStream->Write((uint32_t)encodingID);

			if (m_lastSize != frame->Size())
			{
				m_lastSize = frame->Size();
			}

			CVideoUtil::WriteRect(desktop::CRect::MakeSize(m_lastSize), pStream);

		}

	}
}
