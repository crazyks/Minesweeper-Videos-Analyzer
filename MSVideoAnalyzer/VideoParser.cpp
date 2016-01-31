#include "StdAfx.h"
#include "MSVideoAnalyzer.h"
#include "VideoParser.h"
#include "afxmt.h"

DWORD GetCoreNumber()
{
	SYSTEM_INFO sysInfo;
	::GetSystemInfo(&sysInfo);
	DWORD dwNumCpu = sysInfo.dwNumberOfProcessors;
	return dwNumCpu;
}

CSemaphore gSemaphoreWork((GetCoreNumber() << 4), (GetCoreNumber() << 4));
CSemaphore gSemaphoreWrite((GetCoreNumber() << 1), (GetCoreNumber() << 1));
CMutex gMutexCounter, gMutextStatus;

CString gToolchains[] = {
	_T("\"%s\\core\\driver.exe\" \"%s\\core\\avf2rawvf.exe\" \"%s\" nopause \"|\" \"%s\\core\\rawvf2rawvf.exe\" -s"),
	_T("\"%s\\core\\driver.exe\" \"%s\\core\\mvf2rawvf.exe\" \"%s\" \"|\" \"%s\\core\\rawvf2rawvf.exe\" -s"),
	_T("\"%s\\core\\driver.exe\" \"%s\\core\\rmv2rawvf.exe\" %s nopause \"|\" \"%s\\core\\rawvf2rawvf.exe\" -s"),
	_T("\"%s\\core\\driver.exe\" \"%s\\core\\mvr2rawvf.exe\" \"%s\" \"|\" \"%s\\core\\rawvf2rawvf.exe\" -s"),
	_T("\"%s%s%s%s\"")
};

CString gDlls[] = {
	_T("%s\\libs\\avf2rawvf.dll"),
	_T("%s\\libs\\mvf2rawvf.dll"),
	_T("%s\\libs\\rmv2rawvf.dll"),
	_T("%s\\libs\\mvr2rawvf.dll"),
	_T("%s\\libs\\rawvf2rawvf.dll")
};

HINSTANCE mModules[5];

ICallback* CVideoParser::pCallback = NULL;
CString* CVideoParser::pStrToolchainDir = NULL;
PARSER_STATUS CVideoParser::eStatus = STATUS_PREPARED;
unsigned int CVideoParser::nCounter = 0;
unsigned int CVideoParser::nCounterMax = 0;
ConvertFunc CVideoParser::convert = NULL;
ParseFunc CVideoParser::parseAVF = NULL;
ParseFunc CVideoParser::parseMVF = NULL;
ParseFunc CVideoParser::parseRMV = NULL;
ParseFunc CVideoParser::parseMVR = NULL;
GCFunc CVideoParser::gc = NULL;

CVideoParser::CVideoParser(void)
{
}


CVideoParser::~CVideoParser(void)
{
}

void CVideoParser::RegisterCallback(ICallback* callback)
{
	pCallback = callback;
}


void CVideoParser::Run(vector<CString>& pathList)
{
	SetStatus(STATUS_RUNNING);
	ResetCounter(pathList.size());
	CWinThread* m_pThread = AfxBeginThread(ThreadWorkspace,
		&pathList,
		THREAD_PRIORITY_NORMAL,
		0,
		CREATE_SUSPENDED,
		0);
	m_pThread->ResumeThread();
}


void CVideoParser::Stop()
{
	if (eStatus == STATUS_PREPARED)
	{
		if (pCallback != NULL) pCallback->OnCancelled();
	}
	else SetStatus(STATUS_CANCELING);
}


UINT CVideoParser::ThreadWorkspace(LPVOID pParam)
{
	vector<CString>* pathList = (vector<CString>*) pParam;
	for (vector<CString>::iterator it = pathList->begin(); it != pathList->end(); ++it)
	{
		if (eStatus == STATUS_CANCELING)
		{
			IncreaseCounter(pathList->end() - it);
			break;
		}
		WaitForSingleObject(gSemaphoreWork.m_hObject, INFINITE);
		CWinThread* m_pThread = AfxBeginThread(ThreadProc,
			&(*it),
			THREAD_PRIORITY_NORMAL,
			0,
			CREATE_SUSPENDED,
			0);
		m_pThread->ResumeThread();
	}
	return 0;
}


UINT CVideoParser::ThreadProc(LPVOID pParam)
{
	CString source = *((CString*) pParam);
	ParseFunc parse = GetParseFunc(source);
	CAdvancedVideoInfo info = (parse && convert) ? Parse(convert, parse, source) : Parse(source);
	if (pCallback != NULL) pCallback->OnNotify(info);
	ReleaseSemaphore(gSemaphoreWork.m_hObject, 1, NULL);
	IncreaseCounter();
	return 0;
}


CString CVideoParser::GetFileTitleFromFileName(const CString FileName, CString& ExtendName)
{
	int Where;
	Where=FileName.ReverseFind('\\');
	if(Where==-1)
	{
		Where=FileName.ReverseFind('/');
	}
	CString FileTitle=FileName.Right(FileName.GetLength()-1-Where);
	int Which=FileTitle.ReverseFind('.');
	ExtendName=FileTitle.Right(FileTitle.GetLength()-Which-1);
	if (Which!=-1)
	{
		FileTitle=FileTitle.Left(Which);
	}
	return FileTitle;
}


CString CVideoParser::GetParseToolchain(const CString strFilePath)
{
	CString cmd, ext;
	GetFileTitleFromFileName(strFilePath, ext);
	CString dir = GetToolchainDir();
	TOOLCHAIN toolchain = TOOLCHAIN_MAX;
	if (ext.CompareNoCase(_T("avf")) == 0 || ext.CompareNoCase(_T("fsvf")) == 0)
		toolchain = TOOLCHAIN_AVF;
	else if (ext.CompareNoCase(_T("mvf")) == 0)
		toolchain = TOOLCHAIN_MVF;
	else if (ext.CompareNoCase(_T("rmv")) == 0)
		toolchain = TOOLCHAIN_RMV;
	else if (ext.CompareNoCase(_T("mvr")) == 0)
		toolchain = TOOLCHAIN_MVR;
	else
		toolchain = TOOLCHAIN_MAX;

	cmd.Format(gToolchains[toolchain], dir, dir, strFilePath, dir);
	return cmd;
}


ParseFunc CVideoParser::GetParseFunc(const CString strFilePath)
{
	CGlobalGonfig& config = ((CMSVideoAnalyzerApp*) AfxGetApp())->GetGlobalGonfig();
	if (!config.m_bUseFastParser) return NULL;
	CString ext;
	GetFileTitleFromFileName(strFilePath, ext);
	if (ext.CompareNoCase(_T("avf")) == 0 || ext.CompareNoCase(_T("fsvf")) == 0)
		return parseAVF;
	else if (ext.CompareNoCase(_T("mvf")) == 0)
		return parseMVF;
	else if (ext.CompareNoCase(_T("rmv")) == 0)
		return parseRMV;
	else if (ext.CompareNoCase(_T("mvr")) == 0)
		return parseMVR;
	else
		return NULL;
}


CString CVideoParser::GetToolchainDir(void)
{
	if (pStrToolchainDir == NULL)
	{
		TCHAR exepath[MAX_PATH];
		CString strdir,tmpdir;
		memset(exepath, 0, MAX_PATH);
		GetModuleFileName(NULL, exepath, MAX_PATH);
		tmpdir = exepath;
		strdir = tmpdir.Left(tmpdir.ReverseFind('\\'));
		pStrToolchainDir = new CString(strdir);
	}
	return *pStrToolchainDir;
}

void  CVideoParser::ReleaseVideoInfoNode(VideoInfoNode* begin)
{
	if (gc) gc(begin);
}

CString CVideoParser::VideoInfoNodeToString(VideoInfoNode* begin)
{
	CString str;
	VideoInfoNode* node = begin;
	while(node)
	{
		str += node->msg;
		node = node->pNext;
	}
	return str;
}

CAdvancedVideoInfo CVideoParser::Parse(ConvertFunc convert, ParseFunc parse, CString source)
{
	WaitForSingleObject(gSemaphoreWrite.m_hObject, INFINITE);
	CString m_strOutput;
	m_strOutput.Empty();
	VideoInfoNode* tmpInfo = parse(source.GetBuffer(0));
	source.ReleaseBuffer();
	VideoInfoNode* finalInfo = convert(tmpInfo);
	m_strOutput += VideoInfoNodeToString(finalInfo);
	if (finalInfo) ReleaseVideoInfoNode(finalInfo);
	if (tmpInfo) ReleaseVideoInfoNode(tmpInfo);
	ReleaseSemaphore(gSemaphoreWrite.m_hObject, 1, NULL);
	CAdvancedVideoInfo info = CVideoInfoBuilder::GetInstance().Create(m_strOutput);
	info.mStrPath = source;
	return info;
}


CAdvancedVideoInfo CVideoParser::Parse(const CString source)
{
	WaitForSingleObject(gSemaphoreWrite.m_hObject, INFINITE);
	CString cmdline = GetParseToolchain(source), m_strOutput;
	SECURITY_ATTRIBUTES lsa;
	STARTUPINFO si;
	PROCESS_INFORMATION pi, *lppi;
	HANDLE hReadPipe, hWritePipe;

	lsa.nLength = sizeof(SECURITY_ATTRIBUTES);
	lsa.lpSecurityDescriptor = NULL;
	lsa.bInheritHandle = TRUE;

	lppi = &pi;
	CWaitCursor waitCursor;

	DWORD cchReadBuffer;
	CString strText;
	TCHAR ph[4104];
	m_strOutput.Empty();

	if(!CreatePipe(&hReadPipe, &hWritePipe, &lsa, 0)) goto fail;

	memset(&si, 0, sizeof(STARTUPINFO));
	si.cb = sizeof(STARTUPINFO);
	si.dwFlags = STARTF_USESTDHANDLES | STARTF_USESHOWWINDOW;
	si.wShowWindow = SW_HIDE;
	si.hStdOutput = hWritePipe;

	if(!CreateProcess(NULL, \
		cmdline.GetBuffer(0), \
		NULL, NULL, TRUE, 0, NULL, NULL, &si, &pi))
	{
		cmdline.ReleaseBuffer();
		goto failWhenExec;
	}
	cmdline.ReleaseBuffer();

	for(;;)
	{
		cchReadBuffer = 0;
		if(!PeekNamedPipe(hReadPipe, ph, 1, &cchReadBuffer, NULL, NULL))
			break;

		if(cchReadBuffer)
		{
			if(!ReadFile(hReadPipe, ph, 4096, &cchReadBuffer, NULL))
				break;
			ph[cchReadBuffer] = 0;
			m_strOutput += ph;;
		}
		else
			if(WaitForSingleObject(pi.hProcess, 0) == WAIT_OBJECT_0)
				break;
		Sleep(10);
	}

	ph[cchReadBuffer] = 0;
	m_strOutput += ph;

	CloseHandle(pi.hThread);
	CloseHandle(pi.hProcess);
failWhenExec:
	CloseHandle(hReadPipe);
	CloseHandle(hWritePipe);
fail:
	ReleaseSemaphore(gSemaphoreWrite.m_hObject, 1, NULL);
	CAdvancedVideoInfo info = CVideoInfoBuilder::GetInstance().Create(m_strOutput);
	info.mStrPath = source;
	return info;
}


void CVideoParser::ResetCounter(unsigned int max)
{
	gMutexCounter.Lock();
	nCounter = 0;
	nCounterMax = max;
	gMutexCounter.Unlock();
}


void CVideoParser::IncreaseCounter(unsigned int count /*= 1*/)
{
	gMutexCounter.Lock();
	nCounter += count;
	if (nCounter == nCounterMax)
	{
		if (pCallback != NULL)
		{
			if (eStatus == STATUS_RUNNING) pCallback->OnDismissed();
			else pCallback->OnCancelled();
		}
		SetStatus(STATUS_PREPARED);
	}
	gMutexCounter.Unlock();
}


void CVideoParser::SetStatus(PARSER_STATUS status)
{
	gMutextStatus.Lock();
	eStatus = status;
	gMutextStatus.Unlock();
}


void CVideoParser::Init(void)
{
	convert = LoadConvertFunc();
	parseAVF = LoadParseFunc(TOOLCHAIN_AVF);
	parseMVF = LoadParseFunc(TOOLCHAIN_MVF);
	parseRMV = LoadParseFunc(TOOLCHAIN_RMV);
	parseMVR = LoadParseFunc(TOOLCHAIN_MVR);
}


void CVideoParser::DeInit(void)
{
	for (unsigned int i = 0; i != 5; ++i)
		if(mModules[i]) FreeLibrary(mModules[i]);
}


ParseFunc CVideoParser::LoadParseFunc(const TOOLCHAIN type)
{
	CString dll;
	dll.Format(gDlls[type], GetToolchainDir());
	HINSTANCE hModule=LoadLibrary(dll);
	mModules[type] = hModule;
	return (ParseFunc) GetProcAddress(hModule, "parse");
}


ConvertFunc CVideoParser::LoadConvertFunc()
{
	if (!mModules[TOOLCHAIN_MAX])
	{
		CString dll;
		dll.Format(gDlls[TOOLCHAIN_MAX], GetToolchainDir());
		mModules[TOOLCHAIN_MAX] = LoadLibrary(dll);
	}
	return (ConvertFunc) GetProcAddress(mModules[TOOLCHAIN_MAX], "convert");
}


GCFunc CVideoParser::LoadGCFunc()
{
	if (!mModules[TOOLCHAIN_MAX])
	{
		CString dll;
		dll.Format(gDlls[TOOLCHAIN_MAX], GetToolchainDir());
		mModules[TOOLCHAIN_MAX] = LoadLibrary(dll);
	}
	return (GCFunc) GetProcAddress(mModules[TOOLCHAIN_MAX], "gc");
}