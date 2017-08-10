#include "StdAfx.h"
#include "Gbl.h"
#include "ProjectMgr.h"
#include "Project.h"
#include "Hmi.h"

const CString HMI_TITLE = _T("DHMI 编辑器");

const UINT SYNCHRONIZATION = 1;
const UINT SYNCHRONIZATION_OVER = 2;
const UINT SHELL_TIME = 3;			//!< 启动HMI强行等待的时间

const UINT HMI_RUNPROJ = 4000;
const UINT HMI_STATE = 4001;
const UINT HMI_RUNPROJ_OVER = 4002;
const UINT HMI_CLOSEWITHSAVE = 4004;


using namespace Servers;
using namespace HMI;
void WaitToHmi(CHmi* pHmi, PROCESS_INFORMATION* pPi);
CHmi::CHmi(void)
:Servers::Compile::SNeedRunObj(_T("HMI"))
,m_hHmiWnd(0)						//!< 服务器程序的句柄
,m_ThreadToWati(NULL)
{
	OnHmiQuit();
}

CHmi::~CHmi(void)
{
	//TerminateProcess(m_pi.hProcess, 0);
	//WaitForSingleObject(m_pi.hProcess, INFINITE);		// 等待子进程的退出
}

void CHmi::InitHmiMgr(CWnd* pParent)
{
	HWND hWnd = GetSafeHwnd();
	if(hWnd == NULL){		//!< 要建立窗体句柄，由于肯定这是第一次，所以要查询一下服务程序的运行状态
		CRect rect(0,0,0,0);
		Create(NULL, _T("ServerCtrl"), WS_CHILD, rect, pParent, 101);
	}
}

BEGIN_MESSAGE_MAP(CHmi, CWnd)
	//{{AFX_MSG_MAP(CPLCSocket)
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

//!< 启动HMI程序
bool CHmi::RunHmiExe()
{
	if(IsHmiCreate())		return false;								//!< 如果已经点击启动了,则不再启动了
	CString exePath = CGbl::GetMe().getExePath();						//!< 程序安装路径
	CString exePathName = exePath + CGbl::GetMe().getHmiName();			//!< 编译器路径名字

	STARTUPINFO si = {sizeof(si)};
	BOOL ret = CreateProcess(exePathName, _T(""), NULL, NULL, FALSE, 0, NULL, NULL, &si, &m_pi);
	if (ret)
	{
		m_bCreateHmi = true;
		m_ThreadToWati = new std::thread(std::bind(&WaitToHmi, this, &m_pi));
	}
	SetTimer(SHELL_TIME, 10000, NULL);
	if (CProjectMgr::GetMe().GetProj())
		CProjectMgr::GetMe().GetProj()->SetHmiModify(true);
	return !!ret;
}

void WaitToHmi(CHmi* pHmi, PROCESS_INFORMATION* pPi)
{
	WaitForSingleObject(pPi->hProcess, INFINITE);
	if (CProjectMgr::GetMe().GetProj())
		CProjectMgr::GetMe().GetProj()->SetHmiModify(false);
	pHmi->OnHmiQuit();
}

//!< DHMI退出的事件
void CHmi::OnHmiQuit()
{
	m_bCreateHmi = false;
	CloseHandle(m_pi.hThread);					// 关闭子进程的主线程句柄
	CloseHandle(m_pi.hProcess);					// 关闭子进程句柄
	memset(&m_pi, 0, sizeof(m_pi));
	m_hHmiWnd = NULL;
}

//!< 关闭DHMI软件
void CHmi::SetClose(bool bSave)
{
	if (!m_bCreateHmi)	return;
	if (!FindHmi())		return;
	if (!bSave){
		TerminateProcess(m_pi.hProcess, 0);
		WaitForSingleObject(m_pi.hProcess, INFINITE);		// 等待子进程的退出
	}
	else{
		//::SendMessage(m_hHmiWnd, WM_CLOSE, 0, 0);
		COPYDATASTRUCT cpd;
		cpd.dwData = HMI_CLOSEWITHSAVE;						//!< 关闭并保存
		cpd.lpData = NULL;
		cpd.cbData = 0;
		if(::IsWindow(m_hHmiWnd))
			::SendMessage(m_hHmiWnd, WM_COPYDATA, (WPARAM)GetParent()->GetSafeHwnd(), (LPARAM)&cpd);
	}
}

//!< 启动HMI运行
void CHmi::OnRun()
{
	if(!FindHmi())
		RunHmiExe();
	else if(Synchronization())
		return;

	SetTimer(SYNCHRONIZATION, 3000, NULL);							//!< 启动后，每2秒尝试一次同步，成功后就不尝试了
	SetTimer(SYNCHRONIZATION_OVER, 50000, NULL);					//!< 尝试50秒不尝试了
}

//!< 同步工程
bool CHmi::Synchronization()
{
	if(!CProjectMgr::GetMe().GetProj())	return true;
	if(!FindHmi())						return false;
	CString strPath;
	strPath.Format(_T(" -l %d"), MAX_ITEM_COUNT);
	strPath = strPath + _T(" -p \"") + CProjectMgr::GetMe().GetProj()->GetWholePathName() + _T("\"");

	COPYDATASTRUCT cpd;
	cpd.dwData = HMI_RUNPROJ;		//!< 加载新工程
	cpd.lpData = (LPSTR)(LPCTSTR)strPath;
	cpd.cbData = strPath.GetLength() + 1;
	DWORD dwrst = 0;
	LRESULT lreturn = 0;
	if(::IsWindow(m_hHmiWnd))
		lreturn = ::SendMessageTimeout(m_hHmiWnd, WM_COPYDATA, (WPARAM)GetParent()->GetSafeHwnd(), (LPARAM)&cpd, SMTO_NORMAL, 500, &dwrst);
	return lreturn != 0;
}

//!< 找到Hmi窗体句柄
bool CHmi::FindHmi()
{
	HWND hwnd = ::FindWindow(NULL, HMI_TITLE);
	m_hHmiWnd = hwnd;
	return !!m_hHmiWnd;
}

//!< 看看是不是自己要的数字
bool CHmi::IsINeed(UINT dwData)
{
	if(dwData == HMI_RUNPROJ_OVER)		return true;
	return false;
}

//!< 接收服务器发回来的消息
void CHmi::OnReceive(COPYDATASTRUCT* pCopyDataStruct)
{
	if(pCopyDataStruct->dwData == HMI_RUNPROJ_OVER){
		KillTimer(SYNCHRONIZATION);
		KillTimer(SYNCHRONIZATION_OVER);
	}
}

void CHmi::OnTimer(UINT nIDEvent)
{
	switch(nIDEvent)
	{
	case SYNCHRONIZATION:				//!< 同步工程
		Synchronization();
		break;

	case SYNCHRONIZATION_OVER:
		KillTimer(SYNCHRONIZATION_OVER);
		KillTimer(SYNCHRONIZATION);
		break;

	case SHELL_TIME:
		KillTimer(SHELL_TIME);
		//m_bCreateHmi = false;
		break;
	}
	CWnd::OnTimer(nIDEvent);
}
