// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//  are changed infrequently
//

#pragma once

// Change these values to use different versions
#define WINVER		0x0601
#define _WIN32_WINNT	0x0601
#define _WIN32_IE	0x0700
#define _RICHEDIT_VER	0x0500

#include <atlbase.h>
#include <atlapp.h>

extern CAppModule _Module;

#include <atlwin.h>

#include <atlframe.h>
#include <atlctrls.h>
#include <atldlgs.h>
#include <atlctrlw.h>



#include "../../../../CommonLib/CommonLib.h"
#include "../../../../CommonLib/alloc/alloc.h"
#include "../../../../CommonLib/alloc/stl_alloc.h"
#include "../../../../CommonLib/filesystem/File.h"
#include "../../../../CommonLib/str/convertToString.h"
#include "../../../../CommonLib/str/StringEncoding.h"
#include "../../../../CommonLib/log/logger.h"
#include "../../../../CommonLib/log/ConsoleLogWriter.h"
#include "../../../../CommonLib/log/FileAsyncLogger.h"
#include "../../../../CommonLib/stream/MemoryStream.h"
#include "../../../../CommonLib/network/BaseSocketIP.h"
#include "../../../../CommonLib/network/ClientSocket.h"
#include "../../../../CommonLib/synch/thread.h"
#include "../../../../CommonLib/synch/SyncQueue.h"

#include "../../../../CommonLib/CommonLib_lib.h"
#include "../../mrCommonLib/mrCommonLib_lib.h"

#include "../../mrCommonLib/desktop/win/ScreenCapturerGdi.h"
#include "../../mrCommonLib/desktop/win/dx/ScreenCapturerDxgi.h"
#include "../../mrCommonLib/desktop/DataFrame.h"



#include "../../mrCommonLib/video/VideoEncoder.h"
#include "../../mrCommonLib/video/VideoDecoder.h"

typedef CommonLib::STLAllocator<byte_t> TAlloc;
typedef std::vector<byte_t, TAlloc> TVecBuffer;

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
#pragma comment(lib, "../../../ThirdParty/x265/source/lib/Debug/common.lib")
#pragma comment(lib, "../../../ThirdParty/x265/source/lib/Debug/encoder.lib")
#else
#pragma comment(lib, "../../../ThirdParty/x265/source/lib/release/common.lib")
#pragma comment(lib, "../../../ThirdParty/x265/source/lib/release/encoder.lib")
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

#pragma comment(lib, "../../../ThirdParty/libvpx/x64/Release/vpxmt.lib")
#endif



#if defined _M_IX86
  #pragma comment(linker, "/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='x86' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_IA64
  #pragma comment(linker, "/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='ia64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_X64
  #pragma comment(linker, "/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='amd64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#else
  #pragma comment(linker, "/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#endif
