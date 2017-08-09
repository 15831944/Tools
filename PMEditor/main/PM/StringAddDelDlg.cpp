// SerialAddDelDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "DXPEditor.h"
#include "MyHelp.h"
#include "StringAddDelDlg.h"

// CStringAddDelDlg �Ի���
using namespace Dialog;
IMPLEMENT_DYNAMIC(CStringAddDelDlg, CDialog)

CStringAddDelDlg::CStringAddDelDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CStringAddDelDlg::IDD, pParent)
{

}

CStringAddDelDlg::~CStringAddDelDlg()
{
}

void CStringAddDelDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SERIAL_OUT, m_OutList);
	DDX_Control(pDX, IDC_SERIAL_IN, m_InList);
}


BEGIN_MESSAGE_MAP(CStringAddDelDlg, CDialog)
	ON_BN_CLICKED(IDC_SERIAL_ADD, &CStringAddDelDlg::OnBnClickedSerialAdd)
	ON_BN_CLICKED(IDC_SERIAL_DEL, &CStringAddDelDlg::OnBnClickedSerialDel)
	ON_BN_CLICKED(IDC_SERIAL_ADDALL, &CStringAddDelDlg::OnBnClickedSerialAddall)
	ON_BN_CLICKED(IDC_SERIAL_DELALL, &CStringAddDelDlg::OnBnClickedSerialDelall)
	ON_BN_CLICKED(IDOK, &CStringAddDelDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CStringAddDelDlg::OnBnClickedCancel)
	ON_LBN_DBLCLK(IDC_SERIAL_OUT, &CStringAddDelDlg::OnLbnDblclkSerialOut)
	ON_LBN_DBLCLK(IDC_SERIAL_IN, &CStringAddDelDlg::OnLbnDblclkSerialIn)
	ON_WM_HELPINFO()
END_MESSAGE_MAP()


// CStringAddDelDlg ��Ϣ�������
BOOL CStringAddDelDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	GetDlgItem(IDC_STATIC_OUT)->SetWindowText(m_strOut);
	GetDlgItem(IDC_STATIC_IN)->SetWindowText(m_strIn);
	SetWindowText(m_strTitle);
	CString str;
	foreach(str, m_ltOut)		m_OutList.AddString(str);
	foreach(str, m_ltIn)		m_InList.AddString(str);
	UpdateData(FALSE);

	return TRUE;
}

void CStringAddDelDlg::OnBnClickedSerialAdd()
{
	OnLbnDblclkSerialOut();
}

void CStringAddDelDlg::OnBnClickedSerialDel()
{
	OnLbnDblclkSerialIn();
}

void CStringAddDelDlg::OnBnClickedSerialAddall()
{
	CString text;
	int count = m_OutList.GetCount();
	for(int i = 0; i < count; ++i){
		m_OutList.GetText(i, text);
		m_InList.AddString(text);
	}
	m_OutList.ResetContent();
}

void CStringAddDelDlg::OnBnClickedSerialDelall()
{
	CString text;
	int count = m_InList.GetCount();
	for(int i = 0; i < count; ++i){
		m_InList.GetText(i, text);
		m_OutList.AddString(text);
	}
	m_InList.ResetContent();
}

void CStringAddDelDlg::OnBnClickedOk()
{
	OnOK();
}

void CStringAddDelDlg::OnBnClickedCancel()
{
	OnCancel();
}

//!< ˫��δ����б�
void CStringAddDelDlg::OnLbnDblclkSerialOut()
{
	UINT index;
	CString text;
	index = m_OutList.GetCurSel();
	if(index == -1)		return;
	m_OutList.GetText(index, text);
	m_OutList.DeleteString(index);
	m_InList.AddString(text);
}

//!< ˫��������б�
void CStringAddDelDlg::OnLbnDblclkSerialIn()
{
	UINT index;
	CString text;
	index = m_InList.GetCurSel();
	if(index == -1)		return;
	m_InList.GetText(index, text);
	m_InList.DeleteString(index);
	m_OutList.AddString(text);
}

BOOL Dialog::CStringAddDelDlg::DestroyWindow()
{
	m_ltOut.clear();
	m_ltIn.clear();
	CString str;

	int count = m_OutList.GetCount();
	for(int i = 0; i < count; ++i)
	{
		m_OutList.GetText(i, str);
		m_ltOut.push_back(str);
	}

	count = m_InList.GetCount();
	for(int i = 0; i < count; ++i)
	{
		m_InList.GetText(i, str);
		m_ltIn.push_back(str);
	}
	m_OutList.ResetContent();
	m_InList.ResetContent();
	return CDialog::DestroyWindow();
}

BOOL Dialog::CStringAddDelDlg::OnHelpInfo(HELPINFO* pHelpInfo)
{
	SoftInfo::CMyHelp::GetMe().ShowHelp(_T("����ʦվ�ӿ�����"));
	return CDialog::OnHelpInfo(pHelpInfo);
}
