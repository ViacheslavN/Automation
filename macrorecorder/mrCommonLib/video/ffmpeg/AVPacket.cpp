#include "pch.h"
#include "FFmpeg.h"
#include "AVPacket.h"

namespace mrCommonLib
{
	namespace video
	{
		namespace ffmpeglib
		{
			CAVPacket::CAVPacket()
			{
				m_avpacket = av_packet_alloc();
				if (m_avpacket == nullptr)
					throw CommonLib::CExcBase("Failed to alloc avpacket");

				av_init_packet(m_avpacket);
			}

			CAVPacket::~CAVPacket()
			{
				if (m_avpacket != nullptr)
					av_packet_free(&m_avpacket);
			}

			AVPacket * CAVPacket::GetPacket()
			{
				return m_avpacket;
			}

			void CAVPacket::AttachData(const byte_t *data, uint32_t size)
			{
				m_avpacket->size = size;
				m_avpacket->data = (uint8_t*)data;
			}
		}
	}
}