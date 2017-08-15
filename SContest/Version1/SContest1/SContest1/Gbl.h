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
