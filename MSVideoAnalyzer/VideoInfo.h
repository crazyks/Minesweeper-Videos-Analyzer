#pragma once
#include "BasicVideoInfo.h"

class CVideoInfo :
	public CBasicVideoInfo
{
public:
	CVideoInfo(void);
	~CVideoInfo(void);

public:
	CString mStrZiNi;
	CString mStrZiNis;
	CString mStrHZiNi;
	CString mStrHZiNis;
	CString mStrClicks;
	CString mStrClicksPers;
	CString mStrLeftClicks;
	CString mStrLeftClicksPers;
	CString mStrRightClicks;
	CString mStrRightClicksPers;
	CString mStrDoubleClicks;
	CString mStrDoubleClicksPers;
	CString mStrIOE;
	CString mStrOpenings;
	CString mIslands;
	CString mFlags;
};

