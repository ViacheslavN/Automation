#include "pch.h"
#include "DesktopEnvironment.h"
#include "HandleObject.h"
#include "Impersonator.h"

namespace mrCommonLib
{
	namespace desktop
	{
		namespace win
		{

			CDesktopEnvironment::CDesktopEnvironment()
			{
				applyNewSettings();
			}

			CDesktopEnvironment::~CDesktopEnvironment()
			{
				revertAll();
			}

			void CDesktopEnvironment::setWallpaper(bool enable)
			{
				if (wallpaper_ == enable)
					return;

				wallpaper_ = enable;

				revertAll();
				applyNewSettings();
			}

			void CDesktopEnvironment::setFontSmoothing(bool enable)
			{
				if (font_smoothing_ == enable)
					return;

				font_smoothing_ = enable;

				revertAll();
				applyNewSettings();
			}

			void CDesktopEnvironment::setEffects(bool enable)
			{
				if (effects_ == enable)
					return;

				effects_ = enable;

				revertAll();
				applyNewSettings();
			}

			void CDesktopEnvironment::disableWallpaper()
			{
				SystemParametersInfoW(SPI_SETDESKWALLPAPER, 0, (void*)L"", SPIF_SENDCHANGE);
			}

			void CDesktopEnvironment::disableFontSmoothing()
			{
				SystemParametersInfoW(SPI_SETFONTSMOOTHING, FALSE, 0, SPIF_SENDCHANGE);
			}

			void CDesktopEnvironment::disableEffects()
			{
				BOOL drop_shadow = TRUE;
				if (SystemParametersInfoW(SPI_GETDROPSHADOW, 0, &drop_shadow, 0))
				{
					if (drop_shadow)
					{
						SystemParametersInfoW(SPI_SETDROPSHADOW, 0, FALSE, SPIF_SENDCHANGE);
						drop_shadow_changed_ = true;
					}
				}

				ANIMATIONINFO animation;
				animation.cbSize = sizeof(animation);
				if (SystemParametersInfoW(SPI_GETANIMATION, sizeof(animation), &animation, 0))
				{
					if (animation.iMinAnimate)
					{
						animation.iMinAnimate = FALSE;
						SystemParametersInfoW(SPI_SETANIMATION, sizeof(animation), &animation, SPIF_SENDCHANGE);
						animation_changed_ = true;
					}
				}

				SystemParametersInfoW(SPI_SETDRAGFULLWINDOWS, FALSE, 0, SPIF_SENDCHANGE);
				SystemParametersInfoW(SPI_SETUIEFFECTS, 0, FALSE, SPIF_SENDCHANGE);
				SystemParametersInfoW(SPI_SETCLIENTAREAANIMATION, 0, FALSE, SPIF_SENDCHANGE);
			}

			void CDesktopEnvironment::revertAll()
			{
				if (drop_shadow_changed_)
				{
					SystemParametersInfoW(SPI_SETDROPSHADOW, 0, reinterpret_cast<PVOID>(TRUE), SPIF_SENDCHANGE);
					drop_shadow_changed_ = false;
				}

				if (animation_changed_)
				{
					ANIMATIONINFO animation;
					animation.cbSize = sizeof(animation);
					animation.iMinAnimate = TRUE;

					SystemParametersInfoW(SPI_SETANIMATION, sizeof(animation), &animation, SPIF_SENDCHANGE);

					animation_changed_ = false;
				}

				THandleObj user_token;

				if (!WTSQueryUserToken(WTSGetActiveConsoleSessionId(), user_token.Recieve()))
					return;
				

				// The process of the desktop session is running with "SYSTEM" account.
				// We need the current real user, not "SYSTEM".
				CImpersonator impersonator(user_token);

				HMODULE module = GetModuleHandleW(L"user32.dll");
				if (module)
				{
					// The function prototype is relevant for versions starting from Windows Vista.
					// Older versions have a different prototype.
					typedef BOOL(WINAPI* UpdatePerUserSystemParametersFunc)(DWORD flags);

					UpdatePerUserSystemParametersFunc update_per_user_system_parameters =
						reinterpret_cast<UpdatePerUserSystemParametersFunc>(
							GetProcAddress(module, "UpdatePerUserSystemParameters"));
					if (update_per_user_system_parameters)
					{
						// WARNING! Undocumented function!
						// Any ideas how to update user settings without using it?
						update_per_user_system_parameters(0x06);
					}
				}
			}

			void CDesktopEnvironment::applyNewSettings()
			{
				if (!wallpaper_)
					disableWallpaper();

				if (!font_smoothing_)
					disableFontSmoothing();

				if (!effects_)
					disableEffects();
			}
		}
	}
}