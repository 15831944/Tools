#pragma once
#define INIT_1K 1024


class CGbl
{
private:
	CGbl(void);
	~CGbl(void);

	CString m_strExePath;
	CString m_strDataPath;

public:
	static CGbl& GetMe();
	const CString getExePath(){return m_strExePath;}
	const CString getDataPath(){return m_strDataPath;}

public:
	static CString GetMssageFHResult(HRESULT hr);
	static bool SetSystemTimeFromStr(SYSTEMTIME& st,CString text);
	static CString GetSystemTime2Str(const SYSTEMTIME& st);
	static void SpliteBy(CString text,CString rule,std::vector<CString>& vtStr);
	static bool GetCorrectFileName(CString& str, bool showError=false);
	static CString GetPathFromFilePathName(CString pathName);
	static CString GetNameFromFilePathName(CString pathName);
	static CString GetIPFromNumber(UINT number);
	static UINT GetNumberFromIP(CString ip);
	static bool MakeDir(CString path);
	static bool RemoveFiles(CString path, CString name);
	static bool CopyDirectoryAll(CString csSrcFolder, CString csNewFolder);
	static CString GetCulTime(bool bMillSec = true);
	static bool ChangeVariantType(CComVariant& cvr, UINT vt);
	static LONGLONG VariantCompare(CComVariant cvr1, CComVariant cvr2);
	static CString GetCPUID();
	static CString GetSpecialPath(int id);
	static bool ReadFile2Binary(CString pathName, char** data, int& len);
	static bool WriteBinary2File(CString pathName, char* data, int len);
	static CString UintToCString(UINT numbe);
	static bool StartHighTime();
	static double GetHighTime(bool bReStart = false);
	bool IsNumber(CString str);
};

class PrintTime
{
private:
	__int64 __nStart = 0;
	__int64 __nEnd = 0;
	__int64 __nFrequency = 0;
	bool __bCreated;

public:
	PrintTime(void)
		: __nStart(0)
		, __nEnd(0)
		, __nFrequency(0)
		, __bCreated(false)
	{
		if (!QueryPerformanceFrequency((LARGE_INTEGER *)&__nFrequency))	return;
		if (!QueryPerformanceCounter((LARGE_INTEGER *)&__nStart))		return;
		__bCreated = true;
	}

	~PrintTime(void){
		if (!__bCreated)		return;
		CalcTime();
	}

	double CalcTime()
	{
		if (!QueryPerformanceCounter((LARGE_INTEGER *)&__nEnd))			return -1.0;
		__int64 n = __nEnd - __nStart;
		double t = (double)n / (double)__nFrequency;
		return t;
	}

	void MessageBoxTime()
	{
		double d = CalcTime();
		CString str;
		str.Format(_T("%f"), d);
		MessageBox(NULL, str, _T("Info"), MB_OK);
	}
};
