#pragma once
namespace mrCommonLib
{
	namespace desktop
	{
		class CMouseCursor;
		namespace win
		{

			class CCursorUtil
			{
			public:
				static std::shared_ptr<CMouseCursor> GetMouseCursorFromHCursor(HDC dc, HCURSOR cursor);
			};
		}

	}
}
