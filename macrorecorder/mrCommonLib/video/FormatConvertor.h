#pragma once
#include "../desktop/pixelformat.h"
#include "../desktop/Frame.h"
namespace mrCommonLib
{
	namespace video
	{
		class CConvertPixels
		{
		public:
			static void ConvertToI420(const desktop::IFrame *pFrame, byte_t *pYdata, byte_t* pUdata, byte_t* pVdata, uint32_t y_stride, uint32_t uv_stride);
			static void ConvertToI420(const byte_t *pSrc, uint32_t width, uint32_t height, const desktop::CPixelFormat& foramt, uint32_t stride, byte_t *pYdata, byte_t* pUdata, byte_t* pVdata, uint32_t y_stride, uint32_t uv_stride);
 
			static void ConvertI420ToARGB(const byte_t *pYdata, const byte_t* pUdata, const byte_t* pVdata, uint32_t y_stride, uint32_t uv_stride, desktop::IFrame *pFrame);
			static void ConvertI420ToARGB(const byte_t *pYdata, const byte_t* pUdata, const byte_t* pVdata, uint32_t y_stride, uint32_t uv_stride, byte_t *pDst, uint32_t width, uint32_t height, uint32_t stride);
		};
	}
}