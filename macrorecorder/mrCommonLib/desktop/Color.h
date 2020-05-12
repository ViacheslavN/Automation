#pragma once


namespace mrCommonLib
{
	namespace desktop
	{
		class Color
		{
		public:
			typedef unsigned char ColorComponent;
			typedef unsigned char AlfaChannel;
			typedef uint32_t ColorType;
			enum
			{
				Black = 0x000000,
				Red = 0xFF0000,
				Green = 0x0000FF,
				White = 0xffffff,
				Transparent = 0x00,
				SemiTransparent = 0x80,
				Opaque = 0xff
			};
			
			Color(ColorType rgb = Black, AlfaChannel a = Opaque);
			Color(ColorComponent r, ColorComponent g, ColorComponent b, AlfaChannel a = Opaque);

			ColorType GetRGB() const;
			ColorType GetRGBA() const;
			AlfaChannel GetA() const;
			ColorComponent GetR() const;
			ColorComponent GetG() const;
			ColorComponent GetB() const;
			void SetRGBA(ColorType rgba);

			static Color RndLineColor();
			static Color RndFillColor();

			bool operator==(const Color& a) const;
			bool operator!=(const Color& a) const;

			Color& operator=(const Color& a);
			Color& operator=(ColorType rgba);

		private:
			static Color RndColor(Color::ColorComponent *rgb);
			ColorType m_rgba;
		};
	}
}