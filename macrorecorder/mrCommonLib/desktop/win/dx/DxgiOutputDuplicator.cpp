#include "pch.h"
#include "DxgiTextureMapping.h"
#include "DxgiTextureStaging.h"
#include "DxgiOutputDuplicator.h"

namespace mrCommonLib
{
	namespace desktop {

		namespace win {

			using Microsoft::WRL::ComPtr;

			namespace {

				// Timeout for AcquireNextFrame() call.
				// DxgiDuplicatorController leverages external components to do the capture scheduling. So here
				// DxgiOutputDuplicator does not need to actively wait for a new frame.
				const int kAcquireTimeoutMs = 0;

				CRect RECTToDesktopRect(const RECT& rect)
				{
					return CRect::MakeLTRB(rect.left, rect.top, rect.right, rect.bottom);
				}

				Rotation dxgiRotationToRotation(DXGI_MODE_ROTATION rotation)
				{
					switch (rotation)
					{
					case DXGI_MODE_ROTATION_IDENTITY:
					case DXGI_MODE_ROTATION_UNSPECIFIED:
						return Rotation::CLOCK_WISE_0;

					case DXGI_MODE_ROTATION_ROTATE90:
						return Rotation::CLOCK_WISE_90;

					case DXGI_MODE_ROTATION_ROTATE180:
						return Rotation::CLOCK_WISE_180;

					case DXGI_MODE_ROTATION_ROTATE270:
						return Rotation::CLOCK_WISE_270;
					}

					return Rotation::CLOCK_WISE_0;
				}

			}  // namespace

			CDxgiOutputDuplicator::CDxgiOutputDuplicator(const CD3dDevice& device,
				const ComPtr<IDXGIOutput1>& output,
				const DXGI_OUTPUT_DESC& desc)
				: m_device(device),
				m_output(output),
				m_deviceName(desc.DeviceName),
				m_desktopRect(RECTToDesktopRect(desc.DesktopCoordinates))
			{


				memset(&m_desc, 0, sizeof(m_desc));
			}

			CDxgiOutputDuplicator::CDxgiOutputDuplicator(CDxgiOutputDuplicator&& other) = default;

			CDxgiOutputDuplicator::~CDxgiOutputDuplicator()
			{
				if (m_Duplication)
					m_Duplication->ReleaseFrame();

				m_texture.reset();
			}

			bool CDxgiOutputDuplicator::Initialize()
			{
				if (DuplicateOutput())
				{
					if (m_desc.DesktopImageInSystemMemory)
						m_texture.reset(new CDxgiTextureMapping(m_Duplication.Get()));
					else
						m_texture.reset(new CDxgiTextureStaging(m_device));

					return true;
				}
				else
				{
					m_Duplication.Reset();
					return false;
				}
			}

			bool CDxgiOutputDuplicator::DuplicateOutput()
			{

				static const int kRetryCount = 10;
				static const DWORD kRetryTimeoutMs = 100;

				for (int i = 0; i < kRetryCount; ++i)
				{
					_com_error error =
						m_output->DuplicateOutput(static_cast<IUnknown*>(m_device.D3dDevice()),
							m_Duplication.GetAddressOf());
					if (error.Error() != S_OK || !m_Duplication)
					{
						// DuplicateOutput may temporarily return E_ACCESSDENIED.
						if (error.Error() == E_ACCESSDENIED)
						{
							if (i + 1 < kRetryCount)
							{
								Sleep(kRetryTimeoutMs);
								continue;
							}
						}

						Log.Warning("Failed to duplicate output from IDXGIOutput1, error %1", CommonLib::CComExc(error).what());
						return false;
					}
					else
					{
						break;
					}
				}

				memset(&m_desc, 0, sizeof(m_desc));
				m_Duplication->GetDesc(&m_desc);

				if (m_desc.ModeDesc.Format != DXGI_FORMAT_B8G8R8A8_UNORM)
				{
					Log.ErrorT("IDXGIDuplicateOutput does not use RGBA (8 bit) format, which is required by downstream components, format is %1", m_desc.ModeDesc.Format);
					return false;
				}

				if (static_cast<int>(m_desc.ModeDesc.Width) != m_desktopRect.Width() ||
					static_cast<int>(m_desc.ModeDesc.Height) != m_desktopRect.Height())
				{
					Log.ErrorT("IDXGIDuplicateOutput does not return a same size as its IDXGIOutput1, size returned by IDXGIDuplicateOutput is %1  x  %2, size returned by IDXGIOutput1 is W %3, H %4",
						 m_desc.ModeDesc.Width, m_desc.ModeDesc.Height, m_desktopRect.Width(),	m_desktopRect.Height());
					return false;
				}

				m_rotation = dxgiRotationToRotation(m_desc.Rotation);
				m_unrotatedSize = rotateSize(DesktopSize(), reverseRotation(m_rotation));

				return true;
			}

			bool CDxgiOutputDuplicator::ReleaseFrame()
			{

				_com_error error = m_Duplication->ReleaseFrame();
				if (error.Error() != S_OK)
				{
					Log.Error("Failed to release frame from IDXGIOutputDuplication, error %1", CommonLib::CComExc(error).what());
					return false;
				}

				return true;
			}

			bool CDxgiOutputDuplicator::Duplicate(Context* context, const CPoint& offset, IFramePtr target)
			{

				if (!CRect::MakeSize(target->Size()).ContainsRect(TranslatedDesktopRect(offset)))
				{
					// target size is not large enough to cover current output region.
					return false;
				}

				DXGI_OUTDUPL_FRAME_INFO frame_info;
				memset(&frame_info, 0, sizeof(frame_info));

				ComPtr<IDXGIResource> resource;

				_com_error error = m_Duplication->AcquireNextFrame(kAcquireTimeoutMs,
					&frame_info,
					resource.GetAddressOf());
				if (error.Error() != S_OK && error.Error() != DXGI_ERROR_WAIT_TIMEOUT)
				{
					Log.Error("Failed to capture frame %1", CommonLib::CComExc(error).what());
					return false;
				}

				// We need to merge updated region with the one from context, but only spread
				// updated region from current frame. So keeps a copy of updated region from
				// context here. The |updated_region| always starts from (0, 0).
				CRegion updated_region;
				updated_region.Swap(&context->updated_region);

				if (error.Error() == S_OK && frame_info.AccumulatedFrames > 0 && resource)
				{
					DetectUpdatedRegion(frame_info, &context->updated_region);
					SpreadContextChange(context);

					if (!m_texture->CopyFrom(frame_info, resource.Get()))
						return false;

					updated_region.AddRegion(context->updated_region);

					// TODO(zijiehe): Figure out why clearing context->updated_region() here
					// triggers screen flickering?

					const IFramePtr& source = m_texture->AsDesktopFrame();

					if (m_rotation != Rotation::CLOCK_WISE_0)
					{
						for (CRegion::Iterator it(updated_region); !it.IsAtEnd(); it.Advance())
						{
							// The |updated_region| returned by Windows is rotated, but the |source|
							// frame is not. So we need to rotate it reversely.
							const CRect source_rect =
								rotateRect(it.Rect(), DesktopSize(), reverseRotation(m_rotation));
							rotateDesktopFrame(source, source_rect, m_rotation, offset, target);
						}
					}
					else
					{
						for (CRegion::Iterator it(updated_region); !it.IsAtEnd(); it.Advance())
						{
							// The Rect in |target|, starts from offset.
							CRect dest_rect = it.Rect();
							dest_rect.Translate(offset);
							target->CopyPixelsFrom(source, it.Rect().TopLeft(), dest_rect);
						}
					}

					m_lastFrame = target;//target->share();
					m_lastFrameOffset = offset;

					updated_region.Translate(offset.X(), offset.Y());
					target->UpdatedRegion().AddRegion(updated_region);
					++m_numFramesCaptured;

					return m_texture->Release() && ReleaseFrame();
				}

				if (m_lastFrame)
				{
					// No change since last frame or AcquireNextFrame() timed out, we will export last frame to
					// the target.
					for (CRegion::Iterator it(updated_region); !it.IsAtEnd(); it.Advance())
					{
						// The Rect in |source|, starts from last_frame_offset_.
						CRect source_rect = it.Rect();

						// The Rect in |target|, starts from offset.
						CRect target_rect = source_rect;

						source_rect.Translate(m_lastFrameOffset);
						target_rect.Translate(offset);

						target->CopyPixelsFrom(m_lastFrame, source_rect.TopLeft(), target_rect);
					}

					updated_region.Translate(offset.X(), offset.Y());
					target->UpdatedRegion().AddRegion(updated_region);
				}
				else
				{
					// If we were at the very first frame, and capturing failed, the
					// context->updated_region should be kept unchanged for next attempt.
					context->updated_region.Swap(&updated_region);
				}

				// If AcquireNextFrame() failed with timeout error, we do not need to release the frame.
				return error.Error() == DXGI_ERROR_WAIT_TIMEOUT || ReleaseFrame();
			}

			CRect CDxgiOutputDuplicator::TranslatedDesktopRect(const CPoint& offset) const
			{
				CRect result(CRect::MakeSize(DesktopSize()));
				result.Translate(offset);
				return result;
			}

			CRect CDxgiOutputDuplicator::UntranslatedDesktopRect() const
			{
				return CRect::MakeSize(DesktopSize());
			}

			void CDxgiOutputDuplicator::DetectUpdatedRegion(const DXGI_OUTDUPL_FRAME_INFO& frame_info,CRegion* updated_region)
			{
				if (DoDetectUpdatedRegion(frame_info, updated_region))
				{
					// Make sure even a region returned by Windows API is out of the scope of
					// desktop_rect_, we still won't export it to the target DesktopFrame.
					updated_region->IntersectWith(UntranslatedDesktopRect());
				}
				else
				{
					updated_region->SetRect(UntranslatedDesktopRect());
				}
			}

			bool CDxgiOutputDuplicator::DoDetectUpdatedRegion(const DXGI_OUTDUPL_FRAME_INFO& frame_info,
				CRegion* updated_region)
			{
				//DCHECK(updated_region);
				updated_region->Clear();

				if (frame_info.TotalMetadataBufferSize == 0)
				{
					// This should not happen, since frame_info.AccumulatedFrames > 0.
					Log.Error("frame_info.AccumulatedFrames > 0, but TotalMetadataBufferSize == 0");
					return false;
				}

				if (m_metadata.capacity() < frame_info.TotalMetadataBufferSize)
				{
					m_metadata.clear(); // Avoid data copy
					m_metadata.reserve(frame_info.TotalMetadataBufferSize);
				}

				UINT buff_size = 0;
				DXGI_OUTDUPL_MOVE_RECT* move_rects = reinterpret_cast<DXGI_OUTDUPL_MOVE_RECT*>(m_metadata.data());
				size_t move_rects_count = 0;

				_com_error error = m_Duplication->GetFrameMoveRects(
					static_cast<UINT>(m_metadata.capacity()), move_rects, &buff_size);
				if (error.Error() != S_OK)
				{
					Log.Error("Failed to get move rectangles %1 ", CommonLib::CComExc(error).what());			
					return false;
				}

				move_rects_count = buff_size / sizeof(DXGI_OUTDUPL_MOVE_RECT);

				RECT* dirty_rects = reinterpret_cast<RECT*>(m_metadata.data() + buff_size);
				size_t dirty_rects_count = 0;

				error = m_Duplication->GetFrameDirtyRects(
					static_cast<UINT>(m_metadata.capacity()) - buff_size, dirty_rects, &buff_size);
				if (error.Error() != S_OK)
				{
					Log.Error("Failed to get move rectangles %1 ", CommonLib::CComExc(error).what());
					return false;
				}

				dirty_rects_count = buff_size / sizeof(RECT);

				while (move_rects_count > 0)
				{
					// DirectX capturer API may randomly return unmoved move_rects, which should
					// be skipped to avoid unnecessary wasting of differing and encoding
					// resources.
					// By using testing application it2me_standalone_host_main, this check
					// reduces average capture time by 0.375% (4.07 -> 4.055), and average
					// encode time by 0.313% (8.042 -> 8.016) without other impacts.
					if (move_rects->SourcePoint.x != move_rects->DestinationRect.left ||
						move_rects->SourcePoint.y != move_rects->DestinationRect.top)
					{
						updated_region->AddRect(
							rotateRect(CRect::MakeXYWH(move_rects->SourcePoint.x,
								move_rects->SourcePoint.y,
								move_rects->DestinationRect.right -
								move_rects->DestinationRect.left,
								move_rects->DestinationRect.bottom -
								move_rects->DestinationRect.top),
								m_unrotatedSize, m_rotation));

						updated_region->AddRect(
							rotateRect(CRect::MakeLTRB(move_rects->DestinationRect.left,
								move_rects->DestinationRect.top,
								move_rects->DestinationRect.right,
								move_rects->DestinationRect.bottom),
								m_unrotatedSize, m_rotation));
					}
					else
					{
						Log.InfoT("Unmoved move_rect detected, [%1,%2-%3,%4]",move_rects->DestinationRect.left,
							 move_rects->DestinationRect.top,
							 move_rects->DestinationRect.right,
							 move_rects->DestinationRect.bottom);
					}

					++move_rects;
					--move_rects_count;
				}

				while (dirty_rects_count > 0)
				{
					updated_region->AddRect(rotateRect(
						CRect::MakeLTRB(dirty_rects->left, dirty_rects->top,
							dirty_rects->right, dirty_rects->bottom),
						m_unrotatedSize, m_rotation));

					++dirty_rects;
					--dirty_rects_count;
				}

				return true;
			}

			void CDxgiOutputDuplicator::Setup(Context* context)
			{
				// Always copy entire monitor during the first duplicate() function call.
				context->updated_region.AddRect(UntranslatedDesktopRect());
				m_Contexts.push_back(context);
			}

			void CDxgiOutputDuplicator::Unregister(const Context* const context)
			{
				auto it = std::find(m_Contexts.begin(), m_Contexts.end(), context);
				m_Contexts.erase(it);
			}

			void CDxgiOutputDuplicator::SpreadContextChange(const Context* const source)
			{
				for (Context* dest : m_Contexts)
				{

					if (dest != source)
						dest->updated_region.AddRegion(source->updated_region);
				}
			}

			CSize CDxgiOutputDuplicator::DesktopSize() const
			{
				return m_desktopRect.Size();
			}

			int64_t CDxgiOutputDuplicator::NumFramesCaptured() const
			{
				return m_numFramesCaptured;
			}

			void CDxgiOutputDuplicator::TranslateRect(const CPoint& position)
			{
				m_desktopRect.Translate(position);
			}
		}
	}
}