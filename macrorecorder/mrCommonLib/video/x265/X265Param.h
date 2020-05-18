#pragma once

extern "C"
{
	typedef struct x265_param x265_param;
}

namespace mrCommonLib
{
	namespace video
	{
		namespace x265lib
		{
			typedef std::shared_ptr<class CX265Param> X265ParamPtr;

			class CX265Param
			{
			public:
				CX265Param();
				~CX265Param();

				x265_param* GetHandle() const;
			private:
				x265_param* m_param = nullptr;
			};
		}
	}
}