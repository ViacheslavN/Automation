#include "pch.h"
#include "../pixelformat.h"
#include "../geometry.h"
#include "FrameDib.h"
#include "Hbitmap.h"
#include "BitmapInfo.h"
 

namespace mrCommonLib
{
	namespace desktop
	{
		namespace win
		{
			IFramePtr CFrameDib::Create(const CSize& size, const CPixelFormat& format, HDC hdc)
			{
				try
				{
					HBitmapPtr bitmap = CHBitmap::Ñreate(size, format, hdc);
					return IFramePtr(new CFrameDib(bitmap));
				}
				catch (std::exception& exc)
				{
					CommonLib::CExcBase::RegenExcT("Failed to create FrameDib", exc);
					throw;
				}
			}

			CFrameDib::CFrameDib(HBitmapPtr bitmap) : IBaseFrame(bitmap->GetPixelFormat(), bitmap->GetSize())
			{
				m_bitmap = bitmap;
			}

			byte_t* CFrameDib::Data() const
			{
				return m_bitmap->GetData();
			}

			HBITMAP CFrameDib::Bitmap()
			{
				return m_bitmap->GetBitmap();
			}

		}
	}
}