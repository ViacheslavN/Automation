#include "pch.h"
#include "../../geometry.h"
#include "DxgiFrame.h"
#include "D3dDevice.h"

#include "DxgiDuplicatorController.h"
#include "DxgiAdapterDuplicator.h"
#include "DxgiOutputDuplicator.h"

namespace mrCommonLib
{
	namespace desktop {

		namespace win {

			using Microsoft::WRL::ComPtr;

			namespace {

				bool IsValidRect(const RECT& rect)
				{
					return rect.right > rect.left && rect.bottom > rect.top;
				}

			}  // namespace

			CDxgiAdapterDuplicator::CDxgiAdapterDuplicator(const CD3dDevice& device)
				: m_device(device)
			{
				// Nothing
			}

			CDxgiAdapterDuplicator::CDxgiAdapterDuplicator(CDxgiAdapterDuplicator&&) = default;
			CDxgiAdapterDuplicator::~CDxgiAdapterDuplicator() = default;

			bool CDxgiAdapterDuplicator::Initialize()
			{
				if (DoInitialize())
					return true;

				m_duplicators.clear();
				return false;
			}

			bool CDxgiAdapterDuplicator::DoInitialize()
			{
				for (int i = 0;; ++i)
				{
					ComPtr<IDXGIOutput> output;

					_com_error error = m_device.DxgiAdapter()->EnumOutputs(i, output.GetAddressOf());
				
					if (error.Error() == DXGI_ERROR_NOT_FOUND)
						break;

					if (error.Error() == DXGI_ERROR_NOT_CURRENTLY_AVAILABLE)
					{
						Log.Warning("IDXGIAdapter::EnumOutputs returns NOT_CURRENTLY_AVAILABLE. This may happen when running in session 0");
						break;
					}

					if (error.Error() != S_OK || !output)
					{
						CommonLib::CComExc comExc(error);
						Log.Warning("IDXGIAdapter::EnumOutputs returns an unexpected result %1", comExc.what());
					//	LOG(LS_WARNING) << "IDXGIAdapter::EnumOutputs returns an unexpected result "
					//		<< error.ErrorMessage() << " with error code"
					//		<< error.Error();
						continue;
					}

					DXGI_OUTPUT_DESC desc;
					error = output->GetDesc(&desc);
					if (error.Error() == S_OK)
					{
						if (desc.AttachedToDesktop && IsValidRect(desc.DesktopCoordinates))
						{
							ComPtr<IDXGIOutput1> output1;
							error = output.As(&output1);

							if (error.Error() != S_OK || !output1)
							{
								Log.Warning("Failed to convert IDXGIOutput to IDXGIOutput1, this usually means the system does not support DirectX 11");
								continue;
							}

							CDxgiOutputDuplicator duplicator(m_device, output1, desc);
							if (!duplicator.Initialize())
							{
								Log.WarningT("Failed to initialize DxgiOutputDuplicator on output %1", i);
								continue;
							}

							m_duplicators.push_back(std::move(duplicator));

							m_desktop_rect.UnionWith(m_duplicators.back().DesktopRect());
						}
						else
						{
							Log.ErrorT("%1 is ignored", desc.AttachedToDesktop ? "Attached" : "Detached");
						}
					}
					else
					{
						Log.WarningT("Failed to get output description of device %1, ignore", i);
					}
				}

				if (m_duplicators.empty())
				{
					Log.Warning("Cannot initialize any DxgiOutputDuplicator instance");
				}

				return !m_duplicators.empty();
			}

			void CDxgiAdapterDuplicator::Setup(Context* context)
			{
				context->contexts.resize(m_duplicators.size());

				for (size_t i = 0; i < m_duplicators.size(); ++i)
					m_duplicators[i].Setup(&context->contexts[i]);
			}

			void CDxgiAdapterDuplicator::Unregister(const Context* const context)
			{
				for (size_t i = 0; i < m_duplicators.size(); ++i)
					m_duplicators[i].Unregister(&context->contexts[i]);
			}

			bool CDxgiAdapterDuplicator::Duplicate(Context* context, IFramePtr target)
			{

				for (size_t i = 0; i < m_duplicators.size(); ++i)
				{
					if (!m_duplicators[i].Duplicate(&context->contexts[i],
						m_duplicators[i].DesktopRect().TopLeft(),
						target))
					{
						return false;
					}
				}

				return true;
			}

			bool CDxgiAdapterDuplicator::DuplicateMonitor(Context* context, int monitor_id, IFramePtr target)
			{
				return m_duplicators[monitor_id].Duplicate(&context->contexts[monitor_id], CPoint(), target);
			}

			CRect CDxgiAdapterDuplicator::ScreenRect(int id) const
			{
				return m_duplicators[id].DesktopRect();
			}

			const wstr& CDxgiAdapterDuplicator::DeviceName(int id) const
			{
				return m_duplicators[id].DeviceName();
			}

			int CDxgiAdapterDuplicator::ScreenCount() const
			{
				return static_cast<int>(m_duplicators.size());
			}

			int64_t CDxgiAdapterDuplicator::NumFramesCaptured() const
			{
				int64_t min = std::numeric_limits<int64_t>::max();

				for (const auto& duplicator : m_duplicators)
					min = std::min(min, duplicator.NumFramesCaptured());

				return min;
			}

			void CDxgiAdapterDuplicator::TranslateRect(const CPoint& position)
			{
				m_desktop_rect.Translate(position);

				for (auto& duplicator : m_duplicators)
					duplicator.TranslateRect(position);
			}
		}
	}
}