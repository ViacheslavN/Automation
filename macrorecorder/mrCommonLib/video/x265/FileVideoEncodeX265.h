#pragma once
#include "../VideoEncoder.h"
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
			class CFileVideoEncoderX265 : public IVideoFileEncoder
			{
			public:
				CFileVideoEncoderX265();

				virtual void OpenFile(const char* pszFileName, uint32_t width, uint32_t height, const desktop::CPixelFormat& format);
				virtual void Encode(desktop::IFrame* pFrame);
				virtual void Close();
 			private:
				X265EncoderPtr m_encoder;
				X265PicturePtr m_picture;
				X265ParamPtr m_param;
				desktop::CPixelFormat m_format;
				CommonLib::file::TFilePtr m_file;
			};
		}

	}
}