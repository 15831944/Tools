#include "StdAfx.h"
#include "DXPEditor.h"
#include "MyHelp.h"
#include "ProjectMgr.h"
#include "Project.h"
#include "ItemMgr.h"
#include "ItemGroup.h"
#include "ItemDoc.h"
#include "ItemView.h"
#include "ItemFrame.h"
#include "AddItemGroupDlg.h"

#include "SpaceItem.h"

using namespace Space;
BEGIN_MESSAGE_MAP(CSpaceItem, CWnd)
	//{{AFX_MSG_MAP()
	ON_WM_CREATE()
	ON_WM_SIZE()
//	ON_COMMAND_RANGE(ID_SOLUTION_PROPERTIES, ID_SOLUTION_ADDNEWFOLDER, OnEmptyCommand)
	//}}AFX_MSG_MAP
	ON_WM_SETFOCUS()
	ON_WM_HELPINFO()
	ON_COMMAND(ID_GROUP_ADD, &OnGroupAdd)
	ON_COMMAND(ID_GROUP_EDIT, &OnGroupEdit)
	ON_COMMAND(ID_GROUP_REMOVE, &OnGroupRemove)
	ON_COMMAND(ID_PANE_ITEMGROUP, &OnItemGroup)
	ON_COMMAND(ID_PANE_ITEMDEVICE, &OnItemDevice)
	ON_UPDATE_COMMAND_UI(ID_PANE_ITEMGROUP, &OnUpdateItemGroup)
	ON_UPDATE_COMMAND_UI(ID_PANE_ITEMDEVICE, &OnUpdateItemDevice)
END_MESSAGE_MAP()

CSpaceItem::CSpaceItem(void)
{
	m_uiCulGroupID = 0;
	m_bShowItemGroup = TRUE;
}

CSpaceItem::~CSpaceItem(void)
{
}

int CSpaceItem::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)				return -1;
	//!< Create a ToolBar as usual
	VERIFY(m_wndToolBar.CreateToolBar(WS_TABSTOP|WS_VISIBLE|WS_CHILD|CBRS_TOOLTIPS, this));
	VERIFY(m_wndToolBar.LoadToolBar(IDR_PANE_ITEMVIEW));
// 	if (!m_GroupTree.Create(WS_CLIPCHILDREN|WS_CLIPSIBLINGS|WS_VISIBLE|TVS_HASLINES|TVS_LINESATROOT|TVS_HASBUTTONS|TVS_SHOWSELALWAYS,
// 		CRect(0,0,0,0), this, AFX_IDW_PANE_FIRST))			return 0;
// 	m_GroupTree.ModifyStyleEx(0, WS_EX_STATICEDGE);
// 	m_GroupTree.SetOwner(this);
	if (!m_GroupTree.Create( WS_VISIBLE | TVS_HASLINES | TVS_LINESATROOT | TVS_HASBUTTONS | TVS_SHOWSELALWAYS,
		CRect(0,0,0,0), this, 0xFF ))							return 0;
	m_GroupTree.ModifyStyleEx(0, WS_EX_STATICEDGE);
	m_GroupTree.SetOwner(this);

	UpdateTreeView();
	return 0;
}

void CSpaceItem::OnSize(UINT nType, int cx, int cy)//layout the pane on you
{
	CWnd::OnSize(nType, cx, cy);
	int nTop = 0;
	if (m_wndToolBar.GetSafeHwnd()){
		CSize sz = m_wndToolBar.CalcDockingLayout(cx, LM_HIDEWRAP| LM_HORZDOCK|LM_HORZ | LM_COMMIT);
		m_wndToolBar.MoveWindow(0, nTop, cx, sz.cy);
		m_wndToolBar.Invalidate(FALSE);
		nTop += sz.cy;
	}
	if (m_GroupTree.GetSafeHwnd()){
		m_GroupTree.MoveWindow(0, nTop, cx, cy - nTop);
		m_GroupTree.Invalidate(FALSE);
	}
}

void CSpaceItem::OnSetFocus(CWnd* /*pOldWnd*/)
{
	m_GroupTree.SetFocus();
}

// void CSpaceItem::OnEmptyCommand(UINT)
// {
// 	boost::shared_ptr<CProject> proj = CProjectMgr::GetMe().GetProj();
// 	if(!proj)		return;
// 	proj->ShowInfo();
// }
// 
// void CSpaceItem::OnProjInfo()
// {
// 	boost::shared_ptr<CProject> proj = CProjectMgr::GetMe().GetProj();
// 	if(!proj)		return;
// 	proj->ShowInfo();
// }

void CSpaceItem::OnTreeDblClick(CTreeCtrl* pTreeCtrl, HTREEITEM hItem)
{
//	if(CProjectMgr::GetMe().IsWatch())				return;
// 	if(hItem == m_AddNew)		OnGroupAdd();
// 	else						OnGroupEdit();
	if(hItem != m_AddNew){								//!< 显示某个组的变量
		boost::shared_ptr<MVC::Item::CItemGroup> group = MVC::Item::CItemMgr::GetMe().GetGroup(pTreeCtrl->GetItemText(hItem));
		if(!group)		return;
		IntoGroup(group->getID());
	}
	else
		OnGroupAdd();
}

void CSpaceItem::OnTreeLClick(CTreeCtrl* pTreeCtrl, HTREEITEM hItem)
{
// 	if(hItem != m_AddNew){								//!< 显示某个组的变量
// 		boost::shared_ptr<MVC::Item::CItemGroup> group = MVC::Item::CItemMgr::GetMe().GetGroup(pTreeCtrl->GetItemText(hItem));
// 		if(!group)		return;
// 		IntoGroup(group->getID());
// 	}
}

void CSpaceItem::OnTreeRClick(CTreeCtrl* pTreeCtrl, HTREEITEM hItem, CPoint point)
{
	GetCursorPos(&point);
	if(CProjectMgr::GetMe().IsWatch())			return;
	if(hItem == m_AddNew)						return;

	CMenu popMenu;
	popMenu.LoadMenu(IDR_RBUTTON_MENU);
	CMenu* menu =popMenu.GetSubMenu(1);
	//!< 对于特殊地方的右键会出现不同的菜单项
	if(m_GroupTree.GetItemText(hItem) == _T("System")){
		menu->DeleteMenu(ID_GROUP_EDIT, MF_BYCOMMAND);
		menu->DeleteMenu(ID_GROUP_REMOVE, MF_BYCOMMAND);
	}
	CXTPCommandBars::TrackPopupMenu(menu, TPM_LEFTALIGN | TPM_RIGHTBUTTON, point.x, point.y, this);
}

void CSpaceItem::OnTreeKeyDown(CTreeCtrl* pTreeCtrl, HTREEITEM hItem, UINT nChar)
{
	if(CProjectMgr::GetMe().IsWatch())			return;
	if(nChar != VK_RETURN)						return;
	pTreeCtrl->SelectDropTarget(hItem);
	if(m_AddNew == hItem)						OnTreeDblClick(pTreeCtrl, hItem);
	else										OnTreeLClick(pTreeCtrl, hItem);
}

//!< 要显示某个变量组的变量
void CSpaceItem::IntoGroup(UINT groupid)
{
	boost::shared_ptr<MVC::Item::CItemGroup> group = MVC::Item::CItemMgr::GetMe().GetGroup(groupid);
	if(!group)				return;
	m_uiCulGroupID = groupid;

	if(group->m_ItemDoc){
		POSITION pos = group->m_ItemDoc->GetFirstViewPosition();
		CView* view = group->m_ItemDoc->GetNextView(pos);
		ASSERT(view);
		CFrameWnd* frm = view->GetParentFrame();
		ASSERT(frm);
		frm->ActivateFrame();
	}
	else{
		group->m_ItemDoc = (MVC::Item::CItemDoc*)((CDXPEditorApp *)AfxGetApp())->m_pItemDocMgr->OpenDocumentFile(NULL);
		group->m_ItemDoc->ShowGroup(groupid);
	}
	group->m_ItemDoc->SetTitle(_T(""));
}

void CSpaceItem::UpdateTreeView()
{
	if(!m_GroupTree.GetSafeHwnd())	return;
	m_GroupTree.DeleteAllItems();
	CProjectMgr* projMgr = &CProjectMgr::GetMe();
	if(!projMgr->GetProj())	return;

	boost::shared_ptr<MVC::Item::CItemGroup> group = MVC::Item::CItemMgr::GetMe().GetGroup(0);
	if(group){
		HTREEITEM root = m_GroupTree.InsertItem(group->getName(), 6, 7, TVI_ROOT, TVI_LAST);
		AddGroupChild(root, group->getID());
		m_GroupTree.Expand(root, TVE_EXPAND);
		m_AddNew = m_GroupTree.InsertItem(_T("添加变量组"), 8, 8, TVI_ROOT, TVI_LAST);
	}
}

void CSpaceItem::AddGroupChild(HTREEITEM root, UINT parentID)
{
	MVC::Item::CItemMgr* mgr = &MVC::Item::CItemMgr::GetMe();
	if(!mgr->GetGroup(parentID))		return;
	HTREEITEM item = NULL;
	std::list<UINT> ltChildID = mgr->GetGroup(parentID)->getGroupIDList();
	foreach(UINT i, ltChildID){
		if(!mgr->GetGroup(i))			continue;
		item = m_GroupTree.InsertItem(mgr->GetGroup(i)->getName(), 6, 7, root, TVI_LAST);
		AddGroupChild(item, i);
		m_GroupTree.Expand(item, TVE_EXPAND);
	}
}

void CSpaceItem::OnGroupAdd()
{
	if(CProjectMgr::GetMe().SayWatch())		return;
	Dialog::CAddItemGroupDlg* dlg = &Dialog::CAddItemGroupDlg::GetMe();
	HTREEITEM hItem = m_GroupTree.GetSelectedItem();
	CString strCulGroupName = _T("");
	if(m_AddNew != hItem)		strCulGroupName = m_GroupTree.GetItemText(hItem);
	if(dlg->DoModal(strCulGroupName) == IDOK)
	{
		boost::shared_ptr<MVC::Item::CItemGroup> group = boost::shared_ptr<MVC::Item::CItemGroup> \
			(new MVC::Item::CItemGroup(dlg->m_strGroupName, dlg->m_uiParentID));
		MVC::Item::CItemMgr::GetMe().AddGroup(group, dlg->m_uiParentID);
		MVC::Item::CItemMgr::GetMe().SetModify();
		UpdateTreeView();
//		IntoGroup(group->getID());

		//!< 高亮被修改的项
		SetFocus();
		HTREEITEM hItem = m_GroupTree.GetItemFromName(group->getName());
		if(hItem)		OnTreeLClick(&m_GroupTree, hItem);
	}
}

void CSpaceItem::OnGroupEdit()
{
	if(CProjectMgr::GetMe().SayWatch())		return;

	//!< 找到被选中的变量组
	HTREEITEM hItem = m_GroupTree.GetSelectedItem();
	if(!hItem)								return;
	CString text = m_GroupTree.GetItemText(hItem);
	MVC::Item::CItemMgr* itemMgr = &MVC::Item::CItemMgr::GetMe();
	boost::shared_ptr<MVC::Item::CItemGroup> group = itemMgr->GetGroup(text);
	if(!group)								return;
	if(group->getParentID() == UINT(-1))	return;		//!< System变量组不能被修改
	boost::shared_ptr<MVC::Item::CItemGroup> parentGroup = itemMgr->GetGroup(group->getParentID());
	ASSERT(parentGroup);
	Dialog::CAddItemGroupDlg* dlg = &Dialog::CAddItemGroupDlg::GetMe();

	//!< 运行对话框
	if(IDOK == dlg->DoModal(_T(""), group->getID())){
		if (group->getName() != dlg->m_strGroupName ||
			group->getParentID() != dlg->m_uiParentID)
			MVC::Item::CItemMgr::GetMe().SetModify();
		group->setName(dlg->m_strGroupName);
		group->setParentID(dlg->m_uiParentID);
		UpdateTreeView();

		//!< 高亮被修改的项
		SetFocus();
		hItem = m_GroupTree.GetItemFromName(group->getName());
		if(hItem)		OnTreeLClick(&m_GroupTree, hItem);
	}
}

void CSpaceItem::OnGroupRemove()
{
	if(CProjectMgr::GetMe().SayWatch())		return;

	//!< 找到被选中的变量组
	HTREEITEM hItem = m_GroupTree.GetSelectedItem();
	if(!hItem)								return;
	CString text = m_GroupTree.GetItemText(hItem);
	MVC::Item::CItemMgr* itemMgr = &MVC::Item::CItemMgr::GetMe();
	boost::shared_ptr<MVC::Item::CItemGroup> group = itemMgr->GetGroup(text);
	if(!group)								return;
	
	text = _T("移除 \"") + text +_T("\" 变量组，会将其包含变量和包含子组一并移除，是否确定移除？");
	if(IDYES == MessageBox(text, _T("移除提示"), MB_YESNO)){
		itemMgr->RemoveGroup(group->getID());
		itemMgr->SetModify();
		UpdateTreeView();
		SetFocus();
	}
}

void CSpaceItem::OnItemGroup()
{
	m_bShowItemGroup = TRUE;
}

void CSpaceItem::OnItemDevice()
{
	m_bShowItemGroup = FALSE;
}

void CSpaceItem::OnUpdateItemGroup(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(m_bShowItemGroup?1:0);
}

void CSpaceItem::OnUpdateItemDevice(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(m_bShowItemGroup?0:1);
	pCmdUI->Enable(FALSE);
}

BOOL Space::CSpaceItem::OnHelpInfo(HELPINFO* pHelpInfo)
{
	SoftInfo::CMyHelp::GetMe().ShowHelp(_T("变量组"));
	return CWnd::OnHelpInfo(pHelpInfo);
}
