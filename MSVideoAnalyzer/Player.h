#pragma once
#include "Score.h"

typedef enum _EVENT_TYPE
{
	EVENT_TYPE_TOTAL_TIME_BEG = 0,
	EVENT_TYPE_TOTAL_TIME_INT,
	EVENT_TYPE_TOTAL_TIME_EXP,
	EVENT_TYPE_TOTAL_TIME_ALL,
	EVENT_TYPE_TOTAL_3BVS_BEG,
	EVENT_TYPE_TOTAL_3BVS_INT,
	EVENT_TYPE_TOTAL_3BVS_EXP,
	EVENT_TYPE_TOTAL_3BVS_ALL,
	EVENT_TYPE_BEST_TIME_BEG,
	EVENT_TYPE_BEST_TIME_INT,
	EVENT_TYPE_BEST_TIME_EXP,
	EVENT_TYPE_BEST_TIME_ALL,
	EVENT_TYPE_BEST_3BVS_BEG,
	EVENT_TYPE_BEST_3BVS_INT,
	EVENT_TYPE_BEST_3BVS_EXP,
	EVENT_TYPE_BEST_3BVS_ALL,
	EVENT_TYPE_BEST_COUNT_BEG,
	EVENT_TYPE_BEST_COUNT_INT,
	EVENT_TYPE_BEST_COUNT_EXP,
	EVENT_TYPE_BEST_COUNT_ALL,
	EVENT_COUNT,
} EVENT_TYPE;

class CPlayer
{
public:
	CPlayer(void);
	~CPlayer(void);
public:
	CString m_szName;
	CScore m_asScores[EVENT_COUNT];
};
