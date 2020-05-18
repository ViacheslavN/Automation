#pragma once

#include "VideoEncodingIds.h"
#include "../desktop/geometry.h"
#include "../desktop/pixelformat.h"

namespace mrCommonLib
{
	namespace video
	{
		class CVideoPackage
		{
		public:
			CVideoPackage();
			~CVideoPackage();

			void Load(CommonLib::IReadStream *pStream);
			void Save(CommonLib::IWriteStream *pStream);

			void Clear();

			void SetChange();
			void SetEncoderId(EVideoEncoderId codecId);
			void SetPixelFormat(const desktop::CPixelFormat& format);
			void AddDirtyRect(const desktop::CRect& rect);
			void SetScreenRect(const desktop::CRect& rect);
			void AddEncodeData(const byte_t *pData, size_t nSize);

			EVideoEncoderId GetEncoderId() const;
			uint32_t GetDirtyRectCount() const;
			const desktop::CPixelFormat& GetPixelFormat() const;
			const desktop::CRect& GetDirtyRect(uint32_t idx) const;
			const desktop::CRect& GetScreenRect() const;
			uint32_t GetEncodeDataSize() const;
			const byte_t *GetEncodeData() const;
			bool IsChange() const;
				
		private:
			std::vector<uint8_t> m_data;
			std::vector<desktop::CRect> m_dirtyRects;
			desktop::CRect m_screenRect;
			desktop::CPixelFormat m_format;
			EVideoEncoderId m_codecId = VIDEO_ENCODING_NODE;
			bool m_change = false;


		};
	}
}