#include "StdAfx.h"
#include "ConfigItem.h"

CConfigItem::CConfigItem(void)
	: m_szMatchTail(_T(""))
	, m_unBegNum(10)
	, m_unIntNum(3)
	, m_unExpNum(2)
	, m_unEvents(EVENT_STYLE_FL | EVENT_STYLE_NF)
{
}

CConfigItem::~CConfigItem(void)
{
}