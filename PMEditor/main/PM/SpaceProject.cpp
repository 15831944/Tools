#include "StdAfx.h"
#include "PMApp.h"
#include "MainFrm.h"
#include "SpaceProject.h"
#include "ProjectMgr.h"
#include "Project.h"

#include "ItemMgr.h"
#include "ItemDoc.h"
#include "ItemView.h"

using namespace Space;
BEGIN_MESSAGE_MAP(CSpaceProject, CWnd)
	//{{AFX_MSG_MAP(CSolutionExplorerPane)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_HELPINFO()
	//}}AFX_MSG_MAP
	ON_WM_SETFOCUS()
	ON_COMMAND(ID_PROJ_INFO, &CSpaceProject::OnProjInfo)
	ON_COMMAND(ID_PROJ_SAVE, &CSpaceProject::OnProjSave)
	ON_COMMAND(ID_ADD_ITEM, &CSpaceProject::OnAddItem)
	ON_UPDATE_COMMAND_UI(ID_PROJ_SAVE, &CSpaceProject::OnUpdateProjSave)
	ON_UPDATE_COMMAND_UI(ID_ADD_ITEM, &CSpaceProject::OnUpdateAddItem)
END_MESSAGE_MAP()

CSpaceProject::CSpaceProject(void)
{
}

CSpaceProject::~CSpaceProject(void)
{
}

int CSpaceProject::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)		return -1;

	//!< Create a ToolBar as usual
	VERIFY(m_wndToolBar.CreateToolBar(WS_TABSTOP|WS_VISIBLE|WS_CHILD|CBRS_TOOLTIPS, this));
	VERIFY(m_wndToolBar.LoadToolBar(IDR_PANE_SOLUTIONEXPLORER));

	if (!m_ProjectTree.Create( WS_VISIBLE | TVS_HASLINES | TVS_LINESATROOT | TVS_HASBUTTONS | TVS_SHOWSELALWAYS,
		CRect(0,0,0,0), this, 0xFF ))							return 0;
	m_ProjectTree.ModifyStyleEx(0, WS_EX_STATICEDGE);
	m_ProjectTree.SetOwner(this);

	return 0;
}

void CSpaceProject::OnSize(UINT nType, int cx, int cy)//layout the pane on you
{
	CWnd::OnSize(nType, cx, cy);
	int nTop = 0;
	if (m_wndToolBar.GetSafeHwnd()){
		CSize sz = m_wndToolBar.CalcDockingLayout(cx, LM_HIDEWRAP| LM_HORZDOCK|LM_HORZ | LM_COMMIT);
		m_wndToolBar.MoveWindow(0, nTop, cx, sz.cy);
		m_wndToolBar.Invalidate(FALSE);
		nTop += sz.cy;
	}
	if (m_ProjectTree.GetSafeHwnd()){
		m_ProjectTree.MoveWindow(0, nTop, cx, cy - nTop);
		m_ProjectTree.Invalidate(FALSE);
	}
}

void CSpaceProject::OnSetFocus(CWnd* /*pOldWnd*/)
{
	m_ProjectTree.SetFocus();
}

void CSpaceProject::OnProjInfo()
{
	std::shared_ptr<CProject> proj = CProjectMgr::GetMe().GetProj();
	if(!proj)		return;
	proj->ShowInfo();
}

void CSpaceProject::OnTreeDblClick(CTreeCtrl* pTreeCtrl, HTREEITEM hItem)
{
	CProjectMgr* projMgr = &CProjectMgr::GetMe();
	if(!projMgr->GetProj())	return;
	projMgr->GetProj()->OnTreeDblClick(pTreeCtrl, hItem);
}

void CSpaceProject::OnTreeLClick(CTreeCtrl* pTreeCtrl, HTREEITEM hItem)
{}

void CSpaceProject::OnTreeRClick(CTreeCtrl* pTreeCtrl, HTREEITEM hItem, CPoint point)
{
	GetCursorPos(&point);
	CProjectMgr* projMgr = &CProjectMgr::GetMe();
	if(!projMgr->GetProj())	return;
	projMgr->GetProj()->OnTreeRClick(pTreeCtrl, hItem, point);
}

void CSpaceProject::OnTreeKeyDown(CTreeCtrl* pTreeCtrl, HTREEITEM hItem, UINT nChar)
{
	CProjectMgr* projMgr = &CProjectMgr::GetMe();
	if(!projMgr->GetProj())	return;
	if(nChar==VK_RETURN){
		pTreeCtrl->SelectDropTarget(hItem);
		projMgr->GetProj()->OnTreeEnter(pTreeCtrl, hItem);
	}
}

void CSpaceProject::UpdateTreeView()
{
	m_ProjectTree.DeleteAllItems();
	CProjectMgr* projMgr = &CProjectMgr::GetMe();
	if(!projMgr->GetProj())	return;
	projMgr->GetProj()->UpdateProjView(m_ProjectTree);
}

void CSpaceProject::OnProjSave()
{
	((CMainFrame *)g_App.GetMainWnd())->OnProjSave();
}

void CSpaceProject::OnAddItem()
{
	((CMainFrame *)g_App.GetMainWnd())->OnAddItem();
}

void CSpaceProject::OnUpdateProjSave(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(CProjectMgr::GetMe().GetProj()?TRUE:FALSE);
}

void CSpaceProject::OnUpdateAddItem(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(CProjectMgr::GetMe().GetProj()?TRUE:FALSE);
}

BOOL Space::CSpaceProject::OnHelpInfo(HELPINFO* pHelpInfo)
{
	HTREEITEM hItem = m_ProjectTree.GetSelectedItem();
	CProjectMgr* projMgr = &CProjectMgr::GetMe();
	if(!projMgr->GetProj())		return TRUE;
	projMgr->GetProj()->OnHelpInfo(hItem);
	return CWnd::OnHelpInfo(pHelpInfo);
}
