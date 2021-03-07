// pch.h: This is a precompiled header file.
// Files listed below are compiled only once, improving build performance for future builds.
// This also affects IntelliSense performance, including code completion and many code browsing features.
// However, files listed here are ALL re-compiled if any one of them is updated between builds.
// Do not add files here that you will be updating frequently as this negates the performance advantage.

#ifndef PCH_H
#define PCH_H

// add headers that you want to pre-compile here
#include "framework.h"
#include <optional>

#include "../../../CommonLib/CommonLib.h"
#include "../../../CommonLib/alloc/alloc.h"
#include "../../../CommonLib/filesystem/File.h"
#include "../../../CommonLib/str/convertToString.h"
#include "../../../CommonLib/str/StringEncoding.h"
#include "../../../CommonLib/log/logger.h"
#include "../../../CommonLib/log/LogInfo.h"
#include "../../../CommonLib/stream/stream.h"
#include "../../../CommonLib/filesystem/File.h"
#include "../../../CommonLib/data/ByteArray.h"

#ifdef _WIN32

#include <dwmapi.h>
#include <wtsapi32.h>
#include <D3D11.h>
#include <DXGI.h>
#include <comdef.h>
#include <D3DCommon.h>

#include <wrl/client.h>

#include "../../../CommonLib/exception/WinExc.h"
#include "../../../CommonLib/exception/ComExc.h"
#endif


#endif //PCH_H
