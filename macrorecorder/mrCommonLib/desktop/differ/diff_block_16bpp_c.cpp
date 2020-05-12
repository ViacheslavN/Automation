
#include "pch.h"
#include "diff_block_16bpp_c.h"

#include <cstring>

namespace mrCommonLib
{

	namespace desktop {

		namespace {

			const int kBytesPerPixel = 2;

		} // namespace

		uint8_t diffFullBlock_16bpp_32x32_C(
			const uint8_t* image1, const uint8_t* image2, int bytes_per_row)
		{
			static const int kBlockSize = 32;

			for (int y = 0; y < kBlockSize; ++y)
			{
				if (memcmp(image1, image2, kBlockSize * kBytesPerPixel) != 0)
				{
					return 1U;
				}

				image1 += bytes_per_row;
				image2 += bytes_per_row;
			}

			return 0U;
		}

		uint8_t diffFullBlock_16bpp_16x16_C(
			const uint8_t* image1, const uint8_t* image2, int bytes_per_row)
		{
			static const int kBlockSize = 16;

			for (int y = 0; y < kBlockSize; ++y)
			{
				if (memcmp(image1, image2, kBlockSize * kBytesPerPixel) != 0)
				{
					return 1U;
				}

				image1 += bytes_per_row;
				image2 += bytes_per_row;
			}

			return 0U;
		}
	}
}