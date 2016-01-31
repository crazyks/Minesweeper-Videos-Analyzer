#include "StdAfx.h"
#include "Formatter.h"


CFormatter::CFormatter(void)
{
}


CFormatter::~CFormatter(void)
{
}


CString CFormatter::UINT2CString(const UINT value)
{
	CString ret;
	ret.Format(_T("%d"), value);
	return ret;
}


UINT CFormatter::CString2UINT(const CString value)
{
	return _tstoi(value);
}
