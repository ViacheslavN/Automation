#include "pch.h"
#include "VideoPackage.h"
#include "VideoUtil.h"

namespace mrCommonLib
{
	namespace video
	{
		CVideoPackage::CVideoPackage()
		{

		}

		CVideoPackage::~CVideoPackage()
		{

		}

		void CVideoPackage::Load(CommonLib::IReadStream *pStream)
		{
			try
			{
				Clear();
				m_codecId = (EVideoEncoderId)pStream->ReadIntu32();
				m_format = CVideoUtil::ReadPixelFormat(pStream);
				m_screenRect = CVideoUtil::ReadRect(pStream);
				uint32_t blocs = pStream->ReadIntu32();
				for (uint32_t i = 0; i < blocs; ++i)
				{
					desktop::CRect rect = CVideoUtil::ReadRect(pStream);
					m_dirtyRects.push_back(rect);
				}

				uint32_t dataSize = pStream->ReadIntu32();
				m_data.resize(dataSize);
				pStream->Read(m_data.data(), dataSize);

			}
			catch (std::exception &exc)
			{
				CommonLib::CExcBase::RegenExc("Failed to load video package", exc);
				throw;
			}
		}

		void CVideoPackage::Save(CommonLib::IWriteStream *pStream)
		{
			pStream->Write(uint32_t(m_codecId));
			CVideoUtil::WritePixelFormat(m_format, pStream);
			CVideoUtil::WriteRect(m_screenRect, pStream);
			pStream->Write((uint32_t)m_dirtyRects.size());
			for (size_t i = 0, sz = m_dirtyRects.size(); i < sz; ++i)
			{
				CVideoUtil::WriteRect(m_dirtyRects[i], pStream);
			}

			pStream->Write(uint32_t(m_data.size()));
			pStream->Write(m_data.data(), m_data.size());

		}

		void  CVideoPackage::Clear()
		{
			m_data.clear();
			m_dirtyRects.clear();
			m_codecId = VIDEO_ENCODING_NODE;
		}

		void CVideoPackage::SetEncoderId(EVideoEncoderId codecId)
		{
			m_codecId = codecId;
		}

		void CVideoPackage::AddDirtyRect(const desktop::CRect& rect)
		{
			m_dirtyRects.push_back(rect);
		}

		void CVideoPackage::SetScreenRect(const desktop::CRect& rect)
		{
			m_screenRect = rect;
		}

		void CVideoPackage::SetPixelFormat(const desktop::CPixelFormat& format)
		{
			m_format = format;
		}

		void CVideoPackage::SetEncodeData(const byte_t *pData, size_t nSize)
		{
			m_data.resize(nSize);
			memcpy(m_data.data(), pData, nSize);
		}

		EVideoEncoderId CVideoPackage::GetEncoderId() const
		{
			return m_codecId;
		}

		const desktop::CPixelFormat& CVideoPackage::GetPixelFormat() const
		{
			return m_format;
		}
		
		uint32_t CVideoPackage::GetDirtyRectCount() const
		{
			return (uint32_t)m_dirtyRects.size();
		}

		const desktop::CRect& CVideoPackage::GetDirtyRect(uint32_t idx) const
		{
			return  m_dirtyRects[idx];
		}

		const desktop::CRect& CVideoPackage::GetScreenRect() const
		{
			return m_screenRect;
		}

		uint32_t CVideoPackage::GetEncodeDataSize() const
		{
			return (uint32_t)m_data.size();
		}

		const byte_t *CVideoPackage::GetEncodeData() const
		{
			return m_data.data();
		}
	}
}