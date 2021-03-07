#include "pch.h"
#include "VideoCodecSettingsPackage.h"

namespace mrCommonLib
{
	namespace proto
	{
		CVideoCodecSettingsPackage::CVideoCodecSettingsPackage() : m_encodeId(video::VIDEO_ENCODING_NODE)
		{

		}

		CVideoCodecSettingsPackage::~CVideoCodecSettingsPackage()
		{

		}

		video::EVideoEncoderId CVideoCodecSettingsPackage::GetEncodeId() const
		{
			return m_encodeId;
		}

		void CVideoCodecSettingsPackage::SetEncodeId(video::EVideoEncoderId encodeId)
		{
			m_encodeId = encodeId;
		}

		void CVideoCodecSettingsPackage::Read(CommonLib::IReadStream *pStream)
		{

		}

	}
}