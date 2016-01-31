#pragma once
#include "afxwin.h"


// CCompetitionSettings 对话框

class CCompetitionSettings : public CDialogEx
{
	DECLARE_DYNAMIC(CCompetitionSettings)

public:
	CCompetitionSettings(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CCompetitionSettings();

// 对话框数据
	enum { IDD = IDD_DIALOG_SETUP_GAME };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
private:
	CMSVideoAnalyzerApp* m_pTheApp;
	CComboBox m_Templets;
	CButton m_BtnSaveTemplet;
	CEdit m_StandardBegCount;
	CEdit m_StandardIntCount;
	CEdit m_StandardExpCount;
	CButton m_CheckFL;
	CButton m_CheckNF;
	CButton m_CheckTotalTime;
	CButton m_CheckTotalTimeBeg;
	CButton m_CheckTotalTimeInt;
	CButton m_CheckTotalTimeExp;
	CButton m_CheckTotalTimeAll;
	CButton m_CheckTotal3BVs;
	CButton m_CheckTotal3BVsBeg;
	CButton m_CheckTotal3BVsInt;
	CButton m_CheckTotal3BVsExp;
	CButton m_CheckTotal3BVsAll;
	CButton m_CheckBestTime;
	CButton m_CheckBestTimeBeg;
	CButton m_CheckBestTimeInt;
	CButton m_CheckBestTimeExp;
	CButton m_CheckBestTimeAll;
	CButton m_CheckBest3BVs;
	CButton m_CheckBest3BVsBeg;
	CButton m_CheckBest3BVsInt;
	CButton m_CheckBest3BVsExp;
	CButton m_CheckBest3BVsAll;
	CButton m_CheckBestCount;
	CButton m_CheckBestCountBeg;
	CButton m_CheckBestCountInt;
	CButton m_CheckBestCountExp;
	CButton m_CheckBestCountAll;
	CEdit m_MatchTail;
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedButtonSaveTemplet();
	afx_msg void OnCbnSelchangeComboTemplet();
private:
	void LoadConfiguration(unsigned int index);
public:
	afx_msg void OnBnClickedCheckTotalTime();
	afx_msg void OnBnClickedCheckTotal3bvs();
	afx_msg void OnBnClickedCheckBestTime();
	afx_msg void OnBnClickedCheckBest3bvs();
	afx_msg void OnBnClickedCheckBestCount();
	afx_msg void OnBnClickedButtonGenTail();
};
