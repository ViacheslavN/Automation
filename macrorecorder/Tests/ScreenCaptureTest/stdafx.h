#pragma once
#include <iostream>


#include "../../../../CommonLib/CommonLib.h"
#include "../../../../CommonLib/alloc/alloc.h"
#include "../../../../CommonLib/filesystem/File.h"
#include "../../../../CommonLib/str/convertToString.h"
#include "../../../../CommonLib/str/StringEncoding.h"
#include "../../../../CommonLib/log/logger.h"
#include "../../../../CommonLib/log/ConsoleLogWriter.h"
#include "../../../../CommonLib/stream/MemoryStream.h"

#include "../../../../CommonLib/CommonLib_lib.h"
#include "../../mrCommonLib/mrCommonLib_lib.h"

#include "../../mrCommonLib/desktop/win/ScreenCapturerGdi.h"
#include "../../mrCommonLib/desktop/win/dx/ScreenCapturerDxgi.h"

#include "../../mrCommonLib/video/VideoEncoder.h"

#pragma comment(lib, "DXGI.lib")
#pragma comment(lib, "D3D11.lib")
#pragma comment(lib, "Bcrypt.lib")
#pragma comment(lib, "Secur32.lib")
#pragma comment(lib, "Ws2_32.lib")

#ifdef _DEBUG		  
	#pragma comment(lib, "../../../ThirdParty/libyuv/build/Debug/yuv.lib")
#else
	#pragma comment(lib, "../../../ThirdParty/libyuv/build/Release/yuv.lib")
#endif


#ifdef _DEBUG		  
#pragma comment(lib, "../../../ThirdParty/ffmpeg/libs/debug/libavcodec.lib")
#pragma comment(lib, "../../../ThirdParty/ffmpeg/libs/debug/libavdevice.lib")
#pragma comment(lib, "../../../ThirdParty/ffmpeg/libs/debug/libavfilter.lib")
#pragma comment(lib, "../../../ThirdParty/ffmpeg/libs/debug/libavformat.lib")
#pragma comment(lib, "../../../ThirdParty/ffmpeg/libs/debug/libavutil.lib")
#pragma comment(lib, "../../../ThirdParty/ffmpeg/libs/debug/libswresample.lib")
#pragma comment(lib, "../../../ThirdParty/ffmpeg/libs/debug/libswscale.lib")

#pragma comment(lib, "../../../ThirdParty/libvpx/x64/Debug/vpxmtd.lib")
#else
#pragma comment(lib, "../../../ThirdParty/ffmpeg/libs/release/libavcodec.lib")
#pragma comment(lib, "../../../ThirdParty/ffmpeg/libs/release/libavdevice.lib")
#pragma comment(lib, "../../../ThirdParty/ffmpeg/libs/release/libavfilter.lib")
#pragma comment(lib, "../../../ThirdParty/ffmpeg/libs/release/libavformat.lib")
#pragma comment(lib, "../../../ThirdParty/ffmpeg/libs/release/libavutil.lib")
#pragma comment(lib, "../../../ThirdParty/ffmpeg/libs/release/libswresample.lib")
#pragma comment(lib, "../../../ThirdParty/ffmpeg/libs/release/libswscale.lib")

#pragma comment(lib, "../../../ThirdParty/ibvpx/x64/Release/vpxmt.lib")
#endif
