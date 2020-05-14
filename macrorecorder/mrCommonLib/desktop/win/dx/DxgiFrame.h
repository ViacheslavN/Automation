#pragma once

#include "../../Frame.h"
#include "../../ScreenCapturer.h"
#include "DxgiContext.h"

namespace mrCommonLib
{
	namespace desktop {

		namespace win {

			class CDxgiDuplicatorController;

			// A pair of a SharedFrame and a DxgiDuplicatorController::Context for the client of
			// DxgiDuplicatorController.

			typedef std::shared_ptr<class CDxgiFrame> DxgiFramePtr;
			class CDxgiFrame 
			{
			public:

				using Context = CDxgiFrameContext;

				explicit CDxgiFrame(std::shared_ptr<CDxgiDuplicatorController> controller);
				~CDxgiFrame();

				// Should not be called if prepare() is not executed or returns false.
				IFramePtr Frame() const;

			private:
				// Allows DxgiDuplicatorController to access prepare() and context() function as well as
				// Context class.
				friend class CDxgiDuplicatorController;

				// Prepares current instance with desktop size and source id.
				void Prepare(const CSize& size, ScreenId source_id);

				// Should not be called if prepare() is not executed or returns false.
				Context* GetContext();

				//	ipc::SharedMemoryFactory* const shared_memory_factory_;
				std::optional<CSize> m_LastFrameSize;
				ScreenId m_sourceId = kFullDesktopScreenId;
				IFramePtr m_frame;
				Context m_context;
			};
		}
	}
}