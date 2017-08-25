#include "stdafx.h"
#include "Gbl.h"

CGbl::CGbl(void)
{
	TCHAR exePath[MAX_PATH];
	::ZeroMemory(exePath,sizeof(TCHAR)*MAX_PATH);
	::GetModuleFileName(NULL,exePath,MAX_PATH);
	m_strExePath = exePath;
	m_strExePath = m_strExePath.Left(m_strExePath.ReverseFind('\\'));
	m_strExePath += _T('\\');
	m_strDataPath = m_strExePath;
}

CGbl::~CGbl(void){}

CGbl& CGbl::GetMe()
{
	static CGbl me;
	return me;
}

CString CGbl::GetMssageFHResult(HRESULT hr)
{
	void *pMsgBuf;
	::FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER |
		FORMAT_MESSAGE_FROM_SYSTEM,
		NULL,
		hr,
		MAKELANGID(LANG_NEUTRAL,SUBLANG_DEFAULT),
		(LPTSTR)&pMsgBuf,0,NULL);
	CString ss=(CString)(LPTSTR)pMsgBuf;
	LocalFree(pMsgBuf);
	return ss;
}

bool CGbl::SetSystemTimeFromStr(SYSTEMTIME& st,CString text)
{
	std::vector<CString> vtStr;
	SpliteBy(text,_T("/"),vtStr);
	if(vtStr.size() != 3)	return false;
	st.wMonth = _ttoi(vtStr[0]);
	st.wDay = _ttoi(vtStr[1]);

	SpliteBy(vtStr[2], _T(":"), vtStr);
	if(vtStr.size() != 3)	return false;
	CString year_hour = vtStr[0];
	CString second_mesc = vtStr[2];
	st.wMinute = _ttoi(vtStr[1]);

	SpliteBy(year_hour, _T(" "), vtStr);
	if(vtStr.size() != 2)	return false;
	st.wYear = _ttoi(vtStr[0]);
	st.wHour = _ttoi(vtStr[1]);

	SpliteBy(second_mesc, _T(" "), vtStr);
	if(vtStr.size() != 2)	return false;
	st.wSecond = _ttoi(vtStr[0]);
	st.wMilliseconds = _ttoi(vtStr[1]);

	return true;
}

CString CGbl::GetSystemTime2Str(const SYSTEMTIME& st)
{
	CString text;
	text.Format(_T("%.2d/%.2d/%.4d %.2d:%.2d:%.2d %.3d"), st.wMonth, st.wDay, st.wYear, st.wHour, st.wMinute, st.wSecond, st.wMilliseconds);
	return text;
}

void CGbl::SpliteBy(CString text,CString rule,std::vector<CString>& vtStr)
{
	vtStr.clear();
	if (text.IsEmpty())	return;
	int lenRule = rule.GetLength();
	int index = text.Find(rule);
	CString strValue;
	while (index != -1)
	{
		strValue = text.Left(index);
		vtStr.push_back(strValue);
		text.Delete(0, index + lenRule);
		index = text.Find(rule);
	}
	vtStr.push_back(text);
}

bool CGbl::GetCorrectFileName(CString& str, bool showError/* =false */)
{
	CString correctText = str.Trim();
	CString errStr = _T("\\/:*?\"<>|~.");
	for(int i = 0; i < str.GetLength(); ++i){
		if(errStr.Find(str.GetAt(i)) != -1){
			if(showError)
				MessageBox(NULL, _T("Can not include \\ / : * ? \" < > | ~ . symbol"), _T("Error"), MB_OK | MB_ICONERROR);
			return false;
		}
	}
	str = correctText;
	return true;
}

CString CGbl::GetPathFromFilePathName(CString pathName)
{
	int i = pathName.ReverseFind('\\');
	if(i == -1)			return _T("");

	CString path = pathName.Left(i + 1);
	return path;
}

//!< 从文件的全路径名中，获得文件的名称
CString CGbl::GetNameFromFilePathName(CString pathName)
{
	CString path = GetPathFromFilePathName(pathName);
	pathName.Delete(0, path.GetLength());
	return pathName;
}

//!< 有字符串式的IP地址获得数据类型的IP地址
UINT CGbl::GetNumberFromIP(CString ip)
{
	std::vector<CString> ltIpItem;
	SpliteBy(ip, _T("."), ltIpItem);
	if(ltIpItem.size() != 4)	return UINT(-1);
	UINT number;
	byte ipItem[4];
	for(int i = 0; i < 4; ++i)
		ipItem[i] = _ttoi(ltIpItem[i]);

	memcpy(&number, &ipItem[0], 4);
	return number;
}

bool CGbl::MakeDir(CString path)
{
	int i = 0, j = 0, k = 0;
	while(i < path.GetLength()){  
		i = path.Find(_T("\\"), j + 1);
		if(i == -1)	 			i = path.GetLength();
		short flag = ::_tmkdir(path.Left(i).operator LPCTSTR());
		if((k == 2) && (flag == -1)){
			switch(errno) {
				case EEXIST:	break;
				case ENOENT:
					MessageBox(NULL, _T("Path not exist!"), _T("Error"), MB_OK | MB_ICONERROR);
					return false;
				default:		return false;
			}
		}
		j = i;
		k++;
	}
	return true;
}

bool CGbl::RemoveFiles(CString path, CString name)
{
	CFileFind finder;
	BOOL bFind = finder.FindFile(path + name + _T(".*"));
	std::list<CString> ltDel;

	while (bFind)
	{
		bFind = finder.FindNextFile();
		CString strName = finder.GetFileName();
		if (PathIsDirectory(finder.GetFilePath()))
			continue;
		ltDel.push_back(path + strName);
	}

	bool bDel = true;
	for (auto str : ltDel)
	{
		if (!::DeleteFile(str))
			bDel = false;
	}
	return bDel;
}

bool CGbl::CopyDirectoryAll(CString csSrcFolder, CString csNewFolder)
{
	CFileFind floder;
	BOOL bFind=floder.FindFile(csSrcFolder + _T("*.*"));
	while(bFind){
		bFind = floder.FindNextFile();
		TRACE(_T("%s "),floder.GetFileName());

		if(floder.IsDots())	continue;
		if(floder.IsDirectory()){
			_tmkdir(csNewFolder+floder.GetFileName());
			CopyDirectoryAll(csSrcFolder + _T("\\") + floder.GetFileName() + _T("\\"), csNewFolder + floder.GetFileName() + _T("\\"));
		}
		::SetFileAttributes(csSrcFolder + floder.GetFileName(), FILE_ATTRIBUTE_NORMAL);
		::AfxGetApp()->DoWaitCursor(1);
		::CopyFile(csSrcFolder + floder.GetFileName(), csNewFolder + floder.GetFileName(), FALSE);
		::AfxGetApp()->DoWaitCursor(-1);
	}
	return true;
}

CString CGbl::GetCulTime(bool bMillSec /* = true */)
{
	SYSTEMTIME st;
	GetLocalTime(&st);
	CString str;
	if(bMillSec)
	str.Format(_T("%.4d/%.2d/%.2d %.2d:%.2d:%.2d (%.3d)"),st.wYear,st.wMonth,st.wDay,st.wHour,st.wMinute,st.wSecond,st.wMilliseconds);
	else
		str.Format(_T("%.4d/%.2d/%.2d %.2d:%.2d:%.2d"),st.wYear,st.wMonth,st.wDay,st.wHour,st.wMinute,st.wSecond);
	return str;
}

bool CGbl::ChangeVariantType(CComVariant& cvr, UINT vt)
{
	if(cvr.vt == vt)			return true;
	LRESULT lResult = cvr.ChangeType(vt);
	if(SUCCEEDED(lResult))		return true;
	CComVariant cv = cvr;
	lResult = cv.ChangeType(VT_BSTR);
	if(!SUCCEEDED(lResult))		return false;
	CString strValue = (CString)cv.bstrVal;
	LONGLONG llValue = _ttoi64(strValue);
	float flValue;
	double dlValue;
	if(vt == VT_BOOL)		{	cvr = bool(llValue);	}
	else if(vt == VT_I1)	{	cvr = char(llValue);	}
	else if(vt == VT_UI1)	{	cvr = byte(llValue);	}
	else if(vt == VT_I2)	{	cvr = short(llValue);	}
	else if(vt == VT_UI2)	{	cvr = WORD(llValue);	}
	else if(vt == VT_I4)	{	cvr = int(llValue);		}
	else if(vt == VT_UI4)	{	cvr = UINT(llValue);	}
	else if(vt == VT_I8)	{	cvr = llValue;			}
	else if(vt == VT_UI8)	{	cvr = UINT64(llValue);	}
	else if(vt == VT_R4)	{	memcpy(&flValue, &llValue, sizeof(float));		cvr = flValue;}
	else if(vt == VT_R8)	{	memcpy(&dlValue, &llValue, sizeof(double));		cvr = dlValue;}
	return true;
}

LONGLONG CGbl::VariantCompare(CComVariant cvr1, CComVariant cvr2)
{
	if(cvr1.vt == VT_BSTR || cvr2.vt == VT_BSTR)
	{
		cvr1.ChangeType(VT_BSTR);
		cvr2.ChangeType(VT_BSTR);
		return ::_tcscmp((LPCTSTR)cvr1.bstrVal, (LPCTSTR)cvr2.bstrVal);
	}
	LONGLONG llVal1 = 0, llVal2 = 0;
	if(cvr1.vt == VT_BOOL)		llVal1 = cvr1.boolVal ? 1 : 0;
	else{						ChangeVariantType(cvr1, VT_I8);	llVal1 = cvr1.llVal;}
	if(cvr2.vt == VT_BOOL)		llVal2 = cvr2.boolVal ? 1 : 0;
	else{						ChangeVariantType(cvr2, VT_I8);	llVal2 = cvr2.llVal;}
	return llVal1 - llVal2;
}

CString CGbl::GetCPUID()
{
	CString strCPU = _T("PMLowLevel");
	/*int iEAXValue,iEBXValue,iECXValue,iEDXValue;
	_asm
	{
		mov eax,1
		cpuid
		mov iEAXValue,eax
		mov iEBXValue,ebx
		mov iECXValue,ecx
		mov iEDXValue,edx
	}
	if(iEDXValue & 0x800000)
	{
		strCPU.Format(_T("%.8X%.8X"), iEDXValue, iEAXValue);
	}
	_asm
	{
		mov eax,2
		CPUID
	}
	*/return strCPU;
}

//!< Get system path, include '\', CSIDL_XXX
CString CGbl::GetSpecialPath(int id)
{
	TCHAR szDocument[MAX_PATH] = {0};
	CString path;

	LPITEMIDLIST pidl = NULL;
	SHGetSpecialFolderLocation(NULL, CSIDL_PERSONAL, &pidl);
	if(pidl && SHGetPathFromIDList(pidl, szDocument))
	{
		path = szDocument;
		if(path.Right(1) != _T("\\"))
			path = path + _T("\\");
	}
	return path;
}

bool CGbl::ReadFile2Binary(CString pathName, char** data, int& len)
{
	CFile file;
	CString strText;
	char* pData = NULL;
	try
	{
		file.Open(pathName, CFile::modeRead | CFile::shareDenyRead | CFile::typeBinary);
		len = file.GetLength();
		pData = new char[len];
		memset(pData, 0, len);
		file.Read(pData, len);
		file.Close();
	}
	catch (CFileException* e)
	{
		TCHAR szError[INIT_1K];
		e->GetErrorMessage(szError, INIT_1K);
		AfxMessageBox(szError);
		return false;
	}
	catch (...)
	{
		if (pData)
			delete[] pData;
		pData = NULL;
		return false;
	}
	*data = pData;
	return true;
}

bool CGbl::WriteBinary2File(CString pathName, char* data, int len)
{
	CFile file;
	CFileException ex;
	if(!CGbl::MakeDir(GetPathFromFilePathName(pathName)))
	{
		AfxMessageBox(_T("Create path failed"));
		return false;
	}
	if(FALSE == file.Open(pathName, CFile::modeCreate | CFile::modeReadWrite | CFile::typeBinary, &ex))
	{
		TCHAR szError[INIT_1K];
		ex.GetErrorMessage(szError, INIT_1K);
		AfxMessageBox(szError);
		return false;
	}
	file.Write(data, len);
	file.Close();
	return true;
}
CString CGbl::UintToCString(UINT number)
{
	CString temp;
	temp.Format(_T("%d"),number);
	return temp;
}

bool CGbl::IsNumber(CString str)
{
	int n=str.GetLength();
	int i;
	if (n<=0) 
		return false;
	for(i=0;i<n;i++) 
	{
		if((str.GetAt(i)<'0')||(str.GetAt(i)>'9'))
		{
			if((str.GetAt(i)=='.'))  continue;
			return false;
		}
	}
	return true;
}