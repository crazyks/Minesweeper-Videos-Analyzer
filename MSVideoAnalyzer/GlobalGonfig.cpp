#include "StdAfx.h"
#include "GlobalGonfig.h"


CGlobalGonfig::CGlobalGonfig(void)
{
	CIniFile iniFile(CONFIG_FILE);
	iniFile.GetProfileString(MAIN_SECTION_NAME, KEY_LAST_DIR, m_szLastDir);
	m_bUseFastParser = iniFile.GetProfileInt(MAIN_SECTION_NAME, KEY_FAST_PARSER, TRUE);
	m_bScanSubDirs = iniFile.GetProfileInt(MAIN_SECTION_NAME, KEY_SCAN_SUB_DIRS, FALSE);
	m_unConfigIndex = iniFile.GetProfileInt(CONFIG_SECTION_NAME, KEY_CONFIG_INDEX);
	if (m_unConfigIndex >= MAX_CONFIG_NUM) m_unConfigIndex = 0;
	CString keyname;
	for (unsigned int i = 0; i != MAX_CONFIG_NUM; i++)
	{
		keyname.Format(KEY_CONFIG_TAIL_FORMAT, i);
		iniFile.GetProfileString(CONFIG_SECTION_NAME, keyname, m_arrayConfig[i].m_szMatchTail);
		keyname.Format(KEY_CONFIG_BEG_NUM_FORMAT, i);
		m_arrayConfig[i].m_unBegNum = iniFile.GetProfileInt(CONFIG_SECTION_NAME, keyname, 10);
		keyname.Format(KEY_CONFIG_INT_NUM_FORMAT, i);
		m_arrayConfig[i].m_unIntNum = iniFile.GetProfileInt(CONFIG_SECTION_NAME, keyname, 3);
		keyname.Format(KEY_CONFIG_EXP_NUM_FORMAT, i);
		m_arrayConfig[i].m_unExpNum = iniFile.GetProfileInt(CONFIG_SECTION_NAME, keyname, 2);
		keyname.Format(KEY_CONFIG_EVENTS_FORMAT, i);
		m_arrayConfig[i].m_unEvents = iniFile.GetProfileInt(CONFIG_SECTION_NAME, keyname, (EVENT_STYLE_FL | EVENT_STYLE_NF));
	}
	iniFile.GetProfileString(CONFIG_SECTION_NAME, KEY_MAGIC_STRING, m_szMagic);
	iniFile.GetProfileString(CONFIG_SECTION_NAME, KEY_NAME_LIST_DIR, m_szNameListDir);
}


CGlobalGonfig::~CGlobalGonfig(void)
{
}

void CGlobalGonfig::Save(void)
{
	CIniFile iniFile(CONFIG_FILE);
	iniFile.SetProfileString(MAIN_SECTION_NAME, KEY_LAST_DIR, m_szLastDir);
	iniFile.SetProfileInt(MAIN_SECTION_NAME, KEY_FAST_PARSER, m_bUseFastParser);
	iniFile.SetProfileInt(MAIN_SECTION_NAME, KEY_SCAN_SUB_DIRS, m_bScanSubDirs);
	iniFile.SetProfileInt(CONFIG_SECTION_NAME, KEY_CONFIG_INDEX, m_unConfigIndex);
	CString keyname;
	for (unsigned int i = 0; i != MAX_CONFIG_NUM; i++)
	{
		keyname.Format(KEY_CONFIG_TAIL_FORMAT, i);
		iniFile.SetProfileString(CONFIG_SECTION_NAME, keyname, m_arrayConfig[i].m_szMatchTail);
		keyname.Format(KEY_CONFIG_BEG_NUM_FORMAT, i);
		iniFile.SetProfileInt(CONFIG_SECTION_NAME, keyname, m_arrayConfig[i].m_unBegNum);
		keyname.Format(KEY_CONFIG_INT_NUM_FORMAT, i);
		iniFile.SetProfileInt(CONFIG_SECTION_NAME, keyname, m_arrayConfig[i].m_unIntNum);
		keyname.Format(KEY_CONFIG_EXP_NUM_FORMAT, i);
		iniFile.SetProfileInt(CONFIG_SECTION_NAME, keyname, m_arrayConfig[i].m_unExpNum);
		keyname.Format(KEY_CONFIG_EVENTS_FORMAT, i);
		iniFile.SetProfileInt(CONFIG_SECTION_NAME, keyname, m_arrayConfig[i].m_unEvents);
	}
	iniFile.SetProfileString(CONFIG_SECTION_NAME, KEY_MAGIC_STRING, m_szMagic);
	iniFile.SetProfileString(CONFIG_SECTION_NAME, KEY_NAME_LIST_DIR, m_szNameListDir);
}
