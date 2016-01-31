#include "StdAfx.h"
#include "IniFile.h"   

void CIniFile::Init()  
{  
	m_unMaxSection = 512;  
	m_unSectionNameMaxSize = 33; // 32λUID��   
}  

CIniFile::CIniFile()  
{  
	Init();  
}  

CIniFile::CIniFile(LPCTSTR szFileName)  
{  
	// (1) ����·���������·���Ƿ����   
	// (2) ��"./"��ͷ������������·���Ƿ����   
	// (3) ��"../"��ͷ�����漰���·���Ľ���   

	Init();  

	// ���·��   
	m_szFileName.Format(".//%s", szFileName);  
}  

CIniFile::~CIniFile()    
{  

}  

void CIniFile::SetFileName(LPCTSTR szFileName)  
{  
	m_szFileName.Format(".//%s", szFileName);  
}  

DWORD CIniFile::GetProfileSectionNames(CStringArray &strArray)  
{  
	int nAllSectionNamesMaxSize = m_unMaxSection*m_unSectionNameMaxSize+1;  
	char *pszSectionNames = new char[nAllSectionNamesMaxSize];  
	DWORD dwCopied = 0;  
	dwCopied = ::GetPrivateProfileSectionNames(pszSectionNames, nAllSectionNamesMaxSize, m_szFileName);  

	strArray.RemoveAll();  

	char *pSection = pszSectionNames;  
	do   
	{  
		CString szSection(pSection);  
		if (szSection.GetLength() < 1)  
		{  
			delete[] pszSectionNames;  
			return dwCopied;  
		}  
		strArray.Add(szSection);  

		pSection = pSection + szSection.GetLength() + 1; // next section name   
	} while (pSection && pSection<pszSectionNames+nAllSectionNamesMaxSize);  

	delete[] pszSectionNames;  
	return dwCopied;  
}  

DWORD CIniFile::GetProfileString(LPCTSTR lpszSectionName, LPCTSTR lpszKeyName, CString& szKeyValue, CString defaultValue /*= _T("")*/)  
{  
	DWORD dwCopied = 0;  
	dwCopied = ::GetPrivateProfileString(lpszSectionName, lpszKeyName, defaultValue,   
		szKeyValue.GetBuffer(MAX_PATH), MAX_PATH, m_szFileName);  
	szKeyValue.ReleaseBuffer();  

	return dwCopied;  
}  

int CIniFile::GetProfileInt(LPCTSTR lpszSectionName, LPCTSTR lpszKeyName, int defaultValue)
{
	int nKeyValue = ::GetPrivateProfileInt(lpszSectionName, lpszKeyName, defaultValue, m_szFileName);  

	return nKeyValue;  
}

BOOL CIniFile::SetProfileString(LPCTSTR lpszSectionName, LPCTSTR lpszKeyName, LPCTSTR lpszKeyValue)  
{  
	return ::WritePrivateProfileString(lpszSectionName, lpszKeyName, lpszKeyValue, m_szFileName);  
}  

BOOL CIniFile::SetProfileInt(LPCTSTR lpszSectionName, LPCTSTR lpszKeyName, int nKeyValue)  
{  
	CString szKeyValue;  
	szKeyValue.Format("%d", nKeyValue);  

	return ::WritePrivateProfileString(lpszSectionName, lpszKeyName, szKeyValue, m_szFileName);  
}  

BOOL CIniFile::DeleteSection(LPCTSTR lpszSectionName)  
{  
	return ::WritePrivateProfileSection(lpszSectionName, NULL, m_szFileName);  
}  

BOOL CIniFile::DeleteKey(LPCTSTR lpszSectionName, LPCTSTR lpszKeyName)  
{  
	return ::WritePrivateProfileString(lpszSectionName, lpszKeyName, NULL, m_szFileName);  
}  
