// ItemView.cpp : 实现文件
//

#include "stdafx.h"
#include "PMApp.h"
#include "ProjectMgr.h"
#include "Gbl.h"
#include "SoftInfo.h"
#include "Item.h"
#include "ItemDoc.h"
#include "ItemView.h"
#include "ItemFrame.h"
#include "ItemMgr.h"

// CItemView
using namespace MVC;
using namespace Item;
IMPLEMENT_DYNCREATE(CItemView, CScrollView)

CItemView::CItemView()
{
	m_FreshTime = 0;
}

CItemView::~CItemView()
{
}

BEGIN_MESSAGE_MAP(CItemView, CScrollView)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_ERASEBKGND()
	ON_WM_KEYDOWN()
	ON_WM_TIMER()
	ON_COMMAND(ID_ITEM_ADD, &CItemView::OnItemAdd)
	ON_COMMAND(ID_ITEM_EDIT, &CItemView::OnItemEdit)
	ON_COMMAND(ID_ITEM_DELETE, &CItemView::OnVkdelete)
	ON_COMMAND(ID_VKDELETE, &CItemView::OnVkdelete)
	ON_COMMAND(ID_DELETE, &CItemView::OnVkdelete)
	ON_COMMAND(ID_EDIT_COPY, &CItemView::OnEditCopy)
	ON_COMMAND(ID_EDIT_CUT, &CItemView::OnEditCut)
	ON_COMMAND(ID_EDIT_PASTE, &CItemView::OnEditPaste)
	ON_COMMAND(ID_ITEM_UP, &CItemView::OnItemUp)
	ON_COMMAND(ID_ITEM_DOWN, &CItemView::OnItemDown)
	ON_COMMAND(ID_ITEM_IN, &CItemView::OnItemIn)
	ON_COMMAND(ID_ITEM_OUT, &CItemView::OnItemOut)
	ON_COMMAND(ID_CURRENT_OUT, &CItemView::OnCurrentOut)
	ON_COMMAND(ID_SELECT_OUT, &CItemView::OnSelectOut)
	ON_COMMAND(ID_SELECT_ALL, &CItemView::OnSelectAll)
	ON_COMMAND(ID_FILE_PRINT, &CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CScrollView::OnFilePrintPreview)
	ON_COMMAND(ID_ITEM_EDIT_ALL, &CItemView::OnItemEditAll)
	ON_COMMAND(ID_ITEM_CLONE, &CItemView::OnClone)
	ON_COMMAND(ID_EDIT_UNDO, &CItemView::OnEditUndo)
	ON_COMMAND(ID_EDIT_REDO, &CItemView::OnEditRedo)
	ON_COMMAND(ID_ITEM_WRITE, &CItemView::OnItemWrite)
	ON_COMMAND(ID_SHOW_HEX, &CItemView::OnShowHex)
	ON_UPDATE_COMMAND_UI(ID_EDIT_REDO, &CItemView::OnUpdateEditRedo)
	ON_UPDATE_COMMAND_UI(ID_EDIT_UNDO, &CItemView::OnUpdateEditUndo)
	ON_UPDATE_COMMAND_UI(ID_ITEM_WRITE, &CItemView::OnUpdateItemWrite)
	ON_UPDATE_COMMAND_UI(ID_ITEM_ADD, &CItemView::OnUpdateItemAdd)
	ON_UPDATE_COMMAND_UI(ID_ITEM_EDIT, &CItemView::OnUpdateItemEdit)
	ON_UPDATE_COMMAND_UI(ID_ITEM_EDIT_ALL, &CItemView::OnUpdateItemEditAll)
	ON_UPDATE_COMMAND_UI(ID_DELETE, &CItemView::OnUpdateItemDelete)
	ON_UPDATE_COMMAND_UI(ID_ITEM_UP, &CItemView::OnUpdateItemUp)
	ON_UPDATE_COMMAND_UI(ID_ITEM_DOWN, &CItemView::OnUpdateItemDown)
	ON_UPDATE_COMMAND_UI(ID_ITEM_IN, &CItemView::OnUpdateItemIn)
	ON_UPDATE_COMMAND_UI(ID_ITEM_CLONE, &CItemView::OnUpdateItemClone)
	ON_UPDATE_COMMAND_UI(ID_SHOW_HEX, &CItemView::OnUpdateShowHex)
	ON_WM_HELPINFO()
END_MESSAGE_MAP()

// CItemView 诊断
#ifdef _DEBUG
void CItemView::AssertValid() const{CScrollView::AssertValid();}
#ifndef _WIN32_WCE
void CItemView::Dump(CDumpContext& dc) const{CScrollView::Dump(dc);}
#endif
#endif //_DEBUG

// CItemView 绘图

int CItemView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CScrollView::OnCreate(lpCreateStruct) == -1)		return -1;
	m_ItemGrid.CreateGrid(WS_CHILD|WS_VISIBLE, CRect(0,0,0,0), this, 23433);
	SetTimer(1, 1000, NULL);
	OnTimer(1);		// 为了马上进行这个函数
	return 0;
}

void CItemView::OnSize(UINT nType, int cx, int cy)
{
	CScrollView::OnSize(nType, cx, cy);
	m_ItemGrid.MoveWindow(0, 0, cx, cy);
}

BOOL CItemView::OnEraseBkgnd(CDC* pDC)
{
	return FALSE;
}

void CItemView::OnInitialUpdate()
{
	CScrollView::OnInitialUpdate();
	CSize sizeTotal;
	// TODO: 计算此视图的合计大小
	sizeTotal.cx = sizeTotal.cy = 100;
	SetScrollSizes(MM_TEXT, sizeTotal);
}

void CItemView::OnDraw(CDC* pDC)
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

void MVC::Item::CItemView::OnItemAdd()
{
	if(CProjectMgr::GetMe().SayWatch())		return;
	std::shared_ptr<CItem> item = m_ItemGrid.AddNewItem();

	if(!item)		return;
	std::shared_ptr<SItemUndo> undo = std::shared_ptr<SItemUndo>(new SItemUndo);
	undo->m_bEnd = true;
	undo->m_uiEditType = CGbl::UNDO_TYPE_ADD;
	undo->m_Item = item;
	CItemDoc* pDoc = (CItemDoc *)GetDocument();
	pDoc->AddUndoMember(undo);

	m_ItemGrid.SetFocus();
	m_ItemGrid.RedrawAll();
}

void MVC::Item::CItemView::OnItemUp()
{
	if(CProjectMgr::GetMe().SayWatch())		return;
	m_ItemGrid.ItemUp();
}

void MVC::Item::CItemView::OnItemDown()
{
	if(CProjectMgr::GetMe().SayWatch())		return;
	m_ItemGrid.ItemDown();
}

void MVC::Item::CItemView::OnVkdelete()
{
	if(CProjectMgr::GetMe().SayWatch())		return;
	m_ItemGrid.ItemRemove();
}

void MVC::Item::CItemView::OnEditCopy()
{
	m_ItemGrid.ItemCopy();
}

void MVC::Item::CItemView::OnEditCut()
{
	if(CProjectMgr::GetMe().SayWatch())		return;
	m_ItemGrid.ItemCut();
}

void MVC::Item::CItemView::OnEditPaste()
{
	if(CProjectMgr::GetMe().SayWatch())		return;
	m_ItemGrid.ItemPaste();
}

void MVC::Item::CItemView::OnSelectAll()
{
	m_ItemGrid.SelectAllItem();
}

void MVC::Item::CItemView::OnItemIn()
{
	if(CProjectMgr::GetMe().SayWatch())		return;
	CString extendNames = _T("变量表文件 (*.var;*.vxpt;*.xls;*.xlsx)|*.var;*.vxpt;*.xls;*.xlsx|");
//	extendNames = extendNames + _T("变量表导出文件 (*.vxpt)|*.vxpt|");
//	extendNames = extendNames + _T("Config导出符号变量表文件 (*.xls)|*.xls|");
	extendNames = extendNames + _T("|");
	CFileDialog openDlg(TRUE, (LPCTSTR)extendNames, _T(""), OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, (LPCTSTR)extendNames);
	CItemDoc* pDoc = (CItemDoc *)GetDocument();
	if(IDOK == openDlg.DoModal())
	{
		if(openDlg.GetFileExt() == _T("vxpt") || openDlg.GetFileExt() == _T("var"))
		{
			TiXmlDocument pTiXml(openDlg.GetPathName());
			if(!pTiXml.LoadFile()){
				MessageBox(_T("无法解析文件！"), _T("错误"), MB_ICONEXCLAMATION);
				return;
			}
			pDoc->SerializeIn(pTiXml.RootElement());
		}
		else
		{
			pDoc->SerializeConfig(openDlg.GetPathName(), openDlg.GetFileTitle(), openDlg.GetFileExt());
		}
	}
//	m_ItemGrid.RedrawGrid();
}

void MVC::Item::CItemView::OnItemOut()
{
	m_ItemGrid.OutItemAll();
}

void MVC::Item::CItemView::OnCurrentOut()
{
	m_ItemGrid.OutItemCurrent();
}

void MVC::Item::CItemView::OnSelectOut()
{
	m_ItemGrid.OutItemSelect();
}

void MVC::Item::CItemView::OnItemEdit()
{
	if(CProjectMgr::GetMe().SayWatch())		return;
	m_ItemGrid.ItemEdit();
}

void MVC::Item::CItemView::OnItemEditAll()
{
	if(CProjectMgr::GetMe().SayWatch())		return;
	m_ItemGrid.ItemEditAll();
}

void MVC::Item::CItemView::OnClone()
{
	if(CProjectMgr::GetMe().SayWatch())		return;
	m_ItemGrid.ItemClone();
}

void MVC::Item::CItemView::OnItemWrite()
{
	m_ItemGrid.OnMenuCommand(-1, -1, -1, ID_ITEM_WRITE);
}

void MVC::Item::CItemView::OnShowHex()
{
	SoftInfo::CSoftInfo::GetMe().setShowHex(!SoftInfo::CSoftInfo::GetMe().isShowHex());
}

void MVC::Item::CItemView::OnUpdateShowHex(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(SoftInfo::CSoftInfo::GetMe().isShowHex()?1:0);
}

void MVC::Item::CItemView::OnUpdateItemWrite(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(CProjectMgr::GetMe().IsWatch());
}

void MVC::Item::CItemView::OnUpdateItemAdd(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(!CProjectMgr::GetMe().IsWatch());
}

void MVC::Item::CItemView::OnUpdateItemEdit(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(!CProjectMgr::GetMe().IsWatch());
}

void MVC::Item::CItemView::OnUpdateItemEditAll(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(!CProjectMgr::GetMe().IsWatch());
}

void MVC::Item::CItemView::OnUpdateItemDelete(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(!CProjectMgr::GetMe().IsWatch());
}

void MVC::Item::CItemView::OnUpdateItemUp(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(!CProjectMgr::GetMe().IsWatch());
}

void MVC::Item::CItemView::OnUpdateItemDown(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(!CProjectMgr::GetMe().IsWatch());
}

void MVC::Item::CItemView::OnUpdateItemIn(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(!CProjectMgr::GetMe().IsWatch());
}

void MVC::Item::CItemView::OnUpdateItemClone(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(!CProjectMgr::GetMe().IsWatch());
}

void MVC::Item::CItemView::OnTimer(UINT_PTR nIDEvent)
{
	if(nIDEvent == 1)
	{
		if(CProjectMgr::GetMe().IsWatch() && !m_ItemGrid.IsGridWatch())
		{
			m_ItemGrid.SetGridWatch(true);
			m_FreshTime = SoftInfo::CSoftInfo::GetMe().getFreshDataTime();
			SetTimer(2, m_FreshTime, NULL);
		}
		else if(!CProjectMgr::GetMe().IsWatch() && m_ItemGrid.IsGridWatch())
		{
			m_ItemGrid.SetGridWatch(false);
			KillTimer(2);
		}
		if(m_FreshTime != SoftInfo::CSoftInfo::GetMe().getFreshDataTime() && m_ItemGrid.IsGridWatch())
		{
			m_FreshTime = SoftInfo::CSoftInfo::GetMe().getFreshDataTime();
			SetTimer(2, m_FreshTime, NULL);
		}
	}
	else if(nIDEvent == 2)
	{
		if(IsActive())
			m_ItemGrid.FreshData();
	}
	CScrollView::OnTimer(nIDEvent);
}

// 判断自己是不是激活状态
bool MVC::Item::CItemView::IsActive()
{
	return !!IsWindowVisible();
}

// 显示这个编号的变量，如果没有，返回false
bool MVC::Item::CItemView::ShowItem(UINT id)
{
	long row = m_ItemGrid.GetItemRow(id);
	if(row == -1)	return false;
	m_ItemGrid.m_GI->m_multiSelect->ClearAll();		// 清空所有选择的行
	m_ItemGrid.SetFocus();
	m_ItemGrid.SelectRowAt(row);
	return true;
}

void MVC::Item::CItemView::OnEditUndo()
{
	if(CProjectMgr::GetMe().SayWatch())		return;
	CItemDoc *pDoc = (CItemDoc *)GetDocument();
	if(pDoc)	pDoc->OnUndo();
}

void MVC::Item::CItemView::OnEditRedo()
{
	if(CProjectMgr::GetMe().SayWatch())		return;
	CItemDoc *pDoc = (CItemDoc *)GetDocument();
	if(pDoc)	pDoc->OnRedo();
}

void MVC::Item::CItemView::OnUpdateEditUndo(CCmdUI *pCmdUI)
{
	CItemDoc *pDoc = (CItemDoc *)GetDocument();
	if(pDoc)	pCmdUI->Enable(!pDoc->IsUndoEmpty());
}

void MVC::Item::CItemView::OnUpdateEditRedo(CCmdUI *pCmdUI)
{
	CItemDoc *pDoc = (CItemDoc *)GetDocument();
	if(pDoc)	pCmdUI->Enable(!pDoc->IsRedoEmpty());
}

BOOL MVC::Item::CItemView::OnPreparePrinting(CPrintInfo* pInfo)
{
	return DoPreparePrinting(pInfo);
}

void MVC::Item::CItemView::OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo)
{
	pInfo->SetMaxPage(m_ItemGrid.PrintInit(pDC,pInfo->m_pPD,0,0,m_ItemGrid.GetNumberCols()-1,m_ItemGrid.GetNumberRows()-2));
	m_isPrinting = TRUE;
}

void MVC::Item::CItemView::OnEndPrinting(CDC* pDC, CPrintInfo* pInfo)
{
	m_isPrinting = FALSE;
}

void MVC::Item::CItemView::OnPrint(CDC* pDC, CPrintInfo* pInfo)
{
	m_drawRect = pInfo->m_rectDraw;
	m_printPage = pInfo->m_nCurPage;
	CScrollView::OnPrint(pDC, pInfo);
}


BOOL MVC::Item::CItemView::OnHelpInfo(HELPINFO* pHelpInfo)
{
	SoftInfo::CMyHelp::GetMe().ShowHelp(_T("变量"));
	return CScrollView::OnHelpInfo(pHelpInfo);
}
