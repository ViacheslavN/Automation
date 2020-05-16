#include "pch.h"
#include "vpx.h"
#include "VPXException.h"

namespace mrCommonLib
{
	namespace video
	{
		namespace vpxlib
		{
			CVPXException::CVPXException(vpx_codec_err_t err) : m_err(err)
			{
				CommonLib::CExcBase::AddMsg(GetVPXLibErrorDesc(err));
			}

			CVPXException::~CVPXException() noexcept
			{

			}

			std::shared_ptr<CommonLib::CExcBase> CVPXException::Clone() const
			{
				std::shared_ptr<CommonLib::CExcBase> ptrExc(new CVPXException(*this));
				return ptrExc;
			}

			astr CVPXException::GetVPXLibErrorDesc(vpx_codec_err_t err)
			{
				return vpx_codec_err_to_string(err);
			}
		}
	}
}