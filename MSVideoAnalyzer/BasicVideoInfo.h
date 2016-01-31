#pragma once
class CBasicVideoInfo
{
public:
	CBasicVideoInfo(void);
	~CBasicVideoInfo(void);
	CString GetFile();
	CString GetProgramVersion();
	CString GetLevel();

public:
	CString mStrPath;
	CString mStrParser;
	CString mStrProgram;
	CString mStrVersion;
	CString mStrPlayer;
	CString mStrLevel;
	CString mStrWidth;
	CString mStrHeight;
	CString mStrMines;
	CString mStrMode;
	CString mStrTime;
	CString mStrTimestamp;
	CString mStr3BV;
	CString mStrSolv3BV;
	CString mStr3BVs;
private:
	CString GetFileTitleFromFileName(const CString FileName, CString& ExtendName);
};

