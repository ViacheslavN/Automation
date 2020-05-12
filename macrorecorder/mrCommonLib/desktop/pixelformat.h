#pragma once

namespace mrCommonLib
{
	namespace desktop
	{

		class CPixelFormat
		{
		public:
			CPixelFormat() = default;
			CPixelFormat(const CPixelFormat& other);
			CPixelFormat(uint8_t bits_per_pixel,
				uint16_t red_max,
				uint16_t green_max,
				uint16_t blue_max,
				uint8_t red_shift,
				uint8_t green_shift,
				uint8_t blue_shift);
			~CPixelFormat() = default;

			// True color (32 bits per pixel)
			// 0:7   - blue
			// 8:14  - green
			// 15:21 - red
			// 22:31 - unused
			static CPixelFormat ARGB();

			// High color (16 bits per pixel)
			// 0:4   - blue
			// 5:9   - green
			// 10:15 - red
			static CPixelFormat RGB565();

			// 256 colors (8 bits per pixel)
			// 0:1 - blue
			// 2:4 - green
			// 5:7 - red
			static CPixelFormat RGB332();

			// 64 colors (6 bits per pixel)
			// 0:1 - blue
			// 2:3 - green
			// 4:5 - red
			// 6:7 - unused
			static CPixelFormat RGB222();

			// 8 colors (3 bits per pixel)
			// 0   - blue
			// 1   - green
			// 2   - red
			// 3:7 - unused
			static CPixelFormat RGB111();

			uint8_t BitsPerPixel() const { return m_bits_per_pixel; }
			uint8_t BytesPerPixel() const { return m_bytes_per_pixel; }

			uint16_t RedMax() const { return m_red_max; }
			uint16_t GreenMax() const { return m_green_max; }
			uint16_t BlueMax() const { return m_blue_max; }

			uint8_t RedShift() const { return m_red_shift; }
			uint8_t GreenShift() const { return m_green_shift; }
			uint8_t BlueShift() const { return m_blue_shift; }

			bool IsValid() const;
			void Clear();
			bool IsEqual(const CPixelFormat& other) const;
			void Set(const CPixelFormat& other);

			CPixelFormat& operator=(const CPixelFormat& other);
			bool operator==(const CPixelFormat& other) const;
			bool operator!=(const CPixelFormat& other) const;

		private:
			uint16_t m_red_max = 0;
			uint16_t m_green_max = 0;
			uint16_t m_blue_max = 0;

			uint8_t m_red_shift = 0;
			uint8_t m_green_shift = 0;
			uint8_t m_blue_shift = 0;

			uint8_t m_bits_per_pixel = 0;
			uint8_t m_bytes_per_pixel = 0;
		};
	}
}