#pragma once
#include "IniFile.h"
#include "ConfigItem.h"

#define CONFIG_FILE                _T("config.ini")
#define MAIN_SECTION_NAME          _T("MSVideoAnalyzer")
#define KEY_LAST_DIR               _T("LastDir")
#define KEY_FAST_PARSER            _T("FastParser")
#define KEY_SCAN_SUB_DIRS          _T("ScanSubDirs")
#define CONFIG_SECTION_NAME        _T("Competition")
#define KEY_CONFIG_INDEX           _T("ConfigIndex")
#define KEY_CONFIG_BEG_NUM_FORMAT  _T("BegNumber%d")
#define KEY_CONFIG_INT_NUM_FORMAT  _T("IntNumber%d")
#define KEY_CONFIG_EXP_NUM_FORMAT  _T("ExpNumber%d")
#define KEY_CONFIG_EVENTS_FORMAT   _T("EventsID%d")
#define KEY_CONFIG_TAIL_FORMAT     _T("MatchTail%d")
#define KEY_MAGIC_STRING           _T("Magic")
#define KEY_NAME_LIST_DIR          _T("PlayerNameList")

#define MAX_CONFIG_NUM             (8)

class CGlobalGonfig
{
public:
	static CGlobalGonfig &GetInstance()
	{
		static CGlobalGonfig instance;
		return instance;
	}
	void Save(void);
private:
	CGlobalGonfig(void);
	~CGlobalGonfig(void);
public:
	CString m_szLastDir;
	BOOL m_bUseFastParser;
	BOOL m_bScanSubDirs;
	CString m_szMagic;
	CString m_szNameListDir;
	unsigned int m_unConfigIndex;
	CConfigItem m_arrayConfig[MAX_CONFIG_NUM];
};

