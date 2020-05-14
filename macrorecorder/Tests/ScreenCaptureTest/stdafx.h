#pragma once
#include <iostream>


#include "../../../../CommonLib/CommonLib.h"
#include "../../../../CommonLib/alloc/alloc.h"
#include "../../../../CommonLib/filesystem/File.h"
#include "../../../../CommonLib/str/convertToString.h"
#include "../../../../CommonLib/str/StringEncoding.h"
#include "../../../../CommonLib/log/logger.h"
#include "../../../../CommonLib/log/ConsoleLogWriter.h"


#include "../../../../CommonLib/CommonLib_lib.h"
#include "../../mrCommonLib/mrCommonLib_lib.h"

#include "../../mrCommonLib/desktop/win/ScreenCapturerGdi.h"
#include "../../mrCommonLib/desktop/win/dx/ScreenCapturerDxgi.h"

#pragma comment(lib, "DXGI.lib")
#pragma comment(lib, "D3D11.lib")
#ifdef _DEBUG		  
	#pragma comment(lib, "../../../ThirdParty/libyuv/build/Debug/yuv.lib")
#else
	#pragma comment(lib, "../../../ThirdParty/libyuv/build/Release/yuv.lib")
#endif

