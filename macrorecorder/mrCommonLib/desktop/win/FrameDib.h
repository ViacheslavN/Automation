#pragma once

#include "../BaseFrame.h"
#include "Hbitmap.h"
namespace mrCommonLib
{
	namespace desktop
	{
		namespace win
		{

			class CFrameDib : public IBaseFrame
			{
				public:

					static	IFramePtr Create(const CSize& size, const CPixelFormat& format, HDC hdc);
					HBITMAP Bitmap();

					virtual byte_t* Data() const;

				private:
					CFrameDib(HBitmapPtr bitmap);
				private:
					HBitmapPtr m_bitmap;
			};

		}
	}
}