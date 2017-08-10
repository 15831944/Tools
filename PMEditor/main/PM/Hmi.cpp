#include "StdAfx.h"
#include "Gbl.h"
#include "ProjectMgr.h"
#include "Project.h"
#include "Hmi.h"

const CString HMI_TITLE = _T("DHMI �༭��");

const UINT SYNCHRONIZATION = 1;
const UINT SYNCHRONIZATION_OVER = 2;
const UINT SHELL_TIME = 3;			//!< ����HMIǿ�еȴ���ʱ��

const UINT HMI_RUNPROJ = 4000;
const UINT HMI_STATE = 4001;
const UINT HMI_RUNPROJ_OVER = 4002;
const UINT HMI_CLOSEWITHSAVE = 4004;


using namespace Servers;
using namespace HMI;
void WaitToHmi(CHmi* pHmi, PROCESS_INFORMATION* pPi);
CHmi::CHmi(void)
:Servers::Compile::SNeedRunObj(_T("HMI"))
,m_hHmiWnd(0)						//!< ����������ľ��
,m_ThreadToWati(NULL)
{
	OnHmiQuit();
}

CHmi::~CHmi(void)
{
	//TerminateProcess(m_pi.hProcess, 0);
	//WaitForSingleObject(m_pi.hProcess, INFINITE);		// �ȴ��ӽ��̵��˳�
}

void CHmi::InitHmiMgr(CWnd* pParent)
{
	HWND hWnd = GetSafeHwnd();
	if(hWnd == NULL){		//!< Ҫ���������������ڿ϶����ǵ�һ�Σ�����Ҫ��ѯһ�·�����������״̬
		CRect rect(0,0,0,0);
		Create(NULL, _T("ServerCtrl"), WS_CHILD, rect, pParent, 101);
	}
}

BEGIN_MESSAGE_MAP(CHmi, CWnd)
	//{{AFX_MSG_MAP(CPLCSocket)
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

//!< ����HMI����
bool CHmi::RunHmiExe()
{
	if(IsHmiCreate())		return false;								//!< ����Ѿ����������,����������
	CString exePath = CGbl::GetMe().getExePath();						//!< ����װ·��
	CString exePathName = exePath + CGbl::GetMe().getHmiName();			//!< ������·������

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

//!< DHMI�˳����¼�
void CHmi::OnHmiQuit()
{
	m_bCreateHmi = false;
	CloseHandle(m_pi.hThread);					// �ر��ӽ��̵����߳̾��
	CloseHandle(m_pi.hProcess);					// �ر��ӽ��̾��
	memset(&m_pi, 0, sizeof(m_pi));
	m_hHmiWnd = NULL;
}

//!< �ر�DHMI���
void CHmi::SetClose(bool bSave)
{
	if (!m_bCreateHmi)	return;
	if (!FindHmi())		return;
	if (!bSave){
		TerminateProcess(m_pi.hProcess, 0);
		WaitForSingleObject(m_pi.hProcess, INFINITE);		// �ȴ��ӽ��̵��˳�
	}
	else{
		//::SendMessage(m_hHmiWnd, WM_CLOSE, 0, 0);
		COPYDATASTRUCT cpd;
		cpd.dwData = HMI_CLOSEWITHSAVE;						//!< �رղ�����
		cpd.lpData = NULL;
		cpd.cbData = 0;
		if(::IsWindow(m_hHmiWnd))
			::SendMessage(m_hHmiWnd, WM_COPYDATA, (WPARAM)GetParent()->GetSafeHwnd(), (LPARAM)&cpd);
	}
}

//!< ����HMI����
void CHmi::OnRun()
{
	if(!FindHmi())
		RunHmiExe();
	else if(Synchronization())
		return;

	SetTimer(SYNCHRONIZATION, 3000, NULL);							//!< ������ÿ2�볢��һ��ͬ�����ɹ���Ͳ�������
	SetTimer(SYNCHRONIZATION_OVER, 50000, NULL);					//!< ����50�벻������
}

//!< ͬ������
bool CHmi::Synchronization()
{
	if(!CProjectMgr::GetMe().GetProj())	return true;
	if(!FindHmi())						return false;
	CString strPath;
	strPath.Format(_T(" -l %d"), MAX_ITEM_COUNT);
	strPath = strPath + _T(" -p \"") + CProjectMgr::GetMe().GetProj()->GetWholePathName() + _T("\"");

	COPYDATASTRUCT cpd;
	cpd.dwData = HMI_RUNPROJ;		//!< �����¹���
	cpd.lpData = (LPSTR)(LPCTSTR)strPath;
	cpd.cbData = strPath.GetLength() + 1;
	DWORD dwrst = 0;
	LRESULT lreturn = 0;
	if(::IsWindow(m_hHmiWnd))
		lreturn = ::SendMessageTimeout(m_hHmiWnd, WM_COPYDATA, (WPARAM)GetParent()->GetSafeHwnd(), (LPARAM)&cpd, SMTO_NORMAL, 500, &dwrst);
	return lreturn != 0;
}

//!< �ҵ�Hmi������
bool CHmi::FindHmi()
{
	HWND hwnd = ::FindWindow(NULL, HMI_TITLE);
	m_hHmiWnd = hwnd;
	return !!m_hHmiWnd;
}

//!< �����ǲ����Լ�Ҫ������
bool CHmi::IsINeed(UINT dwData)
{
	if(dwData == HMI_RUNPROJ_OVER)		return true;
	return false;
}

//!< ���շ���������������Ϣ
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
	case SYNCHRONIZATION:				//!< ͬ������
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
