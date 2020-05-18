#pragma once
#include "X265Param.h"

extern "C"
{
	typedef struct x265_encoder x265_encoder;
}

namespace mrCommonLib
{
	namespace video
	{
		namespace x265lib
		{
			typedef std::shared_ptr<class CX265Encoder> X265EncoderPtr;

			class CX265Encoder
			{
			public:
				CX265Encoder(X265ParamPtr pParam);
				~CX265Encoder();

				x265_encoder* GetHandle() const;
				int EncodeHeader(x265_nal **pNal, uint32_t *pi_nal);
				int Encode(x265_nal **pNal, uint32_t *pi_nal, x265_picture *pic_in, x265_picture* pic_out);
			private:
				x265_encoder* m_encoder = nullptr;
				uint32_t m_frameCount = 0;
			};
		}
	}
}