#pragma once  

class CIniFile  
{  
public:  
	CIniFile();  
	CIniFile(LPCTSTR szFileName);  
	virtual ~CIniFile();  

public:  
	// Attributes      
	void SetFileName(LPCTSTR szFileName);  

public:  
	// Operations   
	BOOL SetProfileInt(LPCTSTR lpszSectionName, LPCTSTR lpszKeyName, int nKeyValue);  
	BOOL SetProfileString(LPCTSTR lpszSectionName, LPCTSTR lpszKeyName, LPCTSTR lpszKeyValue);  

	DWORD GetProfileSectionNames(CStringArray& strArray); // ����section����   

	int GetProfileInt(LPCTSTR lpszSectionName, LPCTSTR lpszKeyName, int defaultValue = 0);  
	DWORD GetProfileString(LPCTSTR lpszSectionName, LPCTSTR lpszKeyName, CString& szKeyValue, CString defaultValue = _T(""));  

	BOOL DeleteSection(LPCTSTR lpszSectionName);  
	BOOL DeleteKey(LPCTSTR lpszSectionName, LPCTSTR lpszKeyName);  

private:  
	CString  m_szFileName; // .//Config.ini, ������ļ������ڣ���exe��һ����ͼWriteʱ���������ļ�   

	UINT m_unMaxSection; // ���֧�ֵ�section��(256)   
	UINT m_unSectionNameMaxSize; // section���Ƴ��ȣ�������Ϊ32(Null-terminated)   

	void Init();  
};
