// SoftSetServerDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "DXPEditor.h"
#include "MyHelp.h"
#include "SoftInfo.h"
#include "SoftSetServerDlg.h"


// CSoftSetServerDlg 对话框
using namespace SoftInfo;
IMPLEMENT_DYNAMIC(CSoftSetServerDlg, CDialog)

CSoftSetServerDlg::CSoftSetServerDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSoftSetServerDlg::IDD, pParent)
	, m_uiDataFreshTime(0)
{
	
}

CSoftSetServerDlg::~CSoftSetServerDlg()
{
}

void CSoftSetServerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_uiDataFreshTime);
}


BEGIN_MESSAGE_MAP(CSoftSetServerDlg, CDialog)
	ON_WM_HELPINFO()
END_MESSAGE_MAP()


// CSoftSetServerDlg 消息处理程序
BOOL CSoftSetServerDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	CSoftInfo* csi = &SoftInfo::CSoftInfo::GetMe();
	m_uiDataFreshTime = csi->getFreshDataTime();
	UpdateData(FALSE);
	return TRUE;
}

BOOL CSoftSetServerDlg::DestroyWindow()
{
	UpdateData(TRUE);
	if(m_uiDataFreshTime < 50)			m_uiDataFreshTime = 50;
	if(m_uiDataFreshTime > 3600000)		m_uiDataFreshTime = 3600000;
	UpdateData(FALSE);
	CSoftInfo* csi = &SoftInfo::CSoftInfo::GetMe();
	csi->SetFreshDataTime(m_uiDataFreshTime);
	return CDialog::DestroyWindow();
}

void CSoftSetServerDlg::OnOK()
{
	CWnd* pWnd = GetParent();
	if(!pWnd)										return;
	pWnd->PostMessage(MESSAGE_OK, 0, 0);
}

BOOL SoftInfo::CSoftSetServerDlg::OnHelpInfo(HELPINFO* pHelpInfo)
{
	SoftInfo::CMyHelp::GetMe().ShowHelp(_T("变量监控配置"));
	return CDialog::OnHelpInfo(pHelpInfo);
}
