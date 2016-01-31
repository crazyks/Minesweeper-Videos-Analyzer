
// MSVideoAnalyzer.h : main header file for the MSVideoAnalyzer application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols
#include "GlobalGonfig.h"


// CMSVideoAnalyzerApp:
// See MSVideoAnalyzer.cpp for the implementation of this class
//

class CMSVideoAnalyzerApp : public CWinAppEx
{
public:
	CMSVideoAnalyzerApp();


// Overrides
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// Implementation
	BOOL  m_bHiColorIcons;

	virtual void PreLoadState();
	virtual void LoadCustomState();
	virtual void SaveCustomState();

	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
private:
	CGlobalGonfig& m_sGlobalGonfig;
public:
	CGlobalGonfig& GetGlobalGonfig(void);
};

extern CMSVideoAnalyzerApp theApp;
