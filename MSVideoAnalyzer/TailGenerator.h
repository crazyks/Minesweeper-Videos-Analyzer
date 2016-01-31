#pragma once
#include "afxwin.h"


// CTailGenerator �Ի���

class CTailGenerator : public CDialogEx
{
	DECLARE_DYNAMIC(CTailGenerator)

public:
	CTailGenerator(CWnd* pParent = NULL, CString* pTail = NULL);   // ��׼���캯��
	virtual ~CTailGenerator();

// �Ի�������
	enum { IDD = IDD_DIALOG_GEN_TAIL };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

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
