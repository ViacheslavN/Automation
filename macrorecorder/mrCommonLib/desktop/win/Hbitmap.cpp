#include "pch.h"
#include "../pixelformat.h"
#include "Hbitmap.h"
#include "BitmapInfo.h"
#

namespace mrCommonLib
{
	namespace desktop
	{
		namespace win
		{
			CHBitmap::CHBitmap(HBITMAP bitmap, byte_t *data, const CSize& size, const CPixelFormat& format) : m_bitmap(bitmap), m_data(data), m_size(size), m_format(format)
			{

			}

			CHBitmap::~CHBitmap()
			{
				if (m_bitmap != nullptr)
				{
					DeleteObject(m_bitmap);
				}
			}

			HBITMAP CHBitmap::GetBitmap() const
			{
				return m_bitmap;
			}

			byte_t* CHBitmap::GetData() const
			{
				return m_data;
			}

			const CSize& CHBitmap::GetSize() const
			{
				return m_size;
			}

			const CPixelFormat& CHBitmap::GetPixelFormat() const
			{
				return m_format;
			}

			HBitmapPtr CHBitmap::Ñreate(const CSize& size, const CPixelFormat& format, HDC hdc)
			{
				const int bytesPerRow = size.Width() * format.BytesPerPixel();
				const int buffer_size = bytesPerRow * size.Height();

				CBitmapInfo bmi = { 0 };
				bmi.header.biSize = sizeof(bmi.header);
				bmi.header.biBitCount = format.BitsPerPixel();
				bmi.header.biSizeImage = buffer_size;
				bmi.header.biPlanes = 1;
				bmi.header.biWidth = size.Width();
				bmi.header.biHeight = -size.Height();

				if (format.BitsPerPixel() == 32 || format.BitsPerPixel() == 16)
				{
					bmi.header.biCompression = BI_BITFIELDS;

					bmi.u.mask.red = format.RedMax() << format.RedShift();
					bmi.u.mask.green = format.GreenMax() << format.GreenShift();
					bmi.u.mask.blue = format.BlueMax() << format.BlueShift();
				}
				else
				{
					bmi.header.biCompression = BI_RGB;

					for (uint32_t i = 0; i < 256; ++i)
					{
						const uint32_t red = (i >> format.RedShift())   & format.RedMax();
						const uint32_t green = (i >> format.GreenShift()) & format.GreenMax();
						const uint32_t blue = (i >> format.BlueShift())  & format.BlueMax();

						bmi.u.color[i].rgbRed = static_cast<uint8_t>(red * 0xFF / format.RedMax());
						bmi.u.color[i].rgbGreen = static_cast<uint8_t>(green * 0xFF / format.GreenMax());
						bmi.u.color[i].rgbBlue = static_cast<uint8_t>(blue * 0xFF / format.BlueMax());
					}
				}


				void* data = nullptr;
				HANDLE section = nullptr; // TO DO create shared mem
				HBITMAP bitmap = CreateDIBSection(hdc,	reinterpret_cast<LPBITMAPINFO>(&bmi), DIB_RGB_COLORS,	&data, section,	0);
				if (!bitmap)
				{
					throw CommonLib::CWinExc(L"Filed to create bitmap");
				}
			
				return HBitmapPtr(new CHBitmap(bitmap, (byte_t*)data, size, format));
			}

			void CHBitmap::SaveToFile(const wstr& fileName) const
			{
				BITMAPINFOHEADER bi;
				ZeroMemory(&bi, sizeof(BITMAPINFOHEADER));
				bi.biSize = sizeof(BITMAPINFOHEADER);
				bi.biWidth = m_size.Width();
				bi.biHeight = m_size.Height();
				bi.biPlanes = 1;
				bi.biBitCount = 32;
				bi.biCompression = BI_RGB;
				bi.biSizeImage = bi.biWidth * bi.biHeight * bi.biBitCount / 8;

				BITMAPFILEHEADER bfh;
				//                  bfh.bfType      = ('M' << 8) + 'B';
				bfh.bfType = 0x4D42;
				bfh.bfSize = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + bi.biSizeImage;
				bfh.bfReserved1 = 0;
				bfh.bfReserved2 = 0;
				bfh.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);

				CommonLib::file::TFilePtr pFile = CommonLib::file::CFileCreator::OpenFile(fileName.c_str(), CommonLib::file::ofmCreateAlways, CommonLib::file::arWrite, CommonLib::file::smNoMode, CommonLib::file::oftBinary);


				pFile->Write((byte_t*)&bfh, sizeof(bfh));
				pFile->Write((byte_t*)&bi, sizeof(bi));
				pFile->Write(m_data, bi.biSizeImage);
			}
		}
	}
}


/*
BITMAPINFOHEADER bi;
ZeroMemory(&bi, sizeof(BITMAPINFOHEADER));
bi.biSize              = sizeof(BITMAPINFOHEADER);
bi.biWidth             = nWidth;
bi.biHeight            = nHeight;
bi.biPlanes            = 1;
bi.biBitCount          = 32;
bi.biCompression       = BI_RGB;
bi.biSizeImage         = bi.biWidth * bi.biHeight * bi.biBitCount / 8;

BITMAPFILEHEADER bfh;
//                  bfh.bfType      = ('M' << 8) + 'B';
bfh.bfType      = 0x4D42;
bfh.bfSize      = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + bi.biSizeImage;
bfh.bfReserved1 = 0;
bfh.bfReserved2 = 0;
bfh.bfOffBits   = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);

FILE* file = fopen("D:\\test.bmp", "wb");
fwrite(&bfh, sizeof(bfh), 1, file);
fwrite(&bi, sizeof(bi), 1, file);
fwrite(lpBits, bi.biSizeImage, 1, file);
fclose(file);

*/


/*
void SaveBitmapToFile(const GisString  & file_name, int width, int height, void *pData)
{

  BITMAPINFO bitmapInfo;
  memset(&bitmapInfo, 0, sizeof(BITMAPINFO));
  bitmapInfo.bmiHeader.biBitCount = 32;
  bitmapInfo.bmiHeader.biWidth = width;
  bitmapInfo.bmiHeader.biHeight = height;
  bitmapInfo.bmiHeader.biPlanes = 1;
  bitmapInfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
  bitmapInfo.bmiHeader.biSizeImage = width * height * 4 ;
  FILE* file = _tfopen(file_name.cwstr(), _T("wb"));
  if (file != NULL)
  {
	BITMAPFILEHEADER m_bfh;
	// Format BITMAPFILEHEADER info
	memset(&m_bfh, 0, sizeof(BITMAPFILEHEADER));
	m_bfh.bfType = 'MB';
	m_bfh.bfSize = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + width * height * 4;
	m_bfh.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);
	// Write BITMAPFILEHEADER info
	fwrite(&m_bfh, 1, sizeof(BITMAPFILEHEADER), file);

	// Write BITMAPINFOHEADER info
	fwrite(&bitmapInfo.bmiHeader, 1, sizeof(BITMAPINFOHEADER), file);

	fwrite(pData, width * height * 4 , sizeof(BYTE), file);

	// Close .BMP file
	fclose(file);
  }
}

*/