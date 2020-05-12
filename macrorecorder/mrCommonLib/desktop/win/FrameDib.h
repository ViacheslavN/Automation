#pragma once

#include "../Frame.h"
#include "Hbitmap.h"
namespace mrCommonLib
{
	namespace desktop
	{
		namespace win
		{

			class CFrameDib : public IFrame
			{
				public:

					virtual byte_t* DataAtPos(const CPoint& pos) const;
					virtual byte_t* DataAtPos(int x, int y) const;
					virtual byte_t* Data() const;
					virtual const CSize& Size() const;
					virtual const CPixelFormat& Format() const;
					virtual int Stride() const ;
					virtual bool Contains(int x, int y) const ;

					virtual void CopyPixelsFrom(const byte_t* src_buffer, int src_stride, const CRect& dest_rect);
					virtual void CopyPixelsFrom(IFramePtr src_frame, const CPoint& src_pos, const CRect& dest_rect);
					virtual void CopyFrameInfoFrom(IFramePtr other);

					virtual CRegionPtr UpdatedRegion();

					static	IFramePtr Create(const CSize& size, const CPixelFormat& format, HDC hdc);

					HBITMAP Bitmap();



				private:
					CFrameDib(HBitmapPtr bitmap);
				private:
					HBitmapPtr m_bitmap;
					CPixelFormat m_format;
					CSize m_size;
					uint32_t m_stride;
					CRegionPtr m_updated_region;
			};

		}
	}
}