#pragma once
#include "pch.h"
#include "DataFrame.h"


namespace mrCommonLib
{
	namespace desktop
	{
		CDataFrame::CDataFrame(const CPixelFormat& format, const CSize& size) : IBaseFrame(format, size)
		{
			m_pAlloc = CommonLib::IAlloc::CreateSimpleAlloc();
			uint32_t bufSize = Stride() * m_size.Height();
			m_data = (byte_t*)m_pAlloc->Alloc(bufSize);
		}

		CDataFrame::CDataFrame(const CPixelFormat& format, const CSize& size, CommonLib::IAllocPtr pAlloc) : IBaseFrame(format, size)
		{
			if (m_pAlloc.get() == nullptr)
				m_pAlloc = CommonLib::IAlloc::CreateSimpleAlloc();
			
			uint32_t bufSize = Stride() * m_size.Height();
			m_data = (byte_t*)m_pAlloc->Alloc(bufSize);
		}

		CDataFrame::CDataFrame(const CPixelFormat& format, const CSize& size, byte_t *data) : IBaseFrame(format, size), m_data(data)
		{

		}
		
		CDataFrame::~CDataFrame()
		{
			if(m_pAlloc.get() != nullptr)
				m_pAlloc->Free(m_data);
		}

		byte_t* CDataFrame::Data() const
		{
			return m_data;
		}
	}
}