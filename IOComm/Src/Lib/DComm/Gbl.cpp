#include "stdafx.h"
#include "Gbl.h"

#include <winsock2.h>
#pragma comment(lib,"ws2_32")
#pragma comment(lib,"Version.lib")
#include <direct.h>

HANDLE m_hReport;					//!< Report框的窗体句柄
const bool m_bShowReport = false;	//!< 是否显示Report

CGbl::CGbl(void)
{
	//!< 计算目录
	TCHAR exePath[MAX_PATH];
	::ZeroMemory(exePath,sizeof(TCHAR)*MAX_PATH);
	::GetModuleFileName(NULL,exePath,MAX_PATH);
	m_strExePath = exePath;
	m_strExePath = m_strExePath.Left(m_strExePath.ReverseFind('\\'));		//!< 解析完的程序路径后边没有‘\’
	m_strInstallPath = m_strExePath.Left(m_strExePath.ReverseFind('\\')+1);	//!< 安装程序目录
	m_strExePath += _T('\\');												//!< 可执行程序的目录，后边+‘\’
	m_strSlnPath = m_strInstallPath + _T("Solution\\");						//!< 解决方案默认路径
	m_strDataPath = m_strInstallPath + _T("Data\\");						//!< 主描述文件路径
	m_strDocPath = m_strInstallPath + _T("Doc\\");							//!< 帮助文件路径
	m_strProjDefPath = m_strInstallPath + _T("Projects\\");					//!< 工程默认路径

	InitReport();
}

CGbl::~CGbl(void){}

CGbl& CGbl::GetMe()
{
	static CGbl me;
	return me;
}

//!< 获得软件版本号,type表示版本类型，0全部，1高版本号，2中版本号，3低版本号
const CString CGbl::getVersionStr(UINT type/* = 0*/)
{
	TCHAR szFullPath[MAX_PATH];
	DWORD dwVerInfoSize = 0;
	DWORD dwVerHnd;   
	VS_FIXEDFILEINFO *pFileInfo;   

	GetModuleFileName(NULL, szFullPath, sizeof(szFullPath));
	dwVerInfoSize = GetFileVersionInfoSize(szFullPath, &dwVerHnd);
	CString ver[4] = {_T(""), _T("1"), _T("0"), _T("0")};
	if(dwVerInfoSize)
	{
		// If we were able to get the information, process it:
		HANDLE hMem;
		LPVOID lpvMem;
		unsigned int uInfoSize = 0;

		hMem = GlobalAlloc(GMEM_MOVEABLE, dwVerInfoSize);
		lpvMem = GlobalLock(hMem);
		GetFileVersionInfo(szFullPath, dwVerHnd, dwVerInfoSize, lpvMem);
		::VerQueryValue(lpvMem, (LPTSTR)_T("\\"), (void**)&pFileInfo, &uInfoSize);
		WORD m_nProdVersion[4];

		// Product version from the FILEVERSION of the version info resource
		m_nProdVersion[0] = HIWORD(pFileInfo->dwProductVersionMS);
		m_nProdVersion[1] = LOWORD(pFileInfo->dwProductVersionMS);
		m_nProdVersion[2] = HIWORD(pFileInfo->dwProductVersionLS);
		m_nProdVersion[3] = LOWORD(pFileInfo->dwProductVersionLS);
		ver[1].Format("%d",m_nProdVersion[0]);
		ver[2].Format("%d",m_nProdVersion[1]);
		ver[3].Format("%d",m_nProdVersion[2]);
	}

	ver[0] = ver[1] + _T(".") + ver[2] + _T(".") + ver[3];
	if(type > 3)	return ver[0];
	else			return ver[type];
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
	SpliteBy(text,"/",vtStr);
	if(vtStr.size() != 3)	return false;
	st.wMonth = atoi(vtStr[0]);
	st.wDay = atoi(vtStr[1]);

	SpliteBy(vtStr[2],":",vtStr);
	if(vtStr.size() != 3)	return false;
	CString year_hour = vtStr[0];
	CString second_mesc = vtStr[2];
	st.wMinute = atoi(vtStr[1]);

	SpliteBy(year_hour," ",vtStr);
	if(vtStr.size() != 2)	return false;
	st.wYear = atoi(vtStr[0]);
	st.wHour = atoi(vtStr[1]);

	SpliteBy(second_mesc," ",vtStr);
	if(vtStr.size() != 2)	return false;
	st.wSecond = atoi(vtStr[0]);
	st.wMilliseconds = atoi(vtStr[1]);

	return true;
}

CString CGbl::GetSystemTime2Str(const SYSTEMTIME& st)
{
	CString text;
	text.Format("%.2d/%.2d/%.4d %.2d:%.2d:%.2d %.3d",st.wMonth,st.wDay,st.wYear,st.wHour,st.wMinute,st.wSecond,st.wMilliseconds);
	return text;
}

//!< 将字符串text以rule字符串中任意一个字符为分隔条件分离，并将结果赋到strList中
void CGbl::SpliteBy(CString text,CString rule,std::vector<CString>& vtStr)
{
	int pos= 0;
	vtStr.clear();
	CString value= text.Tokenize(rule,pos);
	while (value != ""){
		vtStr.push_back(value);
		value= text.Tokenize(rule,pos);
	}
}

bool CGbl::GetCorrectFileName(CString& str, bool showError/* =false */)
{
	CString correctText = str.Trim();
	CString errStr = "\\/:*?\"<>|";
	for(int i = 0; i < str.GetLength(); ++i){
		if(errStr.Find(str.GetAt(i)) != -1){
			if(showError)
				MessageBox(NULL, _T("名称中不能含有 \\ / : * ? \" < > | 等符号"), _T("错误"), MB_OK);
			return false;
		}
	}
	str = correctText;
	return true;
}

//!< 超出部分显示省略号,区分汉字跟字母
CString CGbl::StringFormat(CString str,int n)
{
	int i = 0;
	for(; i < str.GetLength() && i < n;){
		if(IsDBCSLeadByte((byte)str.GetAt(i)))		i += 2;
		else										++i;
	}
	return str.Left(i) + _T("...");
}  

//!< 从文件的全路径名中，获得文件的路径
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
// 
// //!< 获得串口数量
// void CGbl::GetSerialCount(std::list<UINT>& ltComID)
// {
// 	ltComID.clear();
// 	HKEY hKey;
// 	UINT id;
// 	int i = 0;
// 	if( ::RegOpenKeyEx( HKEY_LOCAL_MACHINE,_T("Hardware\\DeviceMap\\SerialComm"),NULL,KEY_READ,&hKey ) == ERROR_SUCCESS ) // 打开串口注册表
// 	{ 
// 		char portName[256],commName[256]; 
// 		DWORD dwLong,dwSize; 
// 		while(1) 
// 		{ 
// 			dwLong = dwSize = sizeof(portName); 
// 			if( ::RegEnumValue(hKey,i,portName,&dwLong,NULL,NULL,(PUCHAR)commName,&dwSize ) == ERROR_NO_MORE_ITEMS ) // 枚举串口 
// 				break; 
// 			CString strComm = commName;
// 			GetNumberFromString(strComm, id, 1);
// 			ltComID.push_back(id);
// 			++i;
// 		} 
// 		RegCloseKey(hKey); 
// 	} 
// }

//!< 由数字类型的IP地址获得字符串式的IP地址
CString CGbl::GetIPFromNumber(UINT number)
{
	byte ipByte[4];
	memcpy(&ipByte[0], &number, 4);
	CString ipItem, ipStr = _T("");
	for(int i = 0; i < 4; ++i){
		ipItem.Format("%d", ipByte[i]);
		if(ipStr == _T(""))		ipStr = ipItem;
		else					ipStr = ipStr + _T(".") + ipItem;
	}
	return ipStr;
}

//!< 获得本机的IP地址
CString CGbl::GetHostIPString()
{
// 	CString str;
// 	struct hostent *host;
// 	host=::gethostbyname("localhost");
// 	host=::gethostbyname(host->h_name);
// 	if(host->h_addr_list[0]!=NULL){
// 		str.Format("%u.%u.%u.%u",(unsigned char)host->h_addr_list[0][0],(unsigned char)host->h_addr_list[0][1],
// 			(unsigned char)host->h_addr_list[0][2],(unsigned char)host->h_addr_list[0][3]);
// 	}
// 	return str;
	WSADATA wsaData;
	WSAStartup(MAKEWORD(2,2),&wsaData);
	BYTE *p;
	char name[128];
	struct hostent *hp;
	CString ip;

	if(gethostname(name,sizeof(name))==0){   
		if((hp=gethostbyname(name))!=0)
		{
			p=(BYTE*)hp->h_addr;
			ip.Format("%d.%d.%d.%d", p[0], p[1],p[2],p[3]);
		}
	}
	return  ip;
}

//!< 有字符串式的IP地址获得数据类型的IP地址
bool CGbl::GetNumberFromIP(CString ip, UINT& uiIP)
{
	std::vector<CString> ltIpItem;
	SpliteBy(ip, _T("."), ltIpItem);
	if(ltIpItem.size() != 4)	return false;
	byte ipItem[4];
	for(int i = 0; i < 4; ++i)
		ipItem[i] = atoi(ltIpItem[i]);

	memcpy(&uiIP, &ipItem[0], 4);
	return true;
}

//!< 给IP加上一个数字，在显示循序上加，比如1.1.1.1 + 1 = 1.1.1.2
UINT CGbl::IPAddNumber(UINT ip, UINT number)
{
	byte *ipItem = (byte *)&ip;
	ip = (UINT)(ipItem[0] * 256 * 256 * 256 + ipItem[1] * 256 * 256 + ipItem[2] * 256 + ipItem[3]);
	ip = (UINT)(ip + number);
	ip = (UINT)(ipItem[0] * 256 * 256 * 256 + ipItem[1] * 256 * 256 + ipItem[2] * 256 + ipItem[3]);
	return ip;
}

//!< 创建多层目录
bool CGbl::MakeDir(CString path)
{
	int i = 0, j = 0, k = 0;
	while(i < path.GetLength()){  
		i = path.Find("\\", j+1);
		if(i == -1)	 			i = path.GetLength();
		short flag = ::_mkdir(path.Left(i).operator LPCTSTR());
		if((k == 2) && (flag == -1)){
			switch(errno) {
				case EEXIST:	break;
				case ENOENT:
					AfxMessageBox("路径不存在，创建失败！");
					return false;
				default:		return false;
			}
		}
		j = i;
		k++;
	}
	return true;
}

//!< 复制整个目录(及其子目录和所有内部文件)到指定目录下,目录最后都有'\'
bool CGbl::CopyDirectoryAll(CString csSrcFolder, CString csNewFolder)
{
	CFileFind floder;
	BOOL bFind=floder.FindFile(csSrcFolder + "*.*");
	while(bFind){
		bFind = floder.FindNextFile();
		TRACE(_T("%s "),floder.GetFileName());

		if(floder.IsDots())	continue;
		if(floder.IsDirectory()){
			_mkdir(csNewFolder+floder.GetFileName());
			CopyDirectoryAll(csSrcFolder + "\\" + floder.GetFileName() + "\\", csNewFolder + floder.GetFileName() + "\\");
		}
		::SetFileAttributes(csSrcFolder + floder.GetFileName(), FILE_ATTRIBUTE_NORMAL);
		::AfxGetApp()->DoWaitCursor(1);
		::CopyFile(csSrcFolder + floder.GetFileName(), csNewFolder + floder.GetFileName(), FALSE);
		::AfxGetApp()->DoWaitCursor(-1);
	}
	return true;
}

//!< 获得当前时间的字符串，参数表示是否需要显示毫秒
CString CGbl::GetCulTime(bool bMillSec /* = true */)
{
	SYSTEMTIME st;
	GetLocalTime(&st);
	CString str;
	if(bMillSec)
	str.Format("%.4d/%.2d/%.2d %.2d:%.2d:%.2d (%.3d)",st.wYear,st.wMonth,st.wDay,st.wHour,st.wMinute,st.wSecond,st.wMilliseconds);
	else
		str.Format("%.4d/%.2d/%.2d %.2d:%.2d:%.2d",st.wYear,st.wMonth,st.wDay,st.wHour,st.wMinute,st.wSecond);
	return str;
}

//!< 强行将一个CComVariant的变量转换成vt
bool CGbl::ChangeVariantType(CComVariant& cvr, UINT vt)
{
	if(cvr.vt == vt)			return true;			//!< 不用转，就是要转的类型
	LRESULT lResult = cvr.ChangeType(vt);
	if(SUCCEEDED(lResult))		return true;			//!< 直接转成功了
	CComVariant cv = cvr;
	lResult = cv.ChangeType(VT_BSTR);
	if(!SUCCEEDED(lResult))		return false;			//!< 没转成功
	CString strValue = (CString)cv.bstrVal;
	LONGLONG llValue = _atoi64(strValue);
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

//!< 比较大小，如果大返回整数，小返回负数，等于返回0
LONGLONG CGbl::VariantCompare(CComVariant cvr1, CComVariant cvr2)
{
	if(cvr1.vt == VT_BSTR || cvr2.vt == VT_BSTR)
	{
		cvr1.ChangeType(VT_BSTR);
		cvr2.ChangeType(VT_BSTR);
		return ::strcmp((LPCTSTR)cvr1.bstrVal, (LPCTSTR)cvr2.bstrVal);
	}
	ChangeVariantType(cvr1, VT_I8);
	ChangeVariantType(cvr2, VT_I8);
	LONGLONG llVal1 = cvr1.llVal;
	LONGLONG llVal2 = cvr2.llVal;
	return llVal1 - llVal2;
}

void CGbl::ShowHelp(int id /* = -1 */)
{
	CString strHelpPath = CGbl::GetMe().getDocPath() + _T("DDS系列程序 使用说明.doc");
	::ShellExecute(NULL, _T("open"), strHelpPath, _T(""), _T(""), SW_NORMAL);
}

//!< 显示输出栏
void CGbl::InitReport()
{
	if(m_bShowReport)
	{
		::AllocConsole();
		FILE* pStream;
		freopen_s(&pStream, "CONOUT$", "w", stdout);
		m_hReport = GetStdHandle(STD_OUTPUT_HANDLE);
		DebugMsg(_T("程序启动。。。\r\n"), REPORT_YELLOW, true);
	}
}

//!< 打印信息
void CGbl::DebugMsg(CString strMsg, WORD color, bool bIntensity)
{
	if(!m_bShowReport)									return;
	if(strMsg == _T(""))								return;
	if(color == REPORT_RED)								color = FOREGROUND_RED;
	else if(color == REPORT_GREEN)						color = FOREGROUND_GREEN;
	else if(color == REPORT_BLUE)						color = FOREGROUND_BLUE;
	else if(color == REPORT_YELLOW)						color = FOREGROUND_RED   | FOREGROUND_GREEN;
	else if(color == REPORT_PURPLE)						color = FOREGROUND_RED   | FOREGROUND_BLUE;
	else if(color == REPORT_WATERBLUE)					color = FOREGROUND_GREEN | FOREGROUND_BLUE;
	else if(color == REPORT_WHITE)						color = FOREGROUND_RED   | FOREGROUND_GREEN | FOREGROUND_BLUE;
	else												color = FOREGROUND_RED   | FOREGROUND_GREEN | FOREGROUND_BLUE;
	if(bIntensity)										color |= FOREGROUND_INTENSITY;
	::SetConsoleTextAttribute(m_hReport, color);
	printf(strMsg);
}

//!< 打印信息
void CGbl::DebugMsg(CString strMsg, byte* data, int len, WORD color, bool bIntensity)
{
	if(!m_bShowReport)									return;
	if(strMsg == _T("") && len == 0)					return;
	CString str;
	for(int i = 0; i < len; ++i)
	{
		str.Format("%.2X ", data[i]);
		strMsg += str;
	}
	DebugMsg(strMsg + _T("\r\n"), color, bIntensity);
}

//!< Crc校验
short CGbl::CRC16(unsigned char* puchMsg, unsigned short usDataLen)/* message to calculate CRC upon */
/* quantity of bytes in message */
{
	static  unsigned char auchCRCLo[] = {
		0x00, 0xC0, 0xC1, 0x01, 0xC3, 0x03, 0x02, 0xC2, 0xC6, 0x06, 0x07, 0xC7, 0x05, 0xC5, 0xC4,
		0x04, 0xCC, 0x0C, 0x0D, 0xCD, 0x0F, 0xCF, 0xCE, 0x0E, 0x0A, 0xCA, 0xCB, 0x0B, 0xC9, 0x09,
		0x08, 0xC8, 0xD8, 0x18, 0x19, 0xD9, 0x1B, 0xDB, 0xDA, 0x1A, 0x1E, 0xDE, 0xDF, 0x1F, 0xDD,
		0x1D, 0x1C, 0xDC, 0x14, 0xD4, 0xD5, 0x15, 0xD7, 0x17, 0x16, 0xD6, 0xD2, 0x12, 0x13, 0xD3,
		0x11, 0xD1, 0xD0, 0x10, 0xF0, 0x30, 0x31, 0xF1, 0x33, 0xF3, 0xF2, 0x32, 0x36, 0xF6, 0xF7,
		0x37, 0xF5, 0x35, 0x34, 0xF4, 0x3C, 0xFC, 0xFD, 0x3D, 0xFF, 0x3F, 0x3E, 0xFE, 0xFA, 0x3A,
		0x3B, 0xFB, 0x39, 0xF9, 0xF8, 0x38, 0x28, 0xE8, 0xE9, 0x29, 0xEB, 0x2B, 0x2A, 0xEA, 0xEE,
		0x2E, 0x2F, 0xEF, 0x2D, 0xED, 0xEC, 0x2C, 0xE4, 0x24, 0x25, 0xE5, 0x27, 0xE7, 0xE6, 0x26,
		0x22, 0xE2, 0xE3, 0x23, 0xE1, 0x21, 0x20, 0xE0, 0xA0, 0x60, 0x61, 0xA1, 0x63, 0xA3, 0xA2,
		0x62, 0x66, 0xA6, 0xA7, 0x67, 0xA5, 0x65, 0x64, 0xA4, 0x6C, 0xAC, 0xAD, 0x6D, 0xAF, 0x6F,
		0x6E, 0xAE, 0xAA, 0x6A, 0x6B, 0xAB, 0x69, 0xA9, 0xA8, 0x68, 0x78, 0xB8, 0xB9, 0x79, 0xBB,
		0x7B, 0x7A, 0xBA, 0xBE, 0x7E, 0x7F, 0xBF, 0x7D, 0xBD, 0xBC, 0x7C, 0xB4, 0x74, 0x75, 0xB5,
		0x77, 0xB7, 0xB6, 0x76, 0x72, 0xB2, 0xB3, 0x73, 0xB1, 0x71, 0x70, 0xB0, 0x50, 0x90, 0x91,
		0x51, 0x93, 0x53, 0x52, 0x92, 0x96, 0x56, 0x57, 0x97, 0x55, 0x95, 0x94, 0x54, 0x9C, 0x5C,
		0x5D, 0x9D, 0x5F, 0x9F, 0x9E, 0x5E, 0x5A, 0x9A, 0x9B, 0x5B, 0x99, 0x59, 0x58, 0x98, 0x88,
		0x48, 0x49, 0x89, 0x4B, 0x8B, 0x8A, 0x4A, 0x4E, 0x8E, 0x8F, 0x4F, 0x8D, 0x4D, 0x4C, 0x8C,
		0x44, 0x84, 0x85, 0x45, 0x87, 0x47, 0x46, 0x86, 0x82, 0x42, 0x43, 0x83, 0x41, 0x81, 0x80,
		0x40
	} ;
	static unsigned char auchCRCHi[] = {
		0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81,
		0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,
		0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01,
		0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,
		0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81,
		0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0,
		0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01,
		0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
		0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81,
		0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,
		0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01,
		0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
		0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81,
		0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,
		0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01,
		0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
		0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81,
		0x40
	} ;
	unsigned char uchCRCHi = 0xFF ; /* high byte of CRC initialized */
	unsigned char uchCRCLo = 0xFF ; /* low byte of CRC initialized */
	unsigned uIndex ; /* will index into CRC lookup table */
	while (usDataLen--) /* pass through message buffer */
	{
		uIndex = uchCRCHi ^ *puchMsg++ ; /* calculate the CRC *///这几条没看明白
		uchCRCHi = uchCRCLo ^ auchCRCHi[uIndex];
		uchCRCLo = auchCRCLo[uIndex] ;
	}
	return (uchCRCHi << 8 | uchCRCLo) ;
}