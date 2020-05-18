#include "pch.h"
#include "x265Header.h"
#include "X265Picture.h"

namespace mrCommonLib
{
	namespace video
	{
		namespace x265lib
		{
			CX265Picture::CX265Picture(X265ParamPtr pParam)
			{
				m_picture = x265_picture_alloc();
				x265_picture_init(pParam->GetHandle(), m_picture);

				internalCsp = pParam->GetHandle()->internalCsp;
				m_width = pParam->GetHandle()->sourceWidth;
				m_height = pParam->GetHandle()->sourceHeight;

				uint32_t y_size = m_width * m_height;

				switch (internalCsp)
				{
				case X265_CSP_I444:
				{

					m_imageBuffer.resize(y_size * 3 / 2);
					m_picture->planes[0] = (void*)m_imageBuffer.data();
					m_picture->planes[1] = (void*)(m_imageBuffer.data() + y_size);
					m_picture->planes[2] = (void*)(m_imageBuffer.data() + y_size * 2);
					m_picture->stride[0] = m_width;
					m_picture->stride[1] = m_width;
					m_picture->stride[2] = m_width;
					break;
				}
				case X265_CSP_I420:
				{
					m_imageBuffer.resize(y_size * 3 );
					m_picture->planes[0] = (void*)m_imageBuffer.data();
					m_picture->planes[1] = (void*)(m_imageBuffer.data() + y_size);
					m_picture->planes[2] = (void*)(m_imageBuffer.data() + y_size * 5 / 4);
					m_picture->stride[0] = m_width;
					m_picture->stride[1] = m_width / 2;
					m_picture->stride[2] = m_width / 2;
					break;
				}
				default:
					throw CommonLib::CExcBase("Colorspace Not Support %1", internalCsp);

				}

			}

			void CX265Picture::AttachImg(const byte_t* pYumBuf)
			{

			
			}

			x265_picture * CX265Picture::GetHandle() const
			{
				return m_picture;
			}

			CX265Picture::~CX265Picture()
			{
				if(m_picture != nullptr)
					x265_picture_free(m_picture);
			}
		}
	}
}