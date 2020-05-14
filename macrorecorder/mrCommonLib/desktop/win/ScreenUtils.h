#pragma once

#include "../pixelformat.h"
#include "../ScreenCapturer.h"

namespace mrCommonLib
{
	namespace desktop
	{
		namespace win
		{
			class CScreenUtils
			{
			public:
				// Output the list of active screens into |screens|. Returns true if succeeded, or false if it
				// fails to enumerate the display devices.
				static void GetScreenList(ScreenList& screens);

				// Returns true if |screen| is a valid screen. The screen device key is returned through
				// |device_key| if the screen is valid. The device key can be used in screenRect to verify the
				// screen matches the previously obtained id.
				static bool IsScreenValid( ScreenId screen, wstr& device_key);

				// Get the rect of the entire system in system coordinate system. I.e. the primary monitor
				// always starts from (0, 0).
				static CRect FullScreenRect();

				// Get the rect of the screen identified by |screen|, relative to the primary display's
				// top-left.
				// If the screen device key does not match |device_key|, or the screen does not exist, or any
				// error happens, an empty rect is returned.
				static CRect ScreenRect(ScreenId screen, const wstr& device_key);

				// The number of visible display monitors on a desktop.
				static int ScreenCount();

				// Detects the current pixel format. If the current format is not supported or an error has
				// occurred, then returns PixelFormat::ARGB().
				static CPixelFormat DetectPixelFormat();
 
			};

		}
	}
}