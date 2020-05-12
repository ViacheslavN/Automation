#include "pch.h"
#include "ScreenUtils.h"
#include "HDC.h"
#include "Hbitmap.h"
#include "BitmapInfo.h"
#include "GdiObject.h"

namespace mrCommonLib
{
	namespace desktop
	{
		namespace win
		{
			int CreateShift(uint32_t bits)
			{
				int shift = 0;

				while ((shift < 32) && !(bits & 1))
				{
					bits >>= 1;
					++shift;
				}

				return shift;
			}

			void CScreenUtils::GetScreenList(ScreenList& screens)
			{

				for (int device_index = 0;; ++device_index)
				{
					DISPLAY_DEVICEW device;
					device.cb = sizeof(device);

					// |enum_result| is 0 if we have enumerated all devices.
					if (!EnumDisplayDevicesW(nullptr, device_index, &device, 0))
						break;

					// We only care about active displays.
					if (!(device.StateFlags & DISPLAY_DEVICE_ACTIVE) ||
						(device.StateFlags & DISPLAY_DEVICE_MIRRORING_DRIVER))
					{
						continue;
					}

					screens.push_back({ device_index, CommonLib::StringEncoding::str_w2utf8_safe(device.DeviceName) });
				}

			}

			bool CScreenUtils::IsScreenValid(ScreenId screen, wstr& device_key)
			{
				if (screen == kFullDesktopScreenId)
				{
					device_key.clear();
					return true;
				}

				DISPLAY_DEVICEW device;
				device.cb = sizeof(device);

				if (!EnumDisplayDevicesW(nullptr, screen, &device, 0))
					return false;

				device_key = device.DeviceKey;
				return true;
			}

			// static
			CRect CScreenUtils::FullScreenRect()
			{
				return CRect::MakeXYWH(GetSystemMetrics(SM_XVIRTUALSCREEN),
					GetSystemMetrics(SM_YVIRTUALSCREEN),
					GetSystemMetrics(SM_CXVIRTUALSCREEN),
					GetSystemMetrics(SM_CYVIRTUALSCREEN));
			}

			// static
			CRect CScreenUtils::ScreenRect(ScreenId screen,
				const wstr& device_key)
			{
				if (screen == kFullDesktopScreenId)
					return FullScreenRect();

				DISPLAY_DEVICEW device;
				device.cb = sizeof(device);
				if (!EnumDisplayDevicesW(nullptr, screen, &device, 0))
					return CRect();

				// Verifies the device index still maps to the same display device, to make sure we are
				// capturing the same device when devices are added or removed. DeviceKey is documented as
				// reserved, but it actually contains the registry key for the device and is unique for each
				// monitor, while DeviceID is not.
				if (device.DeviceKey != device_key)
					return CRect();

				DEVMODEW device_mode;
				device_mode.dmSize = sizeof(device_mode);
				device_mode.dmDriverExtra = 0;

				if (!EnumDisplaySettingsExW(device.DeviceName, ENUM_CURRENT_SETTINGS, &device_mode, 0))
					return CRect();

				return CRect::MakeXYWH(device_mode.dmPosition.x,
					device_mode.dmPosition.y,
					device_mode.dmPelsWidth,
					device_mode.dmPelsHeight);
			}

			// static
			int CScreenUtils::ScreenCount()
			{
				return GetSystemMetrics(SM_CMONITORS);
			}

			// static
			CPixelFormat CScreenUtils::DetectPixelFormat()
			{
				CWinHDC desktop_dc(nullptr);
				if (!desktop_dc)
					return CPixelFormat::ARGB();

				HBITMAPObj bitmap(reinterpret_cast<HBITMAP>(
					GetCurrentObject(desktop_dc, OBJ_BITMAP)));

				if (!bitmap)
					return CPixelFormat::ARGB();

				CBitmapInfo bitmap_info;
				memset(&bitmap_info, 0, sizeof(bitmap_info));

				bitmap_info.header.biSize = sizeof(bitmap_info.header);

				if (!GetDIBits(desktop_dc, bitmap, 0, 0, nullptr,
					reinterpret_cast<BITMAPINFO*>(&bitmap_info), DIB_RGB_COLORS))
				{
					return CPixelFormat::ARGB();
				}

				if (bitmap_info.header.biCompression != BI_BITFIELDS)
					return CPixelFormat::ARGB();

				// Now bitmap_info.header.biCompression equals BI_BITFIELDS and we can get a table of colors.
				if (!GetDIBits(desktop_dc, bitmap, 0, 0, nullptr,
					reinterpret_cast<BITMAPINFO*>(&bitmap_info), DIB_RGB_COLORS))
				{
					return CPixelFormat::ARGB();
				}

				if (bitmap_info.header.biBitCount != 32 && bitmap_info.header.biBitCount != 16)
					return CPixelFormat::ARGB();

				const uint16_t red_shift = CreateShift(bitmap_info.u.mask.red);
				const uint16_t green_shift = CreateShift(bitmap_info.u.mask.green);
				const uint16_t blue_shift = CreateShift(bitmap_info.u.mask.blue);

				const uint16_t red_max = bitmap_info.u.mask.red >> red_shift;
				const uint16_t green_max = bitmap_info.u.mask.green >> green_shift;
				const uint16_t blue_max = bitmap_info.u.mask.blue >> blue_shift;

				return CPixelFormat(bitmap_info.header.biBitCount,
					red_max, green_max, blue_max,
					red_shift, green_shift, blue_shift);
			}
		}
	}
}