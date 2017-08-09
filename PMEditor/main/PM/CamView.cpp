// CamView.cpp : 实现文件
//

#include "stdafx.h"
#include "DXPEditor.h"
#include "ProjectMgr.h"
#include "Gbl.h"
#include "SoftInfo.h"
#include "Camera.h"
#include "CamDoc.h"
#include "CamView.h"
#include "CamFrame.h"
#include "CamMgr.h"

// CCamView
using namespace MVC;
using namespace Camera;
IMPLEMENT_DYNCREATE(CCamView, CScrollView)

CCamView::CCamView()
{
}

CCamView::~CCamView()
{
}

BEGIN_MESSAGE_MAP(CCamView, CScrollView)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_ERASEBKGND()
	ON_WM_KEYDOWN()
	ON_WM_TIMER()
	ON_COMMAND(ID_CAM_ADD, &CCamView::OnItemAdd)
	ON_COMMAND(ID_CAM_EDIT, &CCamView::OnItemEdit)
	ON_COMMAND(ID_CAM_DELETE, &CCamView::OnVkdelete)
	ON_COMMAND(ID_VKDELETE, &CCamView::OnVkdelete)
	ON_COMMAND(ID_DELETE, &CCamView::OnVkdelete)
	ON_COMMAND(ID_EDIT_COPY, &CCamView::OnEditCopy)
	ON_COMMAND(ID_EDIT_CUT, &CCamView::OnEditCut)
	ON_COMMAND(ID_EDIT_PASTE, &CCamView::OnEditPaste)
	ON_COMMAND(ID_ITEM_UP, &CCamView::OnItemUp)
	ON_COMMAND(ID_ITEM_DOWN, &CCamView::OnItemDown)
	ON_COMMAND(ID_SELECT_ALL, &CCamView::OnSelectAll)
	ON_COMMAND(ID_FILE_PRINT, &CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CScrollView::OnFilePrintPreview)
//	ON_COMMAND(ID_ITEM_EDIT_ALL, &CCamView::OnItemEditAll)
	ON_COMMAND(ID_EDIT_UNDO, &CCamView::OnEditUndo)
	ON_COMMAND(ID_EDIT_REDO, &CCamView::OnEditRedo)
	ON_UPDATE_COMMAND_UI(ID_EDIT_REDO, &CCamView::OnUpdateEditRedo)
	ON_UPDATE_COMMAND_UI(ID_EDIT_UNDO, &CCamView::OnUpdateEditUndo)
	ON_UPDATE_COMMAND_UI(ID_CAM_ADD, &CCamView::OnUpdateItemAdd)
	ON_UPDATE_COMMAND_UI(ID_CAM_EDIT, &CCamView::OnUpdateItemEdit)
//	ON_UPDATE_COMMAND_UI(ID_ITEM_EDIT_ALL, &CCamView::OnUpdateItemEditAll)
	ON_UPDATE_COMMAND_UI(ID_DELETE, &CCamView::OnUpdateItemDelete)
	ON_UPDATE_COMMAND_UI(ID_ITEM_UP, &CCamView::OnUpdateItemUp)
	ON_UPDATE_COMMAND_UI(ID_ITEM_DOWN, &CCamView::OnUpdateItemDown)
	ON_WM_HELPINFO()
END_MESSAGE_MAP()

// CCamView 诊断
#ifdef _DEBUG
void CCamView::AssertValid() const{CScrollView::AssertValid();}
#ifndef _WIN32_WCE
void CCamView::Dump(CDumpContext& dc) const{CScrollView::Dump(dc);}
#endif
#endif //_DEBUG

// CCamView 绘图

int CCamView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CScrollView::OnCreate(lpCreateStruct) == -1)		return -1;
	m_ItemGrid.CreateGrid(WS_CHILD|WS_VISIBLE, CRect(0,0,0,0), this, 23434);
	SetTimer(1, 1000, NULL);
	OnTimer(1);		//!< 为了马上进行这个函数
	return 0;
}

void CCamView::OnSize(UINT nType, int cx, int cy)
{
	CScrollView::OnSize(nType, cx, cy);
	m_ItemGrid.MoveWindow(0, 0, cx, cy);
}

BOOL CCamView::OnEraseBkgnd(CDC* pDC)
{
	return FALSE;
}

void CCamView::OnInitialUpdate()
{
	CScrollView::OnInitialUpdate();
	CSize sizeTotal;
	// TODO: 计算此视图的合计大小
	sizeTotal.cx = sizeTotal.cy = 100;
	SetScrollSizes(MM_TEXT, sizeTotal);
}

void CCamView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	CRect rect;
	if (m_isPrinting){
		rect = m_drawRect;
		m_ItemGrid.PrintPage(pDC, m_printPage);
	}
	else{
		GetClientRect(&rect);
		CBrush brush(RGB(255,255,255));
		pDC->FillRect(&rect,&brush);
	}
}

void MVC::Camera::CCamView::OnItemAdd()
{
	if(CProjectMgr::GetMe().SayWatch())		return;
	boost::shared_ptr<CCamera> item = m_ItemGrid.AddNewItem();

	if(!item)		return;
	boost::shared_ptr<SCamUndo> undo = boost::shared_ptr<SCamUndo>(new SCamUndo);
	undo->m_bEnd = true;
	undo->m_uiEditType = CGbl::UNDO_TYPE_ADD;
	undo->m_Item = item;
	CCamDoc* pDoc = (CCamDoc *)GetDocument();
	pDoc->AddUndoMember(undo);

	m_ItemGrid.SetFocus();
	m_ItemGrid.RedrawAll();
}

void MVC::Camera::CCamView::OnItemUp()
{
	if(CProjectMgr::GetMe().SayWatch())		return;
	m_ItemGrid.ItemUp();
}

void MVC::Camera::CCamView::OnItemDown()
{
	if(CProjectMgr::GetMe().SayWatch())		return;
	m_ItemGrid.ItemDown();
}

void MVC::Camera::CCamView::OnVkdelete()
{
	if(CProjectMgr::GetMe().SayWatch())		return;
	m_ItemGrid.ItemRemove();
}

void MVC::Camera::CCamView::OnEditCopy()
{
	m_ItemGrid.ItemCopy();
}

void MVC::Camera::CCamView::OnEditCut()
{
	if(CProjectMgr::GetMe().SayWatch())		return;
	m_ItemGrid.ItemCut();
}

void MVC::Camera::CCamView::OnEditPaste()
{
	if(CProjectMgr::GetMe().SayWatch())		return;
	m_ItemGrid.ItemPaste();
}

void MVC::Camera::CCamView::OnSelectAll()
{
	m_ItemGrid.SelectAllItem();
}

void MVC::Camera::CCamView::OnItemEdit()
{
	if(CProjectMgr::GetMe().SayWatch())		return;
	m_ItemGrid.ItemEdit();
}

// void MVC::Item::CCamView::OnItemEditAll()
// {
// 	if(CProjectMgr::GetMe().SayWatch())		return;
// 	m_ItemGrid.ItemEditAll();
// }

void MVC::Camera::CCamView::OnUpdateItemAdd(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(!CProjectMgr::GetMe().IsWatch());
}

void MVC::Camera::CCamView::OnUpdateItemEdit(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(!CProjectMgr::GetMe().IsWatch());
}

// void MVC::Item::CCamView::OnUpdateItemEditAll(CCmdUI *pCmdUI)
// {
// 	pCmdUI->Enable(!CProjectMgr::GetMe().IsWatch());
// }

void MVC::Camera::CCamView::OnUpdateItemDelete(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(!CProjectMgr::GetMe().IsWatch());
}

void MVC::Camera::CCamView::OnUpdateItemUp(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(!CProjectMgr::GetMe().IsWatch());
}

void MVC::Camera::CCamView::OnUpdateItemDown(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(!CProjectMgr::GetMe().IsWatch());
}

void MVC::Camera::CCamView::OnTimer(UINT_PTR nIDEvent)
{
// 	if(nIDEvent == 1)
// 	{
// 		if(CProjectMgr::GetMe().IsWatch() && !m_ItemGrid.IsGridWatch())
// 		{
// 			m_ItemGrid.SetGridWatch(true);
// 			m_FreshTime = SoftInfo::CSoftInfo::GetMe().getFreshDataTime();
// 			SetTimer(2, m_FreshTime, NULL);
// 		}
// 		else if(!CProjectMgr::GetMe().IsWatch() && m_ItemGrid.IsGridWatch())
// 		{
// 			m_ItemGrid.SetGridWatch(false);
// 			KillTimer(2);
// 		}
// 		if(m_FreshTime != SoftInfo::CSoftInfo::GetMe().getFreshDataTime() && m_ItemGrid.IsGridWatch())
// 		{
// 			m_FreshTime = SoftInfo::CSoftInfo::GetMe().getFreshDataTime();
// 			SetTimer(2, m_FreshTime, NULL);
// 		}
// 	}
// 	else if(nIDEvent == 2)
// 	{
// 		if(IsActive())
// 			m_ItemGrid.FreshData();
// 	}
	CScrollView::OnTimer(nIDEvent);
}

//!< 判断自己是不是激活状态
bool MVC::Camera::CCamView::IsActive()
{
	return !!IsWindowVisible();
}

//!< 显示这个编号的变量，如果没有，返回false
bool MVC::Camera::CCamView::ShowItem(UINT id)
{
	long row = m_ItemGrid.GetItemRow(id);
	if(row == -1)	return false;
	m_ItemGrid.m_GI->m_multiSelect->ClearAll();		//!< 清空所有选择的行
	m_ItemGrid.SetFocus();
	m_ItemGrid.SelectRowAt(row);
	return true;
}

void MVC::Camera::CCamView::OnEditUndo()
{
	if(CProjectMgr::GetMe().SayWatch())		return;
	CCamDoc *pDoc = (CCamDoc *)GetDocument();
	if(pDoc)	pDoc->OnUndo();
}

void MVC::Camera::CCamView::OnEditRedo()
{
	if(CProjectMgr::GetMe().SayWatch())		return;
	CCamDoc *pDoc = (CCamDoc *)GetDocument();
	if(pDoc)	pDoc->OnRedo();
}

void MVC::Camera::CCamView::OnUpdateEditUndo(CCmdUI *pCmdUI)
{
	CCamDoc *pDoc = (CCamDoc *)GetDocument();
	if(pDoc)	pCmdUI->Enable(!pDoc->IsUndoEmpty());
}

void MVC::Camera::CCamView::OnUpdateEditRedo(CCmdUI *pCmdUI)
{
	CCamDoc *pDoc = (CCamDoc *)GetDocument();
	if(pDoc)	pCmdUI->Enable(!pDoc->IsRedoEmpty());
}

BOOL MVC::Camera::CCamView::OnPreparePrinting(CPrintInfo* pInfo)
{
	return DoPreparePrinting(pInfo);
}

void MVC::Camera::CCamView::OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo)
{
	pInfo->SetMaxPage(m_ItemGrid.PrintInit(pDC,pInfo->m_pPD,0,0,m_ItemGrid.GetNumberCols()-1,m_ItemGrid.GetNumberRows()-2));
	m_isPrinting = TRUE;
}

void MVC::Camera::CCamView::OnEndPrinting(CDC* pDC, CPrintInfo* pInfo)
{
	m_isPrinting = FALSE;
}

void MVC::Camera::CCamView::OnPrint(CDC* pDC, CPrintInfo* pInfo)
{
	m_drawRect = pInfo->m_rectDraw;
	m_printPage = pInfo->m_nCurPage;
	CScrollView::OnPrint(pDC, pInfo);
}


BOOL MVC::Camera::CCamView::OnHelpInfo(HELPINFO* pHelpInfo)
{
	SoftInfo::CMyHelp::GetMe().ShowHelp(_T("在DView中配置摄像头"));
	return CScrollView::OnHelpInfo(pHelpInfo);
}
