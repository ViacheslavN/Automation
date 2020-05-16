#pragma once


namespace mrCommonLib
{
	namespace video
	{
		namespace vpxlib
		{
			class CVPXException : public CommonLib::CExcBase
			{
			public:
				CVPXException(vpx_codec_err_t err);

				template<typename... Types>
				CVPXException(vpx_codec_err_t err, const astr& format, Types... args) : CommonLib::CExcBase(format, args...), m_err(err)
				{
					CommonLib::CExcBase::AddMsg(GetVPXLibErrorDesc(err));
				}

				~CVPXException() noexcept;
				virtual std::shared_ptr<CommonLib::CExcBase> Clone() const;

				static astr GetVPXLibErrorDesc(vpx_codec_err_t err);
			private:

			private:
				vpx_codec_err_t m_err;

			};
		}
	}
}