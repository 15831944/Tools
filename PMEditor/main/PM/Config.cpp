// Config.cpp : 实现文件
//

#include "stdafx.h"
#include "PMApp.h"
#include "Gbl.h"
#include "ProjectMgr.h"
#include "Project.h"
#include "Config.h"
#include "ServerCtrl.h"

// CConfig
using namespace Servers;
using namespace Config;

const UINT CONFIG_CONNECT = 5000;

IMPLEMENT_DYNAMIC(CConfig, CWnd)

void WaitToConfigClose(CConfig* pConfig, PROCESS_INFORMATION* pPi);

CConfig::CConfig(int id)
:Servers::Compile::SNeedRunObj(_T("DConfig"))
,m_nID(id)
,m_hConfigWnd(NULL)
{
}

BEGIN_MESSAGE_MAP(CConfig, CWnd)
	ON_WM_COPYDATA()
END_MESSAGE_MAP()

// CConfig 消息处理程序

//!< 初始化窗口
bool CConfig::InitWnd()
{
	HWND hWnd = GetSafeHwnd();
	if(hWnd == NULL){		//!< 要建立窗体句柄，由于肯定这是第一次，所以要查询一下服务程序的运行状态
		CRect rect(0,0,0,0);
		Create(NULL, _T("CConfig"), WS_CHILD, rect, g_App.GetMainWnd(), 101);
	}
	hWnd = GetSafeHwnd();
	return hWnd != NULL;
}

//!< 启动运行
void CConfig::OnRun()
{
	if (GetSafeHwnd() == NULL)		return;								//!< 如果已经点击启动了,则不再启动了
	Servers::DXP::CServerCtrl::GetMe().OnRun();
	std::shared_ptr<CProject> proj = CProjectMgr::GetMe().GetProj();
	if (!proj)						return;
	CString exePath = CGbl::GetMe().getExePath();						//!< 程序安装路径
	CString exePathName = exePath + CGbl::GetMe().getConfigName();		//!< 编译器路径名字
	CString cmdLine = _T("\r\nPath=") + proj->GetWholePathName();
	CString strLine;
	strLine.Format("DevID=%d", GetID());
	cmdLine = cmdLine + _T("\r\n") + strLine;
	strLine.Format("Handle=%u", (UINT)GetSafeHwnd());
	cmdLine = cmdLine + _T("\r\n") + strLine;

	STARTUPINFO si = {sizeof(si)};
	//MessageBox(cmdLine, "启动Config");
	BOOL ret = CreateProcess(exePathName, cmdLine.GetBuffer(), NULL, NULL, FALSE, 0, NULL, NULL, &si, &m_pi);
	if (ret){
		m_ThreadToWati = new std::thread(std::bind(&WaitToConfigClose, this, &m_pi));
	}
}

void WaitToConfigClose(CConfig* pConfig, PROCESS_INFORMATION* pPi)
{
	WaitForSingleObject(pPi->hProcess, INFINITE);
	pConfig->OnConfigQuit();
}

//!< 当Config软件关闭退出时
void CConfig::OnConfigQuit()
{
	CloseHandle(m_pi.hThread);					// 关闭子进程的主线程句柄
	CloseHandle(m_pi.hProcess);					// 关闭子进程句柄
	memset(&m_pi, 0, sizeof(m_pi));
	m_hConfigWnd = NULL;
}

BOOL Servers::Config::CConfig::OnCopyData(CWnd* pWnd, COPYDATASTRUCT* pCopyDataStruct)
{
	if (pCopyDataStruct->dwData == CONFIG_CONNECT)
	{
		m_hConfigWnd = pWnd->GetSafeHwnd();
	}
	return CWnd::OnCopyData(pWnd, pCopyDataStruct);
}
