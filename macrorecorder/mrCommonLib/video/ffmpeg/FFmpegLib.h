#pragma once


#ifdef _WIN32

#ifdef _DEBUG		  
#pragma comment(lib, "../../../../../ThirdParty/ffmpeg/libs/debug/libavcodec.lib")
#pragma comment(lib, "../../../../../ThirdParty/ffmpeg/libs/debug/libavdevice.lib")
#pragma comment(lib, "../../../../../ThirdParty/ffmpeg/libs/debug/libavfilter.lib")
#pragma comment(lib, "../../../../../ThirdParty/ffmpeg/libs/debug/libavformat.lib")
#pragma comment(lib, "../../../../../ThirdParty/ffmpeg/libs/debug/libavutil.lib")
#pragma comment(lib, "../../../../../ThirdParty/ffmpeg/libs/debug/libswresample.lib")
#pragma comment(lib, "../../../../../ThirdParty/ffmpeg/libs/debug/libswscale.lib")
#else
#pragma comment(lib, "../../../../../ThirdParty/ffmpeg/libs/release/libavcodec.lib")
#pragma comment(lib, "../../../../../ThirdParty/ffmpeg/libs/release/libavdevice.lib")
#pragma comment(lib, "../../../../../ThirdParty/ffmpeg/libs/release/libavfilter.lib")
#pragma comment(lib, "../../../../../ThirdParty/ffmpeg/libs/release/libavformat.lib")
#pragma comment(lib, "../../../../../ThirdParty/ffmpeg/libs/release/libavutil.lib")
#pragma comment(lib, "../../../../../ThirdParty/ffmpeg/libs/release/libswresample.lib")
#pragma comment(lib, "../../../../../ThirdParty/ffmpeg/libs/release/libswscale.lib")
#endif

#endif

