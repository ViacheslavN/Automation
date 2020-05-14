#include "pch.h"
#include "../geometry.h"
#include "DisplayConfigurationMonitor.h"
#include "ScreenUtils.h"

namespace mrCommonLib
{
	namespace desktop
	{
		namespace win
		{
			bool CDisplayConfigurationMonitor::IsChanged()
			{
				CRect rect = CScreenUtils::FullScreenRect();
				if (!m_initialized)
				{
					m_initialized = true;
					m_rect = rect;
					return false;
				}

				if (rect == m_rect)
					return false;

				m_rect = rect;
				return true;
			}

			void CDisplayConfigurationMonitor::Reset()
			{
				m_initialized = false;
			}
		}
	}
}