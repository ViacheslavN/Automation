#pragma once

#include <DXGI.h>
#include <DXGI1_2.h>

#include "../../frame_rotation.h"
#include "../../frame.h"
#include "D3dDevice.h"
#include "DxgiContext.h"
#include "DxgiTexture.h"


namespace mrCommonLib
{
	namespace desktop {

		namespace win {

			class CDxgiOutputDuplicator
			{
			public:
				using Context = SDxgiOutputContext;

				// Creates an instance of DxgiOutputDuplicator from a D3dDevice and one of its IDXGIOutput1.
				// Caller must maintain the lifetime of device, to make sure it outlives this instance. Only
				// DxgiAdapterDuplicator can create an instance.
				CDxgiOutputDuplicator(const CD3dDevice& device,
					const Microsoft::WRL::ComPtr<IDXGIOutput1>& output,
					const DXGI_OUTPUT_DESC& desc);

				// To allow this class to work with vector.
				CDxgiOutputDuplicator(CDxgiOutputDuplicator&& other);

				// Destructs this instance. We need to make sure texture_ has been released before duplication_.
				~CDxgiOutputDuplicator();

				// Initializes duplication object.
				bool Initialize();

				// Copies the content of current IDXGIOutput to the |target|. To improve the performance, this
				// function copies only regions merged from |context|->updated_region and detectUpdatedRegion().
				// The |offset| decides the offset in the |target| where the content should be copied to. i.e.
				// this function copies the content to the rectangle of (offset.x(), offset.y()) to
				// (offset.x() + desktop_rect_.width(), offset.y() + desktop_rect_.height()).
				// Returns false in case of a failure.
				bool Duplicate(Context* context, const CPoint& offset, IFramePtr target);

				// Returns the desktop rect covered by this DxgiOutputDuplicator.
				const CRect& DesktopRect() const { return m_desktopRect; }

				// Returns the device name from DXGI_OUTPUT_DESC.
				const wstr& DeviceName() const { return m_deviceName; }

				void Setup(Context* context);

				void Unregister(const Context* const context);

				// How many frames have been captured by this DxigOutputDuplicator.
				int64_t NumFramesCaptured() const;

				// Moves |desktop_rect_|. See DxgiDuplicatorController::translateRect().
				void TranslateRect(const CPoint& position);

			private:
				// Calls doDetectUpdatedRegion(). If it fails, this function sets the |updated_region| as
				// entire untranslatedDesktopRect().
				void DetectUpdatedRegion(const DXGI_OUTDUPL_FRAME_INFO& frame_info, CRegion* updated_region);

				// Returns untranslated updated region, which are directly returned by Windows APIs. Returns
				// false in case of a failure.
				bool DoDetectUpdatedRegion(const DXGI_OUTDUPL_FRAME_INFO& frame_info, CRegion* updated_region);

				bool ReleaseFrame();

				// Initializes duplication_ instance. Expects duplication_ is in empty status.
				// Returns false if system does not support IDXGIOutputDuplication.
				bool DuplicateOutput();

				// Returns a Rect with the same size of desktopSize(), but translated by offset.
				CRect TranslatedDesktopRect(const CPoint& offset) const;

				// Returns a Rect with the same size of desktopSize(), but starts from (0, 0).
				CRect UntranslatedDesktopRect() const;

				// Spreads changes from |context| to other registered Context(s) in contexts_.
				void SpreadContextChange(const Context* const context);

				// Returns the size of desktop rectangle current instance representing.
				CSize DesktopSize() const;

				const CD3dDevice m_device;
				const Microsoft::WRL::ComPtr<IDXGIOutput1> m_output;
				const wstr m_deviceName;
				CRect m_desktopRect;
				Microsoft::WRL::ComPtr<IDXGIOutputDuplication> m_Duplication;
				DXGI_OUTDUPL_DESC m_desc;
				std::vector<uint8_t> m_metadata;
				std::unique_ptr<CDxgiTexture> m_texture;
				Rotation m_rotation = Rotation::CLOCK_WISE_0;
				CSize m_unrotatedSize;

				// After each AcquireNextFrame() function call, updated_region_(s) of all active Context(s)
				// need to be updated. Since they have missed the change this time. And during next duplicate()
				// function call, their updated_region_ will be merged and copied.
				std::vector<Context*> m_Contexts;

				// The last full frame of this output and its offset. If on AcquireNextFrame() failed because
				// of timeout, i.e. no update, we can copy content from |last_frame_|.
				IFramePtr m_lastFrame;
				CPoint m_lastFrameOffset;

				int64_t m_numFramesCaptured = 0;
			};
		}
	}
}