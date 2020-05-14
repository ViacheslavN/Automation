#pragma once

#include "DxgiTexture.h"

#include <D3D11.h>
#include <DXGI1_2.h>

namespace mrCommonLib
{
	namespace desktop {

		namespace win {

			// A DxgiTexture which directly maps bitmap from IDXGIResource. This class is used when
			// DXGI_OUTDUPL_DESC.DesktopImageInSystemMemory is true. (This usually means the video card shares
			// main memory with CPU, instead of having its own individual memory.)
			class CDxgiTextureMapping : public CDxgiTexture
			{
			public:
				// Creates a DxgiTextureMapping instance. Caller must maintain the lifetime of input
				// |duplication| to make sure it outlives this instance.
				explicit CDxgiTextureMapping(IDXGIOutputDuplication* duplication);
				~CDxgiTextureMapping() override;

			protected:
				bool CopyFromTexture(const DXGI_OUTDUPL_FRAME_INFO& frame_info,
					ID3D11Texture2D* texture) override;

				bool DoRelease() override;

			private:
				IDXGIOutputDuplication* const m_duplication;
			};
		}
	}
}