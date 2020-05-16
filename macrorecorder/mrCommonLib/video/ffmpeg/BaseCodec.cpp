#include "pch.h"
#include "FFmpeg.h"
#include "BaseCodec.h"
#include "ffmpegUtils.h"

namespace mrCommonLib
{
	namespace video
	{
		namespace ffmpeglib
		{
			CBaseCodec::CBaseCodec(const char* codecName, bool encode)
			{
				AVCodec* codec = nullptr;
				if (encode)
				{
					
					codec = avcodec_find_encoder_by_name(codecName);
				}
				else
				{
					codec = avcodec_find_decoder_by_name(codecName);
				}

				if (!codec)
					throw CommonLib::CExcBase("%1 with name %2 not found", encode ? "Encoder" : "Coder", codecName);

				m_codecContext = AllocContext(codec);

			}

			CBaseCodec::CBaseCodec(AVCodecID codecId, bool encode)
			{
				AVCodec* codec = nullptr;
				if (encode)
				{
				
					codec = avcodec_find_encoder(codecId);
				}
				else
				{
					codec = avcodec_find_decoder(codecId);
				}

				if (!codec)
					throw CommonLib::CExcBase("%1 with codecId %2 not found", encode ? "Encoder" : "Coder", codecId);

				m_codecContext = AllocContext(codec);
			}

			CBaseCodec::~CBaseCodec()
			{
				Clean();
			}

			AVCodecContext* CBaseCodec::GetContext()
			{
				if (m_codecContext == nullptr)
					throw CommonLib::CExcBase("CodecContext is null");

				return m_codecContext;
			}


			AVCodecContext* CBaseCodec::AllocContext(AVCodec* codec)
			{
				AVCodecContext* codecContext = avcodec_alloc_context3(codec);
				if (!codecContext)
					throw  CommonLib::CExcBase("Could not allocate video codec context for codec %1", codec->name);

				// copy the type
				codecContext->codec_type = codec->type;

				return codecContext;
			}

			void CBaseCodec::SetOption(const char* name, const char* value)
			{
				int ret = av_opt_set(GetContext()->priv_data, name, value, 0);
				CFFmpegUtil::CheckAndThrowException(ret);

			}

			void CBaseCodec::SetOption(const char* name, int value)
			{
				int ret = av_opt_set_int(GetContext()->priv_data, name, value, 0);
				CFFmpegUtil::CheckAndThrowException(ret);
			}

			void CBaseCodec::SetOption(const char* name, double value)
			{
				int ret = av_opt_set_double(GetContext()->priv_data, name, value, 0);
				CFFmpegUtil::CheckAndThrowException(ret);
			}

			void CBaseCodec::SetGenericOption(const char* name, const char* value)
			{
				int ret = av_opt_set(GetContext(), name, value, 0);
				CFFmpegUtil::CheckAndThrowException(ret);
			}

			void CBaseCodec::Clean()
			{
				if (m_codecContext != nullptr)
					avcodec_free_context(&m_codecContext);
			}
		}
	}
}