// TailGenerator.cpp : 实现文件
//

#include "stdafx.h"
#include "MSVideoAnalyzer.h"
#include "TailGenerator.h"
#include "afxdialogex.h"
#include "MD5Calculator.h"
#include "CRC32Calculator.h"


// CTailGenerator 对话框

IMPLEMENT_DYNAMIC(CTailGenerator, CDialogEx)

CTailGenerator::CTailGenerator(CWnd* pParent /*=NULL*/, CString* pTail /*= NULL*/)
	: CDialogEx(CTailGenerator::IDD, pParent)
	, m_pTail(pTail)
	, m_pTheApp((CMSVideoAnalyzerApp*) AfxGetApp())
{
}

CTailGenerator::~CTailGenerator()
{
}

void CTailGenerator::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_MAGIC_STRING, m_MagicString);
	DDX_Control(pDX, IDC_EDIT_NAME_LIST, m_NameList);
}


BEGIN_MESSAGE_MAP(CTailGenerator, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_BROWSE_LIST, &CTailGenerator::OnBnClickedButtonBrowseList)
	ON_BN_CLICKED(IDOK, &CTailGenerator::OnBnClickedOk)
END_MESSAGE_MAP()


// CTailGenerator 消息处理程序


BOOL CTailGenerator::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	CGlobalGonfig& config = m_pTheApp->GetGlobalGonfig();
	m_MagicString.SetWindowText(config.m_szMagic);
	m_NameList.SetWindowText(config.m_szNameListDir);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


void CTailGenerator::OnBnClickedButtonBrowseList()
{
	CString currentDir;
	m_NameList.GetWindowText(currentDir);
	while (StrCmp(currentDir.Right(1), _T("\\")) == 0 || StrCmp(currentDir.Right(1), _T("/")) == 0)
	{
		currentDir = currentDir.Left(currentDir.GetLength() - 1);
	}
	CFileDialog FileDialog(TRUE, NULL, currentDir, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT | OFN_FILEMUSTEXIST, _T("All files(*.*)|*.*|"), this);   
	if(FileDialog.DoModal() != IDOK) return;
	CString cStrFile = FileDialog.GetPathName();
	m_NameList.SetWindowText(cStrFile);
}


void CTailGenerator::OnBnClickedOk()
{
	Gen();
	CDialogEx::OnOK();
}


void CTailGenerator::Gen()
{
	CGlobalGonfig& config = m_pTheApp->GetGlobalGonfig();
	m_MagicString.GetWindowText(config.m_szMagic);
	m_NameList.GetWindowText(config.m_szNameListDir);
	CString magic = config.m_szMagic;
	CString md5 = _T("");
	CMD5Calculator::GetMd5(config.m_szNameListDir, md5);
	CString tail;
	tail.Format(_T("%d"), \
		(CCRC32Calculator::GetCRC32(magic) % 100) * \
		(CCRC32Calculator::GetCRC32(md5) % 100));
	m_pTail->Format(_T("%s"), tail);
}