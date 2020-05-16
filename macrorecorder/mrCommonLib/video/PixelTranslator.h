#pragma once
#include "../desktop/pixelformat.h"

#include <memory>

namespace mrCommonLib
{
	namespace video
	{

		class CPixelTranslator
		{
		public:
			virtual ~CPixelTranslator() = default;

			static std::unique_ptr<CPixelTranslator> Create(const desktop::CPixelFormat& source_format, 	const desktop::CPixelFormat& target_format);

			virtual void Translate(const uint8_t* src,
				int src_stride,
				uint8_t* dst,
				int dst_stride,
				int width,
				int height) = 0;
		};
	}
} 