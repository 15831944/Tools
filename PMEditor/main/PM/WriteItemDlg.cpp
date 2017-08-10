// WriteItemDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "PMApp.h"
#include "MyHelp.h"
#include "WriteItemDlg.h"

#include "ItemMgr.h"
#include "Item.h"

using namespace Dialog;
// CWriteItemDlg 对话框
IMPLEMENT_DYNAMIC(CWriteItemDlg, CDialog)

CWriteItemDlg::CWriteItemDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CWriteItemDlg::IDD, pParent)
	, m_strName(_T(""))
	, m_strType(_T(""))
	, m_strValueNew(_T(""))
	, m_bWriteIO(FALSE)
{
}

CWriteItemDlg::~CWriteItemDlg()
{
}

CWriteItemDlg& CWriteItemDlg::GetMe()
{
	static CWriteItemDlg one;
	return one;
}

void CWriteItemDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_NAME, m_strName);
	DDX_Text(pDX, IDC_TYPE, m_strType);
	DDX_Text(pDX, IDC_VALUE_NEW, m_strValueNew);
	DDX_Radio(pDX, IDC_RADIO_PROJ, m_bWriteIO);
}


BEGIN_MESSAGE_MAP(CWriteItemDlg, CDialog)
	ON_BN_CLICKED(IDOK, &CWriteItemDlg::OnBnClickedOk)
	ON_WM_HELPINFO()
END_MESSAGE_MAP()


// CWriteItemDlg 消息处理程序
INT_PTR CWriteItemDlg::DoModal(UINT id)
{
	m_uiID = id;
	return CDialog::DoModal();
}

void CWriteItemDlg::OnBnClickedOk()
{
	UpdateData(TRUE);
	if(m_strValueNew != _T("")){
		MVC::Item::CItemMgr::GetMe().WriteItemValue(m_uiID, !m_bWriteIO, m_strValueNew);
	}
	OnOK();
}

BOOL CWriteItemDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	std::shared_ptr<MVC::Item::CItem> pItem = MVC::Item::CItemMgr::GetMe().GetItem(m_uiID);
	ASSERT(pItem);
	m_strName = pItem->getName();
	m_strType = pItem->GetValTypeStr();
	CString title = m_strName;
	if(pItem->getAccessRight() == 1)		//!< 如果是只读的
	{
		title += _T(" (只读-无法写入)");
		GetDlgItem(IDC_VALUE_NEW)->EnableWindow(FALSE);
		GetDlgItem(IDOK)->EnableWindow(FALSE);
	}
	else
	{
		GetDlgItem(IDC_VALUE_NEW)->EnableWindow(TRUE);
		GetDlgItem(IDOK)->EnableWindow(TRUE);
	}
	SetWindowText(title);
	UpdateData(FALSE);
	return TRUE;
}

BOOL Dialog::CWriteItemDlg::OnHelpInfo(HELPINFO* pHelpInfo)
{
	SoftInfo::CMyHelp::GetMe().ShowHelp(_T("变量监控"));
	return CDialog::OnHelpInfo(pHelpInfo);
}
