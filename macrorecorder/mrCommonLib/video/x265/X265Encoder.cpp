#include "pch.h"
#include "x265Header.h"
#include "X265Encoder.h"

namespace mrCommonLib
{
	namespace video
	{
		namespace x265lib
		{
			CX265Encoder::CX265Encoder(X265ParamPtr pParam)
			{
				m_encoder = x265_encoder_open(pParam->GetHandle());
				if (m_encoder == nullptr)
					throw CommonLib::CExcBase("x265_encoder_open err\n");
			}

			CX265Encoder::~CX265Encoder()
			{
				if(m_encoder != nullptr)
					x265_encoder_close(m_encoder);
			}

			x265_encoder* CX265Encoder::GetHandle() const
			{
				return m_encoder;
			}

			int CX265Encoder::EncodeHeader(x265_nal **pNal, uint32_t *pi_nal)
			{
				return x265_encoder_headers(m_encoder, pNal, pi_nal);
			}

			int CX265Encoder::Encode(x265_nal **pNal,   uint32_t *pi_nal, x265_picture *pic_in, x265_picture* pic_out)
			{
				if (pic_in)
				{
					if (m_frameCount++ % 8 == 0)
					{
						pic_in->sliceType = X265_TYPE_IDR;
					}
					else
					{
						pic_in->sliceType = X265_TYPE_AUTO;
					}

				}
			
				int ret = x265_encoder_encode(m_encoder, pNal, pi_nal, pic_in, pic_out);

				if (pic_in)
					pic_in->pts++;

				return ret;
			}
		}
	}
}