// ProcessDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "PMApp.h"
#include "ProcessDlg.h"
#include "afxdialogex.h"


// CProcessDlg 对话框

IMPLEMENT_DYNAMIC(CProcessDlg, CDialog)

CProcessDlg::CProcessDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CProcessDlg::IDD, pParent)
	, m_strOut(_T(""))
	, m_bReady(false)	// 是否准备好了
	, m_bStop(false)	// 是否点击了中断
	, m_nStart(0)
	, m_nEnd(0)
	, m_nPos(0)
{

}

CProcessDlg::~CProcessDlg()
{
}

void CProcessDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PROGRESS, m_ProgCtrl);
	DDX_Text(pDX, IDC_STATIC_OUT, m_strOut);
}


BEGIN_MESSAGE_MAP(CProcessDlg, CDialog)
	ON_BN_CLICKED(IDCANCEL, &CProcessDlg::OnBnClickedCancel)
	ON_MESSAGE(WM_PROCESS_RANGE, OnSetRange)
	ON_MESSAGE(WM_PROCESS_POS, OnSetPos)
	ON_MESSAGE(WM_PROCESS_DONE, OnDone)
END_MESSAGE_MAP()


// CProcessDlg 消息处理程序


void CProcessDlg::OnBnClickedCancel()
{
	m_bStop = true;	// 是否点击了中断
	Sleep(1000);
	CDialog::OnCancel();
}

BOOL CProcessDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	//m_ProgCtrl.SetState()
	UpdateData(FALSE);
	// TODO:  在此添加额外的初始化
	m_bStop = false;	// 是否点击了中断
	m_bReady = true;
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常:  OCX 属性页应返回 FALSE
}

//void CProcessDlg::SetProcessRange(int nStart, int nEnd)
//{
//	m_ProgCtrl.SetRange32(nStart, nEnd);
//	//UpdateData(FALSE);
//}
//
//void CProcessDlg::SetProcessPos(int nPos)
//{
//	m_ProgCtrl.SetPos(nPos);
//	m_strOut.Format("%d", nPos);
//	UpdateData(FALSE);
//}

LONG CProcessDlg::OnSetRange(WPARAM wParam, LPARAM lParam)
{
	m_nStart = (int)wParam;
	m_nEnd = (int)lParam;
	m_ProgCtrl.SetRange32((int)wParam, (int)lParam);
	UpdateData(FALSE);
	return 0;
}

LONG CProcessDlg::OnSetPos(WPARAM wParam, LPARAM lParam)
{
	m_nPos = (int)wParam;
	m_ProgCtrl.SetPos(m_nPos);
	m_strOut.Format("%d", m_nPos);
	UpdateData(FALSE);
	return 0;
}

LONG CProcessDlg::OnDone(WPARAM wParam, LPARAM lParam)
{
	m_nPos = (int)m_nEnd;
	m_ProgCtrl.SetPos(m_nPos);
	m_strOut.Format("%d", m_nPos);
	UpdateData(FALSE);
	OnOK();
	return 0;
}
