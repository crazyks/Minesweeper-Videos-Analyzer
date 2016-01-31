
// MSVideoAnalyzerView.h : interface of the CMSVideoAnalyzerView class
//

#pragma once
#include "AdvancedVideoInfo.h"
#include "VideoParser.h"
#include "Player.h"
#include <algorithm>
#include <functional>
#include <vector>
#include <map>
#include <set>

using std::vector;
using std::map;
using std::set;
using std::sort;

typedef enum _COLUMNINDEX
{
	COLUMN_INDEX_NUMBER = 0,
	COLUMN_INDEX_FILE,
	COLUMN_INDEX_PROGRAM,
	COLUMN_INDEX_PLAYER,
	COLUMN_INDEX_TIMESTAMP,
	COLUMN_INDEX_LEVEL,
	COLUMN_INDEX_MODE,
	COLUMN_INDEX_TIME,
	COLUMN_INDEX_3BV,
	COLUMN_INDEX_SOLV,
	COLUMN_INDEX_3BVS,
	COLUMN_INDEX_ZINI,
	COLUMN_INDEX_ZINIS,
	COLUMN_INDEX_CLICK,
	COLUMN_INDEX_CLICKS,
	COLUMN_INDEX_LCLICK,
	COLUMN_INDEX_RCLICK,
	COLUMN_INDEX_DCLICK,
	COLUMN_INDEX_IOE,
	COLUMN_INDEX_FLAGS,
	COLUMN_INDEX_OPENINGS,
	COLUMN_INDEX_ISLANDS,
	COLUMN_INDEX_MAX,
} COLUMNINDEX;


class CMSVideoAnalyzerView : public CListView
{
protected: // create from serialization only
	CMSVideoAnalyzerView();
	DECLARE_DYNCREATE(CMSVideoAnalyzerView)

// Attributes
public:
	CMSVideoAnalyzerDoc* GetDocument() const;

// Operations
public:

// Overrides
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual void OnInitialUpdate(); // called first time after construct

// Implementation
public:
	virtual ~CMSVideoAnalyzerView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	afx_msg void OnFilePrintPreview();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBrowse();
	afx_msg void OnExport();
	void Refresh();
private:
	void Refresh(vector<CAdvancedVideoInfo>& data);
	void Refresh(vector<CPlayer>& players);
	void InsertRow(int row, CAdvancedVideoInfo& info);
	void InsertRow(int row, CPlayer& info, unsigned int events);
	void ListFiles(const CString dir, vector<CString>& list, BOOL scanSubDir = TRUE);
	vector<int> GetSelection();
	void GetCellName(const int row, const int columnOffset, CString& columnName);
	void Error(UINT msgId);
	void SaveAsFakeXls(CString& saveTo, CListCtrl& list);
	LPITEMIDLIST GetItemIDListFromFilePath(CString &strFilePath);
	static INT CALLBACK BrowseCallbackProc(IN HWND hWnd, IN UINT uMsg, IN LPARAM lParam, IN LPARAM lpData);
	void DoSummary(void);
	void ClearScoreMap(map<CString, vector<CScore>>& theMap);
	void SortScoreMap(map<CString, vector<CScore>>& theMap, BOOL bAsc = TRUE);
	double GetSumScore(vector<CScore>& scores, BOOL countFL, BOOL countNF, UINT count, double defaultScore);
	double GetBestScore(vector<CScore>& scores, BOOL countFL, BOOL countNF, double defaultScore);
	unsigned int GetCountScore(vector<CScore>& scores, BOOL countFL, BOOL countNF);
private:
	int m_anColumnWidths[COLUMN_INDEX_MAX];
	BOOL m_bColumnInitialized;
	vector<CAdvancedVideoInfo> mData;
	vector<CPlayer> mPlayers;
	BOOL m_bDataHandled;
	BOOL m_bSummaryHandled;
	BOOL m_bIsSummaryMode;
	set<CString> mMD5Set;
public:
	afx_msg void OnNMDblclk(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnSetupCompetition();
	afx_msg void OnPlay();
	afx_msg void OnShowVideoList();
	afx_msg void OnShowCompetitionSummary();
	afx_msg void OnUpdateToolPlay(CCmdUI *pCmdUI);
	afx_msg void OnToolFastparser();
	afx_msg void OnToolScansubdir();
	afx_msg void OnUpdateToolFastparser(CCmdUI *pCmdUI);
	afx_msg void OnUpdateToolScansubdir(CCmdUI *pCmdUI);
};

#ifndef _DEBUG  // debug version in MSVideoAnalyzerView.cpp
inline CMSVideoAnalyzerDoc* CMSVideoAnalyzerView::GetDocument() const
   { return reinterpret_cast<CMSVideoAnalyzerDoc*>(m_pDocument); }
#endif

