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
:m_strComplierName(_T("CommCompl.exe"))		//!< ���������ļ�����������չ������������·��
,m_strServerName(_T("DXP.exe"))				//!< ���������ļ�����������չ������������·��
,m_strHmiName(_T("HMI\\DHMI.exe"))			//!< HMI���ļ�����������չ������������·��
,m_strConfigName(_T("DConfig.exe"))			//!< Config���ļ�����������չ������������·��
{
	//!< ����Ŀ¼
	TCHAR exePath[MAX_PATH];
	::ZeroMemory(exePath,sizeof(TCHAR)*MAX_PATH);
	::GetModuleFileName(NULL,exePath,MAX_PATH);
	m_strExePath = exePath;
	m_strExePath = m_strExePath.Left(m_strExePath.ReverseFind('\\'));		//!< ������ĳ���·�����û�С�\��
	m_strInstallPath = m_strExePath.Left(m_strExePath.ReverseFind('\\')+1);	//!< ��װ����Ŀ¼
	m_strExePath += _T('\\');												//!< ��ִ�г����Ŀ¼�����+��\��
	m_strSlnPath = m_strInstallPath + _T("Solution\\");						//!< �������Ĭ��·��
	m_strDataPath = m_strInstallPath + _T("Data\\");						//!< �������ļ�·��
	m_strDocPath = m_strInstallPath + _T("Doc\\");							//!< �����ļ�·��
	m_strLanguagePath = m_strInstallPath + _T("Language\\");				//!< ���԰�·��
	m_strProjDefPath = m_strInstallPath + _T("Projects\\");					//!< ����Ĭ��·��

	TCHAR szDocPath[MAX_PATH];
	if (SUCCEEDED(::SHGetFolderPath(NULL, CSIDL_PERSONAL | CSIDL_FLAG_CREATE, NULL, 0, szDocPath)))
	{
		m_strProjDefPath = CString(szDocPath) + _T("\\DView\\Projects\\");					//!< ����Ĭ��·��
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

	m_ltCheckStyleStr.push_back(_T("��"));
	m_ltCheckStyleStr.push_back(_T("��"));
	m_ltCheckStyleStr.push_back(_T("ż"));
	m_ltCheckStyleStr.push_back(_T("�ո�"));
	m_ltCheckStyleStr.push_back(_T("����"));

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

//!< �������汾��,type��ʾ�汾���ͣ�0ȫ����1�߰汾�ţ�2�а汾��
const CString CGbl::getVersionStr(UINT type/* = 0*/)
{
	CString ver[4] = {_T(""), _T("2"), _T("6"), _T("2.02091")};
	//!< ���Ҫ��ע����ж��汾��
// 	HKEY hKey;
// 	UINT id;
// 	int i = 0;
// 	bool bReadSucceed = false;
// 	if(::RegOpenKeyEx(HKEY_LOCAL_MACHINE, _T("Software\\Microsoft\\Windows\\CurrentVersion\\Uninstall\\DView"),
// 		NULL, KEY_READ, &hKey) == ERROR_SUCCESS) // �򿪴���ע���
// 	{
// 		char softName[256];
// 		char softVersion[256];
// 		DWORD dwLong,dwSize;
// 		dwLong = dwSize = 256;
// 		
// 		// ��ȡ�汾��
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
// 	//!< ���ע��������,����������ж��汾��
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

//!< ���SVN��,�����Ҫ�Լ���һ��
const CString CGbl::getSVNStr()
{
	//!< �������Ҫ�ύ��SVN�汾��, �Ժ�ɸ������׷�ݵ���Ӧ��SVN����,
	//!< �Ժ�����Ҳ��Ҫ��ԭ����ɾ��, �Ա�����
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
	case 1:		return _T("��ѡ���ļ���");			break;
	case 2:		return _T("����������Ϊ�գ�");		break;
	case 3:		return _T("�����������");			break;
	case 4:		return _T("ȷ��");					break;
	case 5:		return _T("ȡ��");					break;
	case 6:		return _T("����");					break;
	case 7:		return _T("����");					break;
	case 8:		return _T("������");				break;
	case 9:		return _T("����ͼ");				break;
	case 10:	return _T("����");					break;
	case 11:	return _T("��ַ");					break;
	case 12:	return _T("����");					break;
	case 13:	return _T("˵��");					break;
	case 14:	return _T("����");					break;
	case 15:	return _T("ϵͳ");					break;
	case 16:	return _T("����");					break;
	case 17:	return _T("����");					break;
	case 18:	return _T("��־");					break;
	case 19:	return _T("�������");				break;
	case 20:	return _T("");						break;
	case 21:	return _T("");						break;
	case 22:	return _T("");						break;
	case 23:	return _T("");						break;
	case 24:	return _T("");						break;
	default:	break;
	}
	return _T("");
}

//!< ��ò���������
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

//!< ���У����������
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

//!< �������λ����
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

//!< ���ֹͣλ����
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

//!< ��ò�����
CString CGbl::getBaudStr(int index)
{
	UINT num = 0;
	for (CString str : m_ltBaudStr)
		if(num++ == index)
			return str;
	return _T("");
}

//!< ���У������
CString CGbl::getCheckStyleStr(int index)
{
	UINT num = 0;
	for (CString str : m_ltCheckStyleStr)
		if(num++ == index)
			return str;
	return _T("");
}

//!< �������λ
CString CGbl::getDataBitNumStr(int index)
{
	UINT num = 0;
	for (CString str : m_ltDataBitNumStr)
		if(num++ == index)
			return str;
	return _T("");
}

//!< ���ֹͣλ
CString CGbl::getStopBitNumStr(int index)
{
	UINT num = 0;
	for (CString str : m_ltStopBitNumStr)
		if(num++ == index)
			return str;
	return _T("");
}

//!< ��ס���������ˢ��
void CGbl::PrintLock(bool bLock)
{
	CMainFrame* mf = (CMainFrame*)g_App.GetMainWnd();
	if(mf)		mf->GetOutPut()->LockUpdate(bLock);
}

//!< ��������
void CGbl::PrintClear()
{
	CMainFrame* mf = (CMainFrame*)g_App.GetMainWnd();
	if(mf)		mf->GetOutPut()->Clear();
}

//!< ����ı������һ��������ʾ�Ƿ��������2����ͬ����ʾ��true��ʾ����
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

//!< ���ַ���text��rule�ַ�����Ϊ�ָ��������룬�����������vtList��
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


//!< �����������ַ����з����ȥ������type = 0 ��ʾ������ȡ�� = 1 ��ʾ������ȡ
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
	//!< ����Ǵ��Ҳ���ȡ���е㽲������������a01���������ô��
	while(!strNumber.IsEmpty() && strNumber != _T("0"))
	{
		if(strNumber.Left(1) != _T("0"))		break;
		str += _T("0");
		strNumber = strNumber.Right(strNumber.GetLength() - 1);
	}
	return true;
}

//!< �����������ַ����з����ȥ������type = 0 ��ʾ������ȡ�� = 1 ��ʾ������ȡ
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
	if(type == 0)	{str = vtStr[4].c_str();	strInt = vtStr[1].c_str();	strDec = vtStr[3].c_str();}	//!< vtStr[2]��С����
	else			{str = vtStr[1].c_str();	strInt = vtStr[2].c_str();	strDec = vtStr[4].c_str();}	//!< vtStr[3]��С����
}


bool CGbl::GetCorrectFileName(CString& str, bool showError/* =false */)
{
	CString correctText = str.Trim();
	CString errStr = _T("\\/:*?\"<>|~.");
	for(int i = 0; i < str.GetLength(); ++i){
		if(errStr.Find(str.GetAt(i)) != -1){
			if(showError)
				MessageBox(NULL, _T("�����в��ܺ��� \\ / : * ? \" < > | ~ . �ȷ���"), _T("����"), MB_OK | MB_ICONERROR);
			return false;
		}
	}
	str = correctText;
	return true;
}

//!< ����������ʾʡ�Ժ�,���ֺ��ָ���ĸ
CString CGbl::StringFormat(CString str,int n)
{
	int i = 0;
	for(; i < str.GetLength() && i < n;){
		if(IsDBCSLeadByte((byte)str.GetAt(i)))		i += 2;
		else										++i;
	}
	return str.Left(i) + _T("...");
}  

//!< ���ļ���ȫ·�����У�����ļ���·��
CString CGbl::GetPathFromFilePathName(CString pathName)
{
	int i = pathName.ReverseFind('\\');
	if(i == -1)			return _T("");

	CString path = pathName.Left(i + 1);
	return path;
}

//!< ���ļ���ȫ·�����У�����ļ�������
CString CGbl::GetNameFromFilePathName(CString pathName)
{
	CString path = GetPathFromFilePathName(pathName);
	pathName.Delete(0, path.GetLength());
	return pathName;
}

//!< ��ô�������
void CGbl::GetSerialCount(std::list<UINT>& ltComID)
{
	ltComID.clear();
	HKEY hKey;
	int id;
	int i = 0;
	if( ::RegOpenKeyEx( HKEY_LOCAL_MACHINE,_T("Hardware\\DeviceMap\\SerialComm"),NULL,KEY_READ,&hKey ) == ERROR_SUCCESS ) // �򿪴���ע���
	{ 
		char portName[256],commName[256]; 
		DWORD dwLong,dwSize; 
		while(1) 
		{ 
			dwLong = dwSize = sizeof(portName); 
			if( ::RegEnumValue(hKey,i,portName,&dwLong,NULL,NULL,(PUCHAR)commName,&dwSize ) == ERROR_NO_MORE_ITEMS ) // ö�ٴ��� 
				break; 
			CString strComm = commName;
			GetNumberFromString(strComm, id, 1);
			ltComID.push_back(id);
			++i;
		} 
		RegCloseKey(hKey); 
	} 
}

//!< ���������͵�IP��ַ����ַ���ʽ��IP��ַ
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

//!< ��ñ�����IP��ַ
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

//!< ���ַ���ʽ��IP��ַ����������͵�IP��ַ
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

//!< ��IP����һ�����֣�����ʾѭ���ϼӣ�����1.1.1.1 + 1 = 1.1.1.2
UINT CGbl::IPAddNumber(UINT ip, UINT number)
{
	byte *ipItem = (byte *)&ip;
	ip = (UINT)(ipItem[0] * 256 * 256 * 256 + ipItem[1] * 256 * 256 + ipItem[2] * 256 + ipItem[3]);
	ip = (UINT)(ip + number);
	ip = (UINT)(ipItem[0] * 256 * 256 * 256 + ipItem[1] * 256 * 256 + ipItem[2] * 256 + ipItem[3]);
	return ip;
}

//!< �������Ŀ¼
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
					AfxMessageBox("·�������ڣ�����ʧ�ܣ�");
					return false;
				default:		return false;
			}
		}
		j = i;
		k++;
	}
	return true;
}

//!< ɾ��path�ļ�������������Ϊname���ļ�
bool CGbl::RemoveFiles(CString path, CString name)
{
	CFileFind finder;
	BOOL bFind = finder.FindFile(path + name + _T(".*"));
	std::list<CString> ltDel;

	// ���������ļ�
	while (bFind)
	{
		bFind = finder.FindNextFile();
		CString strName = finder.GetFileName();
		// ������Ŀ¼
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

//!< ��������Ŀ¼(������Ŀ¼�������ڲ��ļ�)��ָ��Ŀ¼��,Ŀ¼�����'\'
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

//!< ��õ�ǰʱ����ַ�����������ʾ�Ƿ���Ҫ��ʾ����
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

//!< ǿ�н�һ��CComVariant�ı���ת����vt
bool CGbl::ChangeVariantType(CComVariant& cvr, UINT vt)
{
	if(cvr.vt == vt)			return true;			//!< ����ת������Ҫת������
	LRESULT lResult = cvr.ChangeType(vt);
	if(SUCCEEDED(lResult))		return true;			//!< ֱ��ת�ɹ���
	CComVariant cv = cvr;
	lResult = cv.ChangeType(VT_BSTR);
	if(!SUCCEEDED(lResult))		return false;			//!< ûת�ɹ�
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

//!< �Ƚϴ�С������󷵻�������С���ظ��������ڷ���0
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

//!< ����ƥ���ģ�庯��
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
	if(bMatch)					strOut = strOut + strType + _T("ƥ�䣻 ");
	return bMatch;
}

std::wstring CGbl::Change2WString(std::string str)
{
	int nwlen = mbstowcs(NULL, str.c_str(), str.length());	//!< ����ת������ַ����ĳ���,�������ַ���������
	wchar_t *lpwsz= new wchar_t[nwlen + 1];
	int i = mbstowcs(lpwsz, str.c_str(), str.length());		//!< ת��,ת������ַ����н�����
	lpwsz[nwlen] = 0;
	std::wstring wstr(lpwsz);
	delete []lpwsz;
	return wstr;
}

std::string CGbl::Change2String(std::wstring wstr)
{
	int ilen = wcstombs(NULL, wstr.c_str(), 0);				//!< ����ת�����ַ����ĳ���,�������ַ���������
	char *lpsz = new char[ilen + 1];
	int i = wcstombs(lpsz, wstr.c_str(), ilen);				//!< ת��,û�н�����
	lpsz[ilen] = '\0';
	std::string str(lpsz);
	delete []lpsz;
	return str;
}

//!< ƥ��������ʽ����������ƥ���ķ�������
bool CGbl::RegexMatch(const std::string& strRegex, std::string& strMatch, std::vector<std::string>& vt)
{
	if(strRegex.c_str() == "")							return false;
	vt.clear();
	std::wsmatch what;					//���ƥ��ṹ
	std::wregex expression(CGbl::Change2WString(strRegex));	//��ʼ��������ʽ
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

//!< ƥ��������ʽ����
bool CGbl::RegexMatch(const std::string& strRegex, std::string& strMatch)
{
	std::vector<std::string> vt;
	return RegexMatch(strRegex, strMatch, vt);
}

//!< ���CPU��
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

//!< ��ò���ϵͳ�ؼ�·��, ��\, ��������ϵͳ���� CSIDL_ ��ͷ, ��CSIDL_PERSONAL �ҵ��ĵ�
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

//!< ������ļ��л�ȡ����������
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

//!< ������������д���ļ���
bool CGbl::WriteBinary2File(CString pathName, char* data, int len)
{
	CFile file;
	CFileException ex;
	if(!CGbl::MakeDir(GetPathFromFilePathName(pathName)))
	{
		AfxMessageBox(_T("����Ŀ¼ʧ��"));
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
//		AfxMessageBox(_T("�����޷��ҵ�������������Ч����"), MB_OK | MB_ICONEXCLAMATION);
//		break;
//	case WSANO_DATA	:
//		AfxMessageBox(_T("�����ް�IP������ϵ�������繩Ӧ��"), MB_OK | MB_ICONEXCLAMATION);
//		break;
//	case  WSAENETDOWN:
//		AfxMessageBox(_T("��鵽������ϵͳ�Ĵ���"), MB_OK | MB_ICONEXCLAMATION);
//		break;
//	case WSATRY_AGAIN:
//		AfxMessageBox(_T("��������ʧ�ܣ�������"), MB_OK | MB_ICONEXCLAMATION);
//		break;
//	default:
//		AfxMessageBox(_T("������������"), MB_OK | MB_ICONEXCLAMATION);
//		break;
//	}
//			
//}

__int64 __nStart = 0;
__int64 __nEnd = 0;
__int64 __nFrequency = 0;

//!< �����߾��ȶ�ʱ��,����ͳ�Ƴ���Ч��
bool CGbl::StartHighTime()
{
	if (!QueryPerformanceFrequency((LARGE_INTEGER *)&__nFrequency))	return false;
	if (!QueryPerformanceCounter((LARGE_INTEGER *)&__nStart))		return false;
	return true;
}

//!< ��ö�ʱ����ֵ,�������Ƿ����¼�ʱ
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
		if((str.GetAt(i)<'0')||(str.GetAt(i)>'9'))			//!<֧�ָ�����
		{
			if((str.GetAt(i)=='.'))  continue;
			return false;
		}
	}
	return true;
}