#pragma once
#include "AdvancedVideoInfo.h"

class ICallback
{
public:
	virtual void OnNotify(CAdvancedVideoInfo& info) = 0;
	virtual void OnCancelled() = 0;
	virtual void OnDismissed() = 0;
};

