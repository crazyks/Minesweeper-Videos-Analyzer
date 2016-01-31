#pragma once
#include "VideoInfoBuilder.h"
#include "ICallback.h"
#include <vector>
using std::vector;

typedef struct _VideoInfoNode
{
	char msg[256];
	struct _VideoInfoNode* pNext;
} VideoInfoNode;

typedef VideoInfoNode* (*ConvertFunc)(VideoInfoNode* videoinfo);
typedef VideoInfoNode* (*ParseFunc)(char* filename);
typedef void (*GCFunc)(VideoInfoNode* videoinfo);

typedef enum _TOOLCHAIN
{
	TOOLCHAIN_AVF = 0,
	TOOLCHAIN_MVF,
	TOOLCHAIN_RMV,
	TOOLCHAIN_MVR,
	TOOLCHAIN_MAX
} TOOLCHAIN;

typedef enum _PARSER_STATUS
{
	STATUS_PREPARED = 0,
	STATUS_RUNNING,
	STATUS_CANCELING,
} PARSER_STATUS;

class CVideoParser
{
private:
	CVideoParser(void);
	~CVideoParser(void);
public:
	static void Init(void);
	static void DeInit(void);
	static void Run(vector<CString>& pathList);
	static void Stop();
	static void RegisterCallback(ICallback* callback);

private:
	static CString GetFileTitleFromFileName(const CString FileName, CString& ExtendName);
	static CString GetParseToolchain(const CString strFilePath);
	static ParseFunc GetParseFunc(const CString strFilePath);
	static CString GetToolchainDir(void);
	static CAdvancedVideoInfo Parse(ConvertFunc convert, ParseFunc parse, CString source);
	static CAdvancedVideoInfo Parse(const CString source);
	static UINT ThreadProc(LPVOID pParam);
	static UINT ThreadWorkspace(LPVOID pParam);
	static void ResetCounter(unsigned int max);
	static void IncreaseCounter(unsigned int count = 1);
	static void SetStatus(PARSER_STATUS status);
	static ParseFunc LoadParseFunc(const TOOLCHAIN type);
	static ConvertFunc LoadConvertFunc();
	static GCFunc LoadGCFunc();
	static CString VideoInfoNodeToString(VideoInfoNode* begin);
	static void ReleaseVideoInfoNode(VideoInfoNode* begin);

private:
	static CString* pStrToolchainDir;
	static ICallback* pCallback;
	static PARSER_STATUS eStatus;
	static unsigned int nCounterMax;
	static unsigned int nCounter;
	static ConvertFunc convert;
	static ParseFunc parseAVF;
	static ParseFunc parseMVF;
	static ParseFunc parseRMV;
	static ParseFunc parseMVR;
	static GCFunc gc;
};

