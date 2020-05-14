#pragma once

#include "DxgiContext.h"
#include "D3dDevice.h"
#include "DxgiOutputDuplicator.h"

namespace mrCommonLib
{
	namespace desktop {

		namespace win {

			class CDxgiAdapterDuplicator
			{
			public:
				using Context = CDxgiAdapterContext;

				// Creates an instance of DxgiAdapterDuplicator from a D3dDevice. Only
				// DxgiDuplicatorController can create an instance.
				explicit CDxgiAdapterDuplicator(const CD3dDevice& device);

				// Move constructor, to make it possible to store instances of DxgiAdapterDuplicator in
				// std::vector<>.
				CDxgiAdapterDuplicator(CDxgiAdapterDuplicator&& other);

				~CDxgiAdapterDuplicator();

				// Initializes the DxgiAdapterDuplicator from a D3dDevice.
				bool Initialize();

				// Sequentially calls Duplicate function of all the DxgiOutputDuplicator instances owned by
				// this instance, and writes into |target|.
				bool Duplicate(Context* context, IFramePtr target);

				// Captures one monitor and writes into |target|. |monitor_id| should be between [0, screenCount()).
				bool DuplicateMonitor(Context* context, int monitor_id, IFramePtr target);

				// Returns desktop rect covered by this DxgiAdapterDuplicator.
				const CRect& DesktopRect() const { return m_desktop_rect; }

				// Returns the size of one screen owned by this DxgiAdapterDuplicator. |id| should be between
				// [0, screenCount()).
				CRect ScreenRect(int id) const;

				// Returns the device name of one screen owned by this DxgiAdapterDuplicator. |id| should be
				// between [0, screenCount()).
				const wstr& DeviceName(int id) const;

				// Returns the count of screens owned by this DxgiAdapterDuplicator. These screens can be
				// retrieved by an interger in the range of [0, screenCount()).
				int ScreenCount() const;

				void Setup(Context* context);

				void Unregister(const Context* const context);

				// The minimum num_frames_captured() returned by |duplicators_|.
				int64_t NumFramesCaptured() const;

				// Moves |desktop_rect_| and all underlying |duplicators_|. See
				// DxgiDuplicatorController::translateRect().
				void TranslateRect(const CPoint& position);

			private:
				bool DoInitialize();

				const CD3dDevice m_device;
				std::vector<CDxgiOutputDuplicator> m_duplicators;
				CRect m_desktop_rect;
			};
		}
	}
}