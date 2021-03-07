#pragma once
#include "../video/VideoEncodingIds.h"

namespace mrCommonLib
{
	namespace proto
	{

		class CVideoCodecSettingsPackage
		{
		public:
			CVideoCodecSettingsPackage();
			~CVideoCodecSettingsPackage();


			void Read(CommonLib::IReadStream *pStream);

			video::EVideoEncoderId GetEncodeId() const;
			void SetEncodeId(video::EVideoEncoderId encodeId );
		private:
			video::EVideoEncoderId m_encodeId;
		};

	}
}