// ProgressBar.cpp : 实现文件
//

#include "stdafx.h"
#include "MSVideoAnalyzer.h"
#include "ProgressBar.h"
#include "afxdialogex.h"
#include "afxmt.h"

CMutex gMutex;


// CProgressBar 对话框

IMPLEMENT_DYNAMIC(CProgressBar, CDialogEx)

CProgressBar::CProgressBar(CWnd* pParent /*=NULL*/, vector<CString>* pPathData /*= NULL*/, vector<CAdvancedVideoInfo>* pData /*= NULL*/)
	: CDialogEx(CProgressBar::IDD, pParent)
	, m_pPathData(pPathData)
	, m_pData(pData)
{
}

CProgressBar::~CProgressBar()
{
}

void CProgressBar::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PROGRESS_BAR, m_Progress);
	DDX_Control(pDX, IDC_PROGRESS_INFO, m_Percent);
	DDX_Control(pDX, IDC_BUTTON_CLOSE, m_button);
	DDX_Control(pDX, IDC_PROGRESS_SPEED, m_Speed);
	DDX_Control(pDX, IDC_PROGRESS_REMAINING, m_Remaining);
}


BEGIN_MESSAGE_MAP(CProgressBar, CDialogEx)
	ON_BN_CLICKED(IDOK, &CProgressBar::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CProgressBar::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_BUTTON_CLOSE, &CProgressBar::OnBnClickedButtonClose)
	ON_WM_CLOSE()
END_MESSAGE_MAP()


// CProgressBar 消息处理程序


void CProgressBar::OnBnClickedOk()
{
}


void CProgressBar::OnBnClickedCancel()
{
}


BOOL CProgressBar::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	m_Progress.SetRange32(0, m_pPathData->size());
	m_Progress.SetPos(0);
	m_Percent.SetWindowTextA(_T("0%"));
	CVideoParser::RegisterCallback(this);

	m_StartTime = GetTickCount();
	Start();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


void CProgressBar::OnNotify(CAdvancedVideoInfo& info)
{
	gMutex.Lock();
	m_pData->push_back(info);
	m_Progress.SetPos(m_pData->size());
	CString title, format;
	format.LoadString(IDS_TEXT_ANALYSIS_TITLE);
	title.Format(format, m_pData->size());
	SetWindowText(title);
	CString percent;
	percent.Format(_T("%d%%"), m_pData->size() * 100 / m_pPathData->size());
	m_Percent.SetWindowTextA(percent);
	double speed = m_pData->size() * 1000.0 / (GetTickCount() - m_StartTime);
	m_Speed.SetWindowText(FormatSpeed(speed));
	DWORD remain = (DWORD) ((m_pPathData->size() - m_pData->size()) / speed);
	m_Remaining.SetWindowText(FormatTime(remain));
	gMutex.Unlock();
}


void CProgressBar::OnCancelled()
{
	PostMessage(WM_CLOSE, NULL, NULL);
}


void CProgressBar::OnDismissed()
{
	PostMessage(WM_CLOSE, NULL, NULL);
}


void CProgressBar::Start()
{
	CVideoParser::Run(*m_pPathData);
}


void CProgressBar::OnBnClickedButtonClose()
{
	m_button.EnableWindow(FALSE);
	m_button.SetWindowText(_T("Cancelling..."));
	CVideoParser::Stop();
}


void CProgressBar::OnClose()
{
	CDialogEx::OnOK();

	__super::OnClose();
}


CString CProgressBar::FormatSpeed(const double speed)
{
	CString str;
	str.Format(_T("%d"), (int) speed);
	return str;
}


CString CProgressBar::FormatTime(const DWORD sec)
{
	DWORD second = sec;
	int day = 0, h = 0, m = 0, s = 0;
	day = second / 86400; second %= 86400;
	h = second / 3600; second %= 3600;
	m = second / 60; s = second %= 60;
	CString time;
	if (day == 0) time.Format(_T("%02d:%02d:%02d"), h, m, s);
	else if (day == 1) time.Format(_T("%d Day %02d:%02d:%02d"), day, h, m, s);
	else if (day > 365) time.Format(_T("Over 1 year..."));
	else time.Format(_T("%d Days %02d:%02d:%02d"), day, h, m, s);
	return time;
}
