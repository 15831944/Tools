// EtherNetSetDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "PMApp.h"
#include "MyHelp.h"
#include "Gbl.h"
#include "DevMgr.h"
#include "EtherNetSetDlg.h"


// CEtherNetSetDlg 对话框
using namespace Dialog;
IMPLEMENT_DYNAMIC(CEtherNetSetDlg, CXTResizePropertyPage)

CEtherNetSetDlg::CEtherNetSetDlg(CWnd* pParent /*=NULL*/)
	: CXTResizePropertyPage(CEtherNetSetDlg::IDD)
{

}

CEtherNetSetDlg::~CEtherNetSetDlg()
{
}

void CEtherNetSetDlg::DoDataExchange(CDataExchange* pDX)
{
	CXTResizePropertyPage::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_ETHERNET_HOLDER, m_GridHolder);
	DDX_Control(pDX, IDC_ETHERNET_TREE, m_TreeCtrl);
}


BEGIN_MESSAGE_MAP(CEtherNetSetDlg, CXTResizePropertyPage)
	ON_MESSAGE(XTPWM_PROPERTYGRID_NOTIFY, OnGridNotify)
	ON_BN_CLICKED(ID_ADD_ETHERNET, &CEtherNetSetDlg::OnBnClickedAddEthernet)
	ON_WM_HELPINFO()
END_MESSAGE_MAP()


// CEtherNetSetDlg 消息处理程序
BOOL CEtherNetSetDlg::OnInitDialog()
{
	CXTResizePropertyPage::OnInitDialog();

	//!< 初始化Property表格
	CRect rc;
	m_GridHolder.GetWindowRect(&rc);
	m_GridHolder.ShowWindow(SW_HIDE);
	ScreenToClient(&rc);
	if(!m_PropertyGrid.Create(rc, this, IDC_PROPERTY_GRID))		return FALSE;
	m_PropertyGrid.SetFont(GetFont());							//!< 设置一下字体，以确保每次打开的字体都一样
	//m_PropertyGrid.HighlightChangedItems(TRUE);					//!< 高亮被修改的部分
	m_PropertyGrid.SetTheme(xtpGridThemeNativeWinXP);			//!< 设置主题
	m_Object.ShowInfo(m_PropertyGrid);

	//!< 初始化网卡树
	m_TreeCtrl.SetOwner(this);
	m_TreeCtrl.InitImg();
	//m_TreeCtrl.InsertItem(CGbl::GetHostIPString(), 15, 15, TVI_ROOT, TVI_LAST);
	m_TreeCtrl.InsertItem(_T("Localhost"), 15, 15, TVI_ROOT, TVI_LAST);

	return TRUE;
}

LRESULT CEtherNetSetDlg::OnGridNotify(WPARAM wParam, LPARAM lParam)
{
	if (wParam == XTP_PGN_ITEMVALUE_CHANGED){		//!< 项的值被改变
		CXTPPropertyGridItem* pItem = (CXTPPropertyGridItem*)lParam;
		m_Object.OnItemModify(m_PropertyGrid, pItem->GetID());
	}
	return 0;
}
void CEtherNetSetDlg::OnBnClickedAddEthernet()
{
	// TODO: 在此添加控件通知处理程序代码
}

void CEtherNetSetDlg::OnTreeLClick(CTreeCtrl* pTreeCtrl, HTREEITEM hItem)
{
	UpdateData(FALSE);
}

void CEtherNetSetDlg::OnTreeRClick(CTreeCtrl* pTreeCtrl, HTREEITEM hItem, CPoint point)
{
}

void CEtherNetSetDlg::OnTreeKeyDown(CTreeCtrl* pTreeCtrl, HTREEITEM hItem, UINT nChar)
{
}

BOOL Dialog::CEtherNetSetDlg::OnHelpInfo(HELPINFO* pHelpInfo)
{
	m_Object.OnShowHelp();
	return CDialog::OnHelpInfo(pHelpInfo);
}
