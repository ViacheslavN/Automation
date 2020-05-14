#include "pch.h"
#include "Frame.h"

namespace mrCommonLib
{
	namespace desktop
	{
		void SaveFrameToFile(IFramePtr pFrame, const astr& fileName)
		{
			try
			{
				CSize size = pFrame->Size();

				BITMAPINFOHEADER bi;
				ZeroMemory(&bi, sizeof(BITMAPINFOHEADER));
				bi.biSize = sizeof(BITMAPINFOHEADER);
				bi.biWidth = size.Width();
				bi.biHeight = -size.Height();
				bi.biPlanes = 1;
				bi.biBitCount = 32;
				bi.biCompression = BI_RGB;
				bi.biSizeImage = bi.biWidth * size.Height() * bi.biBitCount / 8;

				BITMAPFILEHEADER bfh;
				//                  bfh.bfType      = ('M' << 8) + 'B';
				bfh.bfType = 0x4D42;
				bfh.bfSize = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + bi.biSizeImage;
				bfh.bfReserved1 = 0;
				bfh.bfReserved2 = 0;
				bfh.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);

				CommonLib::file::TFilePtr pFile = CommonLib::file::CFileCreator::OpenFileA(fileName.c_str(), CommonLib::file::ofmCreateAlways, CommonLib::file::arWrite, CommonLib::file::smNoMode, CommonLib::file::oftBinary);


				pFile->Write((byte_t*)&bfh, sizeof(bfh));
				pFile->Write((byte_t*)&bi, sizeof(bi));
				pFile->Write(pFrame->Data(), bi.biSizeImage);
			}
			catch (std::exception& exc)
			{
				CommonLib::CExcBase::RegenExc("failed to save frame in file: %1", fileName, exc);
			}
		}
	}
}