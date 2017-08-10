// ItemDoc.cpp : 实现文件
//
#include "stdafx.h"
#include "PMApp.h"
#include "ProjectMgr.h"
#include "Gbl.h"
#include "CamDoc.h"
#include "CamMgr.h"
#include "CamGrid.h"
#include "CamView.h"
#include "Camera.h"

// CCamDoc
using namespace MVC;
using namespace Camera;

tagCamUndo::tagCamUndo()
{
	m_uiEditType = 0;
	m_uiEditInfo = 0;
	m_bEnd = false;
}

tagCamUndo::tagCamUndo(UINT type, std::shared_ptr<CCamera> item, UINT info)
{
	if(type == CGbl::UNDO_TYPE_UPD){
		m_Item = std::shared_ptr<CCamera>(new CCamera());
		*m_Item = *item;
	}
	else{m_Item = item;}
	m_uiEditType = type;
	m_uiEditInfo = info;
	m_bEnd = false;
}

IMPLEMENT_DYNCREATE(CCamDoc, CDocument)

CCamDoc::CCamDoc()
{
}

CCamDoc::~CCamDoc()
{
	if(CCamMgr::GetMe().m_pItemDoc == this)
		CCamMgr::GetMe().m_pItemDoc = NULL;
}

BEGIN_MESSAGE_MAP(CCamDoc, CDocument)
END_MESSAGE_MAP()

// CCamDoc 诊断
#ifdef _DEBUG
void CCamDoc::AssertValid() const{CDocument::AssertValid();}
#ifndef _WIN32_WCE
void CCamDoc::Dump(CDumpContext& dc) const{CDocument::Dump(dc);}
#endif
#endif //_DEBUG
#ifndef _WIN32_WCE
// CCamDoc 序列化
void CCamDoc::Serialize(CArchive& ar){if (ar.IsStoring()){}else{}}
#endif

// CCamDoc 命令
BOOL CCamDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())	return FALSE;
	return TRUE;
}

BOOL CCamDoc::OnOpenDocument(LPCTSTR lpszPathName)
{
	if (!CDocument::OnOpenDocument(lpszPathName))	return FALSE;
	return TRUE;
}

CCamView* CCamDoc::GetView()
{
	POSITION pos = GetFirstViewPosition();
	CView* view = GetNextView(pos);
	ASSERT(view);
	return (CCamView*)view;
}

void CCamDoc::ShowAllItem()
{
	CCamView* pView = GetView();
	pView->m_ItemGrid.RedrawGrid();
}

void CCamDoc::AddUndoMember(std::shared_ptr<SCamUndo> itemUndo)
{
	if(!itemUndo)				return;
	CCamMgr::GetMe().SetModify(true);
	m_stItemUndo.push(itemUndo);
	ClearRedo();
	CCamMgr::GetMe().AddEditItem(itemUndo->m_Item->GetID());
	if(itemUndo->m_bEnd)	GetGrid()->OnEditEnd();//CCameraMgr::GetMe().SetEditEnd();
}

void CCamDoc::SetUndoEnd()
{
	if(!m_stItemUndo.empty())	m_stItemUndo.top()->m_bEnd = true;
	CCamMgr::GetMe().SetEditEnd();
}

void CCamDoc::OnUndo()
{
	if(m_stItemUndo.empty())	return;
	CCamGrid* itemGrid = GetGrid();
	if(!itemGrid)				return;
	CCamMgr* itemMgr = &CCamMgr::GetMe();
	std::shared_ptr<SCamUndo> undo;
	itemMgr->SetModify(true);
	itemGrid->EnableUpdate(FALSE);
	do{
		undo = m_stItemUndo.top();
		if(undo->m_uiEditType == CGbl::UNDO_TYPE_ADD)		UndoAdd();
		if(undo->m_uiEditType == CGbl::UNDO_TYPE_DEL)		UndoDel();
		if(undo->m_uiEditType == CGbl::UNDO_TYPE_UPD)		UndoUpd();
		itemMgr->AddEditItem(undo->m_Item->GetID());
	}
	while(!m_stItemUndo.empty() && !m_stItemUndo.top()->m_bEnd);

	itemGrid->EnableUpdate(TRUE);
	itemGrid->RedrawAll();
	itemGrid->OnEditEnd();
}

void CCamDoc::UndoAdd()
{
	std::shared_ptr<SCamUndo> undo = m_stItemUndo.top();
	m_stItemUndo.pop();
	CCamMgr* itemMgr = &CCamMgr::GetMe();
	CCamGrid* itemGrid = GetGrid();
	long row = itemGrid->GetItemRow(undo->m_Item->GetID());

	itemMgr->DeleteItem(undo->m_Item->GetID());
	m_stItemRedo.push(undo);
}

void CCamDoc::UndoDel()
{
	std::shared_ptr<SCamUndo> undo = m_stItemUndo.top();
	m_stItemUndo.pop();
	CCamMgr* camMgr = &CCamMgr::GetMe();
	camMgr->m_vtCam[undo->m_Item->GetID()] = undo->m_Item;
	m_stItemRedo.push(undo);
}

void CCamDoc::UndoUpd()
{
	std::shared_ptr<SCamUndo> undo = m_stItemUndo.top();
	m_stItemUndo.pop();
	CCamMgr* itemMgr = &CCamMgr::GetMe();

	std::shared_ptr<CCamera> item = itemMgr->GetCam(undo->m_Item->GetID());
	std::shared_ptr<SCamUndo> redo = std::shared_ptr<SCamUndo>(new SCamUndo(CGbl::UNDO_TYPE_UPD, item, 0));
	redo->m_bEnd = undo->m_bEnd;
	m_stItemRedo.push(redo);

	*item = *undo->m_Item;
}

void CCamDoc::OnRedo()
{
	if(m_stItemRedo.empty())	return;
	CCamGrid* itemGrid = GetGrid();
	if(!itemGrid)				return;
	CCamMgr* camMgr = &CCamMgr::GetMe();
	std::shared_ptr<SCamUndo> redo;
	camMgr->SetModify(true);
	itemGrid->EnableUpdate(FALSE);
	while(!m_stItemRedo.empty() && !m_stItemRedo.top()->m_bEnd){
		redo = m_stItemRedo.top();
		if(redo->m_uiEditType == CGbl::UNDO_TYPE_ADD)		RedoAdd();
		if(redo->m_uiEditType == CGbl::UNDO_TYPE_DEL)		RedoDel();
		if(redo->m_uiEditType == CGbl::UNDO_TYPE_UPD)		RedoUpd();
		camMgr->AddEditItem(redo->m_Item->GetID());
	}
	if(!m_stItemRedo.empty() && m_stItemRedo.top()->m_bEnd){
		redo = m_stItemRedo.top();
		if(redo->m_uiEditType == CGbl::UNDO_TYPE_ADD)		RedoAdd();
		if(redo->m_uiEditType == CGbl::UNDO_TYPE_DEL)		RedoDel();
		if(redo->m_uiEditType == CGbl::UNDO_TYPE_UPD)		RedoUpd();
		camMgr->AddEditItem(redo->m_Item->GetID());
	}
	itemGrid->EnableUpdate(TRUE);
	itemGrid->OnEditEnd();
}

void CCamDoc::RedoAdd()
{
	std::shared_ptr<SCamUndo> redo = m_stItemRedo.top();
	m_stItemRedo.pop();
	CCamMgr* camMgr = &CCamMgr::GetMe();
	camMgr->m_vtCam[redo->m_Item->GetID()] = redo->m_Item;
	m_stItemUndo.push(redo);
}

void CCamDoc::RedoDel()
{
	std::shared_ptr<SCamUndo> redo = m_stItemRedo.top();
	m_stItemRedo.pop();
	CCamMgr::GetMe().DeleteItem(redo->m_Item->GetID());
	m_stItemUndo.push(redo);
}

void CCamDoc::RedoUpd()
{
	std::shared_ptr<SCamUndo> redo = m_stItemRedo.top();
	m_stItemRedo.pop();
	CCamMgr* camMgr = &CCamMgr::GetMe();

	std::shared_ptr<CCamera> item = camMgr->GetCam(redo->m_Item->GetID());
	std::shared_ptr<SCamUndo> undo = std::shared_ptr<SCamUndo>(new SCamUndo(CGbl::UNDO_TYPE_UPD, item, 0));
	undo->m_bEnd = redo->m_bEnd;
	m_stItemUndo.push(undo);

	*item = *redo->m_Item;
}

void CCamDoc::RedoOne(std::shared_ptr<SCamUndo> redo)
{
}

void CCamDoc::ClearUndo()
{
	while(!m_stItemUndo.empty())		m_stItemUndo.pop();
}

void CCamDoc::ClearRedo()
{
	while(!m_stItemRedo.empty())		m_stItemRedo.pop();
}

CCamGrid* CCamDoc::GetGrid()
{
	CView* pView = (CView *)m_viewList.GetHead();
	if(!pView)											return NULL;
	if(!pView->IsKindOf(RUNTIME_CLASS(CCamView)))		return NULL;
	return &((CCamView *)pView)->m_ItemGrid;
}

void MVC::Camera::CCamDoc::SetTitle(LPCTSTR lpszTitle)
{
	CString strTitle = _T("摄像头一览");
	CString strWatch = _T("（监控状态）");
	if(CProjectMgr::GetMe().IsWatch())		strTitle += strWatch;
	CDocument::SetTitle(strTitle);
}
