#include "StdAfx.h"
#include "VideoInfo.h"


CVideoInfo::CVideoInfo(void)
	: mStrZiNi(_T(""))
	, mStrZiNis(_T(""))
	, mStrHZiNi(_T(""))
	, mStrHZiNis(_T(""))
	, mStrClicks(_T(""))
	, mStrClicksPers(_T(""))
	, mStrLeftClicks(_T(""))
	, mStrLeftClicksPers(_T(""))
	, mStrRightClicks(_T(""))
	, mStrRightClicksPers(_T(""))
	, mStrDoubleClicks(_T(""))
	, mStrDoubleClicksPers(_T(""))
	, mStrIOE(_T(""))
	, mStrOpenings(_T(""))
	, mIslands(_T(""))
	, mFlags(_T(""))
{
}


CVideoInfo::~CVideoInfo(void)
{
}
