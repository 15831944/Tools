// SoftSetCompileDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "PMApp.h"
#include "MyHelp.h"
#include "SoftInfo.h"
#include "SoftSetCompileDlg.h"


// CSoftSetCompileDlg 对话框
using namespace SoftInfo;
IMPLEMENT_DYNAMIC(CSoftSetCompileDlg, CDialog)

CSoftSetCompileDlg::CSoftSetCompileDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSoftSetCompileDlg::IDD, pParent)
	, m_bOutPut(FALSE)
	, m_bOutPutWarning(FALSE)
{

}

CSoftSetCompileDlg::~CSoftSetCompileDlg()
{
}

void CSoftSetCompileDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Check(pDX, IDC_CHECK_ISOUTPUT, m_bOutPut);
	DDX_Check(pDX, IDC_CHECK_ISWARNING, m_bOutPutWarning);
}


BEGIN_MESSAGE_MAP(CSoftSetCompileDlg, CDialog)
	ON_WM_HELPINFO()
END_MESSAGE_MAP()


// CSoftSetCompileDlg 消息处理程序
BOOL CSoftSetCompileDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	CSoftInfo* csi = &SoftInfo::CSoftInfo::GetMe();
	m_bOutPut = csi->isOutPut()?1:0;
	m_bOutPutWarning = csi->isOutWarning()?1:0;
	UpdateData(FALSE);
	return TRUE;
}

BOOL CSoftSetCompileDlg::DestroyWindow()
{
	UpdateData(TRUE);
	CSoftInfo* csi = &SoftInfo::CSoftInfo::GetMe();
	csi->setOutPut(m_bOutPut==1?true:false);
	csi->setOutWarning(m_bOutPutWarning==1?true:false);
	return CDialog::DestroyWindow();
}

void CSoftSetCompileDlg::OnOK()
{
	CWnd* pWnd = GetParent();
	if(!pWnd)										return;
	pWnd->PostMessage(MESSAGE_OK, 0, 0);
}

BOOL SoftInfo::CSoftSetCompileDlg::OnHelpInfo(HELPINFO* pHelpInfo)
{
	SoftInfo::CMyHelp::GetMe().ShowHelp(_T("检查与编译"));
	return CDialog::OnHelpInfo(pHelpInfo);
}
