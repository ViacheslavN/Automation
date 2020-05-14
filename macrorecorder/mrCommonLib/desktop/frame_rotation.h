#pragma once
#include "Frame.h"

namespace mrCommonLib
{
	namespace desktop
	{
		// Represents the rotation of a Frame.
		enum class Rotation
		{
			CLOCK_WISE_0 = 0,
			CLOCK_WISE_90 = 90,
			CLOCK_WISE_180 = 180,
			CLOCK_WISE_270 = 270,
		};

		// Rotates input Frame |source|, copies pixel in an unrotated rectangle
		// |source_rect| into the target rectangle of another Frame |target|.
		// Target rectangle here is the rotated |source_rect| plus |target_offset|.
		// |rotation| specifies |source| to |target| rotation. |source_rect| is in
		// |source| coordinate. |target_offset| is in |target| coordinate.
		// This function triggers check failure if |source| does not cover the
		// |source_rect|, or |target| does not cover the rotated |rect|.
		void rotateDesktopFrame(const IFramePtr source,
			const CRect& source_rect,
			const Rotation& rotation,
			const CPoint& target_offset,
			IFramePtr target);

		// Returns a reverse rotation of |rotation|.
		Rotation reverseRotation(Rotation rotation);

		// Returns a rotated Size of |size|.
		CSize rotateSize(const CSize& size, Rotation rotation);

		// Returns a rotated Rect of |rect|. The |size| represents the size of the Frame which |rect|
		// belongs in.
		CRect rotateRect(const CRect& rect, const CSize& size, Rotation rotation);

	}
}