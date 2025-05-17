#pragma once

namespace mrCommonLib
{
	namespace video
	{
		namespace ffmpeglib
		{
			class CFFmpegException : public CommonLib::CExcBase
			{
			public:
				CFFmpegException(int err);

				template<typename... Types>
				CFFmpegException(int err, const astr& format, Types... args) : CommonLib::CExcBase(format, args...), m_err(err)
				{
					CommonLib::CExcBase::AddMsg(GetFFmpegLibErrorDesc(err));
				}

				~CFFmpegException() noexcept;
				virtual std::shared_ptr<CommonLib::CExcBase> Clone() const;

				static astr GetFFmpegLibErrorDesc(int err);
			private:

			private:
				int m_err;

			};
		}
	}
}