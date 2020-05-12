#pragma once
namespace mrCommonLib
{
	namespace desktop
	{
		namespace win
		{

			class CWinHDC
			{
			public:
				CWinHDC();
				CWinHDC(HWND hwnd);
				~CWinHDC();

				void Close();
				void GetDC(HWND hwnd);
				bool IsValid() const;
				operator HDC() const { return m_dc; }
			private:
				HWND m_hWnd = nullptr;;
				HDC m_dc = nullptr;;
			};

			class CHDC
			{
			public:
				CHDC();
				CHDC(HDC hdc);
				~CHDC();

				HDC Get();
				void Reset(HDC h = nullptr);
				bool IsValid() const;

				operator HDC() const { return m_hdc; }

			private:
				void Close();
				

			private:
				HDC m_hdc = nullptr;
			};
		}
	}
}