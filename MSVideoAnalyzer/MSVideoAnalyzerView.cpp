
// MSVideoAnalyzerView.cpp : implementation of the CMSVideoAnalyzerView class
//

#include "stdafx.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "MSVideoAnalyzer.h"
#endif

#include "MSVideoAnalyzerDoc.h"
#include "MSVideoAnalyzerView.h"
#include "ProgressBar.h"
#include "CompetitionSettings.h"
#include "tchar.h"
#include "shlobj.h" 
#include "resource.h"

#include "CApplication.h"
#include "CRange.h"
#include "CWorkbooks.h"
#include "CWorksheets.h"
#include "CWorkbook.h"
#include "CWorksheet.h"
#include "MD5Calculator.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


bool AscCmp(const CAdvancedVideoInfo& s1, const CAdvancedVideoInfo& s2)
{
	return s1.mStrPath.Compare(s2.mStrPath) < 0;
}

bool AscCmpScore(const CScore& s1, const CScore& s2)
{
	return s1.m_dDoubleValue < s2.m_dDoubleValue;
}

bool DescCmpScore(const CScore& s1, const CScore& s2)
{
	return s1.m_dDoubleValue > s2.m_dDoubleValue;
}


// CMSVideoAnalyzerView

IMPLEMENT_DYNCREATE(CMSVideoAnalyzerView, CListView)

BEGIN_MESSAGE_MAP(CMSVideoAnalyzerView, CListView)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
	ON_COMMAND(ID_TOOL_BROWSE, &CMSVideoAnalyzerView::OnBrowse)
	ON_COMMAND(ID_TOOL_EXPORT, &CMSVideoAnalyzerView::OnExport)
	ON_NOTIFY_REFLECT(NM_DBLCLK, &CMSVideoAnalyzerView::OnNMDblclk)
	ON_COMMAND(ID_TOOL_COMPETITIONSETTINGS, &CMSVideoAnalyzerView::OnSetupCompetition)
	ON_COMMAND(ID_TOOL_PLAY, &CMSVideoAnalyzerView::OnPlay)
	ON_COMMAND(ID_VIEW_SHOWVIDEOLIST, &CMSVideoAnalyzerView::OnShowVideoList)
	ON_COMMAND(ID_VIEW_SHOWCOMPETITIONSUMMARY, &CMSVideoAnalyzerView::OnShowCompetitionSummary)
	ON_UPDATE_COMMAND_UI(ID_TOOL_PLAY, &CMSVideoAnalyzerView::OnUpdateToolPlay)
	ON_COMMAND(ID_TOOL_FASTPARSER, &CMSVideoAnalyzerView::OnToolFastparser)
	ON_COMMAND(ID_TOOL_SCANSUBDIR, &CMSVideoAnalyzerView::OnToolScansubdir)
	ON_UPDATE_COMMAND_UI(ID_TOOL_FASTPARSER, &CMSVideoAnalyzerView::OnUpdateToolFastparser)
	ON_UPDATE_COMMAND_UI(ID_TOOL_SCANSUBDIR, &CMSVideoAnalyzerView::OnUpdateToolScansubdir)
END_MESSAGE_MAP()

// CMSVideoAnalyzerView construction/destruction

CMSVideoAnalyzerView::CMSVideoAnalyzerView()
	: m_bColumnInitialized (FALSE)
	, m_bDataHandled (FALSE)
	, m_bSummaryHandled(FALSE)
	, m_bIsSummaryMode (FALSE)
{
	// 初始化列宽
	m_anColumnWidths[COLUMN_INDEX_NUMBER] = 35;
	m_anColumnWidths[COLUMN_INDEX_FILE] = 150;
	m_anColumnWidths[COLUMN_INDEX_PROGRAM] = 180;
	m_anColumnWidths[COLUMN_INDEX_PLAYER] = 100;
	m_anColumnWidths[COLUMN_INDEX_TIMESTAMP] = 150;
	m_anColumnWidths[COLUMN_INDEX_LEVEL] = 150;
	m_anColumnWidths[COLUMN_INDEX_MODE] = 50;
	m_anColumnWidths[COLUMN_INDEX_TIME] = 50;
	m_anColumnWidths[COLUMN_INDEX_3BV] = 50;
	m_anColumnWidths[COLUMN_INDEX_SOLV] = 50;
	m_anColumnWidths[COLUMN_INDEX_3BVS] = 50;
	m_anColumnWidths[COLUMN_INDEX_ZINI] = 50;
	m_anColumnWidths[COLUMN_INDEX_ZINIS] = 50;
	m_anColumnWidths[COLUMN_INDEX_CLICK] = 50;
	m_anColumnWidths[COLUMN_INDEX_CLICKS] = 50;
	m_anColumnWidths[COLUMN_INDEX_LCLICK] = 50;
	m_anColumnWidths[COLUMN_INDEX_RCLICK] = 50;
	m_anColumnWidths[COLUMN_INDEX_DCLICK] = 50;
	m_anColumnWidths[COLUMN_INDEX_IOE] = 50;
	m_anColumnWidths[COLUMN_INDEX_FLAGS] = 50;
	m_anColumnWidths[COLUMN_INDEX_OPENINGS] = 50;
	m_anColumnWidths[COLUMN_INDEX_ISLANDS] = 50;
}

CMSVideoAnalyzerView::~CMSVideoAnalyzerView()
{
}

BOOL CMSVideoAnalyzerView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CListView::PreCreateWindow(cs);
}

void CMSVideoAnalyzerView::OnInitialUpdate()
{
	CListView::OnInitialUpdate();


	// TODO: You may populate your ListView with items by directly accessing
	//  its list control through a call to GetListCtrl().
	CListCtrl& list = GetListCtrl();//得到内置的listctrl引用
	LONG lStyle;
	lStyle = GetWindowLong(list.m_hWnd, GWL_STYLE);//获取当前窗口风格
	lStyle &= ~LVS_TYPEMASK;                        //清除显示方式位
	lStyle |= LVS_REPORT;                            //设置报表风格
	lStyle |= LVS_SINGLESEL;                      //设置单行选择
	SetWindowLong(list.m_hWnd, GWL_STYLE, lStyle); //设置窗口风格

	DWORD dwStyle = list.GetExtendedStyle();

	//选中某行使整行高亮（只适用于报表风格的listctrl）
	dwStyle |= LVS_EX_FULLROWSELECT;
	dwStyle |= LVS_EX_GRIDLINES;//网格线（只适用与报表风格的listctrl）

	list.SetExtendedStyle(dwStyle);          //设置扩展风格
	list.SetBkColor(RGB(225, 225, 225));     //设置背景颜色
	list.SetTextBkColor(RGB(225, 225, 225)); //设置文本背景颜色
	list.SetTextColor(RGB(10, 10, 128));      //设置文本颜色
}

void CMSVideoAnalyzerView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CMSVideoAnalyzerView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// CMSVideoAnalyzerView diagnostics

#ifdef _DEBUG
void CMSVideoAnalyzerView::AssertValid() const
{
	CListView::AssertValid();
}

void CMSVideoAnalyzerView::Dump(CDumpContext& dc) const
{
	CListView::Dump(dc);
}

CMSVideoAnalyzerDoc* CMSVideoAnalyzerView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CMSVideoAnalyzerDoc)));
	return (CMSVideoAnalyzerDoc*)m_pDocument;
}
#endif //_DEBUG


// CMSVideoAnalyzerView message handlers


void CMSVideoAnalyzerView::OnBrowse()
{
	CGlobalGonfig& config = ((CMSVideoAnalyzerApp*) AfxGetApp())->GetGlobalGonfig();
	TCHAR szDir[MAX_PATH];
	CString path;
	BROWSEINFO bi;
	ITEMIDLIST* pidl;
	vector<CString> list;
	memset(&bi, 0, sizeof(BROWSEINFO));
	bi.hwndOwner = m_hWnd;
	bi.pidlRoot = NULL;
	bi.pszDisplayName = szDir;
	CString title;
	title.LoadString(IDS_TEXT_BROWSE);
	bi.lpszTitle = title;
	bi.ulFlags = BIF_STATUSTEXT|BIF_RETURNONLYFSDIRS|BIF_RETURNFSANCESTORS|BIF_DONTGOBELOWDOMAIN;
	bi.lpfn = (BFFCALLBACK) &CMSVideoAnalyzerView::BrowseCallbackProc;
	bi.lParam = 0;
	bi.iImage = 0;
	pidl = SHBrowseForFolder(&bi);
	if (pidl == NULL) return;
	else
	{
		SHGetPathFromIDList(pidl, szDir);
		path = szDir;
		if (path.Compare(_T("")) == 0) goto fail;
		else goto success;
	}
fail:
	Error(IDS_MSG_INVALID_DIR);
	return;
success:
	config.m_szLastDir = path;
	mMD5Set.clear();
	ListFiles(path, list, config.m_bScanSubDirs);
	if (list.empty())
	{
		Error(IDS_MSG_NO_VIDEO);
		return;
	}
	vector<CAdvancedVideoInfo> info;
	CProgressBar progressBar(this, &list, &info);
	progressBar.DoModal();
	sort(info.begin(), info.end(), AscCmp);
	mData = info;
	m_bDataHandled = FALSE;
	m_bSummaryHandled = FALSE;
	Refresh();
}


void CMSVideoAnalyzerView::OnExport()
{
	CListCtrl& list = GetListCtrl();
	if (list.GetItemCount() == 0)
	{
		Error(IDS_MSG_CANNOT_EXPORT);
		return;
	}
	CFileDialog FileDialog(FALSE, _T("xls"), NULL, OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT, _T("Microsoft Excel 2003(*.xls)|*.xls|All files(*.*)"), this);   
	if(FileDialog.DoModal() != IDOK) return;
	CString cStrFile = FileDialog.GetPathName();
	if(::PathFileExists(cStrFile)) DeleteFile(cStrFile);

	COleVariant covTrue((short)TRUE),covFalse((short)FALSE),covOptional((long)DISP_E_PARAMNOTFOUND,VT_ERROR);

	CApplication app; //Excel程序
	CWorkbooks books; //工作簿集合
	CWorkbook book;  //工作表
	CWorksheets sheets;  //工作簿集合
	CWorksheet sheet; //工作表集合
	CRange range; //使用区域

	CoUninitialize();

	book.PrintPreview(_variant_t(false));
	if(CoInitialize(NULL) == S_FALSE) 
	{
		// Error(IDS_MSG_COM_INIT_FAILED);
		SaveAsFakeXls(cStrFile, list);
		return;
	}

	if(!app.CreateDispatch(_T("Excel.Application"))) //创建IDispatch接口对象
	{
		// Error(IDS_MSG_EXCEL_INIT_FAILED);
		SaveAsFakeXls(cStrFile, list);
		return;
	}


	books = app.get_Workbooks();
	book = books.Add(covOptional);

	sheets = book.get_Worksheets();
	sheet = sheets.get_Item(COleVariant((short)1));  //得到第一个工作表

	CHeaderCtrl *pmyHeaderCtrl= list.GetHeaderCtrl(); //获取表头

	int m_cols = pmyHeaderCtrl-> GetItemCount(); //获取列数
	int m_rows = list.GetItemCount();  //获取行数


	TCHAR lpBuffer[256];  

	HDITEM hdi; //This structure contains information about an item in a header control. This structure has been updated to support header item images and order values.
	hdi.mask = HDI_TEXT;
	hdi.pszText = lpBuffer;
	hdi.cchTextMax = 256; 

	CString colname;
	int iRow,iCol;
	for(iCol = 0; iCol < m_cols; iCol++)//将列表的标题头写入EXCEL 
	{ 
		GetCellName(1 ,iCol + 1, colname); //(colname就是对应表格的A1,B1,C1,D1)
		range = sheet.get_Range(COleVariant(colname), COleVariant(colname));  
		pmyHeaderCtrl-> GetItem(iCol, &hdi); //获取表头每列的信息
		range.put_Value2(COleVariant(hdi.pszText));  //设置每列的内容
		int nWidth = list.GetColumnWidth(iCol) / 6; 
		//得到第iCol+1列 
		range.AttachDispatch(range.get_Item(_variant_t((long)(iCol+1)), vtMissing).pdispVal, true);
		//设置列宽  
		range.put_ColumnWidth(_variant_t((long)nWidth));
	} 

	range = sheet.get_Range(COleVariant( _T("A1")), COleVariant(colname)); 
	range.put_VerticalAlignment(COleVariant((short) - 4108));//xlVAlignCenter = -4108 

	COleSafeArray saRet; //COleSafeArray类是用于处理任意类型和维数的数组的类
	DWORD numElements[]={m_rows, m_cols};   //行列写入数组
	saRet.Create(VT_BSTR, 2, numElements); //创建所需的数组

	range = sheet.get_Range(COleVariant( _T("A2")), covOptional); //从A2开始
	range = range.get_Resize(COleVariant((short)m_rows), COleVariant((short)m_cols)); //表的区域

	long index[2];  

	for(iRow = 1; iRow <= m_rows; iRow++)//将列表内容写入EXCEL 
	{ 
		for (iCol = 1; iCol <= m_cols; iCol++)  
		{ 
			index[0]=iRow-1; 
			index[1]=iCol-1;
			CString szTemp;
			szTemp = list.GetItemText(iRow-1, iCol-1); //取得list控件中的内容
			BSTR bstr = szTemp.AllocSysString(); //The AllocSysString method allocates a new BSTR string that is Automation compatible
			saRet.PutElement(index,bstr); //把list控件中的内容放入saRet
			SysFreeString(bstr);
		} 
	}  

	range.put_Value2(COleVariant(saRet)); //将得到的数据的saRet数组值放入表格

	book.SaveCopyAs(COleVariant(cStrFile)); //保存到cStrFile文件
	book.put_Saved(true);
	books.Close();
	book.ReleaseDispatch();
	books.ReleaseDispatch(); 
	app.ReleaseDispatch();
	app.Quit();
}


void CMSVideoAnalyzerView::SaveAsFakeXls(CString& saveTo, CListCtrl& list)
{
	CHeaderCtrl *pmyHeaderCtrl= list.GetHeaderCtrl(); //获取表头

	int m_cols = pmyHeaderCtrl-> GetItemCount(); //获取列数
	int m_rows = list.GetItemCount();  //获取行数

	TCHAR lpBuffer[256];
	HDITEM hdi; //This structure contains information about an item in a header control. This structure has been updated to support header item images and order values.
	hdi.mask = HDI_TEXT;
	hdi.pszText = lpBuffer;
	hdi.cchTextMax = 256;

	CStdioFile mFile;   
	CFileException mExcept;
	mFile.Open(saveTo, CFile::modeWrite | CFile::modeCreate, &mExcept);
	
	int iRow, iCol;
	for(iCol = 0; iCol < m_cols; iCol++)//将列表的标题头写入EXCEL 
	{
		pmyHeaderCtrl->GetItem(iCol, &hdi); //获取表头每列的信息
		mFile.WriteString(hdi.pszText);
		mFile.WriteString(_T("\t"));
	}
	mFile.WriteString(_T("\n"));
	for(iRow = 1; iRow <= m_rows; iRow++)//将列表内容写入EXCEL 
	{ 
		for (iCol = 1; iCol <= m_cols; iCol++)  
		{
			CString szTemp;
			szTemp = list.GetItemText(iRow-1, iCol-1); //取得list控件中的内容
			mFile.WriteString(szTemp);
			mFile.WriteString(_T("\t"));
		}
		mFile.WriteString(_T("\n"));
	}
	mFile.Flush();
	mFile.Close();
}


void CMSVideoAnalyzerView::InsertRow(int row, CPlayer& info, unsigned int events)
{
	CListCtrl& list = GetListCtrl();
	int nRow = list.InsertItem(row, _T(""));
	CString number;
	static unsigned int baseEventFlags[5] = {EVENT_TOTAL_TIME_BEG, EVENT_TOTAL_3BVS_BEG, EVENT_BEST_TIME_BEG, EVENT_BEST_3BVS_BEG, EVENT_BEST_COUNT_BEG};
	static unsigned int baseEventIndex[5] = {EVENT_TYPE_TOTAL_TIME_BEG, EVENT_TYPE_TOTAL_3BVS_BEG, EVENT_TYPE_BEST_TIME_BEG, EVENT_TYPE_BEST_3BVS_BEG, EVENT_TYPE_BEST_COUNT_BEG};
	int columnIndex = 0;
	number.Format(_T("%03d"), nRow + 1);
	list.SetItemText(nRow, columnIndex++, number);
	list.SetItemText(nRow, columnIndex++, info.m_szName);
	for (int k = 0; k < 5; k++)
	{
		if (events & (EVENT_TOTAL_TIME << k))
		{
			for (int j = 0; j < 4; j++)
			{
				if ((events & (baseEventFlags[k] << j)))
				{
					if (k == 4) number.Format(_T("%d"), info.m_asScores[baseEventIndex[k] + j].m_nIntegerValue);
					else number.Format(_T("%.3lf"), info.m_asScores[baseEventIndex[k] + j].m_dDoubleValue);
					list.SetItemText(nRow, columnIndex++, number);
				}
			}
		}
	}
}


void CMSVideoAnalyzerView::InsertRow(int row, CAdvancedVideoInfo& info)
{
	CListCtrl& list = GetListCtrl();
	int nRow = list.InsertItem(row, _T(""));
	CString number;
	number.Format(_T("%03d"), nRow + 1);
	list.SetItemText(nRow, COLUMN_INDEX_NUMBER, number);
	list.SetItemText(nRow, COLUMN_INDEX_FILE, info.GetFile());
	list.SetItemText(nRow, COLUMN_INDEX_PROGRAM, info.GetProgramVersion());
	list.SetItemText(nRow, COLUMN_INDEX_PLAYER, info.mStrPlayer);
	list.SetItemText(nRow, COLUMN_INDEX_TIMESTAMP, info.mStrTimestamp);
	list.SetItemText(nRow, COLUMN_INDEX_LEVEL, info.GetLevel());
	list.SetItemText(nRow, COLUMN_INDEX_MODE, info.mStrMode);
	list.SetItemText(nRow, COLUMN_INDEX_TIME, info.mStrTime);
	list.SetItemText(nRow, COLUMN_INDEX_3BV, info.mStr3BV);
	list.SetItemText(nRow, COLUMN_INDEX_SOLV, info.mStrSolv3BV);
	list.SetItemText(nRow, COLUMN_INDEX_3BVS, info.mStr3BVs);
	list.SetItemText(nRow, COLUMN_INDEX_ZINI, info.mStrZiNi);
	list.SetItemText(nRow, COLUMN_INDEX_ZINIS, info.mStrZiNis);
	list.SetItemText(nRow, COLUMN_INDEX_CLICK, info.mStrClicks);
	list.SetItemText(nRow, COLUMN_INDEX_CLICKS, info.mStrClicksPers);
	list.SetItemText(nRow, COLUMN_INDEX_LCLICK, info.mStrLeftClicks);
	list.SetItemText(nRow, COLUMN_INDEX_RCLICK, info.mStrRightClicks);
	list.SetItemText(nRow, COLUMN_INDEX_DCLICK, info.mStrDoubleClicks);
	list.SetItemText(nRow, COLUMN_INDEX_IOE, info.mStrIOE);
	list.SetItemText(nRow, COLUMN_INDEX_FLAGS, info.mFlags);
	list.SetItemText(nRow, COLUMN_INDEX_OPENINGS, info.mStrOpenings);
	list.SetItemText(nRow, COLUMN_INDEX_ISLANDS, info.mIslands);
}


void CMSVideoAnalyzerView::Refresh()
{
	if (m_bIsSummaryMode)
	{
		DoSummary();
		Refresh(mPlayers);
	}
	else
	{
		Refresh(mData);
	}
}


void CMSVideoAnalyzerView::Refresh(vector<CAdvancedVideoInfo>& data)
{
	CListCtrl& list = GetListCtrl();
	// 刷新表头
	list.DeleteAllItems();
	if (m_bColumnInitialized)
	{
		int columnCount = list.GetHeaderCtrl()->GetItemCount();
		for (int i = columnCount - 1; i >= 0; i--)
		{
			list.DeleteColumn(i);
		}
	}
	CString columnName;
	for (int i = 0; i < COLUMN_INDEX_MAX; i++)
	{
		columnName.LoadString(IDS_COLUMN_NUMBER + i);
		list.InsertColumn(COLUMN_INDEX_NUMBER + i, columnName, LVCFMT_LEFT, m_anColumnWidths[COLUMN_INDEX_NUMBER + i]);
	}
	m_bColumnInitialized = TRUE;
	// 插入数据
	int nRow = 0;
	for (vector<CAdvancedVideoInfo>::iterator it = data.begin(); it != data.end(); ++it)
	{
		InsertRow(nRow, (*it));
		nRow++;
	}
}


void CMSVideoAnalyzerView::Refresh(vector<CPlayer>& players)
{
	CListCtrl& list = GetListCtrl();
	CGlobalGonfig& config = ((CMSVideoAnalyzerApp*) AfxGetApp())->GetGlobalGonfig();
	CConfigItem& item = config.m_arrayConfig[config.m_unConfigIndex];
	// 刷新表头
	list.DeleteAllItems();
	if (m_bColumnInitialized)
	{
		int columnCount = list.GetHeaderCtrl()->GetItemCount();
		for (int i = columnCount - 1; i >= 0; i--)
		{
			list.DeleteColumn(i);
		}
	}
	CString columnName;
	int columnIndex = 0;
	static unsigned int baseEventFlags[5] = {EVENT_TOTAL_TIME_BEG, EVENT_TOTAL_3BVS_BEG, EVENT_BEST_TIME_BEG, EVENT_BEST_3BVS_BEG, EVENT_BEST_COUNT_BEG};
	static unsigned int baseStrintRes[5] = {IDS_EVENT_TOTAL_TIME_BEG, IDS_EVENT_TOTAL_3BVS_BEG, IDS_EVENT_BEST_TIME_BEG, IDS_EVENT_BEST_3BVS_BEG, IDS_EVENT_BEST_COUNT_BEG};
	columnName.LoadString(IDS_COLUMN_NUMBER);
	list.InsertColumn(columnIndex++, columnName, LVCFMT_LEFT, 35);
	columnName.LoadString(IDS_COLUMN_PLAYER);
	list.InsertColumn(columnIndex++, columnName, LVCFMT_LEFT, 120);
	for (int k = 0; k < 5; k++)
	{
		if (item.m_unEvents & (EVENT_TOTAL_TIME << k))
		{
			for (int j = 0; j < 4; j++)
			{
				if ((item.m_unEvents & (baseEventFlags[k] << j)))
				{
					columnName.LoadString(baseStrintRes[k] + j);
					list.InsertColumn(columnIndex++, columnName, LVCFMT_LEFT, 120);
				}
			}
		}
	}
	m_bColumnInitialized = TRUE;
	// 插入数据
	int nRow = 0;
	for (vector<CPlayer>::iterator it = players.begin(); it != players.end(); ++it)
	{
		InsertRow(nRow, (*it), item.m_unEvents);
		nRow++;
	}
}


void CMSVideoAnalyzerView::ListFiles(const CString dir, vector<CString>& list, BOOL scanSubDir /*= TRUE*/)
{
	static CString exts[] = { _T(".avf"), _T(".mvf"), _T(".rmv"), _T(".mvr"), _T(".fsvf")};
	CFileFind finder;
	CString path, tmpExt;
	path.Format(_T("%s\\*"), dir);
	BOOL bWorking = finder.FindFile(path);  
	while(bWorking)
	{  
		bWorking = finder.FindNextFile();
		if (finder.IsDots()) continue;
		if (finder.IsDirectory() && scanSubDir) ListFiles(finder.GetFilePath(), list, scanSubDir);
		else
		{
			tmpExt = finder.GetFileName().Right(4).MakeLower();
			for (int i = 0; i < 4; i++)
			{
				if (StrCmp(tmpExt, exts[i]) == 0)
				{
					CString md5 = _T("");
					CMD5Calculator::GetMd5(finder.GetFilePath(), md5);
					if (mMD5Set.find(md5) == mMD5Set.end())
					{
						mMD5Set.insert(md5);
						list.push_back(finder.GetFilePath());
					}
					break;
				}
			}
			tmpExt = finder.GetFileName().Right(5).MakeLower();
			if (StrCmp(tmpExt, exts[4]) == 0)
			{
				CString md5 = _T("");
				CMD5Calculator::GetMd5(finder.GetFilePath(), md5);
				if (mMD5Set.find(md5) == mMD5Set.end())
				{
					mMD5Set.insert(md5);
					list.push_back(finder.GetFilePath());
				}
			}
		}
	}
	finder.Close();
}

void CMSVideoAnalyzerView::OnNMDblclk(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	
	OnPlay();

	*pResult = 0;
}


vector<int> CMSVideoAnalyzerView::GetSelection()
{
	CListCtrl& list = GetListCtrl();
	POSITION pos = list.GetFirstSelectedItemPosition();
	vector<int> selections;
	while(pos)
	{
		int nIdx = -1;
		nIdx = list.GetNextSelectedItem(pos);
		if(nIdx >= 0 && nIdx < list.GetItemCount())
		{
			selections.push_back(nIdx);
		}
	}
	return selections;
}


void CMSVideoAnalyzerView::GetCellName(const int row, const int columnOffset, CString& columnName)
{
	columnName.Format(_T("%c%d"), 'A' + columnOffset - 1, row);
}


void CMSVideoAnalyzerView::Error(UINT msgId)
{
	CString msg;
	msg.LoadString(msgId);
	MessageBox(msg, _T("Error"), 0 + 48);
}

LPITEMIDLIST CMSVideoAnalyzerView::GetItemIDListFromFilePath(CString &strFilePath)
{
	if (strFilePath.IsEmpty())
	{
		return NULL;
	}
	// 得到桌面的目录
	LPSHELLFOLDER pDesktopFolder = NULL;
	HRESULT hr = SHGetDesktopFolder(&pDesktopFolder);
	if (FAILED(hr))
	{
		return NULL;
	}
	// 将文件路径转换至OLECHAR格式
	OLECHAR strOleFilePath[MAX_PATH];
	MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, strFilePath, -1, strOleFilePath, MAX_PATH);
	// 得到文件路径对应的ItemIDList
	LPITEMIDLIST pItemIDList = NULL;
	hr = pDesktopFolder->ParseDisplayName(NULL, NULL, strOleFilePath, NULL, &pItemIDList, NULL);
	pDesktopFolder->Release();
	if (FAILED(hr))
	{
		return NULL;
	}
	return pItemIDList;
}

INT CALLBACK CMSVideoAnalyzerView::BrowseCallbackProc(IN HWND hWnd, IN UINT uMsg, IN LPARAM lParam, IN LPARAM lpData)
{
	if(uMsg == BFFM_INITIALIZED)
	{
		CGlobalGonfig& config = ((CMSVideoAnalyzerApp*) AfxGetApp())->GetGlobalGonfig();
		::SendMessage(hWnd, BFFM_SETSELECTION, TRUE, (LPARAM) config.m_szLastDir.GetBuffer(0));
		config.m_szLastDir.ReleaseBuffer();
	}
	return 0;
}

void CMSVideoAnalyzerView::OnSetupCompetition()
{
	CCompetitionSettings dialog;
	if (dialog.DoModal() == IDOK)
	{
		m_bSummaryHandled = FALSE;
		if (m_bIsSummaryMode) Refresh();
	}
}


void CMSVideoAnalyzerView::OnPlay()
{
	if (m_bIsSummaryMode) return;
	vector<int> selections = GetSelection();
	if (!selections.empty())
	{
		CAdvancedVideoInfo info = mData[selections[0]];
		CString cmd;
		cmd.Format(_T("\"%s\""), info.mStrPath);
		ShellExecute(NULL, NULL, cmd, NULL, NULL, SW_SHOWNORMAL);
	}
}


void CMSVideoAnalyzerView::ClearScoreMap(map<CString, vector<CScore>>& theMap)
{
	for (map<CString, vector<CScore>>::iterator it = theMap.begin(); it != theMap.end(); ++it)
	{
		it->second.clear();
	}
	theMap.clear();
}


void CMSVideoAnalyzerView::SortScoreMap(map<CString, vector<CScore>>& theMap, BOOL bAsc /*= TRUE*/)
{
	for (map<CString, vector<CScore>>::iterator it = theMap.begin(); it != theMap.end(); ++it)
	{
		sort(it->second.begin(), it->second.end(), bAsc ? AscCmpScore : DescCmpScore);
	}
}


void CMSVideoAnalyzerView::DoSummary(void)
{
#ifndef _SCORE_MAP_INDEX_
#define _SCORE_MAP_INDEX_
#define MAP_BEG_TIME 0
#define MAP_INT_TIME 1
#define MAP_EXP_TIME 2
#define MAP_BEG_3BVS 3
#define MAP_INT_3BVS 4
#define MAP_EXP_3BVS 5
#define MAP_COUNT 6
#endif
	static map<CString, vector<CScore>> maps[MAP_COUNT];
	static set<CString> playerSet;
	CGlobalGonfig& config = ((CMSVideoAnalyzerApp*) AfxGetApp())->GetGlobalGonfig();
	CString tail = config.m_arrayConfig[config.m_unConfigIndex].m_szMatchTail;
	if (!m_bDataHandled)
	{
		for (UINT i = 0 ; i != MAP_COUNT; i++) ClearScoreMap(maps[i]);
		playerSet.clear();
		for (vector<CAdvancedVideoInfo>::iterator it = mData.begin(); it != mData.end(); ++it)
		{
			CAdvancedVideoInfo& info = *it;
			if (StrCmp(info.mStrPlayer.Right(tail.GetLength()), tail) != 0) continue; // Incorrect ID
			if (StrCmp(info.mStrMode, _T("classic")) != 0) continue; // Incorrect mode
			if (StrCmp(info.mStr3BV, info.mStrSolv3BV) != 0) continue; // Unsolved
			if (playerSet.find(info.mStrPlayer) == playerSet.end())
			{
				playerSet.insert(set<CString>::value_type(info.mStrPlayer));
				for (UINT i = 0 ; i != MAP_COUNT; i++)
				{
					vector<CScore> scores;
					maps[i].insert(map<CString, vector<CScore>>::value_type(info.mStrPlayer, scores));
				}
			}
			UINT indexOffset = 0;
			if (StrCmp(info.mStrLevel, _T("beginner")) == 0) indexOffset = 0;
			else if (StrCmp(info.mStrLevel, _T("intermediate")) == 0) indexOffset = 1;
			else if (StrCmp(info.mStrLevel, _T("expert")) == 0) indexOffset = 2;
			else continue;
			CScore time, bbbvs;
			time.m_bIsNF = _tstoi(info.mStrRightClicks) == 0 ? TRUE : FALSE;
			bbbvs.m_bIsNF = time.m_bIsNF;
			time.m_dDoubleValue = atof(info.mStrTime);
			bbbvs.m_dDoubleValue = atof(info.mStr3BVs);
			maps[MAP_BEG_TIME + indexOffset][info.mStrPlayer].push_back(time);
			maps[MAP_BEG_3BVS + indexOffset][info.mStrPlayer].push_back(bbbvs);
		}
		for (UINT i = 0 ; i != MAP_COUNT; i++) SortScoreMap(maps[i], i <= MAP_EXP_TIME ? TRUE : FALSE);
	}
	// Summary
	if (m_bSummaryHandled) return;
	vector<CPlayer> players;
	for (set<CString>::iterator it = playerSet.begin(); it != playerSet.end(); ++it)
	{
		CString key = (*it);
		CPlayer player;
		player.m_szName = key.Left(key.GetLength() - tail.GetLength());
		CConfigItem& item = config.m_arrayConfig[config.m_unConfigIndex];
		BOOL countFL = (item.m_unEvents & EVENT_STYLE_FL) != 0;
		BOOL countNF = (item.m_unEvents & EVENT_STYLE_NF) != 0;
		if (item.m_unEvents & EVENT_TOTAL_TIME)
		{
			if ((item.m_unEvents & EVENT_TOTAL_TIME_BEG) || (item.m_unEvents & EVENT_TOTAL_TIME_ALL))
				player.m_asScores[EVENT_TYPE_TOTAL_TIME_BEG].m_dDoubleValue = \
				GetSumScore(maps[MAP_BEG_TIME][key], countFL, countNF, item.m_unBegNum, 999.99);
			if ((item.m_unEvents & EVENT_TOTAL_TIME_INT) || (item.m_unEvents & EVENT_TOTAL_TIME_ALL))
				player.m_asScores[EVENT_TYPE_TOTAL_TIME_INT].m_dDoubleValue = \
				GetSumScore(maps[MAP_INT_TIME][key], countFL, countNF, item.m_unIntNum, 999.99);
			if ((item.m_unEvents & EVENT_TOTAL_TIME_EXP) || (item.m_unEvents & EVENT_TOTAL_TIME_ALL))
				player.m_asScores[EVENT_TYPE_TOTAL_TIME_EXP].m_dDoubleValue = \
				GetSumScore(maps[MAP_EXP_TIME][key], countFL, countNF, item.m_unExpNum, 999.99);
			if ((item.m_unEvents & EVENT_TOTAL_TIME_ALL))
				player.m_asScores[EVENT_TYPE_TOTAL_TIME_ALL].m_dDoubleValue = \
				player.m_asScores[EVENT_TYPE_TOTAL_TIME_BEG].m_dDoubleValue + \
				player.m_asScores[EVENT_TYPE_TOTAL_TIME_INT].m_dDoubleValue + \
				player.m_asScores[EVENT_TYPE_TOTAL_TIME_EXP].m_dDoubleValue;
		}
		if (item.m_unEvents & EVENT_TOTAL_3BVS)
		{
			if ((item.m_unEvents & EVENT_TOTAL_3BVS_BEG) || (item.m_unEvents & EVENT_TOTAL_3BVS_ALL))
				player.m_asScores[EVENT_TYPE_TOTAL_3BVS_BEG].m_dDoubleValue = \
				GetSumScore(maps[MAP_BEG_3BVS][key], countFL, countNF, item.m_unBegNum, 0.0);
			if ((item.m_unEvents & EVENT_TOTAL_3BVS_INT) || (item.m_unEvents & EVENT_TOTAL_3BVS_ALL))
				player.m_asScores[EVENT_TYPE_TOTAL_3BVS_INT].m_dDoubleValue = \
				GetSumScore(maps[MAP_INT_3BVS][key], countFL, countNF, item.m_unIntNum, 0.0);
			if ((item.m_unEvents & EVENT_TOTAL_3BVS_EXP) || (item.m_unEvents & EVENT_TOTAL_3BVS_ALL))
				player.m_asScores[EVENT_TYPE_TOTAL_3BVS_EXP].m_dDoubleValue = \
				GetSumScore(maps[MAP_EXP_3BVS][key], countFL, countNF, item.m_unExpNum, 0.0);
			if ((item.m_unEvents & EVENT_TOTAL_3BVS_ALL))
				player.m_asScores[EVENT_TYPE_TOTAL_3BVS_ALL].m_dDoubleValue = \
				player.m_asScores[EVENT_TYPE_TOTAL_3BVS_BEG].m_dDoubleValue + \
				player.m_asScores[EVENT_TYPE_TOTAL_3BVS_INT].m_dDoubleValue + \
				player.m_asScores[EVENT_TYPE_TOTAL_3BVS_EXP].m_dDoubleValue;
		}
		if (item.m_unEvents & EVENT_BEST_TIME)
		{
			if ((item.m_unEvents & EVENT_BEST_TIME_BEG) || (item.m_unEvents & EVENT_BEST_TIME_ALL))
				player.m_asScores[EVENT_TYPE_BEST_TIME_BEG].m_dDoubleValue = \
				GetBestScore(maps[MAP_BEG_TIME][key], countFL, countNF, 999.99);
			if ((item.m_unEvents & EVENT_BEST_TIME_INT) || (item.m_unEvents & EVENT_BEST_TIME_ALL))
				player.m_asScores[EVENT_TYPE_BEST_TIME_INT].m_dDoubleValue = \
				GetBestScore(maps[MAP_INT_TIME][key], countFL, countNF, 999.99);
			if ((item.m_unEvents & EVENT_BEST_TIME_EXP) || (item.m_unEvents & EVENT_BEST_TIME_ALL))
				player.m_asScores[EVENT_TYPE_BEST_TIME_EXP].m_dDoubleValue = \
				GetBestScore(maps[MAP_EXP_TIME][key], countFL, countNF, 999.99);
			if ((item.m_unEvents & EVENT_BEST_TIME_ALL))
				player.m_asScores[EVENT_TYPE_BEST_TIME_ALL].m_dDoubleValue = \
				player.m_asScores[EVENT_TYPE_BEST_TIME_BEG].m_dDoubleValue + \
				player.m_asScores[EVENT_TYPE_BEST_TIME_INT].m_dDoubleValue + \
				player.m_asScores[EVENT_TYPE_BEST_TIME_EXP].m_dDoubleValue;
		}
		if (item.m_unEvents & EVENT_BEST_3BVS)
		{
			if ((item.m_unEvents & EVENT_BEST_3BVS_BEG) || (item.m_unEvents & EVENT_BEST_3BVS_ALL))
				player.m_asScores[EVENT_TYPE_BEST_3BVS_BEG].m_dDoubleValue = \
				GetBestScore(maps[MAP_BEG_3BVS][key], countFL, countNF, 0.0);
			if ((item.m_unEvents & EVENT_BEST_3BVS_INT) || (item.m_unEvents & EVENT_BEST_3BVS_ALL))
				player.m_asScores[EVENT_TYPE_BEST_3BVS_INT].m_dDoubleValue = \
				GetBestScore(maps[MAP_INT_3BVS][key], countFL, countNF, 0.0);
			if ((item.m_unEvents & EVENT_BEST_3BVS_EXP) || (item.m_unEvents & EVENT_BEST_3BVS_ALL))
				player.m_asScores[EVENT_TYPE_BEST_3BVS_EXP].m_dDoubleValue = \
				GetBestScore(maps[MAP_EXP_3BVS][key], countFL, countNF, 0.0);
			if ((item.m_unEvents & EVENT_BEST_3BVS_ALL))
				player.m_asScores[EVENT_TYPE_BEST_3BVS_ALL].m_dDoubleValue = \
				player.m_asScores[EVENT_TYPE_BEST_3BVS_BEG].m_dDoubleValue + \
				player.m_asScores[EVENT_TYPE_BEST_3BVS_INT].m_dDoubleValue + \
				player.m_asScores[EVENT_TYPE_BEST_3BVS_EXP].m_dDoubleValue;
		}
		if (item.m_unEvents & EVENT_BEST_COUNT)
		{
			if ((item.m_unEvents & EVENT_BEST_COUNT_BEG) || (item.m_unEvents & EVENT_BEST_COUNT_ALL))
				player.m_asScores[EVENT_TYPE_BEST_COUNT_BEG].m_nIntegerValue = \
				GetCountScore(maps[MAP_BEG_TIME][key], countFL, countNF);
			if ((item.m_unEvents & EVENT_BEST_COUNT_INT) || (item.m_unEvents & EVENT_BEST_COUNT_ALL))
				player.m_asScores[EVENT_TYPE_BEST_COUNT_INT].m_nIntegerValue = \
				GetCountScore(maps[MAP_INT_TIME][key], countFL, countNF);
			if ((item.m_unEvents & EVENT_BEST_COUNT_EXP) || (item.m_unEvents & EVENT_BEST_COUNT_ALL))
				player.m_asScores[EVENT_TYPE_BEST_COUNT_EXP].m_nIntegerValue = \
				GetCountScore(maps[MAP_EXP_TIME][key], countFL, countNF);
			if ((item.m_unEvents & EVENT_BEST_COUNT_ALL))
				player.m_asScores[EVENT_TYPE_BEST_COUNT_ALL].m_nIntegerValue = \
				player.m_asScores[EVENT_TYPE_BEST_COUNT_BEG].m_nIntegerValue + \
				player.m_asScores[EVENT_TYPE_BEST_COUNT_INT].m_nIntegerValue + \
				player.m_asScores[EVENT_TYPE_BEST_COUNT_EXP].m_nIntegerValue;
		}
		players.push_back(player);
	}
	mPlayers = players;
	m_bSummaryHandled = TRUE;
}


double CMSVideoAnalyzerView::GetSumScore(vector<CScore>& scores, BOOL countFL, BOOL countNF, UINT count, double defaultScore)
{
	double sum = 0.0;
	UINT counter = 0;
	for (vector<CScore>::iterator it = scores.begin() ; it != scores.end(); ++it)
	{
		if (!countNF && it->m_bIsNF) continue;
		if (!countFL && !it->m_bIsNF) continue;
		sum += (*it).m_dDoubleValue;
		counter++;
		if (counter == count) break;
	}
	for (; counter != count; ++counter) sum += defaultScore;
	return sum;
}


double CMSVideoAnalyzerView::GetBestScore(vector<CScore>& scores, BOOL countFL, BOOL countNF, double defaultScore)
{
	for (vector<CScore>::iterator it = scores.begin() ; it != scores.end(); ++it)
	{
		if (!countNF && it->m_bIsNF) continue;
		if (!countFL && !it->m_bIsNF) continue;
		return it->m_dDoubleValue;
	}
	return defaultScore;
}


unsigned int CMSVideoAnalyzerView::GetCountScore(vector<CScore>& scores, BOOL countFL, BOOL countNF)
{
	unsigned int count = 0;
	for (vector<CScore>::iterator it = scores.begin() ; it != scores.end(); ++it)
	{
		if (!countNF && it->m_bIsNF) continue;
		if (!countFL && !it->m_bIsNF) continue;
		count++;
	}
	return count;
}


void CMSVideoAnalyzerView::OnShowVideoList()
{
	if (m_bIsSummaryMode)
	{
		m_bIsSummaryMode = FALSE;
		Refresh();
	}
}


void CMSVideoAnalyzerView::OnShowCompetitionSummary()
{
	if (!m_bIsSummaryMode)
	{
		m_bIsSummaryMode = TRUE;
		Refresh();
	}
}


void CMSVideoAnalyzerView::OnUpdateToolPlay(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(!m_bIsSummaryMode);
}


void CMSVideoAnalyzerView::OnToolFastparser()
{
	CGlobalGonfig& config = ((CMSVideoAnalyzerApp*) AfxGetApp())->GetGlobalGonfig();
	config.m_bUseFastParser = !config.m_bUseFastParser;
}


void CMSVideoAnalyzerView::OnToolScansubdir()
{
	CGlobalGonfig& config = ((CMSVideoAnalyzerApp*) AfxGetApp())->GetGlobalGonfig();
	config.m_bScanSubDirs = !config.m_bScanSubDirs;
}


void CMSVideoAnalyzerView::OnUpdateToolFastparser(CCmdUI *pCmdUI)
{
	CGlobalGonfig& config = ((CMSVideoAnalyzerApp*) AfxGetApp())->GetGlobalGonfig();
	pCmdUI->SetCheck(config.m_bUseFastParser);
}


void CMSVideoAnalyzerView::OnUpdateToolScansubdir(CCmdUI *pCmdUI)
{
	CGlobalGonfig& config = ((CMSVideoAnalyzerApp*) AfxGetApp())->GetGlobalGonfig();
	pCmdUI->SetCheck(config.m_bScanSubDirs);
}
