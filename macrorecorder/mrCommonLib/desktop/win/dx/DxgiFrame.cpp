#include "pch.h"
#include "DxgiFrame.h"
#include "../../DataFrame.h"

namespace mrCommonLib
{
	namespace desktop {

		namespace win {
					   			 		  


			CDxgiFrame::CDxgiFrame(std::shared_ptr<CDxgiDuplicatorController> controller)
				: m_context(controller)
			{
				// Nothing
			}

			CDxgiFrame::~CDxgiFrame() = default;

			void CDxgiFrame::Prepare(const CSize& size, ScreenId source_id)
			{
				if (source_id != m_sourceId)
				{
					// Once the source has been changed, the entire source should be copied.
					m_sourceId = source_id;
					m_context.Reset();
				}

				if (!m_LastFrameSize.has_value() || m_LastFrameSize != size)
				{
					// Save the last frame size.
					m_LastFrameSize.emplace(size);

					// Once the output size changed, recreate the SharedFrame.
					m_frame.reset();
				}

				if (!m_frame)
				{

					m_frame.reset(new CDataFrame(CPixelFormat::ARGB(), size,  CommonLib::IAllocPtr()));
					

					const CSize& frame_size = m_frame->Size();

					// DirectX capturer won't paint each pixel in the frame due to its one
					// capturer per monitor design. So once the new frame is created, we should
					// clear it to avoid the legacy image to be remained on it. See
					// http://crbug.com/708766.
					//DCHECK_EQ(frame->stride(), frame_size.width() * frame->format().bytesPerPixel());
					memset(m_frame->Data(), 0, m_frame->Stride() * frame_size.Height());

				}
			}

			IFramePtr CDxgiFrame::Frame() const
			{
				return m_frame;
			}

			CDxgiFrame::Context* CDxgiFrame::GetContext()
			{
				return &m_context;
			}
		}
	}
}
