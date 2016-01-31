// CompetitionSettings.cpp : 实现文件
//

#include "stdafx.h"
#include "MSVideoAnalyzer.h"
#include "CompetitionSettings.h"
#include "afxdialogex.h"
#include "Formatter.h"
#include "TailGenerator.h"


// CCompetitionSettings 对话框

IMPLEMENT_DYNAMIC(CCompetitionSettings, CDialogEx)

CCompetitionSettings::CCompetitionSettings(CWnd* pParent /*=NULL*/)
	: CDialogEx(CCompetitionSettings::IDD, pParent)
	, m_pTheApp((CMSVideoAnalyzerApp*) AfxGetApp())
{
}

CCompetitionSettings::~CCompetitionSettings()
{
}

void CCompetitionSettings::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_TEMPLET, m_Templets);
	DDX_Control(pDX, IDC_BUTTON_SAVE_TEMPLET, m_BtnSaveTemplet);
	DDX_Control(pDX, IDC_EDIT_BEG_COUNT, m_StandardBegCount);
	DDX_Control(pDX, IDC_EDIT_INT_COUNT, m_StandardIntCount);
	DDX_Control(pDX, IDC_EDIT_EXP_COUNT, m_StandardExpCount);
	DDX_Control(pDX, IDC_CHECK_STYLE_FL, m_CheckFL);
	DDX_Control(pDX, IDC_CHECK_STYLE_NF, m_CheckNF);
	DDX_Control(pDX, IDC_CHECK_TOTAL_TIME, m_CheckTotalTime);
	DDX_Control(pDX, IDC_CHECK_TOTAL_TIME_BEG, m_CheckTotalTimeBeg);
	DDX_Control(pDX, IDC_CHECK_TOTAL_TIME_INT, m_CheckTotalTimeInt);
	DDX_Control(pDX, IDC_CHECK_TOTAL_TIME_EXP, m_CheckTotalTimeExp);
	DDX_Control(pDX, IDC_CHECK_TOTAL_TIME_ALL, m_CheckTotalTimeAll);
	DDX_Control(pDX, IDC_CHECK_TOTAL_3BVS, m_CheckTotal3BVs);
	DDX_Control(pDX, IDC_CHECK_TOTAL_3BVS_BEG, m_CheckTotal3BVsBeg);
	DDX_Control(pDX, IDC_CHECK_TOTAL_3BVS_INT, m_CheckTotal3BVsInt);
	DDX_Control(pDX, IDC_CHECK_TOTAL_3BVS_EXP, m_CheckTotal3BVsExp);
	DDX_Control(pDX, IDC_CHECK_TOTAL_3BVS_ALL, m_CheckTotal3BVsAll);
	DDX_Control(pDX, IDC_CHECK_BEST_TIME, m_CheckBestTime);
	DDX_Control(pDX, IDC_CHECK_BEST_TIME_BEG, m_CheckBestTimeBeg);
	DDX_Control(pDX, IDC_CHECK_BEST_TIME_INT, m_CheckBestTimeInt);
	DDX_Control(pDX, IDC_CHECK_BEST_TIME_EXP, m_CheckBestTimeExp);
	DDX_Control(pDX, IDC_CHECK_BEST_TIME_ALL, m_CheckBestTimeAll);
	DDX_Control(pDX, IDC_CHECK_BEST_3BVS, m_CheckBest3BVs);
	DDX_Control(pDX, IDC_CHECK_BEST_3BVS_BEG, m_CheckBest3BVsBeg);
	DDX_Control(pDX, IDC_CHECK_BEST_3BVS_INT, m_CheckBest3BVsInt);
	DDX_Control(pDX, IDC_CHECK_BEST_3BVS_EXP, m_CheckBest3BVsExp);
	DDX_Control(pDX, IDC_CHECK_BEST_3BVS_ALL, m_CheckBest3BVsAll);
	DDX_Control(pDX, IDC_CHECK_BEST_COUNT, m_CheckBestCount);
	DDX_Control(pDX, IDC_CHECK_BEST_COUNT_BEG, m_CheckBestCountBeg);
	DDX_Control(pDX, IDC_CHECK_BEST_COUNT_INT, m_CheckBestCountInt);
	DDX_Control(pDX, IDC_CHECK_BEST_COUNT_EXP, m_CheckBestCountExp);
	DDX_Control(pDX, IDC_CHECK_BEST_COUNT_ALL, m_CheckBestCountAll);
	DDX_Control(pDX, IDC_EDIT_MATCH_TAIL, m_MatchTail);
}


BEGIN_MESSAGE_MAP(CCompetitionSettings, CDialogEx)
	ON_BN_CLICKED(IDOK, &CCompetitionSettings::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CCompetitionSettings::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_BUTTON_SAVE_TEMPLET, &CCompetitionSettings::OnBnClickedButtonSaveTemplet)
	ON_CBN_SELCHANGE(IDC_COMBO_TEMPLET, &CCompetitionSettings::OnCbnSelchangeComboTemplet)
	ON_BN_CLICKED(IDC_CHECK_TOTAL_TIME, &CCompetitionSettings::OnBnClickedCheckTotalTime)
	ON_BN_CLICKED(IDC_CHECK_TOTAL_3BVS, &CCompetitionSettings::OnBnClickedCheckTotal3bvs)
	ON_BN_CLICKED(IDC_CHECK_BEST_TIME, &CCompetitionSettings::OnBnClickedCheckBestTime)
	ON_BN_CLICKED(IDC_CHECK_BEST_3BVS, &CCompetitionSettings::OnBnClickedCheckBest3bvs)
	ON_BN_CLICKED(IDC_CHECK_BEST_COUNT, &CCompetitionSettings::OnBnClickedCheckBestCount)
	ON_BN_CLICKED(IDC_BUTTON_GEN_TAIL, &CCompetitionSettings::OnBnClickedButtonGenTail)
END_MESSAGE_MAP()


// CCompetitionSettings 消息处理程序


void CCompetitionSettings::OnBnClickedOk()
{
	OnBnClickedButtonSaveTemplet();
	CGlobalGonfig& config = m_pTheApp->GetGlobalGonfig();
	config.m_unConfigIndex = m_Templets.GetCurSel();
	CDialogEx::OnOK();
}


void CCompetitionSettings::OnBnClickedCancel()
{
	CDialogEx::OnCancel();
}


BOOL CCompetitionSettings::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	CGlobalGonfig& config = m_pTheApp->GetGlobalGonfig();
	m_Templets.ResetContent();
	CString name, fmt;
	fmt.LoadString(IDS_CONFIG_NAME_FORMAT);
	for (unsigned int i = 0; i != MAX_CONFIG_NUM; i++)
	{
		name.Format(fmt, i + 1);
		m_Templets.AddString(name);
	}
	m_Templets.SetCurSel(config.m_unConfigIndex);
	config.m_unConfigIndex = m_Templets.GetCurSel();
	LoadConfiguration(config.m_unConfigIndex);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


void CCompetitionSettings::OnBnClickedButtonSaveTemplet()
{
	CGlobalGonfig& config = m_pTheApp->GetGlobalGonfig();
	CConfigItem& item = config.m_arrayConfig[m_Templets.GetCurSel()];
	CString value;
	m_StandardBegCount.GetWindowText(value);
	item.m_unBegNum = CFormatter::CString2UINT(value);
	m_StandardIntCount.GetWindowText(value);
	item.m_unIntNum = CFormatter::CString2UINT(value);
	m_StandardExpCount.GetWindowText(value);
	item.m_unExpNum = CFormatter::CString2UINT(value);
	item.m_unEvents = 0;
	if (m_CheckFL.GetCheck()) item.m_unEvents |= EVENT_STYLE_FL;
	if (m_CheckNF.GetCheck()) item.m_unEvents |= EVENT_STYLE_NF;
	if (m_CheckTotalTime.GetCheck()) item.m_unEvents |= EVENT_TOTAL_TIME;
	if (m_CheckTotalTimeBeg.GetCheck()) item.m_unEvents |= EVENT_TOTAL_TIME_BEG;
	if (m_CheckTotalTimeInt.GetCheck()) item.m_unEvents |= EVENT_TOTAL_TIME_INT;
	if (m_CheckTotalTimeExp.GetCheck()) item.m_unEvents |= EVENT_TOTAL_TIME_EXP;
	if (m_CheckTotalTimeAll.GetCheck()) item.m_unEvents |= EVENT_TOTAL_TIME_ALL;
	if (m_CheckTotal3BVs.GetCheck()) item.m_unEvents |= EVENT_TOTAL_3BVS;
	if (m_CheckTotal3BVsBeg.GetCheck()) item.m_unEvents |= EVENT_TOTAL_3BVS_BEG;
	if (m_CheckTotal3BVsInt.GetCheck()) item.m_unEvents |= EVENT_TOTAL_3BVS_INT;
	if (m_CheckTotal3BVsExp.GetCheck()) item.m_unEvents |= EVENT_TOTAL_3BVS_EXP;
	if (m_CheckTotal3BVsAll.GetCheck()) item.m_unEvents |= EVENT_TOTAL_3BVS_ALL;
	if (m_CheckBestTime.GetCheck()) item.m_unEvents |= EVENT_BEST_TIME;
	if (m_CheckBestTimeBeg.GetCheck()) item.m_unEvents |= EVENT_BEST_TIME_BEG;
	if (m_CheckBestTimeInt.GetCheck()) item.m_unEvents |= EVENT_BEST_TIME_INT;
	if (m_CheckBestTimeExp.GetCheck()) item.m_unEvents |= EVENT_BEST_TIME_EXP;
	if (m_CheckBestTimeAll.GetCheck()) item.m_unEvents |= EVENT_BEST_TIME_ALL;
	if (m_CheckBest3BVs.GetCheck()) item.m_unEvents |= EVENT_BEST_3BVS;
	if (m_CheckBest3BVsBeg.GetCheck()) item.m_unEvents |= EVENT_BEST_3BVS_BEG;
	if (m_CheckBest3BVsInt.GetCheck()) item.m_unEvents |= EVENT_BEST_3BVS_INT;
	if (m_CheckBest3BVsExp.GetCheck()) item.m_unEvents |= EVENT_BEST_3BVS_EXP;
	if (m_CheckBest3BVsAll.GetCheck()) item.m_unEvents |= EVENT_BEST_3BVS_ALL;
	if (m_CheckBestCount.GetCheck()) item.m_unEvents |= EVENT_BEST_COUNT;
	if (m_CheckBestCountBeg.GetCheck()) item.m_unEvents |= EVENT_BEST_COUNT_BEG;
	if (m_CheckBestCountInt.GetCheck()) item.m_unEvents |= EVENT_BEST_COUNT_INT;
	if (m_CheckBestCountExp.GetCheck()) item.m_unEvents |= EVENT_BEST_COUNT_EXP;
	if (m_CheckBestCountAll.GetCheck()) item.m_unEvents |= EVENT_BEST_COUNT_ALL;
	m_MatchTail.GetWindowText(item.m_szMatchTail);
}


void CCompetitionSettings::OnCbnSelchangeComboTemplet()
{
	LoadConfiguration(m_Templets.GetCurSel());
}


void CCompetitionSettings::LoadConfiguration(unsigned int index)
{
	CGlobalGonfig& config = m_pTheApp->GetGlobalGonfig();
	CConfigItem item = config.m_arrayConfig[index];
	m_StandardBegCount.SetWindowText(CFormatter::UINT2CString(item.m_unBegNum));
	m_StandardIntCount.SetWindowText(CFormatter::UINT2CString(item.m_unIntNum));
	m_StandardExpCount.SetWindowText(CFormatter::UINT2CString(item.m_unExpNum));
	m_CheckFL.SetCheck((item.m_unEvents & EVENT_STYLE_FL) != 0);
	m_CheckNF.SetCheck((item.m_unEvents & EVENT_STYLE_NF) != 0);
	m_CheckTotalTime.SetCheck((item.m_unEvents & EVENT_TOTAL_TIME) != 0);
	m_CheckTotalTimeBeg.SetCheck((item.m_unEvents & EVENT_TOTAL_TIME_BEG) != 0);
	m_CheckTotalTimeInt.SetCheck((item.m_unEvents & EVENT_TOTAL_TIME_INT) != 0);
	m_CheckTotalTimeExp.SetCheck((item.m_unEvents & EVENT_TOTAL_TIME_EXP) != 0);
	m_CheckTotalTimeAll.SetCheck((item.m_unEvents & EVENT_TOTAL_TIME_ALL) != 0);
	m_CheckTotal3BVs.SetCheck((item.m_unEvents & EVENT_TOTAL_3BVS) != 0);
	m_CheckTotal3BVsBeg.SetCheck((item.m_unEvents & EVENT_TOTAL_3BVS_BEG) != 0);
	m_CheckTotal3BVsInt.SetCheck((item.m_unEvents & EVENT_TOTAL_3BVS_INT) != 0);
	m_CheckTotal3BVsExp.SetCheck((item.m_unEvents & EVENT_TOTAL_3BVS_EXP) != 0);
	m_CheckTotal3BVsAll.SetCheck((item.m_unEvents & EVENT_TOTAL_3BVS_ALL) != 0);
	m_CheckBestTime.SetCheck((item.m_unEvents & EVENT_BEST_TIME) != 0);
	m_CheckBestTimeBeg.SetCheck((item.m_unEvents & EVENT_BEST_TIME_BEG) != 0);
	m_CheckBestTimeInt.SetCheck((item.m_unEvents & EVENT_BEST_TIME_INT) != 0);
	m_CheckBestTimeExp.SetCheck((item.m_unEvents & EVENT_BEST_TIME_EXP) != 0);
	m_CheckBestTimeAll.SetCheck((item.m_unEvents & EVENT_BEST_TIME_ALL) != 0);
	m_CheckBest3BVs.SetCheck((item.m_unEvents & EVENT_BEST_3BVS) != 0);
	m_CheckBest3BVsBeg.SetCheck((item.m_unEvents & EVENT_BEST_3BVS_BEG) != 0);
	m_CheckBest3BVsInt.SetCheck((item.m_unEvents & EVENT_BEST_3BVS_INT) != 0);
	m_CheckBest3BVsExp.SetCheck((item.m_unEvents & EVENT_BEST_3BVS_EXP) != 0);
	m_CheckBest3BVsAll.SetCheck((item.m_unEvents & EVENT_BEST_3BVS_ALL) != 0);
	m_CheckBestCount.SetCheck((item.m_unEvents & EVENT_BEST_COUNT) != 0);
	m_CheckBestCountBeg.SetCheck((item.m_unEvents & EVENT_BEST_COUNT_BEG) != 0);
	m_CheckBestCountInt.SetCheck((item.m_unEvents & EVENT_BEST_COUNT_INT) != 0);
	m_CheckBestCountExp.SetCheck((item.m_unEvents & EVENT_BEST_COUNT_EXP) != 0);
	m_CheckBestCountAll.SetCheck((item.m_unEvents & EVENT_BEST_COUNT_ALL) != 0);
	m_MatchTail.SetWindowText(item.m_szMatchTail);

	OnBnClickedCheckTotalTime();
	OnBnClickedCheckTotal3bvs();
	OnBnClickedCheckBestTime();
	OnBnClickedCheckBest3bvs();
	OnBnClickedCheckBestCount();
}

void CCompetitionSettings::OnBnClickedCheckTotalTime()
{
	BOOL flag = m_CheckTotalTime.GetCheck() ? TRUE : FALSE;
	m_CheckTotalTimeBeg.EnableWindow(flag);
	m_CheckTotalTimeInt.EnableWindow(flag);
	m_CheckTotalTimeExp.EnableWindow(flag);
	m_CheckTotalTimeAll.EnableWindow(flag);
}


void CCompetitionSettings::OnBnClickedCheckTotal3bvs()
{
	BOOL flag = m_CheckTotal3BVs.GetCheck() ? TRUE : FALSE;
	m_CheckTotal3BVsBeg.EnableWindow(flag);
	m_CheckTotal3BVsInt.EnableWindow(flag);
	m_CheckTotal3BVsExp.EnableWindow(flag);
	m_CheckTotal3BVsAll.EnableWindow(flag);
}


void CCompetitionSettings::OnBnClickedCheckBestTime()
{
	BOOL flag = m_CheckBestTime.GetCheck() ? TRUE : FALSE;
	m_CheckBestTimeBeg.EnableWindow(flag);
	m_CheckBestTimeInt.EnableWindow(flag);
	m_CheckBestTimeExp.EnableWindow(flag);
	m_CheckBestTimeAll.EnableWindow(flag);
}


void CCompetitionSettings::OnBnClickedCheckBest3bvs()
{
	BOOL flag = m_CheckBest3BVs.GetCheck() ? TRUE : FALSE;
	m_CheckBest3BVsBeg.EnableWindow(flag);
	m_CheckBest3BVsInt.EnableWindow(flag);
	m_CheckBest3BVsExp.EnableWindow(flag);
	m_CheckBest3BVsAll.EnableWindow(flag);
}


void CCompetitionSettings::OnBnClickedCheckBestCount()
{
	BOOL flag = m_CheckBestCount.GetCheck() ? TRUE : FALSE;
	m_CheckBestCountBeg.EnableWindow(flag);
	m_CheckBestCountInt.EnableWindow(flag);
	m_CheckBestCountExp.EnableWindow(flag);
	m_CheckBestCountAll.EnableWindow(flag);
}


void CCompetitionSettings::OnBnClickedButtonGenTail()
{
	CString text;
	CTailGenerator generator(this, &text);
	if (generator.DoModal() == IDOK) m_MatchTail.SetWindowText(text);
}
