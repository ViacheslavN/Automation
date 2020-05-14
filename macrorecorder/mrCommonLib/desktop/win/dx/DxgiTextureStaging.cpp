#pragma once
#include "pch.h"
#include "DxgiTextureStaging.h"


namespace mrCommonLib
{
	namespace desktop {

		namespace win {
			using Microsoft::WRL::ComPtr;
			CDxgiTextureStaging::CDxgiTextureStaging(const CD3dDevice& device)
				: m_device(device)
			{
			}

			CDxgiTextureStaging::~CDxgiTextureStaging() = default;

			bool CDxgiTextureStaging::InitializeStage(ID3D11Texture2D* texture)
			{

				D3D11_TEXTURE2D_DESC desc = { 0 };
				texture->GetDesc(&desc);

				desc.ArraySize = 1;
				desc.BindFlags = 0;
				desc.CPUAccessFlags = D3D11_CPU_ACCESS_READ;
				desc.MipLevels = 1;
				desc.MiscFlags = 0;
				desc.SampleDesc.Count = 1;
				desc.SampleDesc.Quality = 0;
				desc.Usage = D3D11_USAGE_STAGING;
				if (m_stage)
				{
					//AssertStageAndSurfaceAreSameObject();

					D3D11_TEXTURE2D_DESC current_desc;
					m_stage->GetDesc(&current_desc);

					const bool recreate_needed =
						(memcmp(&desc, &current_desc, sizeof(D3D11_TEXTURE2D_DESC)) != 0);
					if (!recreate_needed)
						return true;

					// The descriptions are not consistent, we need to create a new ID3D11Texture2D instance.
					m_stage.Reset();
					m_surface.Reset();
				}
				else
				{
				}

				_com_error error = m_device.D3dDevice()->CreateTexture2D(
					&desc, nullptr, m_stage.GetAddressOf());
				if (error.Error() != S_OK || !m_stage)
				{
					Log.Error("Failed to create a new ID3D11Texture2D as stage, error %1", CommonLib::CComExc(error).what());
					return false;
				}

				error = m_stage.As(&m_surface);
				if (error.Error() != S_OK || !m_surface)
				{
					Log.Error("Failed to convert ID3D11Texture2D to IDXGISurface, error %1", CommonLib::CComExc(error).what());
					return false;
				}

				return true;
			}

			void CDxgiTextureStaging::AssertStageAndSurfaceAreSameObject()
			{
				ComPtr<IUnknown> left;
				ComPtr<IUnknown> right;

				bool left_result = SUCCEEDED(m_stage.As(&left));
				bool right_result = SUCCEEDED(m_surface.As(&right));

			//	DCHECK(left_result);
			//	DCHECK(right_result);
			//	DCHECK(left.Get() == right.Get());
			}

			bool CDxgiTextureStaging::CopyFromTexture(const DXGI_OUTDUPL_FRAME_INFO& frame_info,
				ID3D11Texture2D* texture)
			{

				// AcquireNextFrame returns a CPU inaccessible IDXGIResource, so we need to
				// copy it to a CPU accessible staging ID3D11Texture2D.
				if (!InitializeStage(texture))
					return false;

				m_device.Context()->CopyResource(static_cast<ID3D11Resource*>(m_stage.Get()),
					static_cast<ID3D11Resource*>(texture));

				*Rect() = { 0 };

				_com_error error = m_surface->Map(Rect(), DXGI_MAP_READ);
				if (error.Error() != S_OK)
				{
					*Rect() = { 0 };
					Log.Error("Failed to map the IDXGISurface to a bitmap, error %1", CommonLib::CComExc(error).what());
					return false;
				}

				return true;
			}

			bool CDxgiTextureStaging::DoRelease()
			{
				_com_error error = m_surface->Unmap();
				if (error.Error() != S_OK)
				{
					m_stage.Reset();
					m_surface.Reset();
				}

				// If using staging mode, we only need to recreate ID3D11Texture2D instance.
				// This will happen during next CopyFrom call. So this function always returns true.
				return true;
			}

		}
	}
}