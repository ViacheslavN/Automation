#pragma once
namespace mrCommonLib
{
	namespace desktop
	{
		namespace win
		{
			class CDesktopEnvironment
			{
			public:
				CDesktopEnvironment();
				~CDesktopEnvironment();

				void setWallpaper(bool enable);
				void setFontSmoothing(bool enable);
				void setEffects(bool enable);

			private:
				void disableWallpaper();
				void disableFontSmoothing();
				void disableEffects();

				void revertAll();
				void applyNewSettings();

				bool wallpaper_ = false;
				bool font_smoothing_ = false;
				bool effects_ = false;

				bool drop_shadow_changed_ = false;
				bool animation_changed_ = false;
			};

		}
	}
}