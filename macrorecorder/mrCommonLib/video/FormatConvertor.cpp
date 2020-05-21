#include "pch.h"
#include "FormatConvertor.h"
#include "../../../../../ThirdParty/libyuv/include/libyuv/convert.h"
#include "../../../../../ThirdParty/libyuv/include/libyuv/convert_from_argb.h"

namespace mrCommonLib
{
	namespace video
	{

		void CConvertPixels::ConvertToI420(const desktop::IFrame *pFrame, byte_t *pYdata, byte_t* pUdata, byte_t* pVdata, uint32_t y_stride, uint32_t uv_stride)
		{
			const desktop::CSize size = pFrame->Size();
			ConvertToI420(pFrame->Data(), size.Width(), size.Height(), pFrame->Format(), pFrame->Stride(), pYdata, pUdata, pVdata, y_stride, uv_stride);
		}

		void CConvertPixels::ConvertToI420(const byte_t *pSrc, uint32_t width, uint32_t height, const desktop::CPixelFormat& foramt, uint32_t stride,  byte_t *pYdata, byte_t* pUdata, byte_t* pVdata, uint32_t y_stride, uint32_t uv_stride)
		{
			auto convert_to_i420 = libyuv::ARGBToI420;

			if (foramt.BitsPerPixel() == 16)
				convert_to_i420 = libyuv::RGB565ToI420;

			convert_to_i420(pSrc,
				stride,
				pYdata, y_stride,
				pUdata, uv_stride,
				pVdata, uv_stride,
				width,
				height);
		}


		void CConvertPixels::ConvertI420ToARGB(const byte_t *pYdata, const byte_t* pUdata, const byte_t* pVdata, uint32_t y_stride, uint32_t uv_stride, desktop::IFrame *pFrame)
		{
			const desktop::CSize size = pFrame->Size();
			ConvertI420ToARGB(pYdata, pUdata, pVdata, y_stride, uv_stride, pFrame->Data(), size.Width(), size.Height(), pFrame->Stride());
		}

		void CConvertPixels::ConvertI420ToARGB(const byte_t *pYdata, const byte_t* pUdata, const byte_t* pVdata, uint32_t y_stride, uint32_t uv_stride, byte_t *pDst, uint32_t width, uint32_t height,  uint32_t stride)
		{

			libyuv::I420ToARGB(pYdata, y_stride,
				pUdata, uv_stride,
				pVdata, uv_stride,
				pDst,
				stride,
				width,
				height);
		}
 
	}
}