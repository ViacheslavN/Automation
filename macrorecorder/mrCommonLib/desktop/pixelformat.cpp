#include "pch.h"
#include "pixelformat.h"

namespace mrCommonLib
{
	namespace desktop
	{
		CPixelFormat::CPixelFormat(const CPixelFormat& other)
		{
			Set(other);
		}

		CPixelFormat::CPixelFormat(uint8_t bits_per_pixel,
			uint16_t red_max,
			uint16_t green_max,
			uint16_t blue_max,
			uint8_t red_shift,
			uint8_t green_shift,
			uint8_t blue_shift)
			: m_bits_per_pixel(bits_per_pixel),
			m_bytes_per_pixel(bits_per_pixel / 8),
			m_red_max(red_max),
			m_green_max(green_max),
			m_blue_max(blue_max),
			m_red_shift(red_shift),
			m_green_shift(green_shift),
			m_blue_shift(blue_shift)
		{
			// Nothing
		}

		// static
		CPixelFormat CPixelFormat::ARGB()
		{
			return CPixelFormat(32,   // bits per pixel
				255,  // red max
				255,  // green max
				255,  // blue max
				16,   // red shift
				8,    // green shift
				0);   // blue shift
		}

		// static
		CPixelFormat CPixelFormat::RGB565()
		{
			return CPixelFormat(16,  // bits per pixel
				31,  // red max
				63,  // green max
				31,  // blue max
				11,  // red shift
				5,   // green shift
				0);  // blue shift
		}

		// static
		CPixelFormat CPixelFormat::RGB332()
		{
			return CPixelFormat(8,  // bits per pixel
				7,  // red max
				7,  // green max
				3,  // blue max
				5,  // red shift
				2,  // green shift
				0); // blue shift
		}

		// static
		CPixelFormat CPixelFormat::RGB222()
		{
			return CPixelFormat(8,  // bits per pixel
				3,  // red max
				3,  // green max
				3,  // blue max
				4,  // red shift
				2,  // green shift
				0); // blue shift
		}

		// static
		CPixelFormat CPixelFormat::RGB111()
		{
			return CPixelFormat(8,  // bits per pixel
				1,  // red max
				1,  // green max
				1,  // blue max
				2,  // red shift
				1,  // green shift
				0); // blue shift
		}

		bool CPixelFormat::IsValid() const
		{
			if (m_bits_per_pixel == 0 &&
				m_red_max == 0 &&
				m_green_max == 0 &&
				m_blue_max == 0 &&
				m_red_shift == 0 &&
				m_green_shift == 0 &&
				m_blue_shift == 0)
			{
				return false;
			}

			return true;
		}

		void CPixelFormat::Clear()
		{
			m_bits_per_pixel = 0;
			m_bytes_per_pixel = 0;

			m_red_max = 0;
			m_green_max = 0;
			m_blue_max = 0;

			m_red_shift = 0;
			m_green_shift = 0;
			m_blue_shift = 0;
		}

		bool CPixelFormat::IsEqual(const CPixelFormat& other) const
		{

			if (m_bits_per_pixel == other.m_bits_per_pixel &&
				m_red_max == other.m_red_max        &&
				m_green_max == other.m_green_max      &&
				m_blue_max == other.m_blue_max       &&
				m_red_shift == other.m_red_shift      &&
				m_green_shift == other.m_green_shift    &&
				m_blue_shift == other.m_blue_shift)
			{
				return true;
			}

			return false;
		}

		void CPixelFormat::Set(const CPixelFormat& other)
		{
			m_bits_per_pixel = other.m_bits_per_pixel;
			m_bytes_per_pixel = other.m_bytes_per_pixel;

			m_red_max = other.m_red_max;
			m_green_max = other.m_green_max;
			m_blue_max = other.m_blue_max;

			m_red_shift = other.m_red_shift;
			m_green_shift = other.m_green_shift;
			m_blue_shift = other.m_blue_shift;
		}

		CPixelFormat& CPixelFormat::operator=(const CPixelFormat& other)
		{
			Set(other);
			return *this;
		}

		bool CPixelFormat::operator==(const CPixelFormat& other) const
		{
			return IsEqual(other);
		}

		bool CPixelFormat::operator!=(const CPixelFormat& other) const
		{
			return !IsEqual(other);
		}
	}
}