#pragma once
#include "../pixelformat.h"
#include "../region.h"
#include "../geometry.h"
namespace mrCommonLib
{
	namespace desktop
	{
		class CDiffer
		{
		public:
			CDiffer(const CSize& size, const CPixelFormat& format);
			~CDiffer() = default;

			void CalcDirtyRegion(const uint8_t* prev_image,
				const uint8_t* curr_image,
				CRegion* changed_region);

		private:
			typedef uint8_t(*DiffFullBlockFunc)(const uint8_t*, const uint8_t*, int);

			static DiffFullBlockFunc diffFunctionFor32bpp();
			static DiffFullBlockFunc diffFunctionFor16bpp();

			void MarkDirtyBlocks(const uint8_t* prev_image, const uint8_t* curr_image);
			void MergeBlocks(CRegion* dirty_region);

			const CRect screen_rect_;

			int bytes_per_pixel_;
			int bytes_per_row_;
			int bytes_per_block_;

			const int full_blocks_x_;
			const int full_blocks_y_;

			int partial_column_width_;
			int partial_row_height_;

			int block_stride_y_;

			const int diff_width_;
			const int diff_height_;

			std::unique_ptr<uint8_t[]> diff_info_;

			DiffFullBlockFunc diff_full_block_func_;
		};
	}
}