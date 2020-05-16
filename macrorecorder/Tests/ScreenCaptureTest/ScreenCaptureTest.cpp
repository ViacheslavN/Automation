// ScreenCaptureTest.cpp : This file contains the 'main' function. Program execution begins and ends there.
//


#include "stdafx.h"

int main()
{
	std::shared_ptr<CommonLib::IlogWriter> pLogWriter(new CommonLib::CConsoleLogWriter());

	Log.InnitLog(std::shared_ptr<CommonLib::IlogLineFormat>(), pLogWriter);

	try
	{
		//mrCommonLib::desktop::win::CScreenCapturerGdi gdiCapture;
		CommonLib::CWriteMemoryStream writeStream;

		mrCommonLib::desktop::win::CScreenCapturerDxgi dxCapture;
		mrCommonLib::desktop::IFramePtr pFrame = dxCapture.CaptureFrame();

		mrCommonLib::video::IVideoEncoderPtr vedeoEncode = mrCommonLib::video::CreateVideoEncoder(mrCommonLib::video::VIDEO_ENCODING_VP9);
		vedeoEncode->Encode(pFrame.get(), &writeStream);

		/*for (int i = 0; i < 10; ++i)
		{
			mrCommonLib::desktop::IFramePtr pFrame = gdiCapture.CaptureFrame();
			mrCommonLib::desktop::SaveFrameToFile(pFrame, CommonLib::str_format::AStrFormatSafeT("gdiFrame%1.bmp", i));
		}

		mrCommonLib::desktop::win::CScreenCapturerDxgi dxCapture;

		for (int i = 0; i < 10; ++i)
		{
			mrCommonLib::desktop::IFramePtr pFrame = dxCapture.CaptureFrame();
			mrCommonLib::desktop::SaveFrameToFile(pFrame, CommonLib::str_format::AStrFormatSafeT("dxFrame%1.bmp", i));
		}*/
	}
	catch (std::exception& exe)
	{
		std::cout << "Failed: " << "\n";

		astrvec msgChain = CommonLib::CExcBase::GetChainFromExc(exe);


		for (size_t i = 0, sz = msgChain.size(); i < sz; ++i)
		{
			if (i != 0)
				std::cout << "\r\n";

			std::cout << msgChain[i];
		}
	}
	

	return 0;
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
