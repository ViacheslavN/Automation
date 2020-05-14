#include "pch.h"
#include "D3dDevice.h"


namespace mrCommonLib
{
	namespace desktop {

			namespace win {


				using Microsoft::WRL::ComPtr;

				CD3dDevice::CD3dDevice() = default;
				CD3dDevice::CD3dDevice(const CD3dDevice& other) = default;
				CD3dDevice& CD3dDevice::operator=(const CD3dDevice& other) = default;
				CD3dDevice::CD3dDevice(CD3dDevice&& other) = default;
				CD3dDevice& CD3dDevice::operator=(CD3dDevice&& other) = default;
				CD3dDevice::~CD3dDevice() = default;

				void CD3dDevice::Initialize(const ComPtr<IDXGIAdapter>& adapter)
				{
					try
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


						if (feature_level < D3D_FEATURE_LEVEL_11_0)
						{
							Log.WarningT("D3D11CreateDevice returns an instance without DirectX 11 support, level %1. Following initialization may fail", feature_level);
						}

						error = m_d3d_device.As(&m_dxgi_device);
						if (error.Error() != S_OK || !m_dxgi_device)
							throw CommonLib::CComExc("ID3D11Device is not an implementation of IDXGIDevice, this usually "
								"means the system does not support DirectX 11. Error", error);
					}
					catch (std::exception& excSrc)
					{
						CommonLib::CExcBase::RegenExc("Failed to initialize D3dDevice", excSrc);
						throw;
					}
				}

				// static
				std::vector<CD3dDevice> CD3dDevice::EnumDevices()
				{
					ComPtr<IDXGIFactory1> factory;

					_com_error error = CreateDXGIFactory1(__uuidof(IDXGIFactory1),
						reinterpret_cast<void**>(factory.GetAddressOf()));

					if (error.Error() != S_OK || !factory)
						return std::vector<CD3dDevice>();

					std::vector<CD3dDevice> result;
					for (int i = 0;; ++i)
					{
						ComPtr<IDXGIAdapter> adapter;

						error = factory->EnumAdapters(i, adapter.GetAddressOf());
						if (error.Error() == S_OK)
						{
							CD3dDevice device;

							device.Initialize(adapter);
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