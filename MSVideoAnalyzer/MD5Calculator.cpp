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
	if (hFile==INVALID_HANDLE_VALUE)                                        //如果CreateFile调用失败  
	{   
		//提示CreateFile调用失败，并输出错误号。visual studio中可在“工具”>“错误查找”中利用错误号得到错误信息。  
		CloseHandle(hFile);
		return FALSE;
	}
	HCRYPTPROV hProv=NULL;
	if(CryptAcquireContext(&hProv,NULL,NULL,PROV_RSA_FULL,CRYPT_VERIFYCONTEXT)==FALSE)       //获得CSP中一个密钥容器的句柄
	{
		CloseHandle(hFile);
		return FALSE;
	}
	HCRYPTPROV hHash=NULL;
	//初始化对数据流的hash，创建并返回一个与CSP的hash对象相关的句柄。这个句柄接下来将被    CryptHashData调用。
	if(CryptCreateHash(hProv,CALG_MD5,0,0,&hHash)==FALSE)
	{
		CloseHandle(hFile);
		return FALSE;
	}
	DWORD dwFileSize=GetFileSize(hFile,0);    //获取文件的大小
	if (dwFileSize==0xFFFFFFFF)               //如果获取文件大小失败  
	{
		CloseHandle(hFile);
		return FALSE;
	}
	byte* lpReadFileBuffer=new byte[dwFileSize];
	DWORD lpReadNumberOfBytes;
	if (ReadFile(hFile,lpReadFileBuffer,dwFileSize,&lpReadNumberOfBytes,NULL)==0)        //读取文件  
	{
		CloseHandle(hFile);
		return FALSE;
	}
	if(CryptHashData(hHash,lpReadFileBuffer,lpReadNumberOfBytes,0)==FALSE)      //hash文件  
	{
		CloseHandle(hFile);
		return FALSE;
	}
	delete[] lpReadFileBuffer;
	CloseHandle(hFile);          //关闭文件句柄
	BYTE *pbHash;
	DWORD dwHashLen=sizeof(DWORD);
	if (!CryptGetHashParam(hHash,HP_HASHVAL,NULL,&dwHashLen,0)) //我也不知道为什么要先这样调用CryptGetHashParam，这块是参照的msdn       
	{
		return FALSE;
	}
	pbHash=(byte*)malloc(dwHashLen);
	if(CryptGetHashParam(hHash,HP_HASHVAL,pbHash,&dwHashLen,0))//获得md5值 
	{
		for(DWORD i=0;i<dwHashLen;i++)         //输出md5值 
		{
			TCHAR str[3]={0};
			CString strFilePartM=_T("");
			_stprintf(str,_T("%02x"),pbHash[i]);
			strFileMd5+=str;
		}
	} 
	if (pbHash != NULL) free(pbHash);

	//善后工作
	if(CryptDestroyHash(hHash)==FALSE)          //销毁hash对象  
	{
		return FALSE;
	}
	if(CryptReleaseContext(hProv,0)==FALSE)
	{
		return FALSE;
	}
	return TRUE;
}
