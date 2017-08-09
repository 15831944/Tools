// FindDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "DXPEditor.h"
#include "MyHelp.h"
#include "Gbl.h"
#include "FindDlg.h"

#include "ItemMgr.h"
#include "DevMgr.h"

// CFindDlg 对话框
using namespace Dialog;
IMPLEMENT_DYNAMIC(CFindDlg, CDialog)

CFindDlg::CFindDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CFindDlg::IDD, pParent)
	, m_bMatchUpDown(TRUE)
	, m_bMatchWhole(TRUE)
	, m_bUseRegex(FALSE)
{
	m_ltMatchStr.push_back(_T(""));
	m_ltFindArea.push_back(_T("变量"));
	m_ltFindArea.push_back(_T("设备"));
	m_ltFindArea.push_back(_T("变量和设备"));
	m_nAreaSel = 2;
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

// 	DDX_Control(pDX, IDC_CHECK_ITEM_ID, m_CK_Item_ID);
// 	DDX_Control(pDX, IDC_CHECK_ITEM_NAME, m_CK_Item_Name);
// 	DDX_Control(pDX, IDC_CHECK_ITEM_TAG, m_CK_Item_Tag);
// 	DDX_Control(pDX, IDC_CHECK_ITEM_DESP, m_CK_Item_Desp);
// 	DDX_Control(pDX, IDC_CHECK_ITEM_SRC, m_CK_Item_Src);
// 	DDX_Control(pDX, IDC_CHECK_ITEM_ACCESS, m_CK_Item_Access);
// 	DDX_Control(pDX, IDC_CHECK_ITEM_PROJTYPE, m_CK_Item_ProjType);
// 	DDX_Control(pDX, IDC_CHECK_ITEM_IOTYPE, m_CK_Item_IOType);
// 	DDX_Control(pDX, IDC_CHECK_ITEM_CONVERT, m_CK_Item_Convert);
// 	DDX_Control(pDX, IDC_CHECK_ITEM_SCRIPT, m_CK_Item_Script);
// 	DDX_Control(pDX, IDC_CHECK_ITEM_DEVICE, m_CK_Item_Device);
// 	DDX_Control(pDX, IDC_CHECK_ITEM_ADDR, m_CK_Item_Addr);
// 	DDX_Control(pDX, IDC_CHECK_ITEM_FRESHTIME, m_CK_Item_FreshTime);
// 	DDX_Control(pDX, IDC_CHECK_ITEM_ALARMONOFF, m_CK_Item_AlarmOnOff);
// 	DDX_Control(pDX, IDC_CHECK_ITEM_ALARMLIMIT, m_CK_Item_AlarmLimit);
// 	DDX_Control(pDX, IDC_CHECK_ITEM_ALARMAIM, m_CK_Item_AlarmAim);
// 	DDX_Control(pDX, IDC_CHECK_ITEM_ALARMSHIFT, m_CK_Item_AlarmShift);
// 	DDX_Control(pDX, IDC_CHECK_DEV_ID, m_CK_Dev_ID);
// 	DDX_Control(pDX, IDC_CHECK_DEV_NAME, m_CK_Dev_Name);
// 	DDX_Control(pDX, IDC_CHECK_DEV_TAG, m_CK_Dev_Tag);
// 	DDX_Control(pDX, IDC_CHECK_DEV_TYPE, m_CK_Dev_Type);
// 	DDX_Control(pDX, IDC_CHECK_DEV_INTF, m_CK_Dev_Inf);
}

BEGIN_MESSAGE_MAP(CFindDlg, CDialog)
	ON_BN_CLICKED(IDOK, &CFindDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDC_CHECK_REGEX, &OnBnClickedRegex)
	ON_WM_HELPINFO()
END_MESSAGE_MAP()


// CFindDlg 消息处理程序
BOOL CFindDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	foreach(CString str, m_ltMatchStr)		m_cbFindText.AddString(str);
	foreach(CString str, m_ltFindArea)		m_cbFindArea.AddString(str);
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
	MVC::Device::CDevMgr* devMgr = &MVC::Device::CDevMgr::GetMe();
	int count = 0;
	bool bSearchItem = true, bSearchDev = true;
	if(m_nAreaSel == 0)			bSearchDev = false;
	else if(m_nAreaSel == 1)	bSearchItem = false;
	if(bSearchItem)				count += itemMgr->GetItemSize();
	if(bSearchDev)				count += devMgr->GetDeviceSize();

	CString str = _T("查找 \"") + m_strMatch + _T("\"");
	if(m_bUseRegex)
	{
		str = str + _T(", 使用正则表达式");
	}
	else
	{
		if(m_bMatchUpDown)			str = str + _T(", 大小写匹配");
		if(m_bMatchWhole)			str = str + _T(", 全字符匹配");
	}
	CGbl::PrintLock(true);
	CGbl::PrintOut(str, true);
//	CGbl::SetProgressRange(count);
	int matchCount = 0;
	if(bSearchItem)				matchCount += itemMgr->SearchItem(m_strMatch, m_bMatchUpDown, m_bMatchWhole, m_bUseRegex);
	if(bSearchDev)				matchCount += devMgr->SearchDev(m_strMatch, m_bMatchUpDown, m_bMatchWhole, m_bUseRegex);
//	CGbl::SetProgressEnd();
	str.Format("搜索结束  匹配数: %d    合计搜索对象: %d", matchCount, count);
	CGbl::PrintLock(false);
	CGbl::PrintOut(str);
}

BOOL Dialog::CFindDlg::OnHelpInfo(HELPINFO* pHelpInfo)
{
	SoftInfo::CMyHelp::GetMe().ShowHelp(_T("查找"));
	return CDialog::OnHelpInfo(pHelpInfo);
}
