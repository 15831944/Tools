// ReNameDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "PMApp.h"
#include "MyHelp.h"
#include "Gbl.h"
#include "ReNameDlg.h"


// CReNameDlg 对话框
using namespace Dialog;
IMPLEMENT_DYNAMIC(CReNameDlg, CDialog)

CReNameDlg::CReNameDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CReNameDlg::IDD, pParent)
{
	m_strOldName = m_strNewName = _T("");
}

CReNameDlg::~CReNameDlg()
{
}

void CReNameDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_NAME_OLD, m_strOldName);
	DDX_Text(pDX, IDC_NAME_NEW, m_strNewName);
	DDV_MaxChars(pDX, m_strNewName, 100);
}


BEGIN_MESSAGE_MAP(CReNameDlg, CDialog)
	ON_BN_CLICKED(IDOK, &CReNameDlg::OnBnClickedOk)
	ON_WM_HELPINFO()
END_MESSAGE_MAP()


// CReNameDlg 消息处理程序


BOOL CReNameDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	//!< 提供默认值
	if(m_strNewName == _T(""))
		SetDefaultName(m_strOldName);

	return TRUE;  // return TRUE unless you set the focus to a control
}

//!< 设定老名称
void CReNameDlg::SetOldName(CString str)
{
	if(str.GetLength() > 100)
		str = str.Left(100);
	m_strNewName = m_strOldName = str;
}

//!< 设定默认值
void CReNameDlg::SetDefaultName(CString str)
{
	if(str.GetLength() > 100)
		str = str.Left(100);
	m_strNewName = str;
}
void CReNameDlg::OnBnClickedOk()
{
	UpdateData(TRUE);
	m_strNewName = m_strNewName.Trim();
	if(m_strNewName == _T(""))
	{
		MessageBox(_T("新名称不能为空"), _T("错误提示"), MB_OK | MB_ICONEXCLAMATION);
		GotoDlgCtrl(GetDlgItem(IDC_NAME_NEW)->SetFocus());
		return;
	}
	if(!CGbl::GetCorrectFileName(m_strNewName, true)){
		GotoDlgCtrl(GetDlgItem(IDC_NAME_NEW));
		return;
	}
	OnOK();
}

BOOL Dialog::CReNameDlg::OnHelpInfo(HELPINFO* pHelpInfo)
{
	SoftInfo::CMyHelp::GetMe().ShowHelp(_T("工程重命名"));
	return CDialog::OnHelpInfo(pHelpInfo);
}
