#pragma once
#include "../geometry.h"

namespace mrCommonLib
{
	namespace desktop
	{

			typedef std::shared_ptr<class CMouseCursor> TMouseCursorPtr;
			class CMouseCursor
			{
			public:
				CMouseCursor(CommonLib::Data::TVecBuffer&& image, const CSize& size, const CPoint& hotspot);

				CMouseCursor(CMouseCursor&& other) noexcept;
				CMouseCursor& operator=(CMouseCursor&& other) noexcept;

				CMouseCursor(const CMouseCursor& other) = default;
				CMouseCursor& operator=(const CMouseCursor& other) = default;

				~CMouseCursor() = default;

				const CSize& size() const { return m_size; }
				int Width() const { return m_size.Width(); }
				int Height() const { return m_size.Height(); }

				const CPoint& HotSpot() const { return m_hotspot; }
				int HotSpotX() const { return m_hotspot.X(); }
				int HotSpotY() const { return m_hotspot.Y(); }

				const CommonLib::Data::TVecBuffer& ConstImage() const { return m_image; }
				CommonLib::Data::TVecBuffer& Image() { return m_image; }

				int Stride() const;

				bool Equals(const CMouseCursor& other);

			private:
				CommonLib::Data::TVecBuffer m_image;
				CSize m_size;
				CPoint m_hotspot;
			};
	}
}
