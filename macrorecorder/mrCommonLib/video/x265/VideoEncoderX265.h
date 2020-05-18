#pragma once
#include "../BaseVideoEncoder.h"
#include "x265Header.h"
#include "X265Param.h"
#include "X265Picture.h"
#include "X265Encoder.h"

namespace mrCommonLib
{
	namespace video
	{
		namespace x265lib
		{
			class CVideoEncoderX265 : public IBaseVideoEncoder
			{
			public:
				~CVideoEncoderX265() = default;
				CVideoEncoderX265();

				virtual void Encode(desktop::IFrame* pFrame, CVideoPackage *pVideoPackage, bool& bSkip);
			private:
				X265EncoderPtr m_encoder;
				X265PicturePtr m_picture;
				X265ParamPtr m_param;

			};
		}

	}
}