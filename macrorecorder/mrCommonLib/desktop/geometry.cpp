#include "pch.h"
#include "geometry.h"

namespace mrCommonLib
{
	namespace desktop
	{


		CPoint::CPoint(int32_t x, int32_t y): m_x(x),m_y(y)
		{}

		CPoint::CPoint(const CPoint& point)	: m_x(point.m_x),m_y(point.m_y)
		{}

 
		int32_t CPoint::X() const { return m_x; }
		int32_t CPoint::Y() const { return m_y; }

		void CPoint::Set(int32_t x, int32_t y)
		{
			m_x = x;
			m_y = y;
		}

		CPoint CPoint::Add(const CPoint& other) const
		{
			return CPoint(X() + other.X(), Y() + other.Y());
		}

		CPoint CPoint::Subtract(const CPoint& other) const
		{
			return CPoint(X() - other.X(), Y() - other.Y());
		}

		bool CPoint::IsEquals(const CPoint& other) const
		{
			return (m_x == other.m_x && m_y == other.m_y);
		}

		void CPoint::Translate(int32_t x_offset, int32_t y_offset)
		{
			m_x += x_offset;
			m_y += y_offset;
		}

		void CPoint::Translate(const CPoint& offset) { Translate(offset.X(), offset.Y()); }

		CPoint& CPoint::operator=(const CPoint& other)
		{
			if (&other != this)
				Set(other.m_x, other.m_y);

			return *this;
		}

		bool CPoint::operator!=(const CPoint& other) const { return !IsEquals(other); }
		bool CPoint::operator==(const CPoint& other) const { return IsEquals(other); }

		/////////////////////////////////////////////////////////////////////////////////////////////////////

		CSize::CSize(int32_t width, int32_t height)
			: m_width(width),
			m_height(height)
		{}

		CSize::CSize(const CSize& other)
			: m_width(other.m_width),
			m_height(other.m_height)
		{
		}

		int32_t CSize::Width() const { return m_width; }
		int32_t CSize::Height() const { return m_height; }

		void CSize::Set(int32_t width, int32_t height)
		{
			m_width = width;
			m_height = height;
		}

		bool CSize::IsEmpty() const
		{
			return m_width <= 0 || m_height <= 0;
		}

		bool CSize::IsEquals(const CSize& other) const
		{
			return m_width == other.m_width && m_height == other.m_height;
		}

		void CSize::Clear()
		{
			m_width = 0;
			m_height = 0;
		}

		CSize& CSize::operator=(const CSize& other)
		{
			if (&other != this)
				Set(other.m_width, other.m_height);

			return *this;
		}

		bool CSize::operator!=(const CSize& other) const { return !IsEquals(other); }
		bool CSize::operator==(const CSize& other) const { return IsEquals(other); }

		///////////////////////////////////////////////////////////////////////////////////////////////////////


		CRect::CRect::CRect(const CRect& other)
			: m_left(other.m_left),
			m_top(other.m_top),
			m_right(other.m_right),
			m_bottom(other.m_bottom)
		{
			// Nothing
		}

		void CRect::SetTopLeft(const CPoint& top_left)
		{
			m_left = top_left.X();
			m_top = top_left.Y();
		}

		void CRect::SetSize(const CSize& size)
		{
			m_right = m_left + size.Width();
			m_bottom = m_top + size.Height();
		}

		bool CRect::Contains(int32_t x, int32_t y) const
		{
			return (x >= m_left && x < m_right && y >= m_top && y < m_bottom);
		}

		bool CRect::ContainsRect(const CRect& rect) const
		{
			return (rect.m_left >= m_left && rect.m_right <= m_right &&
				rect.m_top >= m_top && rect.m_bottom <= m_bottom);
		}

		void CRect::Translate(int32_t dx, int32_t dy)
		{
			m_left += dx;
			m_right += dx;
			m_top += dy;
			m_bottom += dy;
		}

		CRect CRect::Translated(int32_t dx, int32_t dy) const
		{
			CRect result(*this);
			result.Translate(dx, dy);
			return result;
		}

		void CRect::IntersectWith(const CRect& rect)
		{
			m_left = std::max(Left(), rect.Left());
			m_top = std::max(Top(), rect.Top());
			m_right = std::min(Right(), rect.Right());
			m_bottom = std::min(Bottom(), rect.Bottom());

			if (IsEmpty())
			{
				m_left = 0;
				m_top = 0;
				m_right = 0;
				m_bottom = 0;
			}
		}

		void CRect::UnionWith(const CRect& rect)
		{
			if (IsEmpty())
			{
				*this = rect;
				return;
			}

			if (rect.IsEmpty())
				return;

			m_left = std::min(Left(), rect.Left());
			m_top = std::min(Top(), rect.Top());
			m_right = std::max(Right(), rect.Right());
			m_bottom = std::max(Bottom(), rect.Bottom());
		}

		void CRect::Extend(int32_t left_offset,
			int32_t top_offset,
			int32_t right_offset,
			int32_t bottom_offset)
		{
			m_left -= left_offset;
			m_top -= top_offset;
			m_right += right_offset;
			m_bottom += bottom_offset;
		}

		void CRect::Scale(double horizontal, double vertical)
		{
			m_right += Width() * (horizontal - 1);
			m_bottom += Height() * (vertical - 1);
		}

		void CRect::Move(int32_t x, int32_t y)
		{
			m_right += x - m_left;
			m_bottom += y - m_top;
			m_left = x;
			m_top = y;
		}

		CRect CRect::Moved(int32_t x, int32_t y) const
		{
			CRect moved_rect(*this);
			moved_rect.Move(x, y);
			return moved_rect;
		}

		CRect& CRect::operator=(const CRect& other)
		{
			m_left = other.m_left;
			m_top = other.m_top;
			m_right = other.m_right;
			m_bottom = other.m_bottom;

			return *this;
		}
	}
}
