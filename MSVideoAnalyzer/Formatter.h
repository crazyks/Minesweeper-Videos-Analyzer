#pragma once
class CFormatter
{
private:
	CFormatter(void);
	~CFormatter(void);
public:
	static CString UINT2CString(const UINT value);
	static UINT CString2UINT(const CString value);
};

