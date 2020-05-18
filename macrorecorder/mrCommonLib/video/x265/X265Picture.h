#pragma once

#include "X265Param.h"

namespace mrCommonLib
{
	namespace video
	{		
		namespace x265lib
		{
			typedef std::shared_ptr<class CX265Picture> X265PicturePtr;

			class CX265Picture
			{
			public:
				CX265Picture(X265ParamPtr pParam);
				~CX265Picture();

				void AttachImg(const byte_t* yum);

				x265_picture * GetHandle() const;

			private:
				x265_picture *m_picture = nullptr;
				int internalCsp;
				uint32_t m_width;
				uint32_t m_height;
				std::vector<uint8_t> m_imageBuffer;
			};
		}
	}
}