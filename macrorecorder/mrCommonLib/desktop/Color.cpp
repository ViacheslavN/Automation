
#include "pch.h"
#include "Color.h"

namespace mrCommonLib
{
	namespace desktop
	{
		Color::Color(ColorType rgb, AlfaChannel a) :
			m_rgba((rgb & 0xffffff) | (static_cast<ColorType>(a) << 24))
		{
		}
		Color::Color(ColorComponent r, ColorComponent g, ColorComponent b, AlfaChannel a) :
			m_rgba(static_cast<ColorType>(r)
				| (static_cast<ColorType>(g) << 8)
				| (static_cast<ColorType>(b) << 16)
				| (static_cast<ColorType>(a) << 24))
		{
		}

		Color::ColorType Color::GetRGB() const
		{
			return m_rgba & 0x00ffffff;
		}
		Color::ColorType Color::GetRGBA() const
		{
			return m_rgba;
		}
		Color::ColorComponent Color::GetR() const
		{
			return static_cast<Color::ColorComponent>(m_rgba);
		}
		Color::ColorComponent Color::GetG() const
		{
			return static_cast<Color::ColorComponent>(m_rgba >> 8);
		}
		Color::ColorComponent Color::GetB() const
		{
			return static_cast<Color::ColorComponent>(m_rgba >> 16);
		}
		Color::ColorComponent Color::GetA() const
		{
			return static_cast<Color::ColorComponent>(m_rgba >> 24);
		}

		Color Color::RndLineColor()
		{
			Color::ColorComponent rgb[3];
			rgb[0] = 128;
			rgb[1] = rgb[0] - Color::ColorComponent(rand() % 128);
			rgb[2] = 128 - rgb[1];
			return RndColor(rgb);
		}
		Color Color::RndFillColor()
		{
			Color::ColorComponent rgb[3];
			rgb[0] = 255;
			rgb[1] = rgb[0] - Color::ColorComponent(rand() % 80);
			rgb[2] = 255 - (rgb[1] - 175);
			return RndColor(rgb);
		}

		Color Color::RndColor(Color::ColorComponent *rgb)
		{
			int idx[3];
			idx[0] = rand() % 3;
			while ((idx[1] = rand() % 3) == idx[0]);
			idx[2] = 3 - (idx[0] + idx[1]);

			Color::ColorComponent r = rgb[idx[0]];
			Color::ColorComponent g = rgb[idx[1]];
			Color::ColorComponent b = rgb[idx[2]];
			return Color(r, g, b);
		}

		bool Color::operator==(const Color& c) const
		{
			return m_rgba == c.m_rgba;
		}
		bool Color::operator!=(const Color& c) const
		{
			return !(this->operator ==(c));
		}
		Color& Color::operator=(const Color& a)
		{
			m_rgba = a.m_rgba;
			return *this;
		}
		Color& Color::operator=(ColorType rgba)
		{
			m_rgba = rgba;
			return *this;
		}
		void Color::SetRGBA(ColorType rgba)
		{
			m_rgba = rgba;
		}
	}
}