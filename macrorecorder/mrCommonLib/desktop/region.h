#pragma once
#include "geometry.h"

extern "C" {
#include "x11region/x11region.h"
} // extern "C"

namespace mrCommonLib
{
	namespace desktop
	{
		// Region represents a region of the screen or window.
			//
			// Internally each region is stored as a set of rows where each row contains one
			// or more rectangles aligned vertically.

		typedef std::shared_ptr<class CRegion> CRegionPtr;

		class CRegion
		{
		public:
			// Iterator that can be used to iterate over rectangles of a Region.
			// The region must not be mutated while the iterator is used.
			class Iterator
			{
			public:
				explicit Iterator(const CRegion& target);
				~Iterator() = default;

				bool isAtEnd() const;
				void advance();

				CRect rect() const
				{
					const BoxRec& current = m_rects[m_pos];
					return CRect::MakeLTRB(current.x1, current.y1, current.x2, current.y2);
				}

			private:
				const BoxRec* m_rects;
				long m_count;
				long m_pos;
			};

			CRegion();
			explicit CRegion(const CRect& rect);
			CRegion(const CRect* rects, int count);
			CRegion(const CRegion& other);
			CRegion(CRegion&& other);
			~CRegion();

			CRegion& operator=(const CRegion& other);
			CRegion& operator=(CRegion&& other);

			bool IsEmpty() const;

			bool Equals(const CRegion& region) const;

			// Reset the region to be empty.
			void Clear();

			// Reset region to contain just |rect|.
			void SetRect(const CRect& rect);

			// Adds specified rect(s) or region to the region.
			void AddRect(const CRect& rect);
			void AddRects(const CRect* rects, int count);
			void AddRegion(const CRegion& region);

			// Finds intersection of two regions and stores them in the current region.
			void Intersect(const CRegion& region1, const CRegion& region2);

			// Same as above but intersects content of the current region with |region|.
			void IntersectWith(const CRegion& region);

			// Clips the region by the |rect|.
			void IntersectWith(const CRect& rect);

			// Subtracts |region| from the current content of the region.
			void Subtract(const CRegion& region);

			// Subtracts |rect| from the current content of the region.
			void Subtract(const CRect& rect);

			// Adds (dx, dy) to the position of the region.
			void Translate(int32_t dx, int32_t dy);

			void Swap(CRegion* region);

		private:
			RegionRec m_x11reg;
		};
	}
}
