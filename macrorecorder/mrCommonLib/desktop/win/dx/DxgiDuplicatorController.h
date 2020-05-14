#pragma once

#include "../DisplayConfigurationMonitor.h"
#include "DxgiAdapterDuplicator.h"
#include "DxgiFrame.h"

namespace mrCommonLib
{
	namespace desktop {

		namespace win {
			// A controller for all the objects we need to call Windows DirectX capture APIs It's a singleton
// because only one IDXGIOutputDuplication instance per monitor is allowed per application.
//
// Consumers should create a DxgiDuplicatorController::Context and keep it throughout their
// lifetime, and pass it when calling duplicate(). Consumers can also call isSupported() to
// determine whether the system supports DXGI duplicator or not. If a previous isSupported()
// function call returns true, but a later duplicate() returns false, this usually means the
// display mode is changing. Consumers should retry after a while. (Typically 50 milliseconds,
// but according to hardware performance, this time may vary.)
			class CDxgiDuplicatorController
			{
			public:
				CDxgiDuplicatorController();
				~CDxgiDuplicatorController();

				using Context = CDxgiFrameContext;

				// A collection of D3d information we are interested on, which may impact capturer performance
				// or reliability.
				struct D3dInfo
				{
					// Each video adapter has its own D3D_FEATURE_LEVEL, so this structure contains the minimum
					// and maximium D3D_FEATURE_LEVELs current system supports.
					// Both fields can be 0, which is the default value to indicate no valid D3D_FEATURE_LEVEL
					// has been retrieved from underlying OS APIs.
					D3D_FEATURE_LEVEL min_feature_level;
					D3D_FEATURE_LEVEL max_feature_level;

					// TODO(zijiehe): Add more fields, such as manufacturer name, mode, driver version.
				};

				enum class Result
				{
					SUCCEEDED,
					UNSUPPORTED_SESSION,
					FRAME_PREPARE_FAILED,
					INITIALIZATION_FAILED,
					DUPLICATION_FAILED,
					INVALID_MONITOR_ID,
				};

				// Converts |result| into user-friendly string representation. The return value should not be
				// used to identify error types.
				static const char* ResultName(Result result);

				// See ScreenCapturerWinDirectx::isCurrentSessionSupported().
				static bool IsCurrentSessionSupported();

				// All the following public functions implicitly call initialize() function.

				// Detects whether the system supports DXGI based capturer.
				bool IsSupported();

				// Returns a copy of D3dInfo composed by last Initialize() function call. This function always
				// copies the latest information into |info|. But once the function returns false, the
				// information in |info| may not accurate.
				bool RetrieveD3dInfo(D3dInfo* info);

				// Captures current screen and writes into |frame|.
				// TODO(zijiehe): Windows cannot guarantee the frames returned by each IDXGIOutputDuplication
				// are synchronized. But we are using a totally different threading model than the way Windows
				// suggested, it's hard to synchronize them manually. We should find a way to do it.
				Result Duplicate(CDxgiFrame* frame);

				// Captures one monitor and writes into target. |monitor_id| should >= 0. If |monitor_id| is
				// greater than the total screen count of all the Duplicators, this function returns false.
				Result DuplicateMonitor(CDxgiFrame* frame, int monitor_id);

				// Returns the count of screens on the system. These screens can be retrieved by an integer in
				// the range of [0, screenCount()). If system does not support DXGI based capturer, this
				// function returns 0.
				int ScreenCount();

				// Returns the device names of all screens on the system. These screens can be retrieved by an
				// integer in the range of [0, output->size()). If system does not support DXGI based capturer,
				// this function returns false.
				bool DeviceNames(std::vector<wstr>& output);

			private:
				// DxgiFrameContext calls private unregister(Context*) function in reset().
				friend void CDxgiFrameContext::Reset();

				// Does the real duplication work. Setting |monitor_id| < 0 to capture entire screen. This
				// function calls initialize(). And if the duplication failed, this function calls
				// deinitialize() to ensure the Dxgi components can be reinitialized next time.
				Result DoDuplicate(CDxgiFrame* frame, int monitor_id);

				// Unregisters Context from this instance and all DxgiAdapterDuplicator(s) it owns.
				void Unregister(const Context* const context);

				// All functions below should be called should be after a successful initialize().

				// If current instance has not been initialized, executes doInitialize() function, and returns
				// initialize result. Otherwise directly returns true.
				// This function may calls deinitialize() if initialization failed.
				bool Initialize();

				// Does the real initialization work, this function should only be called in initialize().
				bool DoInitialize();

				// Clears all COM components referred by this instance. So next duplicate() call will
				// eventually initialize this instance again.
				void Deinitialize();

				// A helper function to check whether a Context has been expired.
				bool ContextExpired(const Context* const context) const;

				// Updates Context if needed.
				void Setup(Context* context);

				// Captures all monitors.
				bool DoDuplicateAll(Context* context, IFramePtr target);

				// Captures one monitor.
				bool DoDuplicateOne(Context* context, int monitor_id, IFramePtr target);

				// The minimum numFramesCaptured() returned by |duplicators_|.
				int64_t NumFramesCaptured() const;

				// Returns a DesktopSize to cover entire |desktop_rect_|.
				CSize DesktopSize() const;

				// Returns the size of one screen. |id| should be >= 0. If system does not support DXGI based
				// capturer, or |id| is greater than the total screen count of all the Duplicators, this
				// function returns an empty Rect.
				CRect ScreenRect(int id) const;

				int DoScreenCount() const;

				// Returns the desktop size of the selected screen |monitor_id|. Setting |monitor_id| < 0 to
				// return the entire screen size.
				CSize SelectedDesktopSize(int monitor_id) const;

				// Retries doDuplicateAll() for several times until numFramesCaptured() is large enough.
				// Returns false if doDuplicateAll() returns false, or numFramesCaptured() has never reached
				// the requirement.
				// According to http://crbug.com/682112, dxgi capturer returns a black frame during first
				// several capture attempts.
				bool EnsureFrameCaptured(Context* context, IFramePtr target);

				// Moves |desktop_rect_| and all underlying |duplicators_|, putting top left corner of the
				// desktop at (0, 0). This is necessary because DXGI_OUTPUT_DESC may return negative
				// coordinates. Called from doInitialize() after all DxgiAdapterDuplicator and
				// DxgiOutputDuplicator instances are initialized.
				void TranslateRect();

				// A self-incremented integer to compare with the one in Context. It ensures a Context instance
				// is always initialized after DxgiDuplicatorController.
				int m_identity = 0;
				CRect m_desktop_rect;
				std::vector<CDxgiAdapterDuplicator> m_duplicators;
				D3dInfo m_d3dInfo;
				CDisplayConfigurationMonitor m_displayConfigurationMonitor;
				// A number to indicate how many succeeded duplications have been performed.
				uint32_t m_succeededDuplications = 0;
			};

		}
	}
}