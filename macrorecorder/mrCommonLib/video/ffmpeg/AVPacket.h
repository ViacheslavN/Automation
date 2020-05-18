#pragma once

extern "C"
{
	typedef struct AVPacket	AVPacket;
}

namespace mrCommonLib
{
	namespace video
	{
		namespace ffmpeglib
		{
			class CAVPacket
			{
			public:
				CAVPacket();
				~CAVPacket();

				AVPacket * GetPacket();
				void AttachData(const byte_t *data, uint32_t size);


			private:
				AVPacket *m_avpacket;
			};
		}
	}
}