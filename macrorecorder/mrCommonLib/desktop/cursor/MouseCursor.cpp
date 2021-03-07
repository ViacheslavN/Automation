#include "pch.h"
#include "MouseCursor.h"

namespace mrCommonLib
{
	namespace desktop
	{

			CMouseCursor::CMouseCursor(CommonLib::Data::TVecBuffer&& image, const CSize& size, const CPoint& hotspot)
				: m_image(std::move(image)),
				m_size(size),
				m_hotspot(hotspot)
			{
				// Nothing
			}

			CMouseCursor::CMouseCursor(CMouseCursor&& other) noexcept
			{
				*this = std::move(other);
			}

			CMouseCursor& CMouseCursor::operator=(CMouseCursor&& other) noexcept
			{
				if (&other != this)
				{
					m_image = std::move(other.m_image);
					m_size = other.m_size;
					m_hotspot = other.m_hotspot;

					other.m_size = CSize();
					other.m_hotspot = CPoint();
				}

				return *this;
			}

			int CMouseCursor::Stride() const
			{
				return m_size.Width() * sizeof(uint32_t);
			}

			bool CMouseCursor::Equals(const CMouseCursor& other)
			{
				return m_size.IsEquals(other.m_size) &&
					m_hotspot.IsEquals(other.m_hotspot) &&
					CommonLib::Data::Equals(m_image, other.m_image);
			}
	}
}
