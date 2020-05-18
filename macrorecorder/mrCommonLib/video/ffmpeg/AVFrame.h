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

			class CAVFrame
			{
			public:
				CAVFrame();
				~CAVFrame();

				AVFrame * GetFrame();

			private:
				AVFrame *m_frame;
			};
		}
	}
}