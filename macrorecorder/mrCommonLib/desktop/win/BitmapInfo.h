#pragma once
namespace mrCommonLib
{
	namespace desktop
	{
		namespace win
		{
			struct CBitmapInfo
			{
				BITMAPINFOHEADER header;
				union
				{
					struct
					{
						uint32_t red;
						uint32_t green;
						uint32_t blue;
					} mask;
					RGBQUAD color[256];
				} u;
			};
		}
	}
}