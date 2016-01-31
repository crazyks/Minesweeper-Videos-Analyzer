#include "StdAfx.h"
#include "BasicVideoInfo.h"


CBasicVideoInfo::CBasicVideoInfo(void)
	: mStrPath(_T(""))
	, mStrParser(_T(""))
	, mStrProgram(_T(""))
	, mStrVersion(_T(""))
	, mStrPlayer(_T(""))
	, mStrLevel(_T(""))
	, mStrWidth(_T(""))
	, mStrHeight(_T(""))
	, mStrMines(_T(""))
	, mStrMode(_T(""))
	, mStrTime(_T(""))
	, mStrTimestamp(_T(""))
	, mStr3BV(_T(""))
	, mStrSolv3BV(_T(""))
	, mStr3BVs(_T(""))
{
}


CBasicVideoInfo::~CBasicVideoInfo(void)
{
}


CString CBasicVideoInfo::GetFile()
{
	CString name, title, ext;
	title = GetFileTitleFromFileName(mStrPath, ext);
	name.Format(_T("%s.%s"), title, ext);
	return name;
}


CString CBasicVideoInfo::GetProgramVersion()
{
	CString ver;
	ver.Format(_T("%s %s"), mStrProgram, mStrVersion);
	return ver;
}


CString CBasicVideoInfo::GetLevel()
{
	if (mStrLevel.Compare(_T("")) == 0) return mStrLevel;
	CString level;
	level.Format(_T("%s(%s|%s|%s)"), mStrLevel, mStrWidth, mStrHeight, mStrMines);
	return level;
}


CString CBasicVideoInfo::GetFileTitleFromFileName(const CString FileName, CString& ExtendName)
{
	int Where;
	Where=FileName.ReverseFind('\\');
	if(Where==-1)
	{
		Where=FileName.ReverseFind('/');
	}
	CString FileTitle=FileName.Right(FileName.GetLength()-1-Where);
	int Which=FileTitle.ReverseFind('.');
	ExtendName=FileTitle.Right(FileTitle.GetLength()-Which-1);
	if (Which!=-1)
	{
		FileTitle=FileTitle.Left(Which);
	}
	return FileTitle;
}
