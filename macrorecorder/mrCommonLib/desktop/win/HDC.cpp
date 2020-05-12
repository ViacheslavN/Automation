#include "pch.h"
#include "HDC.h"

#pragma once
namespace mrCommonLib
{
	namespace desktop
	{
		namespace win
		{
			CWinHDC::CWinHDC()
			{}

			CWinHDC::CWinHDC(HWND hwnd)
			{
				GetDC(hwnd);
			}

			CWinHDC::~CWinHDC()
			{
				Close();
			}

			void CWinHDC::Close()
			{
				if(m_dc)
					::ReleaseDC(m_hWnd, m_dc);

				m_dc = nullptr;
			}

			void CWinHDC::GetDC(HWND hwnd)
			{
				Close();

				m_hWnd = hwnd;
				m_dc = ::GetDC(m_hWnd);
			}

			bool CWinHDC::IsValid() const
			{
				return m_dc != nullptr;
			}
			
			//////////////////////////////////////////////////////////////////////////////////////////////////////////////////

			CHDC::CHDC()
			{}
			
			CHDC::CHDC(HDC hdc) :m_hdc(hdc)
			{

			}

			CHDC::~CHDC()
			{
				Close();
			}

			HDC CHDC::Get()
			{
				return m_hdc;
			}

			void CHDC::Reset(HDC hdc)
			{
				Close();
				m_hdc = hdc;
			}
			
			bool CHDC::IsValid() const
			{ 
				return m_hdc != nullptr; 
			}

			void CHDC::Close()
			{
				if (m_hdc)
					DeleteDC(m_hdc);

				m_hdc = nullptr;
			}


		}
	}
}