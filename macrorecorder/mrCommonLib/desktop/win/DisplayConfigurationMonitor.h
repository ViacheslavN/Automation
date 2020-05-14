#pragma once
namespace mrCommonLib
{
	namespace desktop
	{
		namespace win
		{
			class CDisplayConfigurationMonitor
			{

			public:
				bool IsChanged();
				void Reset();

			private:
				CRect m_rect;
				bool m_initialized = false;
			};

		}
	}
}