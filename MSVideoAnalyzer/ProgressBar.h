#pragma once
#include "afxcmn.h"
#include "afxwin.h"
#include "ICallback.h"
#include "AdvancedVideoInfo.h"
#include "VideoParser.h"
#include <vector>
using std::vector;


// CProgressBar 对话框

class CProgressBar : public CDialogEx, public ICallback
{
	DECLARE_DYNAMIC(CProgressBar)

public:
	CProgressBar(CWnd* pParent = NULL, vector<CString>* pPathData = NULL, vector<CAdvancedVideoInfo>* pData = NULL);   // 标准构造函数
	virtual ~CProgressBar();

// 对话框数据
	enum { IDD = IDD_DIALOG_PROGRESS };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
private:
	CProgressCtrl m_Progress;
	CStatic m_Percent;
	vector<CString>* m_pPathData;
	vector<CAdvancedVideoInfo>* m_pData;
public:
	virtual BOOL OnInitDialog();
	void OnNotify(CAdvancedVideoInfo& info);
	void OnCancelled();
	void OnDismissed();
private:
	void Start();
	CButton m_button;
public:
	afx_msg void OnBnClickedButtonClose();
	afx_msg void OnClose();
private:
	CStatic m_Speed;
	CStatic m_Remaining;
	DWORD m_StartTime;
	CString FormatSpeed(const double speed);
	CString FormatTime(const DWORD sec);
};
