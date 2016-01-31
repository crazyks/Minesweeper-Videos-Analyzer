#include "StdAfx.h"
#include "VideoInfoBuilder.h"


CVideoInfoBuilder::CVideoInfoBuilder(void)
{
}


CVideoInfoBuilder::~CVideoInfoBuilder(void)
{
}


CAdvancedVideoInfo CVideoInfoBuilder::Create(const CString source)
{
	vector<CString> lines;
	CString key, value;
	Split(source, _T("\n"), lines);
	CAdvancedVideoInfo info;
	for (vector<CString>::iterator it = lines.begin(); it != lines.end(); ++it)
	{
		int index = (*it).Find(_T(": "));
		if (index < 0) continue;
		key = (*it).Left(index);
		value = (*it).Right((*it).GetLength() - 2 - index);
		value.Remove(0x0d);
		value.Remove(0x09);
		if (key.CompareNoCase(_T("RawVF_Version")) == 0) info.mStrParser = value;
		else if (key.CompareNoCase(_T("Program")) == 0) info.mStrProgram = value;
		else if (key.CompareNoCase(_T("Version")) == 0) info.mStrVersion = value;
		else if (key.CompareNoCase(_T("Player")) == 0) info.mStrPlayer = value;
		else if (key.CompareNoCase(_T("Level")) == 0) info.mStrLevel = value;
		else if (key.CompareNoCase(_T("Width")) == 0) info.mStrWidth = value;
		else if (key.CompareNoCase(_T("Height")) == 0) info.mStrHeight = value;
		else if (key.CompareNoCase(_T("Mines")) == 0) info.mStrMines = value;
		else if (key.CompareNoCase(_T("Mode")) == 0) info.mStrMode = value;
		else if (key.CompareNoCase(_T("Time")) == 0) info.mStrTime = value;
		else if (key.CompareNoCase(_T("Timestamp")) == 0) info.mStrTimestamp = value;
		else if (key.CompareNoCase(_T("3BV")) == 0) info.mStr3BV = value;
		else if (key.CompareNoCase(_T("Solved3BV")) == 0) info.mStrSolv3BV = value;
		else if (key.CompareNoCase(_T("3BV/s")) == 0) info.mStr3BVs = value;
		else if (key.CompareNoCase(_T("ZiNi")) == 0) info.mStrZiNi = value;
		else if (key.CompareNoCase(_T("ZiNi/s")) == 0) info.mStrZiNis = value;
		else if (key.CompareNoCase(_T("HZiNi")) == 0) info.mStrHZiNi = value;
		else if (key.CompareNoCase(_T("HZiNi/s")) == 0) info.mStrHZiNis = value;
		else if (key.CompareNoCase(_T("Clicks")) == 0) info.mStrClicks = value;
		else if (key.CompareNoCase(_T("Clicks/s")) == 0) info.mStrClicksPers = value;
		else if (key.CompareNoCase(_T("LeftClicks")) == 0) info.mStrLeftClicks = value;
		else if (key.CompareNoCase(_T("LeftClicks/s")) == 0) info.mStrLeftClicksPers = value;
		else if (key.CompareNoCase(_T("RightClicks")) == 0) info.mStrRightClicks = value;
		else if (key.CompareNoCase(_T("RightClicks/s")) == 0) info.mStrRightClicksPers = value;
		else if (key.CompareNoCase(_T("DoubleClicks")) == 0) info.mStrDoubleClicks = value;
		else if (key.CompareNoCase(_T("DoubleClicks/s")) == 0) info.mStrDoubleClicksPers = value;
		else if (key.CompareNoCase(_T("WastedClicks")) == 0) info.mStrWastedClicks = value;
		else if (key.CompareNoCase(_T("WastedLeftClicks")) == 0) info.mStrWastedLeftClicks = value;
		else if (key.CompareNoCase(_T("WastedRightClicks")) == 0) info.mStrWastedRightClicks = value;
		else if (key.CompareNoCase(_T("WastedDoubleClicks")) == 0) info.mStrWastedDoubleClicks = value;
		else if (key.CompareNoCase(_T("1.5Clicks")) == 0) info.mStrOneAndHalfClicks = value;
		else if (key.CompareNoCase(_T("IOE")) == 0) info.mStrIOE = value;
		else if (key.CompareNoCase(_T("Correctness")) == 0) info.mStrCorrectness = value;
		else if (key.CompareNoCase(_T("Openings")) == 0) info.mStrOpenings = value;
		else if (key.CompareNoCase(_T("Islands")) == 0) info.mIslands = value;
		else if (key.CompareNoCase(_T("Flags")) == 0) info.mFlags = value;
		else if (key.CompareNoCase(_T("WastedFlags")) == 0) info.mStrWastedFlags = value;
		else if (key.CompareNoCase(_T("Unflags")) == 0) info.mStrUnflags = value;
		else if (key.CompareNoCase(_T("Misflags")) == 0) info.mStrMisflags = value;
		else if (key.CompareNoCase(_T("Misunflags")) == 0) info.mStrMisunflags = value;
		else continue;
	}
	return info;
}


void CVideoInfoBuilder::Split(const CString src, const CString token, vector<CString>& vect)
{
	vect.clear();
	int nend=0;
	int nbegin=0;
	while(nend != -1)
	{
		nend = src.Find(token, nbegin);
		if(nend == -1)
		{
			vect.push_back(src.Mid(nbegin, src.GetLength() - nbegin));
		}
		else
		{
			vect.push_back(src.Mid(nbegin, nend - nbegin));
		}
		nbegin = nend + 1;
	}
}
