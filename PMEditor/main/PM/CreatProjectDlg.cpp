// CreatProjectDlg.cpp : 实现文件
#include "stdafx.h"
#include <io.h>
#include "DXPEditor.h"
#include "MyHelp.h"
#include "Gbl.h"
#include "CreatProjectDlg.h"

// CCreatProjectDlg 对话框
using namespace Dialog;
IMPLEMENT_DYNAMIC(CCreatProjectDlg, CDialog)

CCreatProjectDlg::CCreatProjectDlg()
	: CDialog(CCreatProjectDlg::IDD, NULL)
{
	m_nType = 0;
	m_strName=_T("新建工程");
	m_strPathOnly = m_strAuthor = m_strDescript = _T("");
	m_strVersion = _T("1.0");
}

CCreatProjectDlg::~CCreatProjectDlg()
{
}

CCreatProjectDlg& CCreatProjectDlg::GetMe(bool bCreate/* =true */)
{
	static CCreatProjectDlg one;
	one.m_bCreate=bCreate;
	return one;
}

void CCreatProjectDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_PROJECT_NAME, m_strName);
	DDV_MaxChars(pDX, m_strName, 100);
	DDX_Text(pDX, IDC_PROJECT_PATH, m_strPathOnly);
	DDX_Text(pDX, IDC_PROJECT_AUTHOR, m_strAuthor);
	DDV_MaxChars(pDX, m_strName, 100);
	DDX_Text(pDX, IDC_PROJECT_DESCRIPTION, m_strDescript);
	DDV_MaxChars(pDX, m_strName, 5000);
	DDX_Text(pDX, IDC_VERSION, m_strVersion);
	DDV_MaxChars(pDX, m_strVersion, 100);
}

BEGIN_MESSAGE_MAP(CCreatProjectDlg, CDialog)
	ON_BN_CLICKED(IDOK, &CCreatProjectDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CCreatProjectDlg::OnBnClickedCancel)
	ON_BN_CLICKED(ID_FIND_PATH, &CCreatProjectDlg::OnBnClickedFindPath)
	ON_EN_CHANGE(IDC_PROJECT_NAME, OnChangeProjName)
	ON_WM_HELPINFO()
END_MESSAGE_MAP()

// CCreatProjectDlg 消息处理程序
BOOL Dialog::CCreatProjectDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
//	m_strPathAll = m_strPathOnly + m_strName + _T(".dsl");
	if(!m_bCreate){
		GetDlgItem(IDC_PROJECT_NAME)->EnableWindow(FALSE);
		GetDlgItem(ID_FIND_PATH)->ShowWindow(SW_HIDE);
		SetWindowText(_T("工程属性"));
		switch (m_nType)
		{
		case 0:
			SetDlgItemText(IDC_PROJECT_TYPE, _T("本地/Web工程"));
			break;
		case 1:
			SetDlgItemText(IDC_PROJECT_TYPE, _T("跨平台工程"));
			break;
		default:
			break;
		}
	}
	else
	{
		GetDlgItem(IDC_PROJECT_NAME)->EnableWindow(TRUE);
		GetDlgItem(ID_FIND_PATH)->EnableWindow(TRUE);
		SetWindowText(_T("创建工程"));
	}
	UpdateData(FALSE);
	return TRUE;  // return TRUE unless you set the focus to a control
}

void CCreatProjectDlg::OnBnClickedOk()
{
	UpdateData(TRUE);
	m_strName = m_strName.Trim();
	UpdateData(FALSE);
	OnChangeProjName();
	if(m_strName == _T("")){
		CString text=_T("工程名不能为空！");
		AfxMessageBox(text);
		GotoDlgCtrl(GetDlgItem(IDC_PROJECT_NAME)->SetFocus());
		return;
	}
	if(!CGbl::GetCorrectFileName(m_strName, true)){
		GotoDlgCtrl(GetDlgItem(IDC_PROJECT_NAME));
		return;
	}
	if(m_bCreate && _access((LPCTSTR)(m_strPathOnly + m_strName + _T("\\")), 06)==0)	//!< 冲突
		if(AfxMessageBox("工程冲突，是否覆盖?\n\r覆盖会丢失原有工程！",MB_YESNO)==IDNO)
			return;
	CDialog::OnOK();
}

void CCreatProjectDlg::OnBnClickedCancel()
{
	CDialog::OnCancel();
}

void Dialog::CCreatProjectDlg::OnBnClickedFindPath()
{
	CXTBrowseDialog dlg(this);
	dlg.SetTitle(_T("请选择文件夹"));
	TCHAR path[_MAX_PATH];
	STRCPY_S(path, _MAX_PATH, m_strPathOnly);
	dlg.SetSelPath(path);

	if (dlg.DoModal() == IDOK)
	{
		m_strPathOnly = dlg.GetSelPath();
		if(m_strPathOnly.GetAt(m_strPathOnly.GetLength() - 1) != '\\')
			m_strPathOnly = m_strPathOnly + _T("\\");
//		OnChangeProjName();
		UpdateData(FALSE);
	}
}

void CCreatProjectDlg::OnChangeProjName()
{
	UpdateData(TRUE);
//	m_strPathAll=m_strPathOnly+m_strName+_T(".dsl");
	UpdateData(FALSE);
}

BOOL Dialog::CCreatProjectDlg::DestroyWindow()
{
	return CDialog::DestroyWindow();
}

BOOL Dialog::CCreatProjectDlg::OnHelpInfo(HELPINFO* pHelpInfo)
{
	if(m_bCreate)
		SoftInfo::CMyHelp::GetMe().ShowHelp(_T("创建工程"));
	else
		SoftInfo::CMyHelp::GetMe().ShowHelp(_T("工程属性"));
	return CDialog::OnHelpInfo(pHelpInfo);
}
