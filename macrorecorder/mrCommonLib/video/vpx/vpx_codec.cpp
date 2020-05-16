#include "pch.h"
#include "vpx.h"
#include "vpx_codec.h"

namespace mrCommonLib
{
	namespace video
	{
		namespace vpxlib
		{

			void VpxCodecDeleter::operator()(vpx_codec_ctx_t* codec)
			{
				if (codec)
				{
					vpx_codec_err_t ret = vpx_codec_destroy(codec);
					delete codec;
				}
			}
		}
	}
}