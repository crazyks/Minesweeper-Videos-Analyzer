#pragma once
#include "AdvancedVideoInfo.h"
#include <vector>
using std::vector;

class CVideoInfoBuilder
{
private:
	CVideoInfoBuilder(void);
	~CVideoInfoBuilder(void);

public:
	static CVideoInfoBuilder &GetInstance()
	{
		static CVideoInfoBuilder instance;
		return instance;
	}
	CAdvancedVideoInfo Create(const CString source);

private:
	void Split(const CString src, const CString token, vector<CString>& vect);
};

