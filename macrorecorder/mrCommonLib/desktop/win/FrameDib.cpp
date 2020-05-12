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

			CFrameDib::CFrameDib(HBitmapPtr bitmap) : m_bitmap(bitmap)
			{
				m_format = m_bitmap->GetPixelFormat();
				m_size = m_bitmap->GetSize();
				m_stride = m_format.BytesPerPixel() * m_size.Width();

				m_updated_region.reset(new CRegion());
			}

			byte_t* CFrameDib::DataAtPos(const CPoint& pos) const
			{
				return DataAtPos(pos.X(), pos.Y());
			}

			byte_t* CFrameDib::DataAtPos(int x, int y) const
			{
				return Data() + (Stride()*y + m_format.BytesPerPixel()*x);
			}

			byte_t* CFrameDib::Data() const
			{
				return m_bitmap->GetData();
			}

			const CSize& CFrameDib::Size() const
			{
				return m_size;
			}

			const CPixelFormat& CFrameDib::Format() const
			{
				return m_format;
			}

			int CFrameDib::Stride() const
			{
				return m_stride;
			}

			bool CFrameDib::Contains(int x, int y) const
			{
				return (x >= 0 && x <= m_size.Width() && y >= 0 && y <= m_size.Height());
			}

			void CFrameDib::CopyPixelsFrom(const byte_t* src_buffer, int src_stride, const CRect& dest_rect)
			{
				uint8_t* dest = DataAtPos(dest_rect.TopLeft());
				size_t bytes_per_row = m_format.BytesPerPixel() * dest_rect.Width();

				for (int y = 0; y < dest_rect.Height(); ++y)
				{
					memcpy(dest, src_buffer, bytes_per_row);
					src_buffer += src_stride;
					dest += Stride();
				}
			}

			void CFrameDib::CopyPixelsFrom(IFramePtr src_frame, const CPoint& src_pos, const CRect& dest_rect)
			{
				CopyPixelsFrom(src_frame->DataAtPos(src_pos), src_frame->Stride(), dest_rect);
			}

			void CFrameDib::CopyFrameInfoFrom(IFramePtr other)
			{
				/*CSize size = other->Size();
				CPixelFormat format = other->Format();

				if (format != m_format)
					CommonLib::CExcBase("Failed copy frame, diffrent formats");

				if (m_size != size)
					CommonLib::CExcBase("Failed copy frame, diffrent size");


				CopyPixelsFrom(other->DataAtPos(0, 0), Stride(), CRect::MakeSize(size));*/

				m_updated_region = other->UpdatedRegion();

			}

			HBITMAP CFrameDib::Bitmap()
			{
				return m_bitmap->GetBitmap();
			}

			CRegionPtr CFrameDib::UpdatedRegion()
			{
				return m_updated_region;
			}
		}
	}
}