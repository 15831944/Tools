// ProjEncryptDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "DXPEditor.h"
#include "ProjEncryptDlg.h"
#include "ProjectMgr.h"
#include "Project.h"
#include "MyHelp.h"

// CProjEncryptDlg �Ի���
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


//!< ����������������ͣ����ܡ��޸����롢ȡ�����룩����ʾ�����ء��޸Ľ����ϵĿؼ�
BOOL CProjEncryptDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	switch (m_nType)
	{
	case CProject::ENCRYPT_NEW:
		GetDlgItem(IDC_STATIC_OLD)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_EDIT_OLD)->ShowWindow(SW_HIDE);
		m_strEncryptTitle = _T("����");
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
		m_strEncryptTitle = _T("����");
		break;

	default:
		ASSERT(FALSE);
	}
	UpdateData(FALSE);

	return TRUE;
}

//!< ����������������ͣ����ܡ��޸����롢ȡ�����룩�����ò�ͬ�ķ������д���
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

//!< �ж���������ĺϷ���
void CProjEncryptDlg::OnOKNewEncrypt()
{
	if (m_strEncrypt.GetLength() > 20 || m_strEncrypt.GetLength() < 3)
	{
		MessageBox(_T("���볤�ȴ���������3~20���ַ���"), _T("����"), MB_OK | MB_ICONEXCLAMATION);
		GotoDlgCtrl(GetDlgItem(IDC_EDIT_NEW));
		return;
	}
	if (m_strEncrypt != m_strEncryptAgain)
	{
		MessageBox(_T("������������벻��ͬ��"), _T("����"), MB_OK | MB_ICONEXCLAMATION);
		GotoDlgCtrl(GetDlgItem(IDC_EDIT_AGAIN));
		return;
	}
	OnOK();
}

//!< �ж��޸�����ĺϷ���
void CProjEncryptDlg::OnOKModifyEncrypt()
{
	boost::shared_ptr<CProject> proj = CProjectMgr::GetMe().GetProj();
	if (proj && proj->GetEncryptText() != m_strEncryptOld)
	{
		MessageBox(_T("���������"), _T("����"), MB_OK | MB_ICONEXCLAMATION);
		GotoDlgCtrl(GetDlgItem(IDC_EDIT_OLD));
		return;
	}
	if (m_strEncrypt.GetLength() > 20 || m_strEncrypt.GetLength() < 3)
	{
		MessageBox(_T("���볤�ȴ���������3~20���ַ���"), _T("����"), MB_OK | MB_ICONEXCLAMATION);
		GotoDlgCtrl(GetDlgItem(IDC_EDIT_NEW));
		return;
	}
	if (m_strEncrypt != m_strEncryptAgain)
	{
		MessageBox(_T("������������벻��ͬ��"), _T("����"), MB_OK | MB_ICONEXCLAMATION);
		GotoDlgCtrl(GetDlgItem(IDC_EDIT_AGAIN));
		return;
	}
	OnOK();
}

//!< �ж�ȡ������ĺϷ���
void CProjEncryptDlg::OnOKDelEncrypt()
{
	boost::shared_ptr<CProject> proj = CProjectMgr::GetMe().GetProj();
	if (proj && proj->GetEncryptText() != m_strEncrypt)
	{
		MessageBox(_T("���������"), _T("����"), MB_OK | MB_ICONEXCLAMATION);
		GotoDlgCtrl(GetDlgItem(IDC_EDIT_NEW));
		return;
	}
	OnOK();
}

BOOL Dialog::CProjEncryptDlg::OnHelpInfo(HELPINFO* pHelpInfo)
{
	SoftInfo::CMyHelp::GetMe().ShowHelp(_T("���̼���"));
	return CDialog::OnHelpInfo(pHelpInfo);
}