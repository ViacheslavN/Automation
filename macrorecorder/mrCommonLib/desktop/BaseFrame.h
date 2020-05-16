#pragma once


#include "Frame.h"

namespace mrCommonLib
{
	namespace desktop
	{

		class IBaseFrame : public IFrame
		{
		public:


			IBaseFrame(const CPixelFormat& format, const CSize& size) : m_format(format), m_size(size)
			{
				m_stride = 4 * ((m_size.Width() * m_format.BitsPerPixel()  + 31) / 32);
			}

			virtual ~IBaseFrame()
			{

			}

			virtual byte_t* DataAtPos(const CPoint& pos) const
			{
				return DataAtPos(pos.X(), pos.Y());
			}

			virtual byte_t* DataAtPos(int x, int y) const
			{
				return Data() + (Stride()*y + m_format.BytesPerPixel()*x);
			}

			virtual const CSize& Size() const
			{
				return m_size;
			}

			virtual const CPixelFormat& Format() const
			{
				return m_format;
			}

			virtual int Stride() const
			{
				return m_stride;
			}

			virtual bool Contains(int x, int y) const
			{
				return (x >= 0 && x <= m_size.Width() && y >= 0 && y <= m_size.Height());
			}

			virtual void CopyPixelsFrom(const byte_t* src_buffer, int src_stride, const CRect& dest_rect)
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

			virtual void CopyPixelsFrom(IFramePtr src_frame, const CPoint& src_pos, const CRect& dest_rect)
			{
				CopyPixelsFrom(src_frame->DataAtPos(src_pos), src_frame->Stride(), dest_rect);
			}

			virtual void CopyFrameInfoFrom(IFramePtr other)
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


			virtual CRegion& UpdatedRegion()
			{
				return m_updated_region;
			}

			virtual const CRegion& UpdatedRegionConst() const
			{
				return m_updated_region;
			}

		protected:

			CPixelFormat m_format;
			CSize m_size;
			uint32_t m_stride;
			CRegion m_updated_region;
		};
	}
}