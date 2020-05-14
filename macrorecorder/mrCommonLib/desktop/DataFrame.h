#pragma once

#include "BaseFrame.h"

namespace mrCommonLib
{
	namespace desktop
	{
		class CDataFrame : public IBaseFrame
		{
			public:
				CDataFrame(const CPixelFormat& format, const CSize& size);
				CDataFrame(const CPixelFormat& format, const CSize& size, CommonLib::IAllocPtr pAlloc);
				CDataFrame(const CPixelFormat& format, const CSize& size, byte_t *data);
				virtual ~CDataFrame();

				virtual byte_t* Data() const;

		private:
			byte_t *m_data;
			std::shared_ptr<CommonLib::IAlloc> m_pAlloc;
		};
	}
}