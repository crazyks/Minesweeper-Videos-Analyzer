#pragma once
#include "afxwin.h"


// CTailGenerator 对话框

class CTailGenerator : public CDialogEx
{
	DECLARE_DYNAMIC(CTailGenerator)

public:
	CTailGenerator(CWnd* pParent = NULL, CString* pTail = NULL);   // 标准构造函数
	virtual ~CTailGenerator();

// 对话框数据
	enum { IDD = IDD_DIALOG_GEN_TAIL };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
private:
	CEdit m_MagicString;
	CEdit m_NameList;
	CString* m_pTail;
	CMSVideoAnalyzerApp* m_pTheApp;
private:
	void Gen();
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedButtonBrowseList();
	afx_msg void OnBnClickedOk();
};
