#pragma once


namespace mrCommonLib
{
	namespace desktop {

		namespace win {
			// A wrapper of ID3D11Device and its corresponding context and IDXGIAdapter.
			// This class represents one video card in the system.
			class CD3dDevice
			{
			public:
				CD3dDevice(const CD3dDevice& other);
				CD3dDevice& operator=(const CD3dDevice& other);
				CD3dDevice(CD3dDevice&& other);
				CD3dDevice& operator=(CD3dDevice&& other);
				~CD3dDevice();

				ID3D11Device* D3dDevice() const { return m_d3d_device.Get(); }
				ID3D11DeviceContext* Context() const { return m_context.Get(); }
				IDXGIDevice* DxgiDevice() const { return m_dxgi_device.Get(); }
				IDXGIAdapter* DxgiAdapter() const { return m_dxgi_adapter.Get(); }

				// Returns all D3dDevice instances on the system. Returns an empty vector if anything wrong.
				static std::vector<CD3dDevice> EnumDevices();

			private:
				// Instances of D3dDevice should only be created by EnumDevices() static function.
				CD3dDevice();

				// Initializes the D3dDevice from an IDXGIAdapter.
				void Initialize(const Microsoft::WRL::ComPtr<IDXGIAdapter>& adapter);

				Microsoft::WRL::ComPtr<ID3D11Device> m_d3d_device;
				Microsoft::WRL::ComPtr<ID3D11DeviceContext> m_context;
				Microsoft::WRL::ComPtr<IDXGIDevice> m_dxgi_device;
				Microsoft::WRL::ComPtr<IDXGIAdapter> m_dxgi_adapter;
			};
		}
	}
} // namespace desktop