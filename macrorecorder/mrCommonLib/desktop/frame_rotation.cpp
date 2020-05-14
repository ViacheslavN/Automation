#include "pch.h"
#include "frame_rotation.h"
#include <libyuv/rotate_argb.h>
#include "../../../../ThirdParty/libyuv/include/libyuv//rotate_argb.h"

namespace mrCommonLib
{
	namespace desktop
	{
		libyuv::RotationMode ToLibyuvRotationMode(Rotation rotation)
		{
			switch (rotation)
			{
			case Rotation::CLOCK_WISE_0:
				return libyuv::kRotate0;
			case Rotation::CLOCK_WISE_90:
				return libyuv::kRotate90;
			case Rotation::CLOCK_WISE_180:
				return libyuv::kRotate180;
			case Rotation::CLOCK_WISE_270:
				return libyuv::kRotate270;
			}

			return libyuv::kRotate0;
		}

		CRect rotateAndOffsetRect(const CRect& rect,
			const CSize& size,
			Rotation rotation,
			const CPoint &offset)
		{
			CRect result = rotateRect(rect, size, rotation);
			result.Translate(offset.X(), offset.Y());
			return result;
		}



	Rotation reverseRotation(Rotation rotation)
	{
		switch (rotation)
		{
		case Rotation::CLOCK_WISE_0:
			return rotation;
		case Rotation::CLOCK_WISE_90:
			return Rotation::CLOCK_WISE_270;
		case Rotation::CLOCK_WISE_180:
			return Rotation::CLOCK_WISE_180;
		case Rotation::CLOCK_WISE_270:
			return Rotation::CLOCK_WISE_90;
		}

		return Rotation::CLOCK_WISE_0;
	}

	CSize rotateSize(const CSize& size, Rotation rotation)
	{
		switch (rotation)
		{
		case Rotation::CLOCK_WISE_0:
		case Rotation::CLOCK_WISE_180:
			return size;

		case Rotation::CLOCK_WISE_90:
		case Rotation::CLOCK_WISE_270:
			return CSize(size.Height(), size.Width());
		}

		return CSize();
	}

	CRect rotateRect(const CRect& rect, const CSize& size, Rotation rotation)
	{
		switch (rotation)
		{
		case Rotation::CLOCK_WISE_0:
			return rect;
		case Rotation::CLOCK_WISE_90:
			return CRect::MakeXYWH(size.Height() - rect.Bottom(), rect.Left(),
				rect.Height(), rect.Width());
		case Rotation::CLOCK_WISE_180:
			return CRect::MakeXYWH(size.Width() - rect.Right(), size.Height() - rect.Bottom(),
				rect.Width(), rect.Height());
		case Rotation::CLOCK_WISE_270:
			return CRect::MakeXYWH(rect.Top(), size.Width() - rect.Right(),
				rect.Height(), rect.Width());
		}

		return CRect();
	}

	void rotateDesktopFrame(const IFramePtr source,
		const CRect& source_rect,
		const Rotation& rotation,
		const CPoint& target_offset,
		IFramePtr target)
	{
		//DCHECK(target);
		//DCHECK(Rect::makeSize(source.size()).containsRect(source_rect));

		// The rectangle in |target|.
		const CRect target_rect = rotateAndOffsetRect(
			source_rect, source->Size(), rotation, target_offset);

		//DCHECK(Rect::makeSize(target->size()).containsRect(target_rect));

		if (target_rect.IsEmpty())
			return;

		int result = libyuv::ARGBRotate(
			source->DataAtPos(source_rect.TopLeft()), source->Stride(),
			target->DataAtPos(target_rect.TopLeft()), target->Stride(),
			source_rect.Width(), source_rect.Height(),
			ToLibyuvRotationMode(rotation));
		//DCHECK_EQ(result, 0);
	}
	}
}