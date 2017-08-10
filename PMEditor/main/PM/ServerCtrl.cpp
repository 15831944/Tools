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

const UINT TRY_MAX = 3;							//!< ���Ե�������

const UINT SERVER_RUNCPL = 3000;				//!< ���ع��̲��������������е�
const UINT SERVER_RUN = 3001;					//!< ��������������
const UINT SERVER_PAUSE = 3002;					//!< ��ͣ��������������ȡ��
const UINT SERVER_STOP = 3003;					//!< ֹͣ������
const UINT SERVER_STATE = 3004;					//!< ѯ�ʷ�������ǰ״̬
const UINT SCAN_START = 3005;					//!< ��ʼɨ��
const UINT SCAN_STOP = 3006;					//!< ֹͣɨ��
const UINT FRESH_DEVICE = 3007;					//!< ǿ��ɨ���豸
const UINT SERVER_STATE_BACK = 3010	;			//!< ѯ�ʷ�������ǰ״̬��������ʽ����
const UINT REGIST_SOFT = 3012;					//!< ע�����
//const UINT TIME_LIMIT = 3010;					//!< ����ʱ�ޱ���
//const UINT NOTIME_LIMIT = 3011;				//!< ȡ��ʱ�ޱ���

const UINT LOADINGFILE_OVERTIME_ID = 50;		//!< Ĭ��DXP�����ļ������ʱ��
const UINT LOADINGFILE_OVERTIME = 5000;			//!< Ĭ��DXP�����ļ������ʱ��

const CString SERVER_TITLE = _T("DXP");			//!< DXP��title,������ַ�����Ҫ�ó���,100�겻��

CServerCtrl::CServerCtrl(void)
:Servers::Compile::SNeedRunObj(_T("DXP���ݽ���ƽ̨"))
,m_hServerWnd(NULL)
,m_uiState(3)
,m_uiScanState(0)
,m_bLoadingScanFile(false)
,m_nTryNum(0)									//!< ����Ĵ���
{
}

CServerCtrl::~CServerCtrl(void)
{
}

//!< ���ص�������
CServerCtrl& CServerCtrl::GetMe()
{
	static CServerCtrl one;
	return one;
}

//!< Ϊ��������һ��������,���ڴ�����Ϣ��
void CServerCtrl::InitServerMgr(CWnd* pParent)
{
	HWND hWnd = GetSafeHwnd();
	if(hWnd == NULL)		//!< Ҫ���������������ڿ϶����ǵ�һ�Σ�����Ҫ��ѯһ�·�����������״̬
	{
		Create(NULL, _T("ServerCtrl"), WS_CHILD, CRect(0,0,0,0), pParent, 101);
		HWND sevHwnd = ::FindWindow(NULL, SERVER_TITLE);
		if(sevHwnd)			//!< ��Ϊ�գ���ʾ�������Ѿ��������ˣ����ھͿ��Բ�ѯ״̬��
		{
			m_hServerWnd = sevHwnd;
		}
		else				//!< ��������û��������Ҫ����������
		{
			CString exePath = CGbl::GetMe().getExePath();						//!< ����װ·��
			CString exePathName = exePath + CGbl::GetMe().getServerName();		//!< ������·������
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

//!< ����DXP
void CServerCtrl::CreateServer()
{
	CString exePath = CGbl::GetMe().getExePath();						//!< ����װ·��
	CString exePathName = exePath + CGbl::GetMe().getServerName();		//!< ������·������
	ShellExecute(NULL, "open", exePathName, _T(""), exePath, SW_MINIMIZE);
	CGbl::GetMe().PrintOut(_T("���������������Ժ�..."), true);
}

void CServerCtrl::OnTimer(UINT nIDEvent)
{
	COPYDATASTRUCT cpd;
	CWnd* cWnd = NULL;
	switch(nIDEvent)
	{
	case FIND_STATE:						//!< ��ѯ����״̬
		if(!FindServer())					break;
		FindState();
		break;

	case RUN_CMDLINE:
		if(RunServer(m_strCmdLine))			KillTimer(RUN_CMDLINE);
		else								++m_nTryNum;
		if(m_nTryNum >= TRY_MAX)
		{
			KillTimer(RUN_CMDLINE);
			MessageBox(_T("����ͨ�ŷ���ʧ��"), _T("����"), MB_OK | MB_ICONEXCLAMATION);
		}
		break;

	case RUN_SCANCMDLINE:
		if(ScanStart(m_strScanCmdLine))		KillTimer(RUN_SCANCMDLINE);
		else								++m_nTryNum;
		if(m_nTryNum >= TRY_MAX)
		{
			KillTimer(RUN_SCANCMDLINE);
			MessageBox(_T("����ͨ�ŷ���ʧ��"), _T("����"), MB_OK | MB_ICONEXCLAMATION);
		}
		break;

	case LOADINGFILE_OVERTIME_ID:
		SetLoadingFile(false);		//!< �����ٴε�����л�ɨ����
		KillTimer(LOADINGFILE_OVERTIME_ID);
		break;
	}
	CWnd::OnTimer(nIDEvent);
}

//!< �鿴�������ڲ���
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

//!< �鿴��������״̬
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

//!< ��������
void CServerCtrl::OnRunSev(CString cmdLine)
{
	if(!m_hServerWnd || !RunServer(cmdLine))
	{
		CreateServer();
		m_nTryNum = 0;
		m_strCmdLine = cmdLine;
		SetTimer(RUN_CMDLINE, 1000, NULL);				//!< ÿ�볢��һ����������
	}
}

//!< ����ɨ��
void CServerCtrl::OnScanStart(CString cmdLine)
{
	if(!m_hServerWnd || !ScanStart(cmdLine))
	{
		CreateServer();
		m_nTryNum = 0;
		m_strScanCmdLine = cmdLine;
		SetTimer(RUN_SCANCMDLINE, 1000, NULL);			//!< ÿ�볢��һ����������
	}
}

//!< ��������
void CServerCtrl::OnRun()
{
	std::shared_ptr<CProject> proj = CProjectMgr::GetMe().GetProj();
	if(!proj)				return;
	OnRunSev(proj->GetWholePathName());
}

//!< ��������
bool CServerCtrl::RunServer(CString cmdLine)
{
	if(!m_hServerWnd)		return false;
	COPYDATASTRUCT cpd;
	if(m_uiState == 2)		cpd.dwData = SERVER_RUN;	//!< ����ͣ�лָ�
	else					cpd.dwData = SERVER_RUNCPL;	//!< ��������
	cpd.lpData = (LPSTR)(LPCTSTR)cmdLine;
	cpd.cbData = cmdLine.GetLength();
	CGbl::PrintOut(_T("�������з���..."));
	DWORD dwrst = 0;
	if(::IsWindow(m_hServerWnd))
		::SendMessageTimeout(m_hServerWnd, WM_COPYDATA, (WPARAM)GetParent()->GetSafeHwnd(), (LPARAM)&cpd, SMTO_NORMAL, 500, &dwrst);
	FindState();
	return true;
}

//!< ����ֹͣ
void CServerCtrl::OnStopSev()
{
	if(!m_hServerWnd)		return;
	if(m_uiState != 1 && m_uiState != 2)				return;

	COPYDATASTRUCT cpd;
	cpd.dwData = SERVER_STOP;							//!< ֹͣ
	cpd.lpData = _T("");
	cpd.cbData = 0;
	CGbl::PrintOut(_T("����ֹͣ����..."));
	DWORD dwrst = 0;
	if(::IsWindow(m_hServerWnd))
		::SendMessageTimeout(m_hServerWnd, WM_COPYDATA, (WPARAM)GetParent()->GetSafeHwnd(), (LPARAM)&cpd, SMTO_NORMAL, 500, &dwrst);
	FindState();
}

//!< ��ʼɨ���豸
bool CServerCtrl::ScanStart(CString cmdLine)
{
	FindState();
	if(!m_hServerWnd)						return false;
	if(m_uiState == 1 || m_uiState == 2)	return false;//!< ����״̬����ɨ��
	if(IsLoadingFile())
	{
		CGbl::PrintOut(_T("DXP���ڼ����ļ����޷�����ɨ��"));
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

//!< ֹͣɨ���豸
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

//!< ����ǿ��ɨ���豸�Ĺ���
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

//!< ֪ͨDXP��ע��ҳ��
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

//!< �����ǲ����Լ�Ҫ������
bool CServerCtrl::IsINeed(UINT dwData)
{
	if(dwData == SERVER_STATE)				return true;
	if(dwData == SERVER_STATE_BACK)			return true;
	return false;
}

//!< �ӵ�DXP���¼���Ӧ
void CServerCtrl::OnReceive(COPYDATASTRUCT* pCopyDataStruct)
{
	switch(pCopyDataStruct->dwData)
	{
	case SERVER_STATE:			InitState(*pCopyDataStruct);		break;
	case SERVER_STATE_BACK:		InitStateXml(*pCopyDataStruct);		break;
	}
}

//!< �������ص�DXP״̬��Ϣ
void CServerCtrl::InitState(COPYDATASTRUCT& pCopyDataStruct)
{
	// 4�ֽ�����״̬, 4�ֽ�ɨ��״̬, ���ֽڵ�ǰ����·��, 4�ֽڱ�����������, ���ֽڵ�ǰȨ������
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

//!< �������ص�DXP������ʽ��״̬��Ϣ
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

//!< �������м��״̬
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

//!< ����ɨ��״̬
void CServerCtrl::SetScanState(UINT state)
{
	if(state == 1)							SetLoadingFile(false);
	if(state == 0)							CProjectMgr::GetMe().SetScan(false);
	else									CProjectMgr::GetMe().SetScan(true);
	m_uiScanState = state;
}

//!< �ж��ǲ���ͬһ������
bool CServerCtrl::IsSameProj()
{
	if(m_strProjSev == _T(""))				return true;		//!< Ϊ�˼���ԭ��û�ж������Ϣ
	std::shared_ptr<CProject> proj = CProjectMgr::GetMe().GetProj();
	if(!proj)								return false;
	CString str = proj->GetWholePathName();
	if(m_strProjSev == str)					return true;
	return false;
}