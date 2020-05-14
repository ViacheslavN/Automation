#include "pch.h"
#include "DxgiTextureMapping.h"


namespace mrCommonLib
{
	namespace desktop {

		namespace win {


			CDxgiTextureMapping::CDxgiTextureMapping(IDXGIOutputDuplication* duplication)
				: m_duplication(duplication)
			{
			}

			CDxgiTextureMapping::~CDxgiTextureMapping() = default;

			bool CDxgiTextureMapping::CopyFromTexture(const DXGI_OUTDUPL_FRAME_INFO& frame_info,
				ID3D11Texture2D* texture)
			{

				*Rect() = { 0 };

				_com_error error = m_duplication->MapDesktopSurface(Rect());
				if (error.Error() != S_OK)
				{
					*Rect() = { 0 };
					Log.Error("Failed to map the IDXGIOutputDuplication to a bitmap, error %1", CommonLib::CComExc(error).what());
					return false;
				}

				return true;
			}

			bool CDxgiTextureMapping::DoRelease()
			{
				_com_error error = m_duplication->UnMapDesktopSurface();
				if (error.Error() != S_OK)
				{
					Log.Error("Failed to unmap the IDXGIOutputDuplication, error %1", CommonLib::CComExc(error).what());
					return false;
				}

				return true;
			}
		}
	}
}