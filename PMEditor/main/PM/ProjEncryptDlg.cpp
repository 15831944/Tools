// ProjEncryptDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "DXPEditor.h"
#include "ProjEncryptDlg.h"
#include "ProjectMgr.h"
#include "Project.h"
#include "MyHelp.h"

// CProjEncryptDlg 对话框
using namespace Dialog;
IMPLEMENT_DYNAMIC(CProjEncryptDlg, CDialog)

CProjEncryptDlg::CProjEncryptDlg(int nType, CWnd* pParent /*=NULL*/)
	: CDialog(CProjEncryptDlg::IDD, pParent)
	, m_strEncryptOld(_T(""))
	, m_strEncrypt(_T(""))
	, m_strEncryptAgain(_T(""))
	, m_nType(nType)
{
	if (m_nType != CProject::ENCRYPT_NEW &&
		m_nType != CProject::ENCRYPT_MODIFY &&
		m_nType != CProject::ENCRYPT_DELETE &&
		m_nType != CProject::ENCRYPT_GET)
	{
		ASSERT(FALSE);
		m_nType = CProject::ENCRYPT_NEW;
	}
}

CProjEncryptDlg::~CProjEncryptDlg()
{
}

void CProjEncryptDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_OLD, m_strEncryptOld);
	DDX_Text(pDX, IDC_EDIT_NEW, m_strEncrypt);
	DDX_Text(pDX, IDC_EDIT_AGAIN, m_strEncryptAgain);
}


BEGIN_MESSAGE_MAP(CProjEncryptDlg, CDialog)
	ON_BN_CLICKED(IDOK, &CProjEncryptDlg::OnBnClickedOk)
	ON_WM_HELPINFO()
END_MESSAGE_MAP()


//!< 根据设置密码的类型（加密、修改密码、取消密码），显示、隐藏、修改界面上的控件
BOOL CProjEncryptDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	switch (m_nType)
	{
	case CProject::ENCRYPT_NEW:
		GetDlgItem(IDC_STATIC_OLD)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_EDIT_OLD)->ShowWindow(SW_HIDE);
		m_strEncryptTitle = _T("密码");
		GotoDlgCtrl(GetDlgItem(IDC_EDIT_NEW));
		break;

	case CProject::ENCRYPT_MODIFY:
		GotoDlgCtrl(GetDlgItem(IDC_EDIT_OLD));
		break;

	case CProject::ENCRYPT_DELETE:
	case CProject::ENCRYPT_GET:
		GetDlgItem(IDC_STATIC_OLD)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_EDIT_OLD)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_AGAIN)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_EDIT_AGAIN)->ShowWindow(SW_HIDE);
		GotoDlgCtrl(GetDlgItem(IDC_EDIT_NEW));
		m_strEncryptTitle = _T("密码");
		break;

	default:
		ASSERT(FALSE);
	}
	UpdateData(FALSE);

	return TRUE;
}

//!< 根据设置密码的类型（加密、修改密码、取消密码），调用不同的方法进行处理
void CProjEncryptDlg::OnBnClickedOk()
{
	UpdateData(TRUE);
	if (m_nType == CProject::ENCRYPT_NEW)
		OnOKNewEncrypt();
	else if (m_nType == CProject::ENCRYPT_MODIFY)
		OnOKModifyEncrypt();
	else if (m_nType == CProject::ENCRYPT_DELETE)
		OnOKDelEncrypt();
	else if (m_nType == CProject::ENCRYPT_GET)
		OnOK();
	else
		ASSERT(FALSE);
}

//!< 判断设置密码的合法性
void CProjEncryptDlg::OnOKNewEncrypt()
{
	if (m_strEncrypt.GetLength() > 20 || m_strEncrypt.GetLength() < 3)
	{
		MessageBox(_T("密码长度错误，请输入3~20个字符！"), _T("警告"), MB_OK | MB_ICONEXCLAMATION);
		GotoDlgCtrl(GetDlgItem(IDC_EDIT_NEW));
		return;
	}
	if (m_strEncrypt != m_strEncryptAgain)
	{
		MessageBox(_T("两次输入的密码不相同！"), _T("警告"), MB_OK | MB_ICONEXCLAMATION);
		GotoDlgCtrl(GetDlgItem(IDC_EDIT_AGAIN));
		return;
	}
	OnOK();
}

//!< 判断修改密码的合法性
void CProjEncryptDlg::OnOKModifyEncrypt()
{
	boost::shared_ptr<CProject> proj = CProjectMgr::GetMe().GetProj();
	if (proj && proj->GetEncryptText() != m_strEncryptOld)
	{
		MessageBox(_T("旧密码错误！"), _T("警告"), MB_OK | MB_ICONEXCLAMATION);
		GotoDlgCtrl(GetDlgItem(IDC_EDIT_OLD));
		return;
	}
	if (m_strEncrypt.GetLength() > 20 || m_strEncrypt.GetLength() < 3)
	{
		MessageBox(_T("密码长度错误，请输入3~20个字符！"), _T("警告"), MB_OK | MB_ICONEXCLAMATION);
		GotoDlgCtrl(GetDlgItem(IDC_EDIT_NEW));
		return;
	}
	if (m_strEncrypt != m_strEncryptAgain)
	{
		MessageBox(_T("两次输入的密码不相同！"), _T("警告"), MB_OK | MB_ICONEXCLAMATION);
		GotoDlgCtrl(GetDlgItem(IDC_EDIT_AGAIN));
		return;
	}
	OnOK();
}

//!< 判断取消密码的合法性
void CProjEncryptDlg::OnOKDelEncrypt()
{
	boost::shared_ptr<CProject> proj = CProjectMgr::GetMe().GetProj();
	if (proj && proj->GetEncryptText() != m_strEncrypt)
	{
		MessageBox(_T("旧密码错误！"), _T("警告"), MB_OK | MB_ICONEXCLAMATION);
		GotoDlgCtrl(GetDlgItem(IDC_EDIT_NEW));
		return;
	}
	OnOK();
}

BOOL Dialog::CProjEncryptDlg::OnHelpInfo(HELPINFO* pHelpInfo)
{
	SoftInfo::CMyHelp::GetMe().ShowHelp(_T("工程加密"));
	return CDialog::OnHelpInfo(pHelpInfo);
}