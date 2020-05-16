#include "pch.h"
#include "VideoUtil.h"

namespace mrCommonLib
{
	namespace video
	{
		desktop::CRect CVideoUtil::ReadRect(CommonLib::IReadStream *pStream)
		{
			int32_t x = pStream->ReadIntu32();
			int32_t y = pStream->ReadIntu32();
			int32_t width = pStream->ReadIntu32();
			int32_t height = pStream->ReadIntu32();

			return desktop::CRect::MakeXYWH(x, y, width, height);
		}

		void CVideoUtil::WriteRect(const desktop::CRect& rect, CommonLib::IWriteStream *pStream)
		{
			pStream->Write(rect.X());
			pStream->Write(rect.Y());
			pStream->Write(rect.Width());
			pStream->Write(rect.Height());
		}

		desktop::CPixelFormat CVideoUtil::ReadPixelFormat(CommonLib::IReadStream *pStream)
		{

			uint8_t bits_per_pixel = pStream->ReadByte();
			uint16_t red_max = pStream->Readintu16();
			uint16_t green_max = pStream->Readintu16();
			uint16_t blue_max = pStream->Readintu16();
			uint8_t red_shift = pStream->ReadByte();
			uint8_t green_shift = pStream->ReadByte();
			uint8_t blue_shift = pStream->ReadByte();

			return desktop::CPixelFormat(bits_per_pixel, red_max, green_max, blue_max,
				red_shift, green_shift, blue_shift);



		}

		void CVideoUtil::WritePixelFormat(const desktop::CPixelFormat& pixelFormat, CommonLib::IWriteStream *pStream)
		{
			pStream->Write(pixelFormat.BitsPerPixel());
			pStream->Write(pixelFormat.RedMax());
			pStream->Write(pixelFormat.GreenMax());
			pStream->Write(pixelFormat.BlueMax());
			pStream->Write(pixelFormat.RedShift());
			pStream->Write(pixelFormat.GreenShift());
			pStream->Write(pixelFormat.BlueShift());
		}
	}

}