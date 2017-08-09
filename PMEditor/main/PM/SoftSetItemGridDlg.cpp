// SoftSetItemGridDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "DXPEditor.h"
#include "MyHelp.h"
#include "Gbl.h"
#include "ItemInConfigDlg.h"
#include "SoftInfo.h"
#include "ItemMgr.h"
#include "ItemGrid.h"
#include "SoftSetItemGridDlg.h"


// CSoftSetItemGridDlg 对话框
using namespace SoftInfo;
IMPLEMENT_DYNAMIC(CSoftSetItemGridDlg, CDialog)

CSoftSetItemGridDlg::CSoftSetItemGridDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSoftSetItemGridDlg::IDD, pParent)
{
	m_bCheck0 = m_bCheck1 = m_bCheck2 = m_bCheck3 = m_bCheck4 = TRUE;
	m_bCheck5 = m_bCheck6 = m_bCheck7 = m_bCheck8 = m_bCheck9 = TRUE;
	m_bCheck10 = m_bCheck11 = m_bCheck12 = m_bCheck13 = m_bCheck14 = TRUE;
	m_bCheck15 = m_bCheck16 = m_bCheck17 = m_bCheck18 = m_bCheck19 = TRUE;
	m_bCheck20 = m_bCheck21 = m_bCheck22 = TRUE;
	m_bCheck30 = FALSE;		// Modbus485
	m_uiFloatWidth = 6;
	m_bPrintWaterMark = FALSE;
}

CSoftSetItemGridDlg::~CSoftSetItemGridDlg()
{
}

void CSoftSetItemGridDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Check(pDX, IDC_CHECK0, m_bCheck0);
	DDX_Check(pDX, IDC_CHECK1, m_bCheck1);
	DDX_Check(pDX, IDC_CHECK2, m_bCheck2);
	DDX_Check(pDX, IDC_CHECK3, m_bCheck3);
	DDX_Check(pDX, IDC_CHECK4, m_bCheck4);
	DDX_Check(pDX, IDC_CHECK5, m_bCheck5);
	DDX_Check(pDX, IDC_CHECK6, m_bCheck6);
	DDX_Check(pDX, IDC_CHECK7, m_bCheck7);
	DDX_Check(pDX, IDC_CHECK8, m_bCheck8);
	DDX_Check(pDX, IDC_CHECK9, m_bCheck9);
	DDX_Check(pDX, IDC_CHECK10, m_bCheck10);
	DDX_Check(pDX, IDC_CHECK11, m_bCheck11);
	DDX_Check(pDX, IDC_CHECK12, m_bCheck12);
	DDX_Check(pDX, IDC_CHECK13, m_bCheck13);
	DDX_Check(pDX, IDC_CHECK14, m_bCheck14);
	DDX_Check(pDX, IDC_CHECK15, m_bCheck15);
	DDX_Check(pDX, IDC_CHECK16, m_bCheck16);
	DDX_Check(pDX, IDC_CHECK17, m_bCheck17);
	DDX_Check(pDX, IDC_CHECK18, m_bCheck18);
	DDX_Check(pDX, IDC_CHECK19, m_bCheck19);
	DDX_Check(pDX, IDC_CHECK20, m_bCheck20);
	DDX_Check(pDX, IDC_CHECK21, m_bCheck21);
	DDX_Check(pDX, IDC_CHECK22, m_bCheck22);
	DDX_Check(pDX, IDC_CHECK30, m_bCheck30);
	DDX_Check(pDX, IDC_CHECK_PASTE, m_bCheckPaste);
	DDX_Check(pDX, IDC_CHECK_ITEMIN, m_bCheckItemIn);
	DDX_Check(pDX, IDC_CHECK_HEX, m_bCheckHex);
	DDX_Text(pDX, IDC_FLOAT_WIDTH, m_uiFloatWidth);
}


BEGIN_MESSAGE_MAP(CSoftSetItemGridDlg, CDialog)
	ON_WM_HELPINFO()
END_MESSAGE_MAP()


// CSoftSetItemGridDlg 消息处理程序
BOOL CSoftSetItemGridDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	CSoftInfo* csi = &SoftInfo::CSoftInfo::GetMe();
	m_bCheck0 = !!csi->m_vtColInfo[MVC::Item::CItemGrid::COL_NAME].bShow;
	m_bCheck21 = !!csi->m_vtColInfo[MVC::Item::CItemGrid::COL_ID].bShow;
	m_bCheck1 = !!csi->m_vtColInfo[MVC::Item::CItemGrid::COL_TAG].bShow;
	m_bCheck2 = !!csi->m_vtColInfo[MVC::Item::CItemGrid::COL_SRC].bShow;
	m_bCheck3 = !!csi->m_vtColInfo[MVC::Item::CItemGrid::COL_ACCESS].bShow;
	m_bCheck4 = !!csi->m_vtColInfo[MVC::Item::CItemGrid::COL_TYPE].bShow;
	m_bCheck5 = !!csi->m_vtColInfo[MVC::Item::CItemGrid::COL_IOTYPE].bShow;
	m_bCheck6 = !!csi->m_vtColInfo[MVC::Item::CItemGrid::COL_VALUE].bShow;
	m_bCheck7 = !!csi->m_vtColInfo[MVC::Item::CItemGrid::COL_IOVALUE].bShow;
	m_bCheck8 = !!csi->m_vtColInfo[MVC::Item::CItemGrid::COL_CONVERT].bShow;
	m_bCheck22 = !!csi->m_vtColInfo[MVC::Item::CItemGrid::COL_GROUP].bShow;
	m_bCheck9 = !!csi->m_vtColInfo[MVC::Item::CItemGrid::COL_DESCRIPT].bShow;
	m_bCheck10 = !!csi->m_vtColInfo[MVC::Item::CItemGrid::COL_SCRIPT].bShow;
	m_bCheck11 = !!csi->m_vtColInfo[MVC::Item::CItemGrid::COL_DEVICE].bShow;
	m_bCheck12 = !!csi->m_vtColInfo[MVC::Item::CItemGrid::COL_ADDR].bShow;
	m_bCheck13 = !!csi->m_vtColInfo[MVC::Item::CItemGrid::COL_FRESHTIME].bShow;
	m_bCheck14 = !!csi->m_vtColInfo[MVC::Item::CItemGrid::COL_BIT].bShow;
	m_bCheck15 = !!csi->m_vtColInfo[MVC::Item::CItemGrid::COL_LOLO].bShow;
	m_bCheck16 = !!csi->m_vtColInfo[MVC::Item::CItemGrid::COL_LOW].bShow;
	m_bCheck17 = !!csi->m_vtColInfo[MVC::Item::CItemGrid::COL_HIGH].bShow;
	m_bCheck18 = !!csi->m_vtColInfo[MVC::Item::CItemGrid::COL_HIHI].bShow;
	m_bCheck19 = !!csi->m_vtColInfo[MVC::Item::CItemGrid::COL_AIM].bShow;
	m_bCheck20 = !!csi->m_vtColInfo[MVC::Item::CItemGrid::COL_SHIFT].bShow;
	m_bCheck30 = !!csi->m_vtColInfo[MVC::Item::CItemGrid::COL_MODBUS485].bShow;
	m_uiFloatWidth = csi->getItemFloatWidth();
	Dialog::CItemInConfigDlg *itemIn = &Dialog::CItemInConfigDlg::GetMe();
	m_bCheckPaste = itemIn->m_bKeepPaste;
	m_bCheckItemIn = itemIn->m_bKeepItemIn;
	m_bCheckHex = csi->isShowHex();
//	GetDlgItem(IDC_CHECK14)->EnableWindow(FALSE);
	if (!csi->IsModbus485())	GetDlgItem(IDC_CHECK30)->ShowWindow(SW_HIDE);
	UpdateData(FALSE);
	return TRUE;  // return TRUE unless you set the focus to a control
}

BOOL CSoftSetItemGridDlg::DestroyWindow()
{
	UINT uiShowCount = 0;
	bool bNeedFresh = false;
	UpdateData(TRUE);
	CSoftInfo* csi = &SoftInfo::CSoftInfo::GetMe();
	if(!ChangeValue(csi->m_vtColInfo[MVC::Item::CItemGrid::COL_NAME].bShow, m_bCheck0, uiShowCount))		bNeedFresh = true;
	if(!ChangeValue(csi->m_vtColInfo[MVC::Item::CItemGrid::COL_ID].bShow, m_bCheck21, uiShowCount))			bNeedFresh = true;
	if(!ChangeValue(csi->m_vtColInfo[MVC::Item::CItemGrid::COL_TAG].bShow, m_bCheck1, uiShowCount))			bNeedFresh = true;
	if(!ChangeValue(csi->m_vtColInfo[MVC::Item::CItemGrid::COL_SRC].bShow, m_bCheck2, uiShowCount))			bNeedFresh = true;
	if(!ChangeValue(csi->m_vtColInfo[MVC::Item::CItemGrid::COL_ACCESS].bShow, m_bCheck3, uiShowCount))		bNeedFresh = true;
	if(!ChangeValue(csi->m_vtColInfo[MVC::Item::CItemGrid::COL_TYPE].bShow, m_bCheck4, uiShowCount))		bNeedFresh = true;
	if(!ChangeValue(csi->m_vtColInfo[MVC::Item::CItemGrid::COL_IOTYPE].bShow, m_bCheck5, uiShowCount))		bNeedFresh = true;
	if(!ChangeValue(csi->m_vtColInfo[MVC::Item::CItemGrid::COL_VALUE].bShow, m_bCheck6, uiShowCount))		bNeedFresh = true;
	if(!ChangeValue(csi->m_vtColInfo[MVC::Item::CItemGrid::COL_IOVALUE].bShow, m_bCheck7, uiShowCount))		bNeedFresh = true;
	if(!ChangeValue(csi->m_vtColInfo[MVC::Item::CItemGrid::COL_CONVERT].bShow, m_bCheck8, uiShowCount))		bNeedFresh = true;
	if(!ChangeValue(csi->m_vtColInfo[MVC::Item::CItemGrid::COL_GROUP].bShow, m_bCheck22, uiShowCount))		bNeedFresh = true;
	if(!ChangeValue(csi->m_vtColInfo[MVC::Item::CItemGrid::COL_DESCRIPT].bShow, m_bCheck9, uiShowCount))	bNeedFresh = true;
	if(!ChangeValue(csi->m_vtColInfo[MVC::Item::CItemGrid::COL_SCRIPT].bShow, m_bCheck10, uiShowCount))		bNeedFresh = true;
	if(!ChangeValue(csi->m_vtColInfo[MVC::Item::CItemGrid::COL_DEVICE].bShow, m_bCheck11, uiShowCount))		bNeedFresh = true;
	if(!ChangeValue(csi->m_vtColInfo[MVC::Item::CItemGrid::COL_ADDR].bShow, m_bCheck12, uiShowCount))		bNeedFresh = true;
	if(!ChangeValue(csi->m_vtColInfo[MVC::Item::CItemGrid::COL_FRESHTIME].bShow, m_bCheck13, uiShowCount))	bNeedFresh = true;
	if(!ChangeValue(csi->m_vtColInfo[MVC::Item::CItemGrid::COL_BIT].bShow, m_bCheck14, uiShowCount))		bNeedFresh = true;
	if(!ChangeValue(csi->m_vtColInfo[MVC::Item::CItemGrid::COL_LOLO].bShow, m_bCheck15, uiShowCount))		bNeedFresh = true;
	if(!ChangeValue(csi->m_vtColInfo[MVC::Item::CItemGrid::COL_LOW].bShow, m_bCheck16, uiShowCount))		bNeedFresh = true;
	if(!ChangeValue(csi->m_vtColInfo[MVC::Item::CItemGrid::COL_HIGH].bShow, m_bCheck17, uiShowCount))		bNeedFresh = true;
	if(!ChangeValue(csi->m_vtColInfo[MVC::Item::CItemGrid::COL_HIHI].bShow, m_bCheck18, uiShowCount))		bNeedFresh = true;
	if(!ChangeValue(csi->m_vtColInfo[MVC::Item::CItemGrid::COL_AIM].bShow, m_bCheck19, uiShowCount))		bNeedFresh = true;
	if(!ChangeValue(csi->m_vtColInfo[MVC::Item::CItemGrid::COL_SHIFT].bShow, m_bCheck20, uiShowCount))		bNeedFresh = true;
	if(!ChangeValue(csi->m_vtColInfo[MVC::Item::CItemGrid::COL_MODBUS485].bShow, m_bCheck30, uiShowCount))	bNeedFresh = true;

	if(uiShowCount == 0)		//!< 必须显示1列
		if(!ChangeValue(csi->m_vtColInfo[MVC::Item::CItemGrid::COL_NAME].bShow, TRUE, uiShowCount))			bNeedFresh = true;
	if(m_uiFloatWidth < 1)		m_uiFloatWidth = 1;
	if(m_uiFloatWidth > 12)		m_uiFloatWidth = 12;
	csi->setItemFloatWidth(m_uiFloatWidth);

	Dialog::CItemInConfigDlg *itemIn = &Dialog::CItemInConfigDlg::GetMe();
	itemIn->m_bKeepPaste = m_bCheckPaste;
	itemIn->m_bKeepItemIn = m_bCheckItemIn;

	csi->setShowHex(m_bCheckHex);

	if(bNeedFresh)				MVC::Item::CItemMgr::GetMe().FreshAllGrid();
	return CDialog::DestroyWindow();
}

//!< 将bShow赋值给bSrc，如果bShow，count自增，如果相同返回true
bool SoftInfo::CSoftSetItemGridDlg::ChangeValue(bool& bSrc, BOOL bShow, UINT& count)
{
	if(bShow)					++count;
	bool b = bShow ? true : false;
	bool ret = bSrc == !!b ? true : false;
	bSrc = !!b;
	return ret;
}

void SoftInfo::CSoftSetItemGridDlg::OnOK()
{
	CWnd* pWnd = GetParent();
	if(!pWnd)					return;
	pWnd->PostMessage(MESSAGE_OK, 0, 0);
}

BOOL SoftInfo::CSoftSetItemGridDlg::OnHelpInfo(HELPINFO* pHelpInfo)
{
	SoftInfo::CMyHelp::GetMe().ShowHelp(_T("变量表显示配置"));
	return CDialog::OnHelpInfo(pHelpInfo);
}
