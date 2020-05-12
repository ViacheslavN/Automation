#pragma once

#define DESKTOP__DIFF_BLOCK_16BPP_C_H

#include <cstdint>

namespace mrCommonLib
{

	namespace desktop {

		uint8_t diffFullBlock_16bpp_32x32_C(
			const uint8_t* image1, const uint8_t* image2, int bytes_per_row);

		uint8_t diffFullBlock_16bpp_16x16_C(
			const uint8_t* image1, const uint8_t* image2, int bytes_per_row);

	}

}