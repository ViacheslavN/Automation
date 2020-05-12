#pragma once
#include "../pixelformat.h"
#include "../geometry.h"


namespace mrCommonLib
{
	namespace desktop
	{
		namespace win
		{
			typedef std::shared_ptr<class CHBitmap> HBitmapPtr;

			class CHBitmap
			{
			public:
				~CHBitmap();

				HBITMAP GetBitmap() const;
				byte_t* GetData() const;
				const CSize& GetSize() const;
				const CPixelFormat& GetPixelFormat() const;


				void SaveToFile(const wstr& fileName) const;

				static	HBitmapPtr Ñreate(const CSize& size, const CPixelFormat& format, HDC hdc);

			private:
				CHBitmap(HBITMAP bitmap, byte_t *data, const CSize& size, const CPixelFormat& format);

			private:
				CPixelFormat m_format;
				HBITMAP m_bitmap = NULL;
				byte_t *m_data = nullptr;
				CSize m_size;
			};
		}
	}
}