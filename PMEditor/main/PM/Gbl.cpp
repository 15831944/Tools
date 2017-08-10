#include "stdafx.h"
#include "DXPEditor.h"
#include "MainFrm.h"
#include "SoftInfo.h"
#include "ItemGrid.h"
#include "ItemMgr.h"

//#include <boost/regex.hpp>
#include <wchar.h>
#include <xstring>
#include <winsock2.h>
#pragma comment(lib,"ws2_32")
#pragma comment(lib,"Version.lib")
#include <direct.h>
#include "Gbl.h"

#include <shlobj.h>

CGbl::CGbl(void)
:m_strComplierName(_T("CommCompl.exe"))		//!< 编译器的文件名，包括扩展名，但不包括路径
,m_strServerName(_T("DXP.exe"))				//!< 服务器的文件名，包括扩展名，但不包括路径
,m_strHmiName(_T("HMI\\DHMI.exe"))			//!< HMI的文件名，包括扩展名，但不包括路径
,m_strConfigName(_T("DConfig.exe"))			//!< Config的文件名，包括扩展名，但不包括路径
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
	m_strLanguagePath = m_strInstallPath + _T("Language\\");				//!< 语言包路径
	m_strProjDefPath = m_strInstallPath + _T("Projects\\");					//!< 工程默认路径

	TCHAR szDocPath[MAX_PATH];
	if (SUCCEEDED(::SHGetFolderPath(NULL, CSIDL_PERSONAL | CSIDL_FLAG_CREATE, NULL, 0, szDocPath)))
	{
		m_strProjDefPath = CString(szDocPath) + _T("\\DView\\Projects\\");					//!< 工程默认路径
	}

//	m_uiMaxItemNum = 65535;
	m_uiMaxGroupNum = 1024;
	m_uiMaxDevNum = 1024;

	m_ltBaudStr.push_back(_T("1200"));
	m_ltBaudStr.push_back(_T("2400"));
	m_ltBaudStr.push_back(_T("4800"));
	m_ltBaudStr.push_back(_T("9600"));
	m_ltBaudStr.push_back(_T("19200"));
	m_ltBaudStr.push_back(_T("38400"));
	m_ltBaudStr.push_back(_T("57600"));
	m_ltBaudStr.push_back(_T("115200"));

	m_ltCheckStyleStr.push_back(_T("无"));
	m_ltCheckStyleStr.push_back(_T("奇"));
	m_ltCheckStyleStr.push_back(_T("偶"));
	m_ltCheckStyleStr.push_back(_T("空格"));
	m_ltCheckStyleStr.push_back(_T("符号"));

	m_ltDataBitNumStr.push_back(_T("8"));
	m_ltDataBitNumStr.push_back(_T("7"));
	m_ltDataBitNumStr.push_back(_T("6"));
	m_ltDataBitNumStr.push_back(_T("5"));

	m_ltStopBitNumStr.push_back(_T("1"));
	m_ltStopBitNumStr.push_back(_T("2"));
}

CGbl::~CGbl(void){}

CGbl& CGbl::GetMe()
{
	static CGbl me;
	return me;
}

//!< 获得软件版本号,type表示版本类型，0全部，1高版本号，2中版本号
const CString CGbl::getVersionStr(UINT type/* = 0*/)
{
	CString ver[4] = {_T(""), _T("2"), _T("6"), _T("2.02091")};
	//!< 如果要从注册表中读版本号
// 	HKEY hKey;
// 	UINT id;
// 	int i = 0;
// 	bool bReadSucceed = false;
// 	if(::RegOpenKeyEx(HKEY_LOCAL_MACHINE, _T("Software\\Microsoft\\Windows\\CurrentVersion\\Uninstall\\DView"),
// 		NULL, KEY_READ, &hKey) == ERROR_SUCCESS) // 打开串口注册表
// 	{
// 		char softName[256];
// 		char softVersion[256];
// 		DWORD dwLong,dwSize;
// 		dwLong = dwSize = 256;
// 		
// 		// 获取版本号
// 		if(::RegQueryValueEx(hKey, "DisplayVersion", NULL, &dwLong, (LPBYTE)softVersion, &dwSize) != ERROR_NO_MORE_ITEMS)
// 		{
// 			CString strVersion = softVersion;
// 			std::vector<CString> vtStr;
// 			SpliteBy(strVersion, ".", vtStr);
// 			if(vtStr.size() == 2 || vtStr.size() == 3)
// 			{
// 				bReadSucceed = true;
// 				ver[1] = vtStr[0];
// 				ver[2] = vtStr[1];
// 			}
// 		}
// 		RegCloseKey(hKey);
// 	}
// 	//!< 如果注册表读不到,从软件属性中读版本号
// 	if(!bReadSucceed)
// 	{
// 		TCHAR szFullPath[MAX_PATH];
// 		DWORD dwVerInfoSize = 0;
// 		DWORD dwVerHnd;   
// 		VS_FIXEDFILEINFO *pFileInfo;   
// 
// 		GetModuleFileName(NULL, szFullPath, sizeof(szFullPath));
// 		dwVerInfoSize = GetFileVersionInfoSize(szFullPath, &dwVerHnd);
// 		if(dwVerInfoSize)
// 		{
// 			// If we were able to get the information, process it:
// 			HANDLE hMem;
// 			LPVOID lpvMem;
// 			unsigned int uInfoSize = 0;
// 
// 			hMem = GlobalAlloc(GMEM_MOVEABLE, dwVerInfoSize);
// 			lpvMem = GlobalLock(hMem);
// 			GetFileVersionInfo(szFullPath, dwVerHnd, dwVerInfoSize, lpvMem);
// 			::VerQueryValue(lpvMem, (LPTSTR)_T("\\"), (void**)&pFileInfo, &uInfoSize);
// 			WORD m_nProdVersion[4];
// 
// 			// Product version from the FILEVERSION of the version info resource
// 			m_nProdVersion[0] = HIWORD(pFileInfo->dwProductVersionMS);
// 			m_nProdVersion[1] = LOWORD(pFileInfo->dwProductVersionMS);
// 			m_nProdVersion[2] = HIWORD(pFileInfo->dwProductVersionLS);
// 			m_nProdVersion[3] = LOWORD(pFileInfo->dwProductVersionLS);
// 			ver[1].Format("%d", m_nProdVersion[0]);
// 			ver[2].Format("%d", m_nProdVersion[1]);
// 			ver[3].Format("%d", m_nProdVersion[2]);
// 		}
// 	}

	ver[0] = ver[1] + _T(".") + ver[2] + _T(".") + ver[3];
	if(type > 3)	return ver[0];
	else			return ver[type];
}

//!< 获得SVN号,这个需要自己算一下
const CString CGbl::getSVNStr()
{
	//!< 这个是我要提交的SVN版本号, 以后可根据这个追溯到对应的SVN内容,
	//!< 以后增加也不要将原来的删除, 以备后用
	return _T("1172");
}

const CString CGbl::getExpText(UINT id)
{
	switch(id)
	{
	case 1:		return _T("xml");		break;
	case 2:		return _T("dsl");		break;
	case 3:		return _T("txt");		break;
	default:	break;
	}
	return _T("");
}

const CString CGbl::getLanguageText(UINT id)
{
	switch(id)
	{
	case 1:		return _T("请选择文件夹");			break;
	case 2:		return _T("工程名不能为空！");		break;
	case 3:		return _T("创建解决方案");			break;
	case 4:		return _T("确定");					break;
	case 5:		return _T("取消");					break;
	case 6:		return _T("重试");					break;
	case 7:		return _T("忽略");					break;
	case 8:		return _T("工作区");				break;
	case 9:		return _T("类视图");				break;
	case 10:	return _T("名称");					break;
	case 11:	return _T("地址");					break;
	case 12:	return _T("作者");					break;
	case 13:	return _T("说明");					break;
	case 14:	return _T("密码");					break;
	case 15:	return _T("系统");					break;
	case 16:	return _T("编译");					break;
	case 17:	return _T("查找");					break;
	case 18:	return _T("日志");					break;
	case 19:	return _T("输出窗口");				break;
	case 20:	return _T("");						break;
	case 21:	return _T("");						break;
	case 22:	return _T("");						break;
	case 23:	return _T("");						break;
	case 24:	return _T("");						break;
	default:	break;
	}
	return _T("");
}

//!< 获得波特率索引
UINT CGbl::getBaudIndex(CString str)
{
	UINT num = 0;
	for (CString text : m_ltBaudStr)
	{
		if(text == str)		return num;
		else				++num;
	}
	return 0;
}

//!< 获得校验类型索引
UINT CGbl::getCheckIndex(CString str)
{
	UINT num = 0;
	for (CString text : m_ltCheckStyleStr)
	{
		if(text == str)		return num;
		else				++num;
	}
	return 0;
}

//!< 获得数据位索引
UINT CGbl::getDataIndex(CString str)
{
	UINT num = 0;
	for (CString text : m_ltDataBitNumStr)
	{
		if(text == str)		return num;
		else				++num;
	}
	return 0;
}

//!< 获得停止位索引
UINT CGbl::getStopIndex(CString str)
{
	UINT num = 0;
	for (CString text : m_ltStopBitNumStr)
	{
		if(text == str)		return num;
		else				++num;
	}
	return 0;
}

//!< 获得波特率
CString CGbl::getBaudStr(int index)
{
	UINT num = 0;
	for (CString str : m_ltBaudStr)
		if(num++ == index)
			return str;
	return _T("");
}

//!< 获得校验类型
CString CGbl::getCheckStyleStr(int index)
{
	UINT num = 0;
	for (CString str : m_ltCheckStyleStr)
		if(num++ == index)
			return str;
	return _T("");
}

//!< 获得数据位
CString CGbl::getDataBitNumStr(int index)
{
	UINT num = 0;
	for (CString str : m_ltDataBitNumStr)
		if(num++ == index)
			return str;
	return _T("");
}

//!< 获得停止位
CString CGbl::getStopBitNumStr(int index)
{
	UINT num = 0;
	for (CString str : m_ltStopBitNumStr)
		if(num++ == index)
			return str;
	return _T("");
}

//!< 锁住不让输出框刷新
void CGbl::PrintLock(bool bLock)
{
	CMainFrame* mf = (CMainFrame*)g_App.GetMainWnd();
	if(mf)		mf->GetOutPut()->LockUpdate(bLock);
}

//!< 清空输出栏
void CGbl::PrintClear()
{
	CMainFrame* mf = (CMainFrame*)g_App.GetMainWnd();
	if(mf)		mf->GetOutPut()->Clear();
}

//!< 输出文本，最后一个参数表示是否允许出现2次相同的提示，true表示允许
void CGbl::PrintOut(CString text,bool fresh/* =false */, bool twice /* = true */)
{
	if(!SoftInfo::CSoftInfo::GetMe().isOutPut())			return;
	CMainFrame* mf = (CMainFrame*)g_App.GetMainWnd();
	if(mf)
	{
		if(fresh)		mf->GetOutPut()->Clear();
		if(text == _T(""))									return;
		std::vector<CString> vtStr;
		SpliteBy(text, _T(":"), vtStr);
		if(vtStr.size() < 3)
		{
			mf->GetOutPut()->PrintOut(text);
		}
		else
		{
			UINT type = 3;
			if(vtStr[0].MakeUpper() == _T("INFORMATION"))	type = 0;
			else if(vtStr[0].MakeUpper() == _T("WARNING"))	type = 1;
			else if(vtStr[0].MakeUpper() == _T("ERROR"))	type = 2;
			else											goto OUT_PUT_NORMAL;
			int number = -1;
			bool bResult = GetNumberFromString(vtStr[1], number, 1);
			if(!bResult)									goto OUT_PUT_NORMAL;

			text.Format("%d", number);
			mf->GetOutPut()->PrintOut(type, vtStr[1].Trim(), text, vtStr.back());
		}
	}
	return;

OUT_PUT_NORMAL:
	if(mf)	mf->GetOutPut()->PrintOut(text, twice);
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

//!< 将字符串text以rule字符串作为分隔条件分离，并将结果赋到vtList中
void CGbl::SpliteBy(CString text,CString rule,std::vector<CString>& vtStr)
{
// 	int pos = 0;
// 	vtStr.clear();
// 	CString value = text.Tokenize(rule, pos);
// 	while (value != _T("")){
// 		vtStr.push_back(value);
// 		value = text.Tokenize(rule, pos);
// 	}
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


//!< 将浮点数从字符串中分离出去，参数type = 0 表示从左提取； = 1 表示从右提取
bool CGbl::GetNumberFromString(CString& str, int& number, int type)
{
	std::string strRegex, strMatch;
	if(type == 0)								strRegex = _T("^(\\d*)(\\w*?.*?\\w*?)$");
	else if(type == 1)							strRegex = _T("^(\\w*?.*?\\w*?)(\\d*)$");
	else										return false;
	if(str == _T(""))							return false;
	std::vector<std::string> vtStr;
	strMatch = (LPCTSTR)str;
	if(!CGbl::GetMe().RegexMatch(strRegex, strMatch, vtStr))	return false;
	CString strNumber;
	if(type == 0)	{str = vtStr[2].c_str();	strNumber = vtStr[1].c_str();}
	else			{str = vtStr[1].c_str();	strNumber = vtStr[2].c_str();}
	number = atoi(strNumber);
	if(type == 0)		return true;
	//!< 如果是从右侧提取，有点讲究，就是遇到a01这种情况怎么办
	while(!strNumber.IsEmpty() && strNumber != _T("0"))
	{
		if(strNumber.Left(1) != _T("0"))		break;
		str += _T("0");
		strNumber = strNumber.Right(strNumber.GetLength() - 1);
	}
	return true;
}

//!< 将浮点数从字符串中分离出去，参数type = 0 表示从左提取； = 1 表示从右提取
void CGbl::GetFloatFromString(CString& str, CString& strInt, CString& strDec, UINT type/*=0*/)
{
	strInt = _T("0");
	strDec = _T("0");
	std::string strRegex, strMatch;
	if(type == 0)								strRegex = _T("^(\\d*)(\\.?)(\\d*)(\\w*?.*?\\w*?)$");
	else if(type == 1)							strRegex = _T("^(\\w*?.*?\\w*?)(\\d*)(\\.?)(\\d*)$");
	else										return;
	if(str == _T(""))							return;
	std::vector<std::string> vtStr;
	strMatch = (LPCTSTR)str;
	if(!CGbl::GetMe().RegexMatch(strRegex, strMatch, vtStr))	return;
	if(type == 0)	{str = vtStr[4].c_str();	strInt = vtStr[1].c_str();	strDec = vtStr[3].c_str();}	//!< vtStr[2]是小数点
	else			{str = vtStr[1].c_str();	strInt = vtStr[2].c_str();	strDec = vtStr[4].c_str();}	//!< vtStr[3]是小数点
}


bool CGbl::GetCorrectFileName(CString& str, bool showError/* =false */)
{
	CString correctText = str.Trim();
	CString errStr = _T("\\/:*?\"<>|~.");
	for(int i = 0; i < str.GetLength(); ++i){
		if(errStr.Find(str.GetAt(i)) != -1){
			if(showError)
				MessageBox(NULL, _T("名称中不能含有 \\ / : * ? \" < > | ~ . 等符号"), _T("错误"), MB_OK | MB_ICONERROR);
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

//!< 获得串口数量
void CGbl::GetSerialCount(std::list<UINT>& ltComID)
{
	ltComID.clear();
	HKEY hKey;
	int id;
	int i = 0;
	if( ::RegOpenKeyEx( HKEY_LOCAL_MACHINE,_T("Hardware\\DeviceMap\\SerialComm"),NULL,KEY_READ,&hKey ) == ERROR_SUCCESS ) // 打开串口注册表
	{ 
		char portName[256],commName[256]; 
		DWORD dwLong,dwSize; 
		while(1) 
		{ 
			dwLong = dwSize = sizeof(portName); 
			if( ::RegEnumValue(hKey,i,portName,&dwLong,NULL,NULL,(PUCHAR)commName,&dwSize ) == ERROR_NO_MORE_ITEMS ) // 枚举串口 
				break; 
			CString strComm = commName;
			GetNumberFromString(strComm, id, 1);
			ltComID.push_back(id);
			++i;
		} 
		RegCloseKey(hKey); 
	} 
}

//!< 由数字类型的IP地址获得字符串式的IP地址
CString CGbl::GetIPFromNumber(UINT number)
{
// 	byte ipByte[4];
// 	memcpy(&ipByte[0], &number, 4);
// 	CString ipItem, ipStr = _T("");
// 	for(int i = 0; i < 4; ++i){
// 		ipItem.Format("%d", ipByte[i]);
// 		if(ipStr == _T(""))		ipStr = ipItem;
// 		else					ipStr = ipStr + _T(".") + ipItem;
// 	}
	byte* pByte = (byte*)&number;
	CString ipStr;
	ipStr.Format("%d.%d.%d.%d",pByte[0],pByte[1],pByte[2],pByte[3]);
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
	CString ip = _T("localhost");

	if(gethostname(name,sizeof(name))==0){   
		if((hp=gethostbyname(name))!=0)
		{
			if(WSAGetLastError() == 0)
			{
				p=(BYTE*)hp->h_addr;
				ip.Format("%d.%d.%d.%d", p[0], p[1],p[2],p[3]);
			}
		}
	}
	return  ip;
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
		ipItem[i] = atoi(ltIpItem[i]);

	memcpy(&number, &ipItem[0], 4);
	return number;
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

//!< 删除path文件夹下所有名称为name的文件
bool CGbl::RemoveFiles(CString path, CString name)
{
	CFileFind finder;
	BOOL bFind = finder.FindFile(path + name + _T(".*"));
	std::list<CString> ltDel;

	// 遍历所有文件
	while (bFind)
	{
		bFind = finder.FindNextFile();
		CString strName = finder.GetFileName();
		// 不能是目录
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

void CGbl::SetProgressRange(int nEnd/* = 1000 */)
{
	CMainFrame* mf = (CMainFrame *)g_App.GetMainWnd();
	mf->m_wndProgCtrl.SetRange32(0, nEnd);
	mf->m_wndProgCtrl.SetPos(0);
}

void CGbl::SetProgressStep(int step)
{
	CMainFrame* mf = (CMainFrame *)g_App.GetMainWnd();
	int culPos = mf->m_wndProgCtrl.GetPos();
	int nLow, nUp;
	mf->m_wndProgCtrl.GetRange(nLow, nUp);
	if((culPos + step) >= nUp)
		SetProgressEnd();
	else
	mf->m_wndProgCtrl.SetPos(culPos + step);
}

void CGbl::SetProgressEnd()
{
	CMainFrame* mf = (CMainFrame *)g_App.GetMainWnd();
	mf->m_wndProgCtrl.SetPos(0);
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
	LONGLONG llVal1 = 0, llVal2 = 0;
	if(cvr1.vt == VT_BOOL)		llVal1 = cvr1.boolVal ? 1 : 0;
	else{						ChangeVariantType(cvr1, VT_I8);	llVal1 = cvr1.llVal;}
	if(cvr2.vt == VT_BOOL)		llVal2 = cvr2.boolVal ? 1 : 0;
	else{						ChangeVariantType(cvr2, VT_I8);	llVal2 = cvr2.llVal;}
	return llVal1 - llVal2;
}

//!< 进行匹配的模板函数
bool CGbl::SearchT(CComVariant cvr, CString strMatch, bool bWhole, bool bCase, bool bRegex, CString strType, CString& strOut)
{
	HRESULT lResult = cvr.ChangeType(VT_BSTR);
	if(!SUCCEEDED(lResult))		return false;
	CString myStr = (CString)cvr.bstrVal;
	bool bMatch = false;
	std::string str = (LPCTSTR)myStr;
	if(bRegex)
	{
		bMatch = CGbl::GetMe().RegexMatch((LPCTSTR)strMatch, str);
	}
	else
	{
		if(!bCase)				{myStr = myStr.MakeUpper();	strMatch = strMatch.MakeUpper();}
		if(bWhole)				bMatch = myStr == strMatch;
		else					bMatch = myStr.Find(strMatch) != -1;
	}
	if(bMatch)					strOut = strOut + strType + _T("匹配； ");
	return bMatch;
}

std::wstring CGbl::Change2WString(std::string str)
{
	int nwlen = mbstowcs(NULL, str.c_str(), str.length());	//!< 计算转换后宽字符串的长度,不包含字符串结束符
	wchar_t *lpwsz= new wchar_t[nwlen + 1];
	int i = mbstowcs(lpwsz, str.c_str(), str.length());		//!< 转换,转换后的字符串有结束符
	lpwsz[nwlen] = 0;
	std::wstring wstr(lpwsz);
	delete []lpwsz;
	return wstr;
}

std::string CGbl::Change2String(std::wstring wstr)
{
	int ilen = wcstombs(NULL, wstr.c_str(), 0);				//!< 计算转换后字符串的长度,不包含字符串结束符
	char *lpsz = new char[ilen + 1];
	int i = wcstombs(lpsz, wstr.c_str(), ilen);				//!< 转换,没有结束符
	lpsz[ilen] = '\0';
	std::string str(lpsz);
	delete []lpsz;
	return str;
}

//!< 匹配正则表达式函数，返回匹配后的分组数据
bool CGbl::RegexMatch(const std::string& strRegex, std::string& strMatch, std::vector<std::string>& vt)
{
	if(strRegex.c_str() == "")							return false;
	vt.clear();
	std::wsmatch what;					//存放匹配结构
	std::wregex expression(CGbl::Change2WString(strRegex));	//初始化正则表达式
	std::wstring wstr = CGbl::Change2WString(strMatch);

	if(!std::regex_match(wstr, what, expression))		return false;
	for(int i = 0; i < what.size(); ++i)
	{
		std::wstring wstr = what.str(i);
		std::string str = CGbl::Change2String(wstr);
		vt.push_back(str);
	}
	return !vt.empty();
}

//!< 匹配正则表达式函数
bool CGbl::RegexMatch(const std::string& strRegex, std::string& strMatch)
{
	std::vector<std::string> vt;
	return RegexMatch(strRegex, strMatch, vt);
}

//!< 获得CPU号
CString CGbl::GetCPUID()
{
	CString strCPU = _T("DcceLowLevel");
	int iEAXValue,iEBXValue,iECXValue,iEDXValue;
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
	return strCPU;
}

//!< 获得操作系统关键路径, 带\, 参数都是系统的以 CSIDL_ 开头, 如CSIDL_PERSONAL 我的文档
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

//!< 从这个文件中获取二进制数据
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

//!< 将二进制数据写入文件中
bool CGbl::WriteBinary2File(CString pathName, char* data, int len)
{
	CFile file;
	CFileException ex;
	if(!CGbl::MakeDir(GetPathFromFilePathName(pathName)))
	{
		AfxMessageBox(_T("创建目录失败"));
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
	temp.Format("%d",number);
	return temp;
}

UINT CGbl::DomainToIp(CString domain,int &ip)
{
	struct hostent* temp_hostent;
	WSAData wsdata;
	ip=0;
	WSAStartup(MAKEWORD(2,0),&wsdata);
	char* temp_domain=domain.GetBuffer(0);
	temp_hostent=gethostbyname(temp_domain);
	if(temp_hostent!=NULL)
	{
		CString strip=inet_ntoa(*(struct	 in_addr *)*temp_hostent->h_addr_list);
		WSACleanup();
		ip=GetNumberFromIP(strip);
		return 1;
	}
	return 0;
}
bool	CGbl::Regexmatch(CString strRegex,CString str)
{
	std::regex reg(strRegex);
	bool b;
	char* cstr;
	cstr = str.GetBuffer(0);
	b = std::regex_match(cstr, reg);
	return b;
}
bool CGbl::RegexIP(CString ip)
{
	return Regexmatch("((25[0-5])|(2[0-4]\\d)|(1\\d\\d)|([1-9]\\d)|\\d)(\\.((25[0-5])|(2[0-4]\\d)|(1\\d\\d)|([1-9]\\d)|\\d)){3}",ip);
}
bool	CGbl::RegexDomain(CString domain)
{
	return Regexmatch("[a-zA-Z0-9][-a-zA-Z0-9]{0,62}(\\.[a-zA-Z0-9][-a-zA-Z0-9]{0,62})+\\.?",domain);
}
//void CGbl::DNSError(int i_error)
//{
//	switch(i_error)
//	{
//	case  WSAHOST_NOT_FOUND  :
//		AfxMessageBox(_T("域名无法找到请重新输入有效域名"), MB_OK | MB_ICONEXCLAMATION);
//		break;
//	case WSANO_DATA	:
//		AfxMessageBox(_T("域名无绑定IP，请联系您的网络供应商"), MB_OK | MB_ICONEXCLAMATION);
//		break;
//	case  WSAENETDOWN:
//		AfxMessageBox(_T("检查到网络子系统的错误"), MB_OK | MB_ICONEXCLAMATION);
//		break;
//	case WSATRY_AGAIN:
//		AfxMessageBox(_T("域名解析失败，请重试"), MB_OK | MB_ICONEXCLAMATION);
//		break;
//	default:
//		AfxMessageBox(_T("请检查您的网络"), MB_OK | MB_ICONEXCLAMATION);
//		break;
//	}
//			
//}

__int64 __nStart = 0;
__int64 __nEnd = 0;
__int64 __nFrequency = 0;

//!< 启动高精度定时器,用于统计程序效率
bool CGbl::StartHighTime()
{
	if (!QueryPerformanceFrequency((LARGE_INTEGER *)&__nFrequency))	return false;
	if (!QueryPerformanceCounter((LARGE_INTEGER *)&__nStart))		return false;
	return true;
}

//!< 获得定时器差值,并设置是否重新计时
double CGbl::GetHighTime(bool bReStart)
{
	if (!QueryPerformanceCounter((LARGE_INTEGER *)&__nEnd))			return -1.0;
	__int64 n = __nEnd - __nStart;
	if (bReStart)	__nStart = __nEnd;
	double t = (double)n / (double)__nFrequency;
	return t;
}
bool CGbl::IsNumber(CString str)
{
	int n=str.GetLength();
	int i;
	if (n<=0) 
		return false;
	for(i=0;i<n;i++) 
	{
		if((str.GetAt(i)<'0')||(str.GetAt(i)>'9'))			//!<支持浮点数
		{
			if((str.GetAt(i)=='.'))  continue;
			return false;
		}
	}
	return true;
}