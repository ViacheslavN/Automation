#pragma once

#include "geometry.h"
#include "pixelformat.h"
#include "Frame.h"

namespace mrCommonLib
{
	namespace desktop
	{
		typedef int ScreenId;

		struct CScreen
		{
			ScreenId id;
			astr title;
		};

		typedef std::vector<CScreen> ScreenList;


		static const ScreenId kFullDesktopScreenId = -1;
		static const ScreenId kInvalidScreenId = -2;

		typedef std::shared_ptr<class IScreenCapturer> TScreenCapturerPtr;
		class IScreenCapturer
		{
		public:
			IScreenCapturer(){}
			virtual ~IScreenCapturer() {}

			virtual IFramePtr CaptureFrame() = 0;
			virtual CPixelFormat GetPixelFormat() const = 0;
			virtual int GetScreenCount() = 0;
			virtual void GetScreenList(ScreenList& screens) = 0;
			virtual void SelectScreen(ScreenId screen_id) = 0;
		};

	}
}