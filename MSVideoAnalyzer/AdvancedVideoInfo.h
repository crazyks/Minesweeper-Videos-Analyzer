#pragma once
#include "videoinfo.h"
class CAdvancedVideoInfo :
	public CVideoInfo
{
public:
	CAdvancedVideoInfo(void);
	~CAdvancedVideoInfo(void);

public:
	CString mStrCorrectness;
	CString mStrWastedClicks;
	CString mStrWastedLeftClicks;
	CString mStrWastedRightClicks;
	CString mStrWastedDoubleClicks;
	CString mStrOneAndHalfClicks;
	CString mStrWastedFlags;
	CString mStrUnflags;
	CString mStrMisflags;
	CString mStrMisunflags;
};

