#pragma once
#include "Packages.h"

namespace mrCommonLib
{
	namespace proto
	{
		class CPackegHeader
		{
		public:
			CPackegHeader() =default;
			~CPackegHeader() = default;
			uint32_t GetSize() const;
			ePakageType GetType() const;

			void Read(CommonLib::IReadStream *pStream);
		private:
			uint32_t m_nSize = 0;
			uint32_t m_type;
		};

	 
	}
}