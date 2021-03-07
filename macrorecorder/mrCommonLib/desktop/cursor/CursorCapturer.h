#pragma once
namespace mrCommonLib
{
	namespace desktop
	{

		class CMouseCursor;
		typedef std::shared_ptr<class ICursorCapturer> TCursorCapturerPtr;
		class ICursorCapturer
		{
		public:
			virtual ~ICursorCapturer() = default;

			virtual const std::shared_ptr<CMouseCursor> CaptureCursor() = 0;
			virtual void Reset() = 0;
		};

	}
}
