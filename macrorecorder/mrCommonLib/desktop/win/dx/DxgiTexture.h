#pragma once
#include "../../Frame.h"
#include <D3D11.h>
#include <DXGI1_2.h>

namespace mrCommonLib
{
	namespace desktop {

		namespace win {

			// A texture copied or mapped from a DXGI_OUTDUPL_FRAME_INFO and IDXGIResource.
			class CDxgiTexture
			{
			public:
				// Creates a DxgiTexture instance, which represents the |desktop_size| area of
				// entire screen -- usually a monitor on the system.
				CDxgiTexture();
				virtual ~CDxgiTexture();

				// Copies selected regions of a frame represented by frame_info and resource.
				// Returns false if anything wrong.
				bool CopyFrom(const DXGI_OUTDUPL_FRAME_INFO& frame_info, IDXGIResource* resource);

				const CSize& DesktopSize() const { return m_DesktopSize; }
				uint8_t* Bits() const { return static_cast<uint8_t*>(m_rect.pBits); }
				int Pitch() const { return static_cast<int>(m_rect.Pitch); }

				// Releases the resource currently holds by this instance. Returns false if
				// anything wrong, and this instance should be deprecated in this state. bits,
				// pitch and AsDesktopFrame are only valid after a success CopyFrom() call,
				// but before Release() call.
				bool Release();

				// Returns a DesktopFrame snapshot of a DxgiTexture instance. This
				// DesktopFrame is used to copy a DxgiTexture content to another DesktopFrame
				// only. And it should not outlive its DxgiTexture instance.
				const IFramePtr AsDesktopFrame();

			protected:
				DXGI_MAPPED_RECT* Rect();

				virtual bool CopyFromTexture(const DXGI_OUTDUPL_FRAME_INFO& frame_info,
					ID3D11Texture2D* texture) = 0;

				virtual bool DoRelease() = 0;

			private:
				DXGI_MAPPED_RECT m_rect = { 0 };
				CSize m_DesktopSize;
				IFramePtr m_frame;
			};
		}
	}
}