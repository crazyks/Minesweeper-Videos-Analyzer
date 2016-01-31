#include "StdAfx.h"
#include "MD5Calculator.h"
#include <wincrypt.h>


CMD5Calculator::CMD5Calculator(void)
{
}


CMD5Calculator::~CMD5Calculator(void)
{
}

BOOL CMD5Calculator::GetMd5(CString FileDirectory, CString& strFileMd5)  
{  
	HANDLE hFile=CreateFile(FileDirectory,GENERIC_READ,FILE_SHARE_READ,NULL,OPEN_EXISTING,NULL,NULL);
	if (hFile==INVALID_HANDLE_VALUE)                                        //���CreateFile����ʧ��  
	{   
		//��ʾCreateFile����ʧ�ܣ����������š�visual studio�п��ڡ����ߡ�>��������ҡ������ô���ŵõ�������Ϣ��  
		CloseHandle(hFile);
		return FALSE;
	}
	HCRYPTPROV hProv=NULL;
	if(CryptAcquireContext(&hProv,NULL,NULL,PROV_RSA_FULL,CRYPT_VERIFYCONTEXT)==FALSE)       //���CSP��һ����Կ�����ľ��
	{
		CloseHandle(hFile);
		return FALSE;
	}
	HCRYPTPROV hHash=NULL;
	//��ʼ������������hash������������һ����CSP��hash������صľ��������������������    CryptHashData���á�
	if(CryptCreateHash(hProv,CALG_MD5,0,0,&hHash)==FALSE)
	{
		CloseHandle(hFile);
		return FALSE;
	}
	DWORD dwFileSize=GetFileSize(hFile,0);    //��ȡ�ļ��Ĵ�С
	if (dwFileSize==0xFFFFFFFF)               //�����ȡ�ļ���Сʧ��  
	{
		CloseHandle(hFile);
		return FALSE;
	}
	byte* lpReadFileBuffer=new byte[dwFileSize];
	DWORD lpReadNumberOfBytes;
	if (ReadFile(hFile,lpReadFileBuffer,dwFileSize,&lpReadNumberOfBytes,NULL)==0)        //��ȡ�ļ�  
	{
		CloseHandle(hFile);
		return FALSE;
	}
	if(CryptHashData(hHash,lpReadFileBuffer,lpReadNumberOfBytes,0)==FALSE)      //hash�ļ�  
	{
		CloseHandle(hFile);
		return FALSE;
	}
	delete[] lpReadFileBuffer;
	CloseHandle(hFile);          //�ر��ļ����
	BYTE *pbHash;
	DWORD dwHashLen=sizeof(DWORD);
	if (!CryptGetHashParam(hHash,HP_HASHVAL,NULL,&dwHashLen,0)) //��Ҳ��֪��ΪʲôҪ����������CryptGetHashParam������ǲ��յ�msdn       
	{
		return FALSE;
	}
	pbHash=(byte*)malloc(dwHashLen);
	if(CryptGetHashParam(hHash,HP_HASHVAL,pbHash,&dwHashLen,0))//���md5ֵ 
	{
		for(DWORD i=0;i<dwHashLen;i++)         //���md5ֵ 
		{
			TCHAR str[3]={0};
			CString strFilePartM=_T("");
			_stprintf(str,_T("%02x"),pbHash[i]);
			strFileMd5+=str;
		}
	} 
	if (pbHash != NULL) free(pbHash);

	//�ƺ���
	if(CryptDestroyHash(hHash)==FALSE)          //����hash����  
	{
		return FALSE;
	}
	if(CryptReleaseContext(hProv,0)==FALSE)
	{
		return FALSE;
	}
	return TRUE;
}
