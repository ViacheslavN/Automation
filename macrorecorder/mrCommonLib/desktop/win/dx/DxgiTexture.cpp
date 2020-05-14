#include "pch.h"
#include "../../geometry.h"
#include "../../DataFrame.h"
#include "DxgiTexture.h"


namespace mrCommonLib
{
	namespace desktop {

		namespace win {
			using Microsoft::WRL::ComPtr;

			namespace {

				class DxgiDesktopFrame : public CDataFrame
				{
				public:
					explicit DxgiDesktopFrame(const CDxgiTexture& texture): CDataFrame(CPixelFormat::ARGB(), texture.DesktopSize(), texture.Bits())
					{}

					~DxgiDesktopFrame() override = default;
				};

			} // namespace

			CDxgiTexture::CDxgiTexture() = default;
			CDxgiTexture::~CDxgiTexture() = default;

			bool CDxgiTexture::CopyFrom(const DXGI_OUTDUPL_FRAME_INFO& frame_info, IDXGIResource* resource)
			{
				//DCHECK_GT(frame_info.AccumulatedFrames, 0);
				//DCHECK(resource);

				ComPtr<ID3D11Texture2D> texture;

				_com_error error = resource->QueryInterface(__uuidof(ID3D11Texture2D),
					reinterpret_cast<void**>(texture.GetAddressOf()));
				if (error.Error() != S_OK || !texture)
				{
					//LOG(LS_ERROR) << "Failed to convert IDXGIResource to ID3D11Texture2D, error "
					//	<< error.ErrorMessage() << ", code " << error.Error();
					return false;
				}

				D3D11_TEXTURE2D_DESC desc = { 0 };
				texture->GetDesc(&desc);

				m_DesktopSize.Set(desc.Width, desc.Height);

				return CopyFromTexture(frame_info, texture.Get());
			}

			const IFramePtr CDxgiTexture::AsDesktopFrame()
			{
				if (!m_frame)
					m_frame.reset(new DxgiDesktopFrame(*this));

				return m_frame;
			}

			bool CDxgiTexture::Release()
			{
				m_frame.reset();
				return DoRelease();
			}

			DXGI_MAPPED_RECT* CDxgiTexture::Rect()
			{
				return &m_rect;
			}
		}
	}
}