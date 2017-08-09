// CamFrame.cpp : 实现文件
//

#include "stdafx.h"
#include "DXPEditor.h"
#include "CamFrame.h"
#include "CamMgr.h"
#include "CamGrid.h"
#include "CamView.h"

// CCamFrame
using namespace MVC;
using namespace Camera;
IMPLEMENT_DYNCREATE(CCamFrame, CMDIChildWnd)

CCamFrame::CCamFrame()
{
}

CCamFrame::~CCamFrame()
{
}

BEGIN_MESSAGE_MAP(CCamFrame, CMDIChildWnd)
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_WM_CLOSE()
	ON_WM_MDIACTIVATE()
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CCamFrame 消息处理程序
int CCamFrame::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CMDIChildWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	return 0;
}

BOOL CCamFrame::OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext) 
{
	CreateView(pContext, AFX_IDW_PANE_FIRST);
	return TRUE;
}

BOOL CCamFrame::DestroyWindow() 
{
	return CMDIChildWnd::DestroyWindow();
}

void CCamFrame::OnDestroy() 
{
	CMDIChildWnd::OnDestroy();
}

void CCamFrame::OnClose() 
{
	CMDIChildWnd::OnClose();
}

BOOL CCamFrame::OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult) 
{
	return CMDIChildWnd::OnNotify(wParam, lParam, pResult);
}

void CCamFrame::OnMDIActivate(BOOL bActivate, CWnd* pActivateWnd, CWnd* pDeactivateWnd) 
{
	if(bActivate)
	{
		CView* pView = GetActiveView();
		if(pView->IsKindOf(RUNTIME_CLASS(CCamView)))
			SetTimer(1, 200, NULL);
	}
	CMDIChildWnd::OnMDIActivate(bActivate, pActivateWnd, pDeactivateWnd);
}

void MVC::Camera::CCamFrame::OnTimer(UINT_PTR nIDEvent)
{
	if(nIDEvent == 1)
	{
		KillTimer(1);
		CView* pView = GetActiveView();
		if(pView->IsKindOf(RUNTIME_CLASS(CCamView)))
			((CCamView *)pView)->m_ItemGrid.SetFocus();
	}

	CXTPFrameWndBase<CMDIChildWnd>::OnTimer(nIDEvent);
}
