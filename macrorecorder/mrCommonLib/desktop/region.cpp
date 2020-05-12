#include "pch.h"
#include "region.h"

namespace mrCommonLib
{
	namespace desktop
	{
		namespace {

			inline RegionPtr region_cast(const RegionRec* region)
			{
				return reinterpret_cast<RegionPtr>(const_cast<RegionRec*>(region));
			}

		} // namespace

		CRegion::CRegion()
		{
			miRegionInit(&m_x11reg, NullBox, 0);
		}

		CRegion::CRegion(const CRect& rect)
		{
			if (!rect.IsEmpty())
			{
				BoxRec box;
				box.x1 = rect.Left();
				box.x2 = rect.Right();
				box.y1 = rect.Top();
				box.y2 = rect.Bottom();
				miRegionInit(&m_x11reg, &box, 0);
			}
			else
			{
				miRegionInit(&m_x11reg, NullBox, 0);
			}
		}

		CRegion::CRegion(const CRect* rects, int count)
		{
			miRegionInit(&m_x11reg, NullBox, 0);
			AddRects(rects, count);
		}

		CRegion::CRegion(const CRegion& other)
		{
			miRegionInit(&m_x11reg, NullBox, 0);
			miRegionCopy(&m_x11reg, region_cast(&other.m_x11reg));
		}

		CRegion::CRegion(CRegion&& other)
		{
			*this = std::move(other);
		}

		CRegion::~CRegion()
		{
			miRegionUninit(&m_x11reg);
		}

		CRegion& CRegion::operator=(const CRegion& other)
		{
			if (this == &other)
				return *this;

			miRegionCopy(&m_x11reg, region_cast(&other.m_x11reg));
			return *this;
		}

		CRegion& CRegion::operator=(CRegion&& other)
		{
			if (this == &other)
				return *this;

			miRegionUninit(&m_x11reg);

			m_x11reg.extents = other.m_x11reg.extents;
			m_x11reg.data = other.m_x11reg.data;

			miRegionInit(&other.m_x11reg, NullBox, 0);

			return *this;
		}

		bool CRegion::IsEmpty() const
		{
			return (miRegionNotEmpty(region_cast(&m_x11reg)) == FALSE);
		}

		bool CRegion::Equals(const CRegion& region) const
		{
			if (IsEmpty() && region.IsEmpty())
				return true;

			return (miRegionsEqual(region_cast(&m_x11reg), region_cast(&region.m_x11reg)) != FALSE);
		}

		void CRegion::Clear()
		{
			miRegionEmpty(&m_x11reg);
		}

		void CRegion::SetRect(const CRect& rect)
		{
			Clear();
			AddRect(rect);
		}

		void CRegion::AddRect(const CRect& rect)
		{
			if (!rect.IsEmpty())
			{
				CRegion temp(rect);
				AddRegion(temp);
			}
		}

		void CRegion::AddRects(const CRect* rects, int count)
		{
			for (int i = 0; i < count; ++i)
				AddRect(rects[i]);
		}

		void CRegion::AddRegion(const CRegion& region)
		{
			miUnion(&m_x11reg, &m_x11reg, region_cast(&region.m_x11reg));
		}

		void CRegion::Intersect(const CRegion& region1, const CRegion& region2)
		{
			miIntersect(&m_x11reg, region_cast(&region1.m_x11reg), region_cast(&region2.m_x11reg));
		}

		void CRegion::IntersectWith(const CRegion& region)
		{
			miIntersect(&m_x11reg, &m_x11reg, region_cast(&region.m_x11reg));
		}

		void CRegion::IntersectWith(const CRect& rect)
		{
			CRegion region;
			region.AddRect(rect);
			IntersectWith(region);
		}

		void CRegion::Subtract(const CRegion& region)
		{
			miSubtract(&m_x11reg, &m_x11reg, region_cast(&region.m_x11reg));
		}

		void CRegion::Subtract(const CRect& rect)
		{
			CRegion region;
			region.AddRect(rect);
			Subtract(region);
		}

		void CRegion::Translate(int32_t dx, int32_t dy)
		{
			miTranslateRegion(&m_x11reg, dx, dy);
		}

		void CRegion::Swap(CRegion* region)
		{
			std::swap(m_x11reg.extents, region->m_x11reg.extents);
			std::swap(m_x11reg.data, region->m_x11reg.data);
		}

		CRegion::Iterator::Iterator(const CRegion& region)
			: m_rects(REGION_RECTS(&region.m_x11reg)),
			m_count(REGION_NUM_RECTS(&region.m_x11reg)),
			m_pos(0)
		{
			// Nothing
		}

		bool CRegion::Iterator::isAtEnd() const
		{
			return m_pos >= m_count;
		}

		void CRegion::Iterator::advance()
		{
			++m_pos;
		}
	}
}