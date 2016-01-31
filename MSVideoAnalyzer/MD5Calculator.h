#pragma once
class CMD5Calculator
{
private:
	CMD5Calculator(void);
	~CMD5Calculator(void);
public:
	static BOOL GetMd5(CString FileDirectory, CString& strFileMd5);
};

