// FindDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "PMApp.h"
#include "MyHelp.h"
#include "Gbl.h"
#include "FindDlg.h"

#include "ItemMgr.h"

// CFindDlg �Ի���
using namespace Dialog;
IMPLEMENT_DYNAMIC(CFindDlg, CDialog)

CFindDlg::CFindDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CFindDlg::IDD, pParent)
	, m_bMatchUpDown(TRUE)
	, m_bMatchWhole(TRUE)
	, m_bUseRegex(FALSE)
{
	m_ltMatchStr.push_back(_T(""));
	m_ltFindArea.push_back(_T("����"));
	m_nAreaSel = 0;
}

CFindDlg::~CFindDlg()
{
}

CFindDlg& CFindDlg::GetMe()
{
	static CFindDlg one;
	return one;
}

void CFindDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_FINDTEXT, m_cbFindText);
	DDX_Control(pDX, IDC_COMBO_FINDAREA, m_cbFindArea);
	DDX_Check(pDX, IDC_CHECK_MAKEUPDOWN, m_bMatchUpDown);
	DDX_Check(pDX, IDC_CHECK_WHOLEMATCH, m_bMatchWhole);
	DDX_Check(pDX, IDC_CHECK_REGEX, m_bUseRegex);
}

BEGIN_MESSAGE_MAP(CFindDlg, CDialog)
	ON_BN_CLICKED(IDOK, &CFindDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDC_CHECK_REGEX, &OnBnClickedRegex)
	ON_WM_HELPINFO()
END_MESSAGE_MAP()


// CFindDlg ��Ϣ�������
BOOL CFindDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	for (CString str : m_ltMatchStr)		m_cbFindText.AddString(str);
	for (CString str : m_ltFindArea)		m_cbFindArea.AddString(str);
	m_cbFindText.SetWindowText(m_strMatch);
	m_cbFindArea.SetCurSel(m_nAreaSel);
	CheckEnabled();
	return TRUE;
}

void CFindDlg::CheckEnabled()
{
	if(m_bUseRegex)
	{
		GetDlgItem(IDC_CHECK_WHOLEMATCH)->EnableWindow(FALSE);
		GetDlgItem(IDC_CHECK_MAKEUPDOWN)->EnableWindow(FALSE);
	}
	else
	{
		GetDlgItem(IDC_CHECK_WHOLEMATCH)->EnableWindow(TRUE);
		GetDlgItem(IDC_CHECK_MAKEUPDOWN)->EnableWindow(TRUE);
	}
	UpdateData(FALSE);
}

void CFindDlg::OnBnClickedRegex()
{
	UpdateData(TRUE);
	CheckEnabled();
}

void CFindDlg::OnBnClickedOk()
{
	UpdateData(TRUE);
	m_cbFindText.GetWindowText(m_strMatch);
	m_nAreaSel = m_cbFindArea.GetCurSel();
	m_ltMatchStr.remove(m_strMatch);
	m_ltMatchStr.push_front(m_strMatch);
	BeginFind();
	OnOK();
}

void CFindDlg::BeginFind()
{
	if(m_strMatch == _T(""))	return;
	MVC::Item::CItemMgr* itemMgr = &MVC::Item::CItemMgr::GetMe();
	int count = 0;
	bool bSearchItem = true, bSearchDev = true;
	if(m_nAreaSel == 0)			bSearchDev = false;
	else if(m_nAreaSel == 1)	bSearchItem = false;
	if(bSearchItem)				count += itemMgr->GetItemSize();

	CString str = _T("���� \"") + m_strMatch + _T("\"");
	if(m_bUseRegex)
	{
		str = str + _T(", ʹ��������ʽ");
	}
	else
	{
		if(m_bMatchUpDown)			str = str + _T(", ��Сдƥ��");
		if(m_bMatchWhole)			str = str + _T(", ȫ�ַ�ƥ��");
	}
	CGbl::PrintLock(true);
	CGbl::PrintOut(str, true);

	int matchCount = 0;
	if(bSearchItem)				matchCount += itemMgr->SearchItem(m_strMatch, m_bMatchUpDown, m_bMatchWhole, m_bUseRegex);

	str.Format("��������  ƥ����: %d    �ϼ���������: %d", matchCount, count);
	CGbl::PrintLock(false);
	CGbl::PrintOut(str);
}

BOOL Dialog::CFindDlg::OnHelpInfo(HELPINFO* pHelpInfo)
{
	SoftInfo::CMyHelp::GetMe().ShowHelp(_T("����"));
	return CDialog::OnHelpInfo(pHelpInfo);
}
