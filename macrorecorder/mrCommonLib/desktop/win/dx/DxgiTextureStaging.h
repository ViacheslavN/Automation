#pragma once

#include <D3D11.h>
#include <DXGI1_2.h>

#include "D3dDevice.h"
#include "DxgiTexture.h"

namespace mrCommonLib
{
	namespace desktop {

		namespace win {
			// A pair of an ID3D11Texture2D and an IDXGISurface. We need an ID3D11Texture2D instance to copy
			// GPU texture to RAM, but an IDXGISurface instance to map the texture into a bitmap buffer. These
			// two instances are pointing to a same object.
			//
			// An ID3D11Texture2D is created by an ID3D11Device, so a DxgiTexture cannot be shared between two
			// DxgiAdapterDuplicators.
			class CDxgiTextureStaging : public CDxgiTexture
			{
			public:
				// Creates a DxgiTextureStaging instance. Caller must maintain the lifetime of input device to
				// make sure it outlives this instance.
				explicit CDxgiTextureStaging(const CD3dDevice& device);
				~CDxgiTextureStaging() override;

			protected:
				// Copies selected regions of a frame represented by frame_info and texture.
				// Returns false if anything wrong.
				bool CopyFromTexture(const DXGI_OUTDUPL_FRAME_INFO& frame_info,
					ID3D11Texture2D* texture) override;

				bool DoRelease() override;

			private:
				// Initializes stage_ from a CPU inaccessible IDXGIResource. Returns false if it failed to
				// execute Windows APIs, or the size of the texture is not consistent with desktop_rect.
				bool InitializeStage(ID3D11Texture2D* texture);

				// Makes sure stage_ and surface_ are always pointing to a same object.
				// We need an ID3D11Texture2D instance for ID3D11DeviceContext::CopySubresourceRegion, but an
				// IDXGISurface for IDXGISurface::Map.
				void AssertStageAndSurfaceAreSameObject();

				const CD3dDevice m_device;
				Microsoft::WRL::ComPtr<ID3D11Texture2D> m_stage;
				Microsoft::WRL::ComPtr<IDXGISurface> m_surface;
			};
		}
	}
}