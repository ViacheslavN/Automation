#pragma once
#include <D3D11.h>
#include <DXGI.h>
#include <comdef.h>
#include <wrl/client.h>

namespace mrCommonLib
{
	namespace desktop {

		namespace win {
			// A wrapper of ID3D11Device and its corresponding context and IDXGIAdapter.
			// This class represents one video card in the system.
			class D3dDevice
			{
			public:
				D3dDevice(const D3dDevice& other);
				D3dDevice& operator=(const D3dDevice& other);
				D3dDevice(D3dDevice&& other);
				D3dDevice& operator=(D3dDevice&& other);
				~D3dDevice();

				ID3D11Device* d3dDevice() const { return m_d3d_device.Get(); }
				ID3D11DeviceContext* context() const { return m_context.Get(); }
				IDXGIDevice* dxgiDevice() const { return m_dxgi_device.Get(); }
				IDXGIAdapter* dxgiAdapter() const { return m_dxgi_adapter.Get(); }

				// Returns all D3dDevice instances on the system. Returns an empty vector if anything wrong.
				static std::vector<D3dDevice> enumDevices();

			private:
				// Instances of D3dDevice should only be created by EnumDevices() static function.
				D3dDevice();

				// Initializes the D3dDevice from an IDXGIAdapter.
				void initialize(const Microsoft::WRL::ComPtr<IDXGIAdapter>& adapter);

				Microsoft::WRL::ComPtr<ID3D11Device> m_d3d_device;
				Microsoft::WRL::ComPtr<ID3D11DeviceContext> m_context;
				Microsoft::WRL::ComPtr<IDXGIDevice> m_dxgi_device;
				Microsoft::WRL::ComPtr<IDXGIAdapter> m_dxgi_adapter;
			};
		}
	}
} // namespace desktop