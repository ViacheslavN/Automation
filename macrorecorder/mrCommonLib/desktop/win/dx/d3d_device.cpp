#include "pch.h"
#include "d3d_device.h"


namespace mrCommonLib
{
	namespace desktop {

			namespace win {


				using Microsoft::WRL::ComPtr;

				D3dDevice::D3dDevice() = default;
				D3dDevice::D3dDevice(const D3dDevice& other) = default;
				D3dDevice& D3dDevice::operator=(const D3dDevice& other) = default;
				D3dDevice::D3dDevice(D3dDevice&& other) = default;
				D3dDevice& D3dDevice::operator=(D3dDevice&& other) = default;
				D3dDevice::~D3dDevice() = default;

				void D3dDevice::initialize(const ComPtr<IDXGIAdapter>& adapter)
				{
					m_dxgi_adapter = adapter;
					if (!m_dxgi_adapter)
					{
						throw CommonLib::CExcBase("An empty IDXGIAdapter instance has been received");
						 
					}

					D3D_FEATURE_LEVEL feature_level;

					// Default feature levels contain D3D 9.1 through D3D 11.0.
					_com_error error = D3D11CreateDevice(
						adapter.Get(), D3D_DRIVER_TYPE_UNKNOWN, nullptr,
						D3D11_CREATE_DEVICE_BGRA_SUPPORT | D3D11_CREATE_DEVICE_SINGLETHREADED,
						nullptr, 0, D3D11_SDK_VERSION, m_d3d_device.GetAddressOf(), &feature_level,
						m_context.GetAddressOf());
					if (error.Error() != S_OK || !m_d3d_device || !m_context)
						throw CommonLib::CComExc("D3D11CreateDeivce returns error ", error);
		

					/*if (feature_level < D3D_FEATURE_LEVEL_11_0)
					{
					
					}*/

					error = m_d3d_device.As(&m_dxgi_device);
					if (error.Error() != S_OK || !m_dxgi_device)
						throw CommonLib::CComExc("ID3D11Device is not an implementation of IDXGIDevice, this usually "
							"means the system does not support DirectX 11. Error", error);
				}

				// static
				std::vector<D3dDevice> D3dDevice::enumDevices()
				{
					ComPtr<IDXGIFactory1> factory;

					_com_error error = CreateDXGIFactory1(__uuidof(IDXGIFactory1),
						reinterpret_cast<void**>(factory.GetAddressOf()));

					if (error.Error() != S_OK || !factory)
						return std::vector<D3dDevice>();

					std::vector<D3dDevice> result;
					for (int i = 0;; ++i)
					{
						ComPtr<IDXGIAdapter> adapter;

						error = factory->EnumAdapters(i, adapter.GetAddressOf());
						if (error.Error() == S_OK)
						{
							D3dDevice device;

							device.initialize(adapter);
							result.emplace_back(std::move(device));
						}
						else if (error.Error() == DXGI_ERROR_NOT_FOUND)
						{
							break;
						}
					}

					return result;
				}
		}
	}
}