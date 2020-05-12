#pragma once

namespace mrCommonLib
{
	namespace desktop
	{

		class CPoint
		{
		public:
			CPoint() = default;
			CPoint(int32_t x, int32_t y);
			CPoint(const CPoint& point);
			~CPoint() = default;

			int32_t X() const;
			int32_t Y() const;

			void Set(int32_t x, int32_t y);
			CPoint Add(const CPoint& other) const;
			CPoint Subtract(const CPoint& other) const;
			bool IsEquals(const CPoint& other) const;
			void Translate(int32_t x_offset, int32_t y_offset);
			void Translate(const CPoint& offset);

			CPoint& operator=(const CPoint& other);
			bool operator!=(const CPoint& other) const;
			bool operator==(const CPoint& other) const;
		private:
			int32_t m_x = 0;
			int32_t m_y = 0;
		};


		class CSize
		{
		public:
			CSize() = default;
			CSize(int32_t width, int32_t height);
			CSize(const CSize& other);
			~CSize() = default;

			int32_t Width() const;
			int32_t Height() const;
			void Set(int32_t width, int32_t height);
			bool IsEmpty() const;
			bool IsEquals(const CSize& other) const;
			void Clear();

			CSize& operator=(const CSize& other);
			bool operator!=(const CSize& other) const;
			bool operator==(const CSize& other) const;

		private:
			int32_t m_width = 0;
			int32_t m_height = 0;
		};


		class CRect
		{
		public:
			CRect() = default;
			CRect(const CRect& other);
			~CRect() = default;

			static CRect MakeXYWH(int32_t x, int32_t y, int32_t width, int32_t height)
			{
				return CRect(x, y, x + width, y + height);
			}

			static CRect MakeXYWH(const CPoint& left_top, const CSize& size)
			{
				return CRect::MakeXYWH(left_top.X(), left_top.Y(), size.Width(), size.Height());
			}

			static CRect MakeWH(int32_t width, int32_t height)
			{
				return CRect(0, 0, width, height);
			}

			static CRect MakeLTRB(int32_t left, int32_t top, int32_t right, int32_t bottom)
			{
				return CRect(left, top, right, bottom);
			}

			static CRect MakeSize(const CSize& size)
			{
				return CRect(0, 0, size.Width(), size.Height());
			}

			int32_t Left() const { return m_left; }
			int32_t Top() const { return m_top; }
			int32_t Right() const { return m_right; }
			int32_t Bottom() const { return m_bottom; }

			int32_t X() const { return m_left; }
			int32_t Y() const { return m_top; }
			int32_t Width() const { return m_right - m_left; }
			int32_t Height() const { return m_bottom - m_top; }

			CPoint TopLeft() const { return CPoint(Left(), Top()); }
			void SetTopLeft(const CPoint& top_left);

			CSize Size() const { return CSize(Width(), Height()); }
			void SetSize(const CSize& size);

			bool IsEmpty() const { return m_left >= m_right || m_top >= m_bottom; }

			bool IsEquals(const CRect& other) const
			{
				return m_left == other.m_left  && m_top == other.m_top   &&
					m_right == other.m_right && m_bottom == other.m_bottom;
			}

			// Returns true if point lies within the rectangle boundaries.
			bool Contains(int32_t x, int32_t y) const;

			// Returns true if |rect| lies within the boundaries of this rectangle.
			bool ContainsRect(const CRect& rect) const;

			void Translate(int32_t dx, int32_t dy);
			void Translate(const CPoint& pt) { Translate(pt.X(), pt.Y()); };

			CRect Translated(int32_t dx, int32_t dy) const;
			CRect Translated(const CPoint& pt) const { return Translated(pt.X(), pt.Y()); }

			// Finds intersection with |rect|.
			void IntersectWith(const CRect& rect);

			// Extends the rectangle to cover |rect|. If |this| is empty, replaces |this|
			// with |rect|; if |rect| is empty, this function takes no effect.
			void UnionWith(const CRect& rect);

			// Enlarges current Rect by subtracting |left_offset| and |top_offset|
			// from |left_| and |top_|, and adding |right_offset| and |bottom_offset| to
			// |right_| and |bottom_|. This function does not normalize the result, so
			// |left_| and |top_| may be less than zero or larger than |right_| and
			// |bottom_|.
			void Extend(int32_t left_offset, int32_t top_offset,
				int32_t right_offset, int32_t bottom_offset);

			// Scales current Rect. This function does not impact the |top_| and |left_|.
			void Scale(double horizontal, double vertical);

			void Move(const CPoint& pt) { Move(pt.X(), pt.Y()); }
			void Move(int32_t x, int32_t y);

			CRect Moved(const CPoint& pt) const { return Moved(pt.X(), pt.Y()); };
			CRect Moved(int32_t x, int32_t y) const;

			CRect& operator=(const CRect& other);

			bool operator!=(const CRect& other) const { return !IsEquals(other); }
			bool operator==(const CRect& other) const { return IsEquals(other); }

		private:
			CRect(int32_t left, int32_t top, int32_t right, int32_t bottom)
				: m_left(left),
				m_top(top),
				m_right(right),
				m_bottom(bottom)
			{
			}

			int32_t m_left = 0;
			int32_t m_top = 0;
			int32_t m_right = 0;
			int32_t m_bottom = 0;
		};
	}
}
