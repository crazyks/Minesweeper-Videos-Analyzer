
// MSVideoAnalyzer.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "afxwinappex.h"
#include "afxdialogex.h"
#include "MSVideoAnalyzer.h"
#include "MainFrm.h"

#include "MSVideoAnalyzerDoc.h"
#include "MSVideoAnalyzerView.h"
#include "VideoParser.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMSVideoAnalyzerApp

BEGIN_MESSAGE_MAP(CMSVideoAnalyzerApp, CWinAppEx)
	ON_COMMAND(ID_APP_ABOUT, &CMSVideoAnalyzerApp::OnAppAbout)
	// Standard file based document commands
	ON_COMMAND(ID_FILE_NEW, &CWinAppEx::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, &CWinAppEx::OnFileOpen)
END_MESSAGE_MAP()


// CMSVideoAnalyzerApp construction

CMSVideoAnalyzerApp::CMSVideoAnalyzerApp()
	: m_sGlobalGonfig(CGlobalGonfig::GetInstance())
{
	m_bHiColorIcons = TRUE;

	// TODO: replace application ID string below with unique ID string; recommended
	// format for string is CompanyName.ProductName.SubProduct.VersionInformation
	SetAppID(_T("MSVideoAnalyzer.AppID.NoVersion"));

	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

// The one and only CMSVideoAnalyzerApp object

CMSVideoAnalyzerApp theApp;


// CMSVideoAnalyzerApp initialization

BOOL CMSVideoAnalyzerApp::InitInstance()
{
	// InitCommonControlsEx() is required on Windows XP if an application
	// manifest specifies use of ComCtl32.dll version 6 or later to enable
	// visual styles.  Otherwise, any window creation will fail.
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// Set this to include all the common control classes you want to use
	// in your application.
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinAppEx::InitInstance();


	// Initialize OLE libraries
	if (!AfxOleInit())
	{
		AfxMessageBox(IDP_OLE_INIT_FAILED);
		return FALSE;
	}

	AfxEnableControlContainer();

	EnableTaskbarInteraction(FALSE);

	// AfxInitRichEdit2() is required to use RichEdit control	
	// AfxInitRichEdit2();

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	// of your final executable, you should remove from the following
	// the specific initialization routines you do not need
	// Change the registry key under which our settings are stored
	// TODO: You should modify this string to be something appropriate
	// such as the name of your company or organization
	SetRegistryKey(_T("Local AppWizard-Generated Applications"));
	LoadStdProfileSettings(0);  // Load standard INI file options (including MRU)


	InitContextMenuManager();

	InitKeyboardManager();

	InitTooltipManager();
	CMFCToolTipInfo ttParams;
	ttParams.m_bVislManagerTheme = TRUE;
	theApp.GetTooltipManager()->SetTooltipParams(AFX_TOOLTIP_TYPE_ALL,
		RUNTIME_CLASS(CMFCToolTipCtrl), &ttParams);

	// Register the application's document templates.  Document templates
	//  serve as the connection between documents, frame windows and views
	CSingleDocTemplate* pDocTemplate;
	pDocTemplate = new CSingleDocTemplate(
		IDR_MAINFRAME,
		RUNTIME_CLASS(CMSVideoAnalyzerDoc),
		RUNTIME_CLASS(CMainFrame),       // main SDI frame window
		RUNTIME_CLASS(CMSVideoAnalyzerView));
	if (!pDocTemplate)
		return FALSE;
	AddDocTemplate(pDocTemplate);


	// Parse command line for standard shell commands, DDE, file open
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);



	// Dispatch commands specified on the command line.  Will return FALSE if
	// app was launched with /RegServer, /Register, /Unregserver or /Unregister.
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;

	// The one and only window has been initialized, so show and update it
	m_pMainWnd->ShowWindow(SW_SHOW);
	m_pMainWnd->UpdateWindow();
	// call DragAcceptFiles only if there's a suffix
	//  In an SDI app, this should occur after ProcessShellCommand
	CVideoParser::Init();
	return TRUE;
}

int CMSVideoAnalyzerApp::ExitInstance()
{
	//TODO: handle additional resources you may have added
	AfxOleTerm(FALSE);

	this->CleanState();
	m_sGlobalGonfig.Save();
	CVideoParser::DeInit();

	return CWinAppEx::ExitInstance();
}

// CMSVideoAnalyzerApp message handlers


// CAboutDlg dialog used for App About

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()

protected: 
	RECT m_pRectLink;
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BOOL CAboutDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	GetDlgItem(IDC_LINK)->GetWindowRect(&m_pRectLink); 
	ScreenToClient(&m_pRectLink);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()

void CAboutDlg::OnMouseMove(UINT nFlags, CPoint point)
{
	if(point.x > m_pRectLink.left && point.x < m_pRectLink.right && point.y > m_pRectLink.top && point.y < m_pRectLink.bottom)
		SetCursor(LoadCursor(NULL, IDC_HAND));
	else
		SetCursor(LoadCursor(NULL, IDC_ARROW));

	CDialogEx::OnMouseMove(nFlags, point);
}

HBRUSH CAboutDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	if (pWnd->GetDlgCtrlID() == IDC_LINK)
	{
		pDC->SetTextColor(RGB(0, 0, 255));
	}
	else if (pWnd->GetDlgCtrlID() == IDC_BUILD_INFO)
	{
		pDC->SetTextColor(RGB(200, 0, 0));
	}

	// TODO:  如果默认的不是所需画笔，则返回另一个画笔
	return hbr;
}


void CAboutDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	if(point.x > m_pRectLink.left && point.x < m_pRectLink.right && point.y > m_pRectLink.top && point.y < m_pRectLink.bottom) 
	{
		if (nFlags == MK_LBUTTON)//鼠标左键按下 
		{
			ShellExecute(NULL, _T("open"),_T("mailto:crazyks@yeah.net"), NULL, NULL, SW_SHOWNORMAL);
		}
	}

	CDialogEx::OnLButtonDown(nFlags, point);
}

// App command to run the dialog
void CMSVideoAnalyzerApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

// CMSVideoAnalyzerApp customization load/save methods

void CMSVideoAnalyzerApp::PreLoadState()
{
	BOOL bNameValid;
	CString strName;
	bNameValid = strName.LoadString(IDS_EDIT_MENU);
	ASSERT(bNameValid);
	GetContextMenuManager()->AddMenu(strName, IDR_POPUP_EDIT);
}

void CMSVideoAnalyzerApp::LoadCustomState()
{
}

void CMSVideoAnalyzerApp::SaveCustomState()
{
}

CGlobalGonfig& CMSVideoAnalyzerApp::GetGlobalGonfig()
{
	return m_sGlobalGonfig;
}

// CMSVideoAnalyzerApp message handlers

