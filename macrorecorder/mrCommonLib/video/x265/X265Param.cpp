#include "pch.h"
#include "x265Header.h"
#include "X265Param.h"

namespace mrCommonLib
{
	namespace video
	{
		namespace x265lib
		{
			CX265Param::CX265Param()
			{
				m_param = x265_param_alloc();
				x265_param_default(m_param);
				m_param->logLevel = X265_LOG_NONE;
			}

			CX265Param::~CX265Param()
			{
				if (m_param != nullptr)
					x265_param_free(m_param);
			}

			x265_param* CX265Param::GetHandle() const
			{
				return m_param;
			}
		}
	}
}