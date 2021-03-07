#include "pch.h"
#include "PackageHeader.h"


namespace mrCommonLib
{
	namespace proto
	{
		uint32_t CPackegHeader::GetSize() const
		{
			return m_nSize;
		}

		ePakageType CPackegHeader::GetType() const
		{
			return (ePakageType)m_type;
		}

		void CPackegHeader::Read(CommonLib::IReadStream *pReadDev)
		{
			m_nSize = pReadDev->ReadIntu32();
			m_type = pReadDev->ReadIntu32();
		}
	}
}