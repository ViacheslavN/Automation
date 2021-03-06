#pragma once

#include <cstdint>

namespace mrCommonLib
{

	namespace desktop {

		uint8_t diffFullBlock_32bpp_32x32_AVX2(
			const uint8_t* image1, const uint8_t* image2, int bytes_per_row);

		uint8_t diffFullBlock_32bpp_16x16_AVX2(
			const uint8_t* image1, const uint8_t* image2, int bytes_per_row);

	}
}