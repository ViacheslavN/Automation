#include "pch.h"
#include "DxgiContext.h"
#include "DxgiDuplicatorController.h"

namespace mrCommonLib
{
	namespace desktop {

		namespace win {
			CDxgiAdapterContext::CDxgiAdapterContext() = default;
			CDxgiAdapterContext::CDxgiAdapterContext(const CDxgiAdapterContext& context) = default;
			CDxgiAdapterContext& CDxgiAdapterContext::operator=(const CDxgiAdapterContext& other) = default;
			CDxgiAdapterContext::~CDxgiAdapterContext() = default;

			CDxgiFrameContext::CDxgiFrameContext(std::shared_ptr<CDxgiDuplicatorController>& controller)
				: m_controller(controller)
			{
				// Nothing
			}

			CDxgiFrameContext::~CDxgiFrameContext()
			{
				Reset();
			}

			void CDxgiFrameContext::Reset()
			{
				m_controller->Unregister(this);
				m_controllerId = 0;
			}
		}
	}
}