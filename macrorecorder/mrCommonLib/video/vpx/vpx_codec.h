#pragma once

extern "C"
{
	typedef struct vpx_codec_ctx vpx_codec_ctx_t;
}

namespace mrCommonLib
{
	namespace video
	{
		namespace vpxlib
		{
			struct VpxCodecDeleter
			{
				void operator()(vpx_codec_ctx_t* codec);
			};

			typedef std::unique_ptr<vpx_codec_ctx_t, VpxCodecDeleter> VpxCodecPtr;

		} // namespace codec
	}
}