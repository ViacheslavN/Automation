#include "pch.h"
#include "Impersonator.h"

namespace mrCommonLib
{
	namespace desktop
	{
		namespace win
		{
			CImpersonator::CImpersonator(HANDLE hToken, bool bPipeToken)
			{
				if (!hToken)
					throw CommonLib::CExcBase(L"Failed Impersonation, token is null");

				if (bPipeToken)
				{
					if (!ImpersonateNamedPipeClient(hToken))
						throw CommonLib::CWinExc(L"Failed to impersonation");

					return;
				}

				if (!ImpersonateLoggedOnUser(hToken))
					throw CommonLib::CWinExc(L"Failed to impersonation");
			}

			CImpersonator::CImpersonator()
			{
				if (!ImpersonateAnonymousToken(GetCurrentThread()))
					throw CommonLib::CWinExc(L"Failed to anonymous impersonation");
			}

			CImpersonator::CImpersonator(const wchar_t* pszUserName, const wchar_t* pszDomain, const wchar_t* pszPassword, DWORD dwLogonType, DWORD dwLogonProvider, THandleObj& userToken)
			{
				if(!::LogonUser(pszUserName, pszDomain, pszPassword, dwLogonType, dwLogonProvider, userToken.Recieve()))
					throw CommonLib::CWinExc(L"Failed to impersonation");
			}
						
			CImpersonator::~CImpersonator()
			{
				::RevertToSelf();
			}

			void CImpersonator::GetUserDomain(const wstr& userName, wstr& domain, wstr& shortuser, bool& isUPN)
			{
				auto it = std::find(userName.begin(), userName.end(), (L'\\'));
				isUPN = false;
				if (it != userName.end())
				{
					domain.assign(userName.begin(), it);
					shortuser.assign(it + 1, userName.end());
				}
				else
				{
					it = std::find(userName.begin(), userName.end(), (L'@'));
					if (it != userName.end())
						isUPN = true;
					else
					{
						domain = L".";
						shortuser = userName;
					}
				}
			}

			ImpersonatorPtr CImpersonator::LogonUser(const wstr & userName, const wstr& pwd, DWORD logonType, THandleObj& userToken)
			{
				wstr domain, shortUserName;
				bool isUPN = false;
				GetUserDomain(userName, domain, shortUserName, isUPN);

				if (isUPN)
					return ImpersonatorPtr(new CImpersonator(userName.c_str(), NULL, pwd.c_str(), logonType, LOGON32_PROVIDER_DEFAULT, userToken));
				else
					return ImpersonatorPtr(new CImpersonator(shortUserName.c_str(), domain.c_str(), pwd.c_str(), logonType, LOGON32_PROVIDER_DEFAULT, userToken));
			}

			ImpersonatorPtr CImpersonator::LogonUser(const astr & userName, const astr& pwd, DWORD logonType, THandleObj& userToken)
			{
				return LogonUser(CommonLib::StringEncoding::str_a2w_safe(userName), CommonLib::StringEncoding::str_a2w_safe(pwd), logonType, userToken);
			}

			ImpersonatorPtr CImpersonator::LoggedOnUser(HANDLE user_token)
			{
				return ImpersonatorPtr(new CImpersonator(user_token));
			}

			ImpersonatorPtr CImpersonator::LoggedPipeClient(HANDLE named_pipe)
			{
				return ImpersonatorPtr(new CImpersonator(named_pipe, true));
			}

			ImpersonatorPtr CImpersonator::Anonymous()
			{
				return ImpersonatorPtr(new CImpersonator());
			}
		}
	}
}