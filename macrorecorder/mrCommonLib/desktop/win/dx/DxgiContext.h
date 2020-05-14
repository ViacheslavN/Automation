#pragma once

#include "../../region.h"

namespace mrCommonLib
{
	namespace desktop {

		namespace win {
			class CDxgiDuplicatorController;

			// A DxgiOutputContext stores the status of a single DxgiFrame of DxgiOutputDuplicator.
			struct SDxgiOutputContext final
			{
				// The updated region DxgiOutputDuplicator::detectUpdatedRegion() output during last
				// duplicate() function call. It's always relative to the (0, 0).
				CRegion updated_region;
			};

			// A DxgiAdapterContext stores the status of a single DxgiFrame of DxgiAdapterDuplicator.
			struct CDxgiAdapterContext final
			{
				CDxgiAdapterContext();
				CDxgiAdapterContext(const CDxgiAdapterContext& other);
				CDxgiAdapterContext& operator=(const CDxgiAdapterContext& other);
				~CDxgiAdapterContext();

				// Child DxgiOutputContext belongs to this AdapterContext.
				std::vector<SDxgiOutputContext> contexts;
			};

			// A DxgiFrameContext stores the status of a single DxgiFrame of DxgiDuplicatorController.
			struct CDxgiFrameContext final
			{
			public:
				explicit CDxgiFrameContext(std::shared_ptr<CDxgiDuplicatorController>& controller);

				// Unregister this Context instance from DxgiDuplicatorController during destructing.
				~CDxgiFrameContext();

				// Reset current Context, so it will be reinitialized next time.
				void Reset();

				// A Context will have an exactly same |controller_id| as DxgiDuplicatorController, to ensure
				// it has been correctly setted up after each DxgiDuplicatorController::initialize().
				int m_controllerId = 0;

				// Child DxgiAdapterContext belongs to this DxgiFrameContext.
				std::vector<CDxgiAdapterContext> m_contexts;

				std::shared_ptr<CDxgiDuplicatorController> m_controller;
			};
		}
	}
}