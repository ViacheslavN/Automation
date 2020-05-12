#include "pch.h"
#include "differ.h"
#include "../../../../../ThirdParty/libyuv/include/libyuv/cpu_id.h"
#include "diff_block_16bpp_c.h"
#include "diff_block_32bpp_avx2.h"
#include "diff_block_32bpp_sse2.h"
#include "diff_block_32bpp_sse3.h"
#include "diff_block_32bpp_c.h"

namespace mrCommonLib
{

	namespace desktop
	{
		namespace {

			const int kBlockSize = 16;

			// Check for diffs in upper-left portion of the block. The size of the portion to check is
			// specified by the |width| and |height| values.
			// Note that if we force the capturer to always return images whose width and height are multiples
			// of kBlockSize, then this will never be called.
			uint8_t diffPartialBlock(const uint8_t* prev_image,
				const uint8_t* curr_image,
				int bytes_per_row,
				int bytes_per_block,
				int height)
			{
				for (int y = 0; y < height; ++y)
				{
					if (memcmp(prev_image, curr_image, bytes_per_block) != 0)
						return 1U;

					prev_image += bytes_per_row;
					curr_image += bytes_per_row;
				}

				return 0U;
			}



		} // namespace

		CDiffer::CDiffer(const CSize& size, const CPixelFormat& format)
			: screen_rect_(CRect::MakeSize(size)),
			diff_width_(((size.Width() + kBlockSize - 1) / kBlockSize) + 1),
			diff_height_(((size.Height() + kBlockSize - 1) / kBlockSize) + 1),
			full_blocks_x_(size.Width() / kBlockSize),
			full_blocks_y_(size.Height() / kBlockSize)
		{
			bytes_per_pixel_ = format.BytesPerPixel();
			bytes_per_row_ = size.Width() * bytes_per_pixel_;
			bytes_per_block_ = kBlockSize * bytes_per_pixel_;

			const int diff_info_size = diff_width_ * diff_height_;

			diff_info_ = std::make_unique<uint8_t[]>(diff_info_size);
			memset(diff_info_.get(), 0, diff_info_size);

			// Calc size of partial blocks which may be present on right and bottom edge.
			partial_column_width_ = size.Width() - (full_blocks_x_ * kBlockSize);
			partial_row_height_ = size.Height() - (full_blocks_y_ * kBlockSize);

			// Offset from the start of one block-row to the next.
			block_stride_y_ = bytes_per_row_ * kBlockSize;

			switch (format.BitsPerPixel())
			{
			case 32:
				diff_full_block_func_ = diffFunctionFor32bpp();
				break;

			case 16:
				diff_full_block_func_ = diffFunctionFor16bpp();
				break;

			default:
				break;
			}

			if (diff_full_block_func_ == nullptr)
				throw CommonLib::CExcBase("Invalid bpp %1", format.BitsPerPixel());
		}

		// static
		CDiffer::DiffFullBlockFunc CDiffer::diffFunctionFor32bpp()
		{
			DiffFullBlockFunc func = nullptr;

			if (libyuv::TestCpuFlag(libyuv::kCpuHasAVX2))
			{
				if constexpr (kBlockSize == 16)
					func = diffFullBlock_32bpp_16x16_AVX2;
				else if constexpr (kBlockSize == 32)
					func = diffFullBlock_32bpp_32x32_AVX2;
			}
			else if (libyuv::TestCpuFlag(libyuv::kCpuHasSSSE3))
			{

				if constexpr (kBlockSize == 16)
					func = diffFullBlock_32bpp_16x16_SSE3;
				else if constexpr (kBlockSize == 32)
					func = diffFullBlock_32bpp_32x32_SSE3;
			}
			else if (libyuv::TestCpuFlag(libyuv::kCpuHasSSE2))
			{

				if constexpr (kBlockSize == 16)
					func = diffFullBlock_32bpp_16x16_SSE2;
				else if constexpr (kBlockSize == 32)
					func = diffFullBlock_32bpp_32x32_SSE2;
			}
			else
			{
				if constexpr (kBlockSize == 16)
					func = diffFullBlock_32bpp_16x16_C;
				else if constexpr (kBlockSize == 32)
					func = diffFullBlock_32bpp_32x32_C;
			}

			return func;
		}

		// static
		CDiffer::DiffFullBlockFunc CDiffer::diffFunctionFor16bpp()
		{
			DiffFullBlockFunc func = nullptr;

			if constexpr (kBlockSize == 16)
				func = diffFullBlock_16bpp_16x16_C;
			else if constexpr (kBlockSize == 32)
				func = diffFullBlock_16bpp_32x32_C;

			return func;
		}

		// Identify all of the blocks that contain changed pixels.
		void CDiffer::markDirtyBlocks(const uint8_t* prev_image, const uint8_t* curr_image)
		{
			const uint8_t* prev_block_row_start = prev_image;
			const uint8_t* curr_block_row_start = curr_image;

			// Offset from the start of one diff_info row to the next.
			const int diff_stride = diff_width_;

			uint8_t* is_diff_row_start = diff_info_.get();

			for (int y = 0; y < full_blocks_y_; ++y)
			{
				const uint8_t* prev_block = prev_block_row_start;
				const uint8_t* curr_block = curr_block_row_start;

				uint8_t* is_different = is_diff_row_start;

				for (int x = 0; x < full_blocks_x_; ++x)
				{
					// Mark this block as being modified so that it gets
					// incorporated into a dirty rect.
					*is_different = diff_full_block_func_(prev_block, curr_block, bytes_per_row_);

					prev_block += bytes_per_block_;
					curr_block += bytes_per_block_;

					++is_different;
				}

				// If there is a partial column at the end, handle it.
				// This condition should rarely, if ever, occur.
				if (partial_column_width_ != 0)
				{
					*is_different = diffPartialBlock(prev_block,
						curr_block,
						bytes_per_row_,
						bytes_per_block_,
						kBlockSize);
				}

				// Update pointers for next row.
				prev_block_row_start += block_stride_y_;
				curr_block_row_start += block_stride_y_;

				is_diff_row_start += diff_stride;
			}

			// If the screen height is not a multiple of the block size, then this
			// handles the last partial row. This situation is far more common than
			// the 'partial column' case.
			if (partial_row_height_ != 0)
			{
				const uint8_t* prev_block = prev_block_row_start;
				const uint8_t* curr_block = curr_block_row_start;

				uint8_t* is_different = is_diff_row_start;

				for (int x = 0; x < full_blocks_x_; ++x)
				{
					*is_different = diffPartialBlock(prev_block,
						curr_block,
						bytes_per_row_,
						bytes_per_block_,
						partial_row_height_);

					prev_block += bytes_per_block_;
					curr_block += bytes_per_block_;
					++is_different;
				}

				if (partial_column_width_ != 0)
				{
					*is_different =
						diffPartialBlock(prev_block,
							curr_block,
							bytes_per_row_,
							partial_column_width_ * bytes_per_pixel_,
							partial_row_height_);
				}
			}
		}

		//
		// After the dirty blocks have been identified, this routine merges adjacent
		// blocks into a region.
		// The goal is to minimize the region that covers the dirty blocks.
		//
		void CDiffer::mergeBlocks(CRegionPtr dirty_region)
		{
			uint8_t* is_diff_row_start = diff_info_.get();
			const int diff_stride = diff_width_;

			for (int y = 0; y < diff_height_; ++y)
			{
				uint8_t* is_different = is_diff_row_start;

				for (int x = 0; x < diff_width_; ++x)
				{
					// We've found a modified block. Look at blocks to the right and
					// below to group this block with as many others as we can.
					if (*is_different != 0)
					{
						// Width and height of the rectangle in blocks.
						int width = 1;
						int height = 1;

						*is_different = 0;

						// Group with blocks to the right.
						// We can keep looking until we find an unchanged block because
						// we have a boundary block which is never marked as having
						// diffs.
						uint8_t* right = is_different + 1;

						while (*right != 0)
						{
							*right++ = 0;
							++width;
						}

						// Group with blocks below.
						// The entire width of blocks that we matched above much match
						// for each row that we add.
						uint8_t* bottom = is_different;
						bool found_new_row;

						do
						{
							found_new_row = true;
							bottom += diff_stride;
							right = bottom;

							for (int x2 = 0; x2 < width; ++x2)
							{
								if (*right++ == 0)
									found_new_row = false;
							}

							if (found_new_row)
							{
								++height;

								// We need to go back and erase the diff markers so
								// that we don't try to add these blocks a second time.
								right = bottom;

								for (int x2 = 0; x2 < width; ++x2)
								{
									*right++ = 0;
								}
							}
						} while (found_new_row);

						CRect dirty_rect = CRect::MakeXYWH(x * kBlockSize, y * kBlockSize,
							width * kBlockSize, height * kBlockSize);

						dirty_rect.IntersectWith(screen_rect_);

						// Add rect to region.
						dirty_region->AddRect(dirty_rect);
					}

					// Increment to next block in this row.
					++is_different;
				}

				// Go to start of next row.
				is_diff_row_start += diff_stride;
			}
		}

		void CDiffer::calcDirtyRegion(const uint8_t* prev_image,
			const uint8_t* curr_image,
			CRegionPtr dirty_region)
		{
			dirty_region->Clear();

			// Identify all the blocks that contain changed pixels.
			markDirtyBlocks(prev_image, curr_image);

			//
			// Now that we've identified the blocks that have changed, merge adjacent
			// blocks to minimize the number of rects that we return.
			//
			mergeBlocks(dirty_region);
		}

	}

}