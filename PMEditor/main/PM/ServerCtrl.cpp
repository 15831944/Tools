#include "StdAfx.h"
#include "PMApp.h"
#include "MainFrm.h"
#include "ProjectMgr.h"
#include "Project.h"
#include "Gbl.h"
#include "DevMgr.h"
//#include "Power.h"
#include "ServerCtrl.h"

using namespace Servers;
using namespace DXP;

const UINT TRY_MAX = 3;							//!< 重试的最大次数

const UINT SERVER_RUNCPL = 3000;				//!< 加载工程并启动，带命令行的
const UINT SERVER_RUN = 3001;					//!< 启动服务器运行
const UINT SERVER_PAUSE = 3002;					//!< 暂停服务器，现在已取消
const UINT SERVER_STOP = 3003;					//!< 停止服务器
const UINT SERVER_STATE = 3004;					//!< 询问服务器当前状态
const UINT SCAN_START = 3005;					//!< 开始扫描
const UINT SCAN_STOP = 3006;					//!< 停止扫描
const UINT FRESH_DEVICE = 3007;					//!< 强制扫描设备
const UINT SERVER_STATE_BACK = 3010	;			//!< 询问服务器当前状态的描述格式返回
const UINT REGIST_SOFT = 3012;					//!< 注册软件
//const UINT TIME_LIMIT = 3010;					//!< 发送时限报文
//const UINT NOTIME_LIMIT = 3011;				//!< 取消时限报文

const UINT LOADINGFILE_OVERTIME_ID = 50;		//!< 默认DXP加载文件的最短时间
const UINT LOADINGFILE_OVERTIME = 5000;			//!< 默认DXP加载文件的最短时间

const CString SERVER_TITLE = _T("DXP");			//!< DXP的title,用这个字符串需要该程序,100年不变

CServerCtrl::CServerCtrl(void)
:Servers::Compile::SNeedRunObj(_T("DXP数据交互平台"))
,m_hServerWnd(NULL)
,m_uiState(3)
,m_uiScanState(0)
,m_bLoadingScanFile(false)
,m_nTryNum(0)									//!< 试验的次数
{
}

CServerCtrl::~CServerCtrl(void)
{
}

//!< 返回单件对象
CServerCtrl& CServerCtrl::GetMe()
{
	static CServerCtrl one;
	return one;
}

//!< 为本对象建立一个窗体句柄,用于传递消息的
void CServerCtrl::InitServerMgr(CWnd* pParent)
{
	HWND hWnd = GetSafeHwnd();
	if(hWnd == NULL)		//!< 要建立窗体句柄，由于肯定这是第一次，所以要查询一下服务程序的运行状态
	{
		Create(NULL, _T("ServerCtrl"), WS_CHILD, CRect(0,0,0,0), pParent, 101);
		HWND sevHwnd = ::FindWindow(NULL, SERVER_TITLE);
		if(sevHwnd)			//!< 不为空，表示服务器已经建立好了，现在就可以查询状态了
		{
			m_hServerWnd = sevHwnd;
		}
		else				//!< 服务器还没建立，需要启动服务器
		{
			CString exePath = CGbl::GetMe().getExePath();						//!< 程序安装路径
			CString exePathName = exePath + CGbl::GetMe().getServerName();		//!< 编译器路径名字
			ShellExecute(NULL, "open", exePathName, _T(""), exePath, SW_NORMAL);
		}
		SetTimer(FIND_STATE, 1000, NULL);
	}
}

BEGIN_MESSAGE_MAP(CServerCtrl, CWnd)
	//{{AFX_MSG_MAP(CPLCSocket)
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

//!< 启动DXP
void CServerCtrl::CreateServer()
{
	CString exePath = CGbl::GetMe().getExePath();						//!< 程序安装路径
	CString exePathName = exePath + CGbl::GetMe().getServerName();		//!< 编译器路径名字
	ShellExecute(NULL, "open", exePathName, _T(""), exePath, SW_MINIMIZE);
	CGbl::GetMe().PrintOut(_T("正在启动服务，请稍候..."), true);
}

void CServerCtrl::OnTimer(UINT nIDEvent)
{
	COPYDATASTRUCT cpd;
	CWnd* cWnd = NULL;
	switch(nIDEvent)
	{
	case FIND_STATE:						//!< 查询服务状态
		if(!FindServer())					break;
		FindState();
		break;

	case RUN_CMDLINE:
		if(RunServer(m_strCmdLine))			KillTimer(RUN_CMDLINE);
		else								++m_nTryNum;
		if(m_nTryNum >= TRY_MAX)
		{
			KillTimer(RUN_CMDLINE);
			MessageBox(_T("启动通信服务失败"), _T("警告"), MB_OK | MB_ICONEXCLAMATION);
		}
		break;

	case RUN_SCANCMDLINE:
		if(ScanStart(m_strScanCmdLine))		KillTimer(RUN_SCANCMDLINE);
		else								++m_nTryNum;
		if(m_nTryNum >= TRY_MAX)
		{
			KillTimer(RUN_SCANCMDLINE);
			MessageBox(_T("启动通信服务失败"), _T("警告"), MB_OK | MB_ICONEXCLAMATION);
		}
		break;

	case LOADINGFILE_OVERTIME_ID:
		SetLoadingFile(false);		//!< 可以再次点击运行或扫描了
		KillTimer(LOADINGFILE_OVERTIME_ID);
		break;
	}
	CWnd::OnTimer(nIDEvent);
}

//!< 查看服务器在不在
bool CServerCtrl::FindServer()
{
	HWND hwnd = ::FindWindow(NULL, SERVER_TITLE);
	m_hServerWnd = hwnd;
	if(!hwnd)
	{
		SetState(3);
		SetScanState(0);
		SetLoadingFile(false);
	}
	return m_hServerWnd;
}

//!< 查看服务器的状态
void CServerCtrl::FindState()
{
	if(!m_hServerWnd)
	{
		SetState(3);
		SetScanState(0);
		SetLoadingFile(false);
	}
	else{
//		if(!CProjectMgr::GetMe().GetProj())		return;
		COPYDATASTRUCT cpd;
		cpd.dwData = SERVER_STATE;
		cpd.cbData = 0;
		cpd.lpData = NULL;
		DWORD dwrst = 0;
		if(::IsWindow(m_hServerWnd))
			::SendMessageTimeout(m_hServerWnd, WM_COPYDATA, (WPARAM)GetParent()->GetSafeHwnd(), (LPARAM)&cpd, SMTO_NORMAL, 500, &dwrst);
	}
}

//!< 启动运行
void CServerCtrl::OnRunSev(CString cmdLine)
{
	if(!m_hServerWnd || !RunServer(cmdLine))
	{
		CreateServer();
		m_nTryNum = 0;
		m_strCmdLine = cmdLine;
		SetTimer(RUN_CMDLINE, 1000, NULL);				//!< 每秒尝试一次启动服务
	}
}

//!< 启动扫描
void CServerCtrl::OnScanStart(CString cmdLine)
{
	if(!m_hServerWnd || !ScanStart(cmdLine))
	{
		CreateServer();
		m_nTryNum = 0;
		m_strScanCmdLine = cmdLine;
		SetTimer(RUN_SCANCMDLINE, 1000, NULL);			//!< 每秒尝试一次启动服务
	}
}

//!< 启动运行
void CServerCtrl::OnRun()
{
	std::shared_ptr<CProject> proj = CProjectMgr::GetMe().GetProj();
	if(!proj)				return;
	OnRunSev(proj->GetWholePathName());
}

//!< 启动运行
bool CServerCtrl::RunServer(CString cmdLine)
{
	if(!m_hServerWnd)		return false;
	COPYDATASTRUCT cpd;
	if(m_uiState == 2)		cpd.dwData = SERVER_RUN;	//!< 从暂停中恢复
	else					cpd.dwData = SERVER_RUNCPL;	//!< 重新运行
	cpd.lpData = (LPSTR)(LPCTSTR)cmdLine;
	cpd.cbData = cmdLine.GetLength();
	CGbl::PrintOut(_T("正在运行服务..."));
	DWORD dwrst = 0;
	if(::IsWindow(m_hServerWnd))
		::SendMessageTimeout(m_hServerWnd, WM_COPYDATA, (WPARAM)GetParent()->GetSafeHwnd(), (LPARAM)&cpd, SMTO_NORMAL, 500, &dwrst);
	FindState();
	return true;
}

//!< 启动停止
void CServerCtrl::OnStopSev()
{
	if(!m_hServerWnd)		return;
	if(m_uiState != 1 && m_uiState != 2)				return;

	COPYDATASTRUCT cpd;
	cpd.dwData = SERVER_STOP;							//!< 停止
	cpd.lpData = _T("");
	cpd.cbData = 0;
	CGbl::PrintOut(_T("正在停止服务..."));
	DWORD dwrst = 0;
	if(::IsWindow(m_hServerWnd))
		::SendMessageTimeout(m_hServerWnd, WM_COPYDATA, (WPARAM)GetParent()->GetSafeHwnd(), (LPARAM)&cpd, SMTO_NORMAL, 500, &dwrst);
	FindState();
}

//!< 开始扫描设备
bool CServerCtrl::ScanStart(CString cmdLine)
{
	FindState();
	if(!m_hServerWnd)						return false;
	if(m_uiState == 1 || m_uiState == 2)	return false;//!< 运行状态不能扫描
	if(IsLoadingFile())
	{
		CGbl::PrintOut(_T("DXP正在加载文件，无法进行扫描"));
		return false;
	}
	COPYDATASTRUCT cpd;
	cpd.dwData = SCAN_START;
	cpd.lpData = (LPSTR)(LPCTSTR)cmdLine;
	cpd.cbData = cmdLine.GetLength();
	DWORD dwrst = 0;
	if(::IsWindow(m_hServerWnd))
		::SendMessageTimeout(m_hServerWnd, WM_COPYDATA, (WPARAM)GetParent()->GetSafeHwnd(), (LPARAM)&cpd, SMTO_NORMAL, 500, &dwrst);
	SetLoadingFile(true);
	SetTimer(LOADINGFILE_OVERTIME_ID, LOADINGFILE_OVERTIME, NULL);
	FindState();
	return true;
}

//!< 停止扫描设备
void CServerCtrl::OnScanStop()
{
	FindState();
	if(m_uiScanState == 0)					return;
	COPYDATASTRUCT cpd;
	cpd.dwData = SCAN_STOP;
	cpd.cbData = 0;
	cpd.lpData = NULL;
	DWORD dwrst = 0;
	if(::IsWindow(m_hServerWnd))
		::SendMessageTimeout(m_hServerWnd, WM_COPYDATA, (WPARAM)GetParent()->GetSafeHwnd(), (LPARAM)&cpd, SMTO_NORMAL, 500, &dwrst);
}

//!< 发送强制扫描设备的功能
void CServerCtrl::SendFreshDev(int devID)
{
	COPYDATASTRUCT cpd;
	cpd.dwData = FRESH_DEVICE;
	cpd.cbData = 4;
	cpd.lpData = &devID;
	DWORD dwrst = 0;
	if(::IsWindow(m_hServerWnd))
		::SendMessageTimeout(m_hServerWnd, WM_COPYDATA, (WPARAM)GetParent()->GetSafeHwnd(), (LPARAM)&cpd, SMTO_NORMAL, 500, &dwrst);
}

//!< 通知DXP打开注册页面
void CServerCtrl::RegistSoft()
{
	COPYDATASTRUCT cpd;
	cpd.dwData = REGIST_SOFT;
	cpd.cbData = 0;
	cpd.lpData = 0;
	DWORD dwrst = 0;
	if(::IsWindow(m_hServerWnd))
		::SendMessageTimeout(m_hServerWnd, WM_COPYDATA, (WPARAM)GetParent()->GetSafeHwnd(), (LPARAM)&cpd, SMTO_NORMAL, 500, &dwrst);
}

//!< 看看是不是自己要的数字
bool CServerCtrl::IsINeed(UINT dwData)
{
	if(dwData == SERVER_STATE)				return true;
	if(dwData == SERVER_STATE_BACK)			return true;
	return false;
}

//!< 接到DXP的事件回应
void CServerCtrl::OnReceive(COPYDATASTRUCT* pCopyDataStruct)
{
	switch(pCopyDataStruct->dwData)
	{
	case SERVER_STATE:			InitState(*pCopyDataStruct);		break;
	case SERVER_STATE_BACK:		InitStateXml(*pCopyDataStruct);		break;
	}
}

//!< 解析返回的DXP状态信息
void CServerCtrl::InitState(COPYDATASTRUCT& pCopyDataStruct)
{
	// 4字节运行状态, 4字节扫描状态, 多字节当前工程路径, 4字节变量限制数量, 多字节当前权限名称
	if(pCopyDataStruct.cbData < 8)		return;
	UINT state = 0, scan_state = 0;
	char *data = new char[pCopyDataStruct.cbData];
	memcpy(data, pCopyDataStruct.lpData, pCopyDataStruct.cbData);
	memcpy(&state, &data[0], 4);
	memcpy(&scan_state, &data[4], 4);
	if(pCopyDataStruct.cbData > 8)
		m_strProjSev = (CString)(char *)&data[8];
	else
		m_strProjSev = _T("");
	delete[] data;
	SetState(state);
	SetScanState(scan_state);
}

//!< 解析返回的DXP描述格式的状态信息
void CServerCtrl::InitStateXml(COPYDATASTRUCT& pCopyDataStruct)
{
	CString strAll = (char*)pCopyDataStruct.lpData;
	if(strAll.IsEmpty())			{ASSERT(FALSE);		return;}
	std::vector<CString> vtStr, vtLR;
	CGbl::SpliteBy(strAll, _T("|"), vtStr);
	if(vtStr.size() < 2)			{ASSERT(FALSE);		return;}
	CString str, strL, strR;

	for (auto str : vtStr)
	{
		vtLR.clear();
		CGbl::SpliteBy(str, _T("?"), vtLR);
		if(vtLR.size() != 2)		continue;
		strL = vtLR[0].Trim();
		strR = vtLR[1].Trim();

		if(strL == _T("RunState")){				SetState(atoi(strR.GetBuffer()));}
		else if(strL == _T("ScanState")){		SetScanState(atoi(strR.GetBuffer()));}
		else if(strL == _T("ProjPathName")){	m_strProjSev = strR;}
	}
}

//!< 设置运行监控状态
void CServerCtrl::SetState(UINT state)
{
	if(state == 1 || state == 2)			SetLoadingFile(false);
	m_uiState = state;
	if(!IsSameProj())
	{
		CProjectMgr::GetMe().SetWatch(false);
		return;
	}
	if(state == 0 || state == 3)			CProjectMgr::GetMe().SetWatch(false);
	else									CProjectMgr::GetMe().SetWatch(true);
}

//!< 设置扫描状态
void CServerCtrl::SetScanState(UINT state)
{
	if(state == 1)							SetLoadingFile(false);
	if(state == 0)							CProjectMgr::GetMe().SetScan(false);
	else									CProjectMgr::GetMe().SetScan(true);
	m_uiScanState = state;
}

//!< 判断是不是同一个工程
bool CServerCtrl::IsSameProj()
{
	if(m_strProjSev == _T(""))				return true;		//!< 为了兼容原来没有读这个信息
	std::shared_ptr<CProject> proj = CProjectMgr::GetMe().GetProj();
	if(!proj)								return false;
	CString str = proj->GetWholePathName();
	if(m_strProjSev == str)					return true;
	return false;
}