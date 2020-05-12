#pragma once
#include "HandleObject.h"

namespace mrCommonLib
{
	namespace desktop
	{
		namespace win
		{
			typedef std::shared_ptr<class CImpersonator> ImpersonatorPtr;
			class CImpersonator
			{
			public:
				CImpersonator(HANDLE hToken, bool bPipeToken = false);
				CImpersonator();
				CImpersonator(const wchar_t* pszUserName, const wchar_t* pszDomain, const wchar_t* pszPassword, DWORD dwLogonType, DWORD dwLogonProvider, THandleObj& userToken);
				~CImpersonator();

				static ImpersonatorPtr LoggedOnUser(HANDLE user_token);
				static ImpersonatorPtr LoggedPipeClient(HANDLE named_pipe);
				static ImpersonatorPtr Anonymous();
				static ImpersonatorPtr LogonUser(const wstr & userName, const wstr& pwd, DWORD logonType, THandleObj& userToken);
				static ImpersonatorPtr LogonUser(const astr & userName, const astr& pwd, DWORD logonType, THandleObj& userToken);
			private:

				static void GetUserDomain(const wstr& userName, wstr& domain, wstr& shortuser, bool& isUPN);
		
			};
		}
	}
}				