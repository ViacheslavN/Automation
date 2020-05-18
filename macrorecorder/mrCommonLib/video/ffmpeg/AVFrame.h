#pragma once

extern "C"
{
	typedef struct AVFrame	AVFrame;
}

namespace mrCommonLib
{
	namespace video
	{
		namespace ffmpeglib
		{
			class CBaseCodec;

			class CAVFrame
			{
			public:
				CAVFrame();
				CAVFrame(CBaseCodec *pCodec);

				~CAVFrame();

				AVFrame * GetFrame();

			private:
				AVFrame *m_frame;
			};
		}
	}
}