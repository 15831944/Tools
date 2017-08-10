// ItemInConfigDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "DXPEditor.h"
#include "MyHelp.h"
#include "MainFrm.h"
#include "ItemMgr.h"
#include "ItemGroup.h"
#include "ItemDoc.h"
#include "ItemView.h"
#include "ItemFrame.h"

#include "AddItemGroupDlg.h"
#include "ItemInConfigDlg.h"

#include "DevMgr.h"
#include "DeviceOne.h"


// CItemInConfigDlg 对话框
using namespace Dialog;
IMPLEMENT_DYNAMIC(CItemInConfigDlg, CDialog)

CItemInConfigDlg::CItemInConfigDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CItemInConfigDlg::IDD, pParent)
{
	m_nSameNameType = 0;
	m_nBaseIDSame = 0;
	m_strGroupName = _T("");
	m_strItemSameName = _T("");
	m_strTitle = _T("变量表导入配置");
	m_GroupTree.SetOwner(this);
	m_bKeep = FALSE;
	m_bUseBaseID = FALSE;
	m_nBaseID = 100;
	m_bNameName = FALSE;
	m_bTagName = FALSE;
	m_bDespName = FALSE;
	m_bUseAdvanceOptions = FALSE;

	m_nSameNameAddr = 1;

	m_nNameAddr = 1;
	m_nTagAddr = 1;
	m_nDespAddr = 1;
}

CItemInConfigDlg::~CItemInConfigDlg()
{
}

CItemInConfigDlg& CItemInConfigDlg::GetMe()
{
	static CItemInConfigDlg one;
	return one;
}

void CItemInConfigDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_PARENT_NAME, m_strGroupName);
	DDX_Control(pDX, IDC_GROUP_TREE, m_GroupTree);
	DDX_Control(pDX, IDC_CB_DEVICE, m_cbDevice);
	DDX_Radio(pDX, IDC_RADIO_ITEMNAME1, m_nSameNameType);
	DDX_Radio(pDX, IDC_RADIO_SAMENAME_FRONT, m_nSameNameAddr);
	DDX_Text(pDX, IDC_ITEMIN_STR, m_strItemSameName);
	DDX_Check(pDX, IDC_CHECK_KEEP, m_bKeep);
	DDX_Check(pDX, IDC_CHECK_ADVANCE_OPTIONS, m_bUseAdvanceOptions);
	DDX_Check(pDX, IDC_CHECK_BASEID, m_bUseBaseID);
	DDX_Radio(pDX, IDC_RADIO_SAMEID1, m_nBaseIDSame);
	DDX_Text(pDX, IDC_EDIT_BASEID, m_nBaseID);
	DDX_Check(pDX, IDC_CHECK_NAME, m_bNameName);
	DDX_Check(pDX, IDC_CHECK_TAG, m_bTagName);
	DDX_Check(pDX, IDC_CHECK_DESP, m_bDespName);
	DDX_Text(pDX, IDC_ITEMIN_NAMESTR, m_strNameName);
	DDX_Text(pDX, IDC_ITEMIN_TAGSTR, m_strTagName);
	DDX_Text(pDX, IDC_ITEMIN_DESPSTR, m_strDespName);
	DDX_Radio(pDX, IDC_RADIO_NAME_FRONT, m_nNameAddr);
	DDX_Radio(pDX, IDC_RADIO_TAG_FRONT, m_nTagAddr);
	DDX_Radio(pDX, IDC_RADIO_DESP_FRONT, m_nDespAddr);
}


BEGIN_MESSAGE_MAP(CItemInConfigDlg, CDialog)
	ON_BN_CLICKED(ID_ADD_GROUP, &CItemInConfigDlg::OnBnClickedAddGroup)
	ON_BN_CLICKED(IDOK, &CItemInConfigDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CItemInConfigDlg::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_RADIO_ITEMNAME1, &CItemInConfigDlg::OnBnClickedRadioItemname1)
	ON_BN_CLICKED(IDC_RADIO_ITEMNAME2, &CItemInConfigDlg::OnBnClickedRadioItemname2)
	ON_BN_CLICKED(IDC_RADIO_ITEMNAME3, &CItemInConfigDlg::OnBnClickedRadioItemname3)
	ON_BN_CLICKED(IDC_RADIO_ITEMNAME4, &CItemInConfigDlg::OnBnClickedRadioItemname4)
	ON_BN_CLICKED(IDC_CHECK_ADVANCE_OPTIONS, &CItemInConfigDlg::OnBnClickAdvanceOptions)
	ON_BN_CLICKED(IDC_CHECK_BASEID, &CItemInConfigDlg::OnBnClickBaseID)
	ON_BN_CLICKED(IDC_CHECK_NAME, &CItemInConfigDlg::OnBnClickNameStr)
	ON_BN_CLICKED(IDC_CHECK_TAG, &CItemInConfigDlg::OnBnClickTagStr)
	ON_BN_CLICKED(IDC_CHECK_DESP, &CItemInConfigDlg::OnBnClickDespStr)
	ON_WM_HELPINFO()
END_MESSAGE_MAP()


// CItemInConfigDlg 消息处理程序

INT_PTR CItemInConfigDlg::DoModal(UINT type/* = 0*/, int groupId /* = -1 */)
{
	m_uiShowType = type;
	if(m_uiShowType == ItemIn)
	{
		m_strTitle = _T("变量导入配置");
		m_bKeep = m_bKeepItemIn;
		m_bUseAdvanceOptions = m_bUseAdvanceOptionsItemIn;
	}
	else
	{
		m_uiShowType = Paste;
		m_strTitle = _T("变量粘贴配置");
		m_bKeep = m_bKeepPaste;
		m_bUseAdvanceOptions = m_bUseAdvanceOptionsPaste;
	}
	if(groupId == -1)	groupId = 0;
	MVC::Item::CItemMgr* mgr = &MVC::Item::CItemMgr::GetMe();
	std::shared_ptr<MVC::Item::CItemGroup> group = mgr->GetGroup(groupId);
	SetGroupName(group->getName());
	if(m_bUseAdvanceOptions)
	{
		if(m_uiShowType == ItemIn)	m_bKeep = m_bKeepItemIn = FALSE;
		else						m_bKeep = m_bKeepPaste = FALSE;
	}
	if(m_bKeep)		return IDOK;
	return CDialog::DoModal();
}

//!< 添加变量组
BOOL CItemInConfigDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	SetWindowText(m_strTitle);
	m_GroupTree.InitImg();

	//!< 初始化设备列表
	MVC::Device::CDevMgr* devMgr = &MVC::Device::CDevMgr::GetMe();
	std::list<CString> ltName;
	int defIndex = 0;
	devMgr->GetDeviceNameList(ltName, defIndex);
	m_cbDevice.ResetContent();
	m_strDevice = _T("");
	for (CString name : ltName)	m_cbDevice.AddString(name);
	if(!ltName.empty())				m_cbDevice.SetCurSel(0);


	//!< 重名处理
	if(m_nSameNameType != 3)
	{
		GetDlgItem(IDC_ITEMIN_STR)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_RADIO_SAMENAME_FRONT)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_RADIO_SAMENAME_BACK)->ShowWindow(SW_HIDE);
	}
	else
	{
		GetDlgItem(IDC_ITEMIN_STR)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_RADIO_SAMENAME_FRONT)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_RADIO_SAMENAME_BACK)->ShowWindow(SW_SHOW);
	}
	UpdateGroupTree();

	//!< 看看高级选项
	OnBnClickAdvanceOptions();
	OnBnClickAdvanceOptions();
	if(m_bUseBaseID)
	{
		GetDlgItem(IDC_EDIT_BASEID)->EnableWindow(TRUE);
		GetDlgItem(IDC_RADIO_SAMEID1)->EnableWindow(TRUE);
		GetDlgItem(IDC_RADIO_SAMEID2)->EnableWindow(TRUE);
		GetDlgItem(IDC_RADIO_SAMEID3)->EnableWindow(TRUE);
	}
	else
	{
		GetDlgItem(IDC_EDIT_BASEID)->EnableWindow(FALSE);
		GetDlgItem(IDC_RADIO_SAMEID1)->EnableWindow(FALSE);
		GetDlgItem(IDC_RADIO_SAMEID2)->EnableWindow(FALSE);
		GetDlgItem(IDC_RADIO_SAMEID3)->EnableWindow(FALSE);
	}
	if(m_bNameName)
	{
		GetDlgItem(IDC_ITEMIN_NAMESTR)->EnableWindow(TRUE);
		GetDlgItem(IDC_RADIO_NAME_FRONT)->EnableWindow(TRUE);
		GetDlgItem(IDC_RADIO_NAME_BACK)->EnableWindow(TRUE);
	}
	else
	{
		GetDlgItem(IDC_ITEMIN_NAMESTR)->EnableWindow(FALSE);
		GetDlgItem(IDC_RADIO_NAME_FRONT)->EnableWindow(FALSE);
		GetDlgItem(IDC_RADIO_NAME_BACK)->EnableWindow(FALSE);
	}
	if(m_bTagName)
	{
		GetDlgItem(IDC_ITEMIN_TAGSTR)->EnableWindow(TRUE);
		GetDlgItem(IDC_RADIO_TAG_FRONT)->EnableWindow(TRUE);
		GetDlgItem(IDC_RADIO_TAG_BACK)->EnableWindow(TRUE);
	}
	else
	{
		GetDlgItem(IDC_ITEMIN_TAGSTR)->EnableWindow(FALSE);
		GetDlgItem(IDC_RADIO_TAG_FRONT)->EnableWindow(FALSE);
		GetDlgItem(IDC_RADIO_TAG_BACK)->EnableWindow(FALSE);
	}
	if(m_bDespName)
	{
		GetDlgItem(IDC_ITEMIN_DESPSTR)->EnableWindow(TRUE);
		GetDlgItem(IDC_RADIO_DESP_FRONT)->EnableWindow(TRUE);
		GetDlgItem(IDC_RADIO_DESP_BACK)->EnableWindow(TRUE);
	}
	else
	{
		GetDlgItem(IDC_ITEMIN_DESPSTR)->EnableWindow(FALSE);
		GetDlgItem(IDC_RADIO_DESP_FRONT)->EnableWindow(FALSE);
		GetDlgItem(IDC_RADIO_DESP_BACK)->EnableWindow(FALSE);
	}
	UpdateData(FALSE);
	return TRUE;
}

int CItemInConfigDlg::GetDeviceID()
{
	if(m_strDevice == _T(""))	return -1;
	std::shared_ptr<MVC::Device::CDeviceOne> device = MVC::Device::CDevMgr::GetMe().GetDevice(m_strDevice);
	if(!device)					return -1;
	return device->getID();
}

void CItemInConfigDlg::UpdateGroupTree()
{
	m_GroupTree.DeleteAllItems();
	MVC::Item::CItemMgr* mgr = &MVC::Item::CItemMgr::GetMe();
	std::shared_ptr<MVC::Item::CItemGroup> group = mgr->GetGroup(0);
	HTREEITEM root = NULL, selItem = NULL;
	if(group){
		root = m_GroupTree.InsertItem(group->getName(), 6, 7, TVI_ROOT, TVI_LAST);
		if(group->getID() == m_uiGroupID)		selItem = root;
		AddGroupChild(root, selItem, group->getID());
		m_GroupTree.Expand(root, TVE_EXPAND);
	}
	m_GroupTree.SelectItem(selItem);
	m_GroupTree.SelectDropTarget(selItem);
}

void CItemInConfigDlg::AddGroupChild(HTREEITEM root, HTREEITEM& selItem, UINT parentID)
{
	MVC::Item::CItemMgr* mgr = &MVC::Item::CItemMgr::GetMe();
	if(!mgr->GetGroup(parentID))		return;
	HTREEITEM item = NULL;
	std::list<UINT> ltChildID = mgr->GetGroup(parentID)->getGroupIDList();
	for (UINT i : ltChildID){
		if(!mgr->GetGroup(i))	continue;
		item = m_GroupTree.InsertItem(mgr->GetGroup(i)->getName(), 6, 7, root, TVI_LAST);
		if(i == m_uiGroupID)			selItem = item;
		AddGroupChild(item, selItem,i);
		m_GroupTree.Expand(item, TVE_EXPAND);
	}
}

//!< 用户可以在这里临时添加变量组
void CItemInConfigDlg::OnBnClickedAddGroup()
{
	Dialog::CAddItemGroupDlg* dlg = &Dialog::CAddItemGroupDlg::GetMe();
	std::shared_ptr<MVC::Item::CItemGroup> group;
	if(dlg->DoModal() == IDOK)
	{
		group = std::shared_ptr<MVC::Item::CItemGroup>(new MVC::Item::CItemGroup(dlg->m_strGroupName, dlg->m_uiParentID));
		MVC::Item::CItemMgr::GetMe().AddGroup(group, dlg->m_uiParentID);
		CString text = m_strGroupName;
		UpdateGroupTree();
		SetGroupName(text);

		//!< 如果当前CItemGroupFrame被打开了，则它需要重画
		CMainFrame* pMainFram = (CMainFrame *)g_App.GetMainWnd();
		ASSERT(pMainFram);
		pMainFram->GetSpaceItem()->UpdateTreeView();
	}
}

void CItemInConfigDlg::OnBnClickedOk()
{
	UpdateData(TRUE);
	if(m_uiShowType == ItemIn)
	{
		m_bKeepItemIn = m_bKeep;
		m_bUseAdvanceOptionsItemIn = m_bUseAdvanceOptions;
	}
	else
	{
		m_bKeepPaste = m_bKeep;
		m_bUseAdvanceOptionsPaste = m_bUseAdvanceOptions;
	}
	m_cbDevice.GetWindowText(m_strDevice);
	OnOK();
}

void CItemInConfigDlg::OnBnClickedCancel()
{
	OnCancel();
}

void CItemInConfigDlg::OnTreeDblClick(CTreeCtrl* treeCtrl, HTREEITEM item)
{
	OnTreeLClick(treeCtrl, item);
}

void CItemInConfigDlg::OnTreeLClick(CTreeCtrl* treeCtrl, HTREEITEM item)
{
	SetGroupName(treeCtrl->GetItemText(item));
	UpdateData(FALSE);
}

void CItemInConfigDlg::OnTreeRClick(CTreeCtrl* treeCtrl, HTREEITEM item, CPoint point)
{
	OnTreeLClick(treeCtrl, item);
}

void CItemInConfigDlg::OnTreeKeyDown(CTreeCtrl* treeCtrl, HTREEITEM item, UINT nChar)
{
	if(nChar == VK_RETURN){
		treeCtrl->SelectDropTarget(item);
		OnTreeLClick(treeCtrl, item);
	}
}

void CItemInConfigDlg::SetGroupName(CString name)
{
	std::shared_ptr<MVC::Item::CItemGroup> group;
	group = MVC::Item::CItemMgr::GetMe().GetGroup(name);
	if(!group)		return;
	m_strGroupName = group->getName();
	m_uiGroupID = group->getID();
}

void Dialog::CItemInConfigDlg::OnBnClickedRadioItemname1()
{
	UpdateData(TRUE);
	if(m_nSameNameType != 3)
	{
		GetDlgItem(IDC_ITEMIN_STR)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_RADIO_SAMENAME_FRONT)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_RADIO_SAMENAME_BACK)->ShowWindow(SW_HIDE);
	}
	else
	{
		GetDlgItem(IDC_ITEMIN_STR)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_RADIO_SAMENAME_FRONT)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_RADIO_SAMENAME_BACK)->ShowWindow(SW_SHOW);
	}
}

void Dialog::CItemInConfigDlg::OnBnClickedRadioItemname2()
{
	UpdateData(TRUE);
	if(m_nSameNameType != 3)
	{
		GetDlgItem(IDC_ITEMIN_STR)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_RADIO_SAMENAME_FRONT)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_RADIO_SAMENAME_BACK)->ShowWindow(SW_HIDE);
	}
	else
	{
		GetDlgItem(IDC_ITEMIN_STR)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_RADIO_SAMENAME_FRONT)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_RADIO_SAMENAME_BACK)->ShowWindow(SW_SHOW);
	}
	UpdateData(FALSE);
}

void Dialog::CItemInConfigDlg::OnBnClickedRadioItemname3()
{
	UpdateData(TRUE);
	if(m_nSameNameType != 3)
	{
		GetDlgItem(IDC_ITEMIN_STR)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_RADIO_SAMENAME_FRONT)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_RADIO_SAMENAME_BACK)->ShowWindow(SW_HIDE);
	}
	else
	{
		GetDlgItem(IDC_ITEMIN_STR)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_RADIO_SAMENAME_FRONT)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_RADIO_SAMENAME_BACK)->ShowWindow(SW_SHOW);
	}
}

void Dialog::CItemInConfigDlg::OnBnClickedRadioItemname4()
{
	UpdateData(TRUE);
	if(m_nSameNameType != 3)
	{
		GetDlgItem(IDC_ITEMIN_STR)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_RADIO_SAMENAME_FRONT)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_RADIO_SAMENAME_BACK)->ShowWindow(SW_HIDE);
	}
	else
	{
		GetDlgItem(IDC_ITEMIN_STR)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_RADIO_SAMENAME_FRONT)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_RADIO_SAMENAME_BACK)->ShowWindow(SW_SHOW);
	}
}

void Dialog::CItemInConfigDlg::OnBnClickAdvanceOptions()
{
	CRect rect;
	GetWindowRect(&rect);
	m_bUseAdvanceOptions = !m_bUseAdvanceOptions;
	if(m_bUseAdvanceOptions)
	{
		GetDlgItem(IDC_CHECK_ADVANCE_OPTIONS)->SetWindowText(_T("高级 <<"));
		MoveWindow(rect.left, rect.top, 567, rect.Height());
	}
	else
	{
		GetDlgItem(IDC_CHECK_ADVANCE_OPTIONS)->SetWindowText(_T("高级 >>"));
		MoveWindow(rect.left, rect.top, 372, rect.Height());
	}
	UpdateData(FALSE);
}

void Dialog::CItemInConfigDlg::OnBnClickBaseID()
{
	UpdateData();
	if(m_bUseBaseID)
	{
		GetDlgItem(IDC_EDIT_BASEID)->EnableWindow(TRUE);
		GetDlgItem(IDC_RADIO_SAMEID1)->EnableWindow(TRUE);
		GetDlgItem(IDC_RADIO_SAMEID2)->EnableWindow(TRUE);
		GetDlgItem(IDC_RADIO_SAMEID3)->EnableWindow(TRUE);
	}
	else
	{
		GetDlgItem(IDC_EDIT_BASEID)->EnableWindow(FALSE);
		GetDlgItem(IDC_RADIO_SAMEID1)->EnableWindow(FALSE);
		GetDlgItem(IDC_RADIO_SAMEID2)->EnableWindow(FALSE);
		GetDlgItem(IDC_RADIO_SAMEID3)->EnableWindow(FALSE);
	}
}

void Dialog::CItemInConfigDlg::OnBnClickNameStr()
{
	UpdateData();
	if(m_bNameName)
	{
		GetDlgItem(IDC_ITEMIN_NAMESTR)->EnableWindow(TRUE);
		GetDlgItem(IDC_RADIO_NAME_FRONT)->EnableWindow(TRUE);
		GetDlgItem(IDC_RADIO_NAME_BACK)->EnableWindow(TRUE);
	}
	else
	{
		GetDlgItem(IDC_ITEMIN_NAMESTR)->EnableWindow(FALSE);
		GetDlgItem(IDC_RADIO_NAME_FRONT)->EnableWindow(FALSE);
		GetDlgItem(IDC_RADIO_NAME_BACK)->EnableWindow(FALSE);
	}
}

void Dialog::CItemInConfigDlg::OnBnClickTagStr()
{
	UpdateData();
	if(m_bTagName)
	{
		GetDlgItem(IDC_ITEMIN_TAGSTR)->EnableWindow(TRUE);
		GetDlgItem(IDC_RADIO_TAG_FRONT)->EnableWindow(TRUE);
		GetDlgItem(IDC_RADIO_TAG_BACK)->EnableWindow(TRUE);
	}
	else
	{
		GetDlgItem(IDC_ITEMIN_TAGSTR)->EnableWindow(FALSE);
		GetDlgItem(IDC_RADIO_TAG_FRONT)->EnableWindow(FALSE);
		GetDlgItem(IDC_RADIO_TAG_BACK)->EnableWindow(FALSE);
	}
}

void Dialog::CItemInConfigDlg::OnBnClickDespStr()
{
	UpdateData();
	if(m_bDespName)
	{
		GetDlgItem(IDC_ITEMIN_DESPSTR)->EnableWindow(TRUE);
		GetDlgItem(IDC_RADIO_DESP_FRONT)->EnableWindow(TRUE);
		GetDlgItem(IDC_RADIO_DESP_BACK)->EnableWindow(TRUE);
	}
	else
	{
		GetDlgItem(IDC_ITEMIN_DESPSTR)->EnableWindow(FALSE);
		GetDlgItem(IDC_RADIO_DESP_FRONT)->EnableWindow(FALSE);
		GetDlgItem(IDC_RADIO_DESP_BACK)->EnableWindow(FALSE);
	}
}

BOOL Dialog::CItemInConfigDlg::OnHelpInfo(HELPINFO* pHelpInfo)
{
	if(m_uiShowType == 0)
		SoftInfo::CMyHelp::GetMe().ShowHelp(_T("变量导出、导入"));
	else
		SoftInfo::CMyHelp::GetMe().ShowHelp(_T("复制、剪切、粘贴"));
	return __super::OnHelpInfo(pHelpInfo);
}
