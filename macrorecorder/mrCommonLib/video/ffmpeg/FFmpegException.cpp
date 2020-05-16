#include "pch.h"
#include "FFmpeg.h"
#include "FFmpegException.h"

namespace mrCommonLib
{
	namespace video
	{
		namespace ffmpeglib
		{
			CFFmpegException::CFFmpegException(int err) : m_err(err)
			{
				CommonLib::CExcBase::AddMsg(GetFFmpegLibErrorDesc(err));
			}

			CFFmpegException::~CFFmpegException() noexcept
			{

			}

			std::shared_ptr<CommonLib::CExcBase> CFFmpegException::Clone() const
			{
				std::shared_ptr<CFFmpegException> ptrExc(new CFFmpegException(*this));
				return ptrExc;
			}

			astr CFFmpegException::GetFFmpegLibErrorDesc(int err)
			{
				char errorBuf[AV_ERROR_MAX_STRING_SIZE];
				return av_make_error_string(errorBuf, AV_ERROR_MAX_STRING_SIZE, err);
			}
		}
	}
}