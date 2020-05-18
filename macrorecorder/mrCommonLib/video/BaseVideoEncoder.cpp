#include "pch.h"
#include "BaseVideoEncoder.h"
#include "VideoUtil.h"

namespace mrCommonLib
{
	namespace video
	{

		void IBaseVideoEncoder::FillPacketInfo(EVideoEncoderId encodingID, const desktop::IFrame* frame, CVideoPackage *pVideoPackage)
		{
			pVideoPackage->SetEncoderId(encodingID);
			pVideoPackage->SetPixelFormat(frame->Format());

			if (m_lastSize != frame->Size())
			{
				m_lastSize = frame->Size();
				pVideoPackage->SetChange();
			}

			pVideoPackage->SetScreenRect(desktop::CRect::MakeSize(m_lastSize));
		}

	}
}
