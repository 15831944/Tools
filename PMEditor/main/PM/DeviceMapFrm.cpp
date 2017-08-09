// DeviceMapFrm.cpp : 实现文件
//

#include "stdafx.h"
#include "DXPEditor.h"
#include "MainFrm.h"
#include "DeviceMapFrm.h"
#include "DeviceMapView.h"

using namespace MVC;
using namespace Device;
// CDeviceMapFrm

IMPLEMENT_DYNCREATE(CDeviceMapFrm, CMDIChildWnd)

CDeviceMapFrm::CDeviceMapFrm()
:m_bFirstLoad(true)
{
}

CDeviceMapFrm::~CDeviceMapFrm()
{
}

BEGIN_MESSAGE_MAP(CDeviceMapFrm, CMDIChildWnd)
	ON_WM_MDIACTIVATE()
END_MESSAGE_MAP()

// CDeviceMapFrm 消息处理程序
void CDeviceMapFrm::OnMDIActivate(BOOL bActivate, CWnd* pActivateWnd, CWnd* pDeactivateWnd) 
{
	CView* pView = GetActiveView();
	if(pView->IsKindOf(RUNTIME_CLASS(CDeviceMapView)))
		CMDIChildWnd::OnMDIActivate(bActivate, pActivateWnd, pDeactivateWnd);
	if(!bActivate || !m_bFirstLoad)		return;
	CMainFrame* mf = (CMainFrame *)g_App.GetMainWnd();
	mf->OnShowViewDevice();
	m_bFirstLoad = false;
}