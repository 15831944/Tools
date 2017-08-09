// ItemFrame.cpp : 实现文件
//

#include "stdafx.h"
#include "DXPEditor.h"
#include "ItemFrame.h"
#include "ItemMgr.h"
#include "ItemGroup.h"
#include "ItemGrid.h"
#include "ItemView.h"

// CItemFrame
using namespace MVC;
using namespace Item;
IMPLEMENT_DYNCREATE(CItemFrame, CMDIChildWnd)

CItemFrame::CItemFrame()
{
}

CItemFrame::~CItemFrame()
{
}

BEGIN_MESSAGE_MAP(CItemFrame, CMDIChildWnd)
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_WM_CLOSE()
	ON_WM_MDIACTIVATE()
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CItemFrame 消息处理程序
int CItemFrame::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CMDIChildWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	return 0;
}

BOOL CItemFrame::OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext) 
{
	CreateView(pContext, AFX_IDW_PANE_FIRST);
	return TRUE;
}

BOOL CItemFrame::DestroyWindow() 
{
	return CMDIChildWnd::DestroyWindow();
}

void CItemFrame::OnDestroy() 
{
	CMDIChildWnd::OnDestroy();
}

void CItemFrame::OnClose() 
{
	CMDIChildWnd::OnClose();
}

BOOL CItemFrame::OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult) 
{
	return CMDIChildWnd::OnNotify(wParam, lParam, pResult);
}

void CItemFrame::OnMDIActivate(BOOL bActivate, CWnd* pActivateWnd, CWnd* pDeactivateWnd) 
{
	if(bActivate)
	{
		CView* pView = GetActiveView();
		if(pView->IsKindOf(RUNTIME_CLASS(CItemView)))
			SetTimer(1, 200, NULL);
	}
	CMDIChildWnd::OnMDIActivate(bActivate, pActivateWnd, pDeactivateWnd);
}

void MVC::Item::CItemFrame::OnTimer(UINT_PTR nIDEvent)
{
	if(nIDEvent == 1)
	{
		KillTimer(1);
		CView* pView = GetActiveView();
		if(pView->IsKindOf(RUNTIME_CLASS(CItemView)))
			((CItemView *)pView)->m_ItemGrid.SetFocus();
	}

	CXTPFrameWndBase<CMDIChildWnd>::OnTimer(nIDEvent);
}
