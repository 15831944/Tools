// DeviceMapView.cpp : 实现文件
//

#include "stdafx.h"
#include "PMApp.h"
#include "MyHelp.h"
#include "MainFrm.h"
#include "MyHelp.h"
#include "ProjectMgr.h"
#include "Gbl.h"
#include "SoftInfo.h"
#include "ServerCtrl.h"
#include "DeviceMapView.h"
#include "DeviceMapDoc.h"
#include "DeviceOne.h"
#include "ScanMgr.h"
#include "XmlMgr.h"
#include "XmlDevice.h"
#include "DevMgr.h"
#include "DevCompanyDlg.h"
#include "BehaviorParamDlg.h"
#include "DMemDC.h"
#include "DBMgr.h"
#include "DOPCMgr.h"
#include "RongYuMgr.h"

using namespace MVC;
using namespace Device;

// CDeviceMapView 诊断
#ifdef _DEBUG
void CDeviceMapView::AssertValid() const{CScrollView::AssertValid();}
#ifndef _WIN32_WCE
void CDeviceMapView::Dump(CDumpContext& dc) const{CScrollView::Dump(dc);}
#endif
#endif //_DEBUG

// CDeviceMapView
//IMPLEMENT_DYNCREATE(CDeviceMapView, CScrollView)
IMPLEMENT_DYNCREATE(CDeviceMapView, CScrollView)

CDeviceMapView::CDeviceMapView()
{
	m_bHostLineSelect = false;
	m_bDropNewIn = false;
	m_bDropOldIn = false;

	m_strHeader = _T("header");			//!< 悬浮标题
	m_strBody = _T("body");				//!< 悬浮内容
	m_strFooter = _T("footer");			//!< 底部文本
	m_strHtml = _T("");					//!< 超链接文本
}

CDeviceMapView::~CDeviceMapView()
{
}

BEGIN_MESSAGE_MAP(CDeviceMapView, CScrollView)
	ON_WM_PAINT()
	ON_WM_RBUTTONUP()
	ON_WM_LBUTTONDOWN()
	ON_WM_RBUTTONDOWN()
	ON_WM_DESTROY()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_CHAR()
	ON_WM_ERASEBKGND()
	ON_WM_KEYDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEWHEEL()
	ON_WM_HELPINFO()
	ON_MESSAGE(UNM_HYPERLINK_CLICKED, OnHyperlinkClicked)
	ON_COMMAND(ID_ADD_MAINDEV, &OnAddMainDev)
	ON_COMMAND(ID_ADD_SLAVE, &OnAddSlave)
	ON_COMMAND(ID_DEVINTERFACE_SET, &OnDevinterfaceSet)
	ON_COMMAND(ID_DELETE, &OnDevDelete)
	ON_COMMAND(ID_VKDELETE, &OnDevDelete)
	ON_COMMAND(ID_DEV_PARAINFO, &OnDevParainfo)
	ON_COMMAND(ID_DEV_OUT, &OnDevOut)
	ON_COMMAND(ID_DEV_IN, &OnDevIn)
	ON_COMMAND(ID_EDIT_COPY, &CDeviceMapView::OnEditCopy)
	ON_COMMAND(ID_COPY_WITH_CHILD, &CDeviceMapView::OnEditCopyWithChild)
	ON_COMMAND(ID_EDIT_CUT, &CDeviceMapView::OnEditCut)
	ON_COMMAND(ID_EDIT_PASTE, &CDeviceMapView::OnEditPaste)
	ON_COMMAND(ID_INTERFACE_SET, &CDeviceMapView::OnInterfaceSet)
	ON_COMMAND(ID_EDIT_UNDO, &CDeviceMapView::OnEditUndo)
	ON_COMMAND(ID_EDIT_REDO, &CDeviceMapView::OnEditRedo)
	ON_COMMAND_RANGE(ID_ZOOM80, ID_ZOOM200, &CDeviceMapView::OnZoom)
	ON_COMMAND(ID_SCAN_ADD, &OnAddScan)
	ON_COMMAND(ID_SCAN_SET, &CDeviceMapView::OnScanSet)
	ON_COMMAND(ID_SCAN_START, &CDeviceMapView::OnScanStart)
	ON_COMMAND(ID_SCAN_STOP, &CDeviceMapView::OnScanStop)
	ON_COMMAND(ID_H, &CDeviceMapView::OnH)
	ON_COMMAND(ID_V, &CDeviceMapView::OnV)
	ON_COMMAND(ID_ADDINF2PROJ, &CDeviceMapView::OnAddScanInf2Proj)
	ON_COMMAND(ID_FRESH_STATE, &CDeviceMapView::OnFreshState)
	ON_COMMAND(ID_PROGRAM, &CDeviceMapView::OnProgram)
	ON_UPDATE_COMMAND_UI(ID_ADD_MAINDEV, &CDeviceMapView::OnUpdateAddMainDev)
	ON_UPDATE_COMMAND_UI(ID_ADD_SLAVE, &CDeviceMapView::OnUpdateAddSlave)
	ON_UPDATE_COMMAND_UI(ID_DEVINTERFACE_SET, &CDeviceMapView::OnUpdateDevInfo)
	ON_UPDATE_COMMAND_UI(ID_INTERFACE_SET, &CDeviceMapView::OnUpdateInterfaceSet)
	ON_UPDATE_COMMAND_UI(ID_DELETE, &CDeviceMapView::OnUpdateDelete)
	ON_UPDATE_COMMAND_UI(ID_DEV_PARAINFO, &CDeviceMapView::OnUpdateParainfo)
	ON_UPDATE_COMMAND_UI(ID_DEV_OUT, &CDeviceMapView::OnUpdateDevOut)
	ON_UPDATE_COMMAND_UI(ID_DEV_IN, &CDeviceMapView::OnUpdateDevIn)
	ON_UPDATE_COMMAND_UI(ID_EDIT_COPY, &CDeviceMapView::OnUpdateEditCopy)
	ON_UPDATE_COMMAND_UI(ID_COPY_WITH_CHILD, &CDeviceMapView::OnUpdateEditCopyWithChild)
	ON_UPDATE_COMMAND_UI(ID_EDIT_CUT, &CDeviceMapView::OnUpdateEditCut)
	ON_UPDATE_COMMAND_UI(ID_EDIT_PASTE, &CDeviceMapView::OnUpdateEditPaste)
	ON_UPDATE_COMMAND_UI(ID_EDIT_UNDO, &CDeviceMapView::OnUpdateEditUndo)
	ON_UPDATE_COMMAND_UI(ID_EDIT_REDO, &CDeviceMapView::OnUpdateEditRedo)
	ON_UPDATE_COMMAND_UI(ID_PROGRAM, &CDeviceMapView::OnUpdateProgram)
END_MESSAGE_MAP()


// CDeviceMapView 绘图
CDeviceMapDoc* CDeviceMapView::GetDocument() // non-debug version is inline
{ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CDeviceMapDoc)));
return (CDeviceMapDoc*)m_pDocument;}

BOOL MVC::Device::CDeviceMapView::PreCreateWindow(CREATESTRUCT& cs)
{
	cs.style |= WS_CLIPCHILDREN|WS_CLIPSIBLINGS;
	cs.dwExStyle |= WS_EX_STATICEDGE;
	return CScrollView::PreCreateWindow(cs);
}

void CDeviceMapView::OnInitialUpdate()
{
	CScrollView::OnInitialUpdate();
	CSize sizeTotal;
	//!< 计算此视图的合计大小
	sizeTotal = GetDocument()->GetViewSize();
	SetScrollSizes(MM_TEXT, sizeTotal);
	m_DropTarget.Register(this);
	m_ToolTip.Create(this);
}

void CDeviceMapView::OnUpdate(CView* /*pSender*/, LPARAM /*lHint*/, CObject* /*pHint*/)
{
	CSize szTotalPage(0, 0);
	CSize szPage(80, 400);
	CSize szLine(30, 30);

	//std::shared_ptr<CDeviceOne> device;
	CDevMgr* mgr = &CDevMgr::GetMe();
	for (auto device : mgr->m_vtDevice)
	{
		if(!device)			continue;
		CPoint ptDev = device->GetLTPt();
		if(szTotalPage.cx < (ptDev.x + CDeviceMapDoc::DEV_WIDTH + 20))
			szTotalPage.cx = ptDev.x + CDeviceMapDoc::DEV_WIDTH + 20;
		if(szTotalPage.cy < (ptDev.y + CDeviceMapDoc::DEV_HEIGH + 40))
			szTotalPage.cy = ptDev.y + CDeviceMapDoc::DEV_HEIGH + 40;
	}
	UINT size = SoftInfo::CSoftInfo::GetMe().getZoomSize();
	szTotalPage.cx = (szTotalPage.cx * size) / 100 + 300;
	szTotalPage.cy = (szTotalPage.cy * size) / 100 + 200;
	szPage.cx = (szPage.cx * size) / 100;
	szPage.cy = (szPage.cy * size) / 100;
	szLine.cx = (szLine.cx * size) / 100;
	szLine.cy = (szLine.cy * size) / 100;
	SetScrollSizes(MM_TEXT, szTotalPage, szPage, szLine);
	Invalidate(FALSE);
}

void CDeviceMapView::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	CXTPClientRect rClient(this);
	int size = (int)SoftInfo::CSoftInfo::GetMe().getZoomSize();
	if(size < 100)
	{
		rClient.right = rClient.left + (int)(rClient.Width() * 100 / size);
		rClient.bottom = rClient.top + (int)(rClient.Height() * 100 / size);
	}
	Tool::CDMemDC mdc(&dc, rClient, GetXtremeColor(COLOR_WINDOW));

	//!< 经过缩放后，滚轮的起始点也改变了
	CPoint point = GetScrollPosition();
	point.x = (point.x * 100) / size;
	point.y = (point.y * 100) / size;
	mdc.SetViewportOrg(-point);

	OnDraw(&mdc);
	mdc.SetZoomStyle(size, 100, size, 100);
}

void CDeviceMapView::OnDraw(CDC* pDC)
{
	CDeviceMapDoc* pDoc = GetDocument();
	CDevMgr* mgr = &CDevMgr::GetMe();

	CRect rc;
	CBrush bs;
	bs.CreateSolidBrush(::GetSysColor(COLOR_WINDOW));
	pDC->GetClipBox(&rc);
	pDC->FillRect(rc,&bs);

	pDC->SetBkMode(TRANSPARENT);
	OnDrawHostLine(pDC);												//!< 画母线

	std::list<std::shared_ptr<CDeviceOne> > select_list;				//!< 被选中的设备和映射为从设备的设备
	//std::shared_ptr<CDeviceOne> device;
	for (auto device : mgr->m_vtDevice)
	{
		if(!device)					continue;
		if(device->IsSelect())		select_list.push_back(device);
		else						device->OnDraw(pDC);				//!< 每个设备自己画自己
	}
	for (auto device : select_list)	device->OnDraw(pDC);				//!< 画所有被选中的设备

	if(m_bDropNewIn)													//!< 需要要设备拖动图标了
	{
		CPoint point;
		GetCursorPos(&point);
		ScreenToClient(&point);
		CPoint srcPoint = point;
		pDC->DPtoLP(&point);
		int width = point.x - srcPoint.x;
		int heigh = point.y - srcPoint.y;
		UINT zoomSize = SoftInfo::CSoftInfo::GetMe().getZoomSize();
		point.x = (point.x + 20 - width) * 100 / zoomSize + width;
		point.y = (point.y + 20 - heigh) * 100 / zoomSize + heigh;

		CDC memdc;
		memdc.CreateCompatibleDC(pDC);
		CBitmap bitmap;
		//bitmap.LoadBitmap(IDB_BMP_DEVICEDEFAULT);
		bitmap.LoadBitmap(IDB_BMP_DEVICELIST);
		memdc.SelectObject(&bitmap);
		BITMAP bm;
		bitmap.GetObject(sizeof(BITMAP),&bm);
		UINT index = 0;			//!< 我们公司的设备是2
		std::shared_ptr<XmlInfo::CXmlDevice> xmlDev = g_App.m_XmlMgr->GetDevice(m_uiDropDevType);
		if(!xmlDev)			return;
		if(xmlDev->getIcon() >= bm.bmWidth / 32)		xmlDev->m_uiDevIcon = 0;
		pDC->BitBlt(point.x, point.y, 32, 32, &memdc, 32 * xmlDev->getIcon(), 0, SRCAND);
		Invalidate(FALSE);
	}
}

void CDeviceMapView::OnDrawHostLine(CDC* pDC)
{
	CPen pen1, pen2, pen3, pen4, pen5;
	pen1.CreatePen(PS_SOLID,1,COLORREF(RGB(255, 255, 255)));	//!< 白色
	if(m_bHostLineSelect)	pen2.CreatePen(PS_SOLID,1,COLORREF(RGB(80, 80, 238)));	//!< 浅灰1
	else					pen2.CreatePen(PS_SOLID,1,COLORREF(RGB(241, 239, 226)));	//!< 浅灰1
	pen3.CreatePen(PS_SOLID,1,COLORREF(RGB(172, 168, 153)));	//!< 中灰
	pen4.CreatePen(PS_SOLID,1,COLORREF(RGB(113, 111, 100)));	//!< 深灰
	CPen* pOldPen = pDC->SelectObject(&pen1);
	CSize size = GetTotalSize();
	UINT zoomsize = SoftInfo::CSoftInfo::GetMe().getZoomSize();
	size.cx = (size.cx * 100) / zoomsize;
	size.cy = (size.cy * 100) / zoomsize;
	if(SoftInfo::CSoftInfo::GetMe().getHDevMap()){
		pDC->SelectObject(pen2);					//!< 左上外侧的灰线
		pDC->MoveTo(CDeviceMapDoc::DEV_HOSTLINE_X, CDeviceMapDoc::DEV_HOSTLINE_Y + 5);
		pDC->LineTo(CDeviceMapDoc::DEV_HOSTLINE_X, CDeviceMapDoc::DEV_HOSTLINE_Y);
		pDC->LineTo(size.cx - 200, CDeviceMapDoc::DEV_HOSTLINE_Y);

		pDC->SelectObject(pen1);					//!< 左上内侧的白线
		pDC->MoveTo(CDeviceMapDoc::DEV_HOSTLINE_X + 1, CDeviceMapDoc::DEV_HOSTLINE_Y + 4);
		pDC->LineTo(CDeviceMapDoc::DEV_HOSTLINE_X + 1, CDeviceMapDoc::DEV_HOSTLINE_Y + 1);
		pDC->LineTo(size.cx - 201, CDeviceMapDoc::DEV_HOSTLINE_Y + 1);

		pDC->SelectObject(pen2);					//!< 中间的灰线
		pDC->MoveTo(CDeviceMapDoc::DEV_HOSTLINE_X + 2, CDeviceMapDoc::DEV_HOSTLINE_Y + 2);
		pDC->LineTo(size.cx - 201, CDeviceMapDoc::DEV_HOSTLINE_Y + 2);
		pDC->MoveTo(CDeviceMapDoc::DEV_HOSTLINE_X + 2, CDeviceMapDoc::DEV_HOSTLINE_Y + 3);
		pDC->LineTo(size.cx - 201, CDeviceMapDoc::DEV_HOSTLINE_Y + 3);

		pDC->SelectObject(pen3);					//!< 右下内侧的中灰线
		pDC->MoveTo(CDeviceMapDoc::DEV_HOSTLINE_X + 2, CDeviceMapDoc::DEV_HOSTLINE_Y + 4);
		pDC->LineTo(size.cx - 201, CDeviceMapDoc::DEV_HOSTLINE_Y + 4);
		pDC->LineTo(size.cx - 201, CDeviceMapDoc::DEV_HOSTLINE_Y + 1);

		pDC->SelectObject(pen4);					//!< 右下外侧的深灰线
		pDC->MoveTo(CDeviceMapDoc::DEV_HOSTLINE_X + 1, CDeviceMapDoc::DEV_HOSTLINE_Y + 5);
		pDC->LineTo(size.cx - 200, CDeviceMapDoc::DEV_HOSTLINE_Y + 5);
		pDC->LineTo(size.cx - 200, CDeviceMapDoc::DEV_HOSTLINE_Y);
	}
	else{
		pDC->SelectObject(pen2);					//!< 左上外侧的灰线
		pDC->MoveTo(CDeviceMapDoc::DEV_HOSTLINE_X + 5, CDeviceMapDoc::DEV_HOSTLINE_Y);
		pDC->LineTo(CDeviceMapDoc::DEV_HOSTLINE_X, CDeviceMapDoc::DEV_HOSTLINE_Y);
		pDC->LineTo(CDeviceMapDoc::DEV_HOSTLINE_X, size.cy - 200);

		pDC->SelectObject(pen1);					//!< 左上内侧的白线
		pDC->MoveTo(CDeviceMapDoc::DEV_HOSTLINE_X + 4, CDeviceMapDoc::DEV_HOSTLINE_Y + 1);
		pDC->LineTo(CDeviceMapDoc::DEV_HOSTLINE_X + 1, CDeviceMapDoc::DEV_HOSTLINE_Y + 1);
		pDC->LineTo(CDeviceMapDoc::DEV_HOSTLINE_X + 1, size.cy - 201);

		pDC->SelectObject(pen2);					//!< 中间的灰线
		pDC->MoveTo(CDeviceMapDoc::DEV_HOSTLINE_X + 2, CDeviceMapDoc::DEV_HOSTLINE_Y + 2);
		pDC->LineTo(CDeviceMapDoc::DEV_HOSTLINE_X + 2, size.cy - 201);
		pDC->MoveTo(CDeviceMapDoc::DEV_HOSTLINE_X + 3, CDeviceMapDoc::DEV_HOSTLINE_Y + 2);
		pDC->LineTo(CDeviceMapDoc::DEV_HOSTLINE_X + 3, size.cy - 201);

		pDC->SelectObject(pen3);					//!< 右下内侧的中灰线
		pDC->MoveTo(CDeviceMapDoc::DEV_HOSTLINE_X + 4, CDeviceMapDoc::DEV_HOSTLINE_Y + 2);
		pDC->LineTo(CDeviceMapDoc::DEV_HOSTLINE_X + 4, size.cy - 201);
		pDC->LineTo(CDeviceMapDoc::DEV_HOSTLINE_X + 1, size.cy - 201);

		pDC->SelectObject(pen4);					//!< 右下外侧的深灰线
		pDC->MoveTo(CDeviceMapDoc::DEV_HOSTLINE_X + 5, CDeviceMapDoc::DEV_HOSTLINE_Y + 1);
		pDC->LineTo(CDeviceMapDoc::DEV_HOSTLINE_X + 5, size.cy - 200);
		pDC->LineTo(CDeviceMapDoc::DEV_HOSTLINE_X, size.cy - 200);
	}
	pDC->SelectObject(pOldPen);
}

// CDeviceMapView 消息处理程序
void CDeviceMapView::OnDrawInit(CDC* pDC)	// 画初始化状态
{
}

BOOL CDeviceMapView::OnEraseBkgnd(CDC* pDC)
{
	return FALSE;
}

void CDeviceMapView::OnRButtonUp(UINT nFlags, CPoint point)
{
	CClientDC dc(this);
	OnPrepareDC(&dc);
	CPoint devPoint = point;
	dc.DPtoLP(&devPoint);				//!< 设备如果要弹出菜单，需要这个坐标
	ClientToScreen(&point);

	CDeviceMapDoc *pDoc = GetDocument();
	std::shared_ptr<CDeviceOne> pExpandDevice;
	std::shared_ptr<CDeviceOne> pDevice, pSelectDevice;
	CDevMgr* devMgr = &CDevMgr::GetMe();
	CProjectMgr* projMgr = &CProjectMgr::GetMe();
	for (auto pDevice : devMgr->m_vtDevice){
		if(!pDevice)	continue;
		pDevice->SetSelect(false);
		if(pDevice->IsInMyRect(devPoint)){
			pDevice->SetSelect(true);
			pSelectDevice = pDevice;
		}
	}
	//!< 弹出选中设备的菜单
	if(pSelectDevice){
		pSelectDevice->OnRButtonUp(nFlags, point, this);	//!< 让设备去弹出本该属于它的右键菜单
	}
	//!< 弹出拓扑图菜单
	else{													//!< 如果没有选中设备，就弹出来公共的菜单
		CMenu popMenu;
		popMenu.LoadMenu(IDR_RBUTTON_MENU);
		CMenu* menu = popMenu.GetSubMenu(3);
		if(pDoc->IsUndoEmpty())					menu->EnableMenuItem(ID_EDIT_UNDO, MF_DISABLED|MF_BYCOMMAND);
		if(pDoc->IsRedoEmpty())					menu->EnableMenuItem(ID_EDIT_REDO, MF_DISABLED|MF_BYCOMMAND);
		if(devMgr->m_ltDevClipBoard.empty())	menu->EnableMenuItem(ID_EDIT_PASTE, MF_DISABLED|MF_BYCOMMAND);
		if(projMgr->IsWatch())
		{
			menu->EnableMenuItem(ID_EDIT_UNDO, MF_DISABLED|MF_BYCOMMAND);
			menu->EnableMenuItem(ID_EDIT_REDO, MF_DISABLED|MF_BYCOMMAND);
			menu->EnableMenuItem(ID_EDIT_PASTE, MF_DISABLED|MF_BYCOMMAND);
			menu->EnableMenuItem(ID_INTERFACE_SET, MF_DISABLED|MF_BYCOMMAND);
			menu->EnableMenuItem(ID_ADD_MAINDEV, MF_DISABLED|MF_BYCOMMAND);
		}
		else
		{
			menu->DeleteMenu(ID_FRESH_STATE, MF_BYCOMMAND);
		}

		UINT zoomSize = SoftInfo::CSoftInfo::GetMe().getZoomSize();
		if(zoomSize == 80)			menu->CheckMenuRadioItem(ID_ZOOM80, ID_ZOOM200, ID_ZOOM80, 1);
		else if(zoomSize == 100)	menu->CheckMenuRadioItem(ID_ZOOM80, ID_ZOOM200, ID_ZOOM100, 1);
		else if(zoomSize == 150)	menu->CheckMenuRadioItem(ID_ZOOM80, ID_ZOOM200, ID_ZOOM150, 1);
		else if(zoomSize == 200)	menu->CheckMenuRadioItem(ID_ZOOM80, ID_ZOOM200, ID_ZOOM200, 1);

		bool bHZ = SoftInfo::CSoftInfo::GetMe().getHDevMap();
		if(bHZ)						menu->CheckMenuRadioItem(ID_H, ID_H, ID_H, 1);
		else						menu->CheckMenuRadioItem(ID_V, ID_V, ID_V, 1);

		//!< 设置扫描显示的文本
		if(projMgr->IsScan())		menu->ModifyMenu(ID_SCAN_START, MF_BYCOMMAND, ID_SCAN_START, _T("刷新扫描数据(&F)"));
		else						menu->ModifyMenu(ID_SCAN_START, MF_BYCOMMAND, ID_SCAN_START, _T("开始扫描(&R)"));
		if(!projMgr->IsScan())		menu->EnableMenuItem(ID_SCAN_STOP, MF_DISABLED|MF_BYCOMMAND);

		CXTPCommandBars::TrackPopupMenu(menu, TPM_LEFTALIGN|TPM_NONOTIFY, point.x, point.y, this);
	}
	m_bHostLineSelect = false;
	if(IsInHostLineRect(point))		m_bHostLineSelect = true;
}

void CDeviceMapView::OnLButtonDown(UINT nFlags, CPoint point)
{
	CClientDC dc(this);
	OnPrepareDC(&dc);
	dc.DPtoLP(&point);					//!< 小坐标转打坐标

	CDeviceMapDoc*pDoc=GetDocument();
	std::shared_ptr<CDeviceOne> pExpandDevice;
	std::shared_ptr<CDeviceOne> pDevice;
	CDevMgr* mgr = &CDevMgr::GetMe();
	int stype=0;
	ClearSelect();
	for (auto pDevice : mgr->m_vtDevice)
	{
		if(!pDevice)	continue;
		stype = pDevice->IsInMyRect(point);
		if(stype == 2)		pExpandDevice = pDevice;
		if(stype > 0)		{pDevice->SetSelect(true);	break;}
	}
	m_bHostLineSelect = false;
	if(IsInHostLineRect(point))		m_bHostLineSelect = true;
	if(pExpandDevice)	//!< 需要显示或不显示自己的子设备
	{
		pExpandDevice->SetExpand(!pExpandDevice->IsExpand());
		for (UINT id : pExpandDevice->m_ltChildID)
		{
			pDevice = mgr->GetDevice(id);
			ASSERT(pDevice);
			pDevice->ShowHideChild(pExpandDevice->IsExpand());
		}
		pDoc->FreshMap();
	}

	Invalidate(FALSE);
	dc.LPtoDP(&point);
	CScrollView::OnLButtonDown(nFlags, point);
}


void MVC::Device::CDeviceMapView::OnLButtonUp(UINT nFlags, CPoint point)
{
	CScrollView::OnLButtonUp(nFlags, point);
}

void CDeviceMapView::OnRButtonDown(UINT nFlags, CPoint point)
{
	OnLButtonDown(nFlags,point);
	CScrollView::OnRButtonDown(nFlags, point);
}

void CDeviceMapView::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	CClientDC dc(this);
	OnPrepareDC(&dc);
	dc.DPtoLP(&point);					//!< 小坐标转打坐标

	CDeviceMapDoc*pDoc=GetDocument();
	CDevMgr* mgr = &CDevMgr::GetMe();
	std::shared_ptr<CDeviceOne> pDevice;
	ClearSelect();
	m_bHostLineSelect = IsInHostLineRect(point);	//!< 选中母线

	for (auto pDevice : mgr->m_vtDevice)
	{
		if(!pDevice)		continue;
		if(pDevice->IsInMyRect(point))
		{
			pDevice->SetSelect(true);
			OnDevinterfaceSet();					//!< 双击一个设备，弹出设备属性对话框
			break;
		}
	}

	Invalidate(FALSE);
	dc.LPtoDP(&point);
	CScrollView::OnLButtonDblClk(nFlags, point);
}

BOOL MVC::Device::CDeviceMapView::OnMouseMove(UINT nFlags, CPoint point)
{
	ScreenToClient(&point);
	CClientDC dc(this);
	OnPrepareDC(&dc);
	dc.DPtoLP(&point);					//!< 小坐标转打坐标

	CRect rect;
	Tool::TooltipText txtGroup;

	std::shared_ptr<CDeviceOne> device;
	CDevMgr* devMgr = &CDevMgr::GetMe();

	//!< 遍历所有节点，找到对应的CDCCEInterFace_Tooltip
	for (auto device : devMgr->m_vtDevice)
	{
		if(!device)		continue;
		if(device->IsInMyRect(point))
		{
			device->GetWatchInfo(txtGroup);
			m_ToolTip.AddTooltipText(this,txtGroup);
			return TRUE;
		}
	}
	CScrollView::OnMouseMove(nFlags, point);
	return FALSE;
}

void MVC::Device::CDeviceMapView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	switch(nChar)
	{
	case VK_DELETE:		OnDevDelete();			break;
	case VK_UP:			OnSelectUp();			break;
	case VK_DOWN:		OnSelectDown();			break;
	case VK_LEFT:		OnSelectLeft();			break;
	case VK_RIGHT:		OnSelectRight();		break;
	case VK_ESCAPE:		ClearSelect();			Invalidate(FALSE);			break;
	case VK_ADD:		OnMouseWheel(MK_CONTROL, 20, CPoint(0, 0));			break;
	case VK_SUBTRACT:	OnMouseWheel(MK_CONTROL, -20, CPoint(0, 0));		break;
	}
	CScrollView::OnKeyDown(nChar, nRepCnt, nFlags);
}

//!< 看看当前被拖拽的设备与被选中的设备是否能连接
bool MVC::Device::CDeviceMapView::IfDevCanLink()
{
// 	if(!m_DropDevice)									return false;
// 	std::shared_ptr<CDeviceOne> dev = GetSelectDev();
// 	if(!dev)											return true;	//!< 可以连到空白处
// 	if(dev == m_DropDevice)								return false;	//!< 自己不能连到自己上
// 	if(dev->getID() == m_DropDevice->getParentID())		return false;	//!< 自己不能再一次连到父亲下边
// 	if(dev->IsMyParent(m_DropDevice->getID()))			return false;	//!< 自己不能连到孩子下边
	return true;
}

bool CDeviceMapView::IsInHostLineRect(CPoint point)
{
	point.x = (point.x * 100) / SoftInfo::CSoftInfo::GetMe().getZoomSize();
	point.y = (point.y * 100) / SoftInfo::CSoftInfo::GetMe().getZoomSize();
	CSize size = GetTotalSize();
	CRect rect(CDeviceMapDoc::DEV_HOSTLINE_X - 3, CDeviceMapDoc::DEV_HOSTLINE_Y - 3, 0, 0);
	if(SoftInfo::CSoftInfo::GetMe().getHDevMap()){
		rect.right = size.cx - 30 + 3;
		rect.bottom = CDeviceMapDoc::DEV_HOSTLINE_Y + 5 + 3;
	}
	else{
		rect.right = CDeviceMapDoc::DEV_HOSTLINE_X + 5 + 3;
		rect.bottom = size.cy - 30 + 3;
	}
	return rect.PtInRect(point);
}

void CDeviceMapView::OnDestroy()
{
	m_DropTarget.Revoke();
}

void CDeviceMapView::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	CScrollView::OnChar(nChar, nRepCnt, nFlags);
	if(::IsCharAlpha(nChar)){
		m_strInWord += char(nChar);
	}
	else if(nChar == 13)
	{
		if(m_strInWord.MakeUpper() == _T("HZ")){		//!< 设置横纵方向
			SoftInfo::CSoftInfo::GetMe().setHDevMap(!SoftInfo::CSoftInfo::GetMe().getHDevMap());
			FreshDeviceMap();
		}
		else if(m_strInWord.MakeUpper() == _T("ADD")){	//!< 添加设备
			OnAddDev();
		}
		else if(m_strInWord.MakeUpper() == _T("NEW")){
			OnAddDev();
		}
		else if(m_strInWord.MakeUpper() == _T("DEL")){	//!< 删除设备
			OnDevDelete();
		}
		else if(m_strInWord.MakeUpper() == _T("INF")){	//!< 设置工程师站
			OnInterfaceSet();
		}
		else if(m_strInWord.MakeUpper() == _T("CLOSE")){
			((CMainFrame *)g_App.GetMainWnd())->OnClose();			return;
		}
		else if(m_strInWord.MakeUpper() == _T("SAVE")){
			((CMainFrame *)g_App.GetMainWnd())->OnProjSave();
		}
		else if(m_strInWord.MakeUpper() == _T("SAVEAS")){
			((CMainFrame *)g_App.GetMainWnd())->OnProjBackUp();
		}
		else if(m_strInWord.MakeUpper() == _T("OPEN")){
			((CMainFrame *)g_App.GetMainWnd())->OnProjOpen();		return;		//不return会崩溃噢!
		}
		else if(m_strInWord.MakeUpper() == _T("PROJINFO")){
			((CMainFrame *)g_App.GetMainWnd())->OnProjInfo();
		}
		else if(m_strInWord.MakeUpper() == _T("START")){
			((CMainFrame *)g_App.GetMainWnd())->OnServerRun();
		}
		else if(m_strInWord.MakeUpper() == _T("STOP")){
			((CMainFrame *)g_App.GetMainWnd())->OnServerStop();
		}
		else if(m_strInWord.MakeUpper() == _T("CHECK")){
			((CMainFrame *)g_App.GetMainWnd())->OnCompileCheck();
		}
		else if(m_strInWord.MakeUpper() == _T("HMI")){
			((CMainFrame *)g_App.GetMainWnd())->OnHmiStart();
		}
		else if(m_strInWord.MakeUpper() == _T("FIND")){
			((CMainFrame *)g_App.GetMainWnd())->OnFind();
		}
		else if(m_strInWord.MakeUpper() == _T("HELP")){
			((CMainFrame *)g_App.GetMainWnd())->OnHelpShow();
		}
		else if(m_strInWord.MakeUpper() == _T("OPC")){
			Servers::DOPC::CDOPCMgr::GetMe().OnSetDOPC();
		}
		else if(m_strInWord.MakeUpper() == _T("DB")){
			Servers::DB::CDBMgr::GetMe().OnSetDB();
		}
		else if(m_strInWord.MakeUpper() == _T("RONGYU")){
			Servers::RongYu::CRongYuMgr::GetMe().OnSetRongYu();
		}
#ifdef _DEBUG
		else if(m_strInWord.MakeUpper() == _T("STARTPAGE")){
			g_App.ShowStartPage();
		}
		else if(m_strInWord.MakeUpper() == _T("RY")){
			VARIANT var;
			var.vt=VT_I4;
			VARIANT nRYType = ((CMainFrame *)g_App.GetMainWnd())->m_SevCommer.Execute(106, var, 0, 0, 0);
			CString str;
			str.Format("冗余类型:%d", nRYType.intVal);
			MessageBox(str, _T("提示"));
		}
		else if(m_strInWord.MakeUpper() == _T("CRYA")){
			VARIANT var;
			var.vt=VT_I4;
			var.intVal = 1;
			VARIANT nRet = ((CMainFrame *)g_App.GetMainWnd())->m_SevCommer.Execute(205, var, 0, 0, 0);
		}
		else if(m_strInWord.MakeUpper() == _T("CRYB")){
			VARIANT var;
			var.vt=VT_I4;
			var.intVal = 2;
			VARIANT nRet = ((CMainFrame *)g_App.GetMainWnd())->m_SevCommer.Execute(205, var, 0, 0, 0);
		}
#endif
		else{
			OnDevinterfaceSet();
		}
		m_strInWord = "";
	}
	CScrollView::OnChar(nChar, nRepCnt, nFlags);
}

void CDeviceMapView::OnH()
{
	SoftInfo::CSoftInfo::GetMe().setHDevMap(true);
	FreshDeviceMap();
}

void CDeviceMapView::OnV()
{
	SoftInfo::CSoftInfo::GetMe().setHDevMap(false);
	FreshDeviceMap();
}

//!< 将该设备的所有扫描上来的接口设置为工程接口
void CDeviceMapView::OnAddScanInf2Proj()
{
	std::shared_ptr<CDeviceOne> device = GetSelectDev();
	std::shared_ptr<CDeviceInterface> inf;
	int size = device->m_vtInterface.size();
	for(int i = 0; i < size; ++i)
//	foreach(inf, device->m_vtInterface)
	{
		inf = device->m_vtInterface[i];
		if(!inf)			continue;
		if(inf->IsProj())	continue;
		inf->SetProj(true);
		inf->SetID(i);
	}
	CDevMgr::GetMe().SetModify();
}

//!< 重新画一遍设备拓扑
void CDeviceMapView::FreshDeviceMap()
{
	std::shared_ptr<CDeviceOne> dev = GetSelectDev(false);
	if (dev)
	{
		ShowDeviceAt(dev->getID());
	}
	else
	{
		CDeviceMapDoc* pDoc = GetDocument();
		pDoc->FreshMap();
	}
}

//!< 取消所有设备的选中状态
void CDeviceMapView::ClearSelect()
{
	CDevMgr* devMgr = &CDevMgr::GetMe();
	for (std::shared_ptr<CDeviceOne> device : devMgr->m_vtDevice){
		if(!device)		continue;
		device->SetSelect(false);
	}
	m_bHostLineSelect = false;
}

//!< 添加设备，如果没有被选中的则添加根设备，否则添加从设备
void CDeviceMapView::OnAddDev()
{
	if(GetSelectDev())		OnAddSlave();
	else					OnAddMainDev();
}

void CDeviceMapView::OnAddMainDev()
{
	if(CProjectMgr::GetMe().SayWatch())		return;
	//!< 先让用户找添加设备的类型
	Dialog::CDevCompanyDlg devCompanyDlg;
	if(IDOK != devCompanyDlg.DoModal())	return;

	//!< 然后让用户配置新设备的信息
	OnAddMainDev(devCompanyDlg.GetDevID());
}

void MVC::Device::CDeviceMapView::OnAddSlave()
{
	if(CProjectMgr::GetMe().SayWatch())		return;
	//!< 然后再让用户添加设备的类型，规定从设备数量
	std::shared_ptr<CDeviceOne> selDev = GetSelectDev();
	if(!selDev)							return;
	std::shared_ptr<XmlInfo::CXmlDevice> xmlDev = selDev->GetXmlInfo();
	if(!xmlDev)							return;
	if(!selDev->CanAddSlave()){
		return;
	}

	Dialog::CDevCompanyDlg devCompanyDlg;
	if(IDOK != devCompanyDlg.DoModal())		return;

	//!< 最后打开配置新设备的对话框
	OnAddSlave(devCompanyDlg.GetDevID());
}

//!< 添加主设备，这里已经指定了哪个设备
void CDeviceMapView::OnAddMainDev(UINT devID)
{
	if(CProjectMgr::GetMe().SayWatch())		return;
	Dialog::CPropertyDlg* propertyDlg = &Dialog::CPropertyDlg::GetMe();
	CDevMgr* devMgr = &CDevMgr::GetMe();
	if(!devMgr->m_DeviceProperty.SetType(0, devID))	return;
	ClearSelect();
	CDeviceMapDoc* pDoc = GetDocument();
	if(IDOK == propertyDlg->DoModal(devMgr->m_DeviceProperty))
	{
		std::shared_ptr<CDeviceOne> device = std::shared_ptr<CDeviceOne>(new CDeviceOne());
		device->CopyFrom(*devMgr->m_DeviceProperty.m_ShowDev);
		device->setLevel(0);
		if(!device->InitDevType(devID))		return;			//!< 初始化设备的类型信息
		if(!devMgr->AddNewDevice(device))	return;
		device->SetSelect(true);
		std::shared_ptr<SDevUndo> sdu = std::shared_ptr<SDevUndo>(new SDevUndo(CGbl::UNDO_TYPE_ADD, device));
		pDoc->AddUndoMember(sdu);
		pDoc->SetUndoEnd();
		FreshDeviceMap();
	}
}

//!< 添加从设备，这里已经指定了哪个设备
void CDeviceMapView::OnAddSlave(UINT devID)
{
	if(CProjectMgr::GetMe().SayWatch())		return;
	//!< 规定从设备数量
	std::shared_ptr<CDeviceOne> selDev = GetSelectDev();
	if(!selDev)							return;
	std::shared_ptr<XmlInfo::CXmlDevice> xmlDev = selDev->GetXmlInfo();
	if(!xmlDev)							return;
	if(!selDev->CanAddSlaveType(devID))
	{
		CString strError;
		strError.Format("所选设备不支持链接该类型的从设备", xmlDev->getMaxInf2Slave());
		CGbl::PrintOut(strError);
		return;
	}
	if(!selDev->CanAddSlave())
	{
		return;
	}

	CDevMgr* devMgr = &CDevMgr::GetMe();
	CDeviceMapDoc* pDoc = GetDocument();
	std::shared_ptr<CDeviceOne> device;

	Dialog::CPropertyDlg* dlg = &Dialog::CPropertyDlg::GetMe();
	devMgr->m_SlaveProperty.SetType(0, devID, selDev->getID());
	if(IDOK == dlg->DoModal(devMgr->m_SlaveProperty))
	{
		device = std::shared_ptr<CDeviceOne>(new CDeviceOne());
		device->CopyFrom(*devMgr->m_SlaveProperty.m_ShowDev);
		device->setParentID(selDev->getID());
		device->setLevel(selDev->getLevel() + 1);
		device->SetSelect(true);
		if(!device->InitDevType(devID))		return;		//!< 初始化设备的类型信息
		if(!devMgr->AddNewDevice(device))	return;
		devMgr->CheckAndConnect();
		std::shared_ptr<SDevUndo> sdu = std::shared_ptr<SDevUndo>(new SDevUndo(CGbl::UNDO_TYPE_ADD, device));
		pDoc->AddUndoMember(sdu);
		pDoc->SetUndoEnd();
		FreshDeviceMap();
	}
}

void CDeviceMapView::OnAddScan()
{
	CDevMgr* devMgr = &CDevMgr::GetMe();
	std::shared_ptr<CDeviceOne> device = GetSelectDev(false);		//!< 所有设备都找
	if(!device)					return;
	if(device->IsProj())		return;

	//!< 如果父亲不是工程设备需要提示
	std::shared_ptr<CDeviceOne> parent = CDevMgr::GetMe().GetDevice(device->getParentID());
	if(parent && !parent->IsProj())
	{
		CString str = _T("父设备必须为工程设备，是否将其加入到工程中(包括父设备上级的多层父设备)？");
		if(IDYES != MessageBox(str, _T("提示"), MB_YESNO | MB_ICONINFORMATION))		return;
	}
	OnAddScan(device->getID());
	GetDocument()->FreshMap();
}

void CDeviceMapView::OnAddScan(UINT devID)
{
	std::shared_ptr<CDeviceOne> device = CDevMgr::GetMe().GetDevice(devID);
	if(!device)					return;
	if(device->IsProj())		return;
	device->setProj(true);
	CDevMgr::GetMe().SetModify();
	OnAddScan(device->getParentID());		//!< 如果有父设备，也要置成工程设备
}

//!< 选中这个设备，并显示这个设备
void MVC::Device::CDeviceMapView::ShowDeviceAt(UINT devID)
{
	CDevMgr* devMgr = &CDevMgr::GetMe();
	std::shared_ptr<CDeviceOne> culDevice = devMgr->GetDevice(devID);
	if(!culDevice)		return;
	ClearSelect();
	culDevice->SetSelect(true);
	UINT id = culDevice->getParentID();

	//!< 循环让父设备展开，直到没有父设备为止
	while(id != UINT(-1))
	{
		std::shared_ptr<CDeviceOne> parent = devMgr->GetDevice(id);
		ASSERT(parent);
		if(!parent->IsExpand()){
			parent->SetExpand(true);
			parent->ShowHideChild(true);
		}
		id = parent->getParentID();
	}
	CDeviceMapDoc* pDoc = GetDocument();
	pDoc->FreshMap();

	CPoint pt = culDevice->GetLTPt();
	ShowPoint(pt);
}

//!< 显示这个点
void MVC::Device::CDeviceMapView::ShowPoint(CPoint pt)
{
	int nZoom = SoftInfo::CSoftInfo::GetMe().getZoomSize();
	pt.x -= (CDeviceMapDoc::DEV_HOSTLINE_X + 50);
	pt.y -= (CDeviceMapDoc::DEV_HOSTLINE_Y + 50);
	if(pt.x < 0)	pt.x = 0;
	if(pt.y < 0)	pt.y = 0;
	if(nZoom >= 50 && nZoom <= 500){
		pt.x = pt.x * nZoom / 100;
		pt.y = pt.y * nZoom / 100;
	}
	//CScrollView::ScrollToDevicePosition(pt);
	CScrollView::ScrollToPosition(pt);
	//Invalidate(FALSE);
}

void MVC::Device::CDeviceMapView::OnInterfaceSet()
{
	CDevMgr::GetMe().OnSetInf();
}

void CDeviceMapView::OnDevinterfaceSet()
{
//	if(CProjectMgr::GetMe().SayWatch())		return;
	//!< 找到当前被选中的设备，然后弹出编辑它的对话框
	CDevMgr* devMgr = &CDevMgr::GetMe();
	std::shared_ptr<CDeviceOne> device = GetSelectDev();
	if(!device||CProjectMgr::GetMe().IsWatch())					//!<增加监控状态下的判断
		return;	
	if(device->getParentID() == UINT(-1)){		//!< 如果是根设备
		Dialog::CPropertyDlg* propertyDlg = &Dialog::CPropertyDlg::GetMe();
		devMgr->m_DeviceProperty.SetType(1, device->getID());
		if(IDOK == propertyDlg->DoModal(devMgr->m_DeviceProperty)){
			if(*device == *devMgr->m_DeviceProperty.m_ShowDev)	return;
			devMgr->UndoUpdDevice(device->getID());
			device->CopyFrom(*devMgr->m_DeviceProperty.m_ShowDev);
			FreshDeviceMap();
		}
	}
	else{										//!< 如果是从设备
		Dialog::CPropertyDlg* propertyDlg = &Dialog::CPropertyDlg::GetMe();
		devMgr->m_SlaveProperty.SetType(1, device->getID(), device->getParentID());
		if(IDOK == propertyDlg->DoModal(devMgr->m_SlaveProperty)){
			if(*device == *devMgr->m_DeviceProperty.m_ShowDev)	return;
			devMgr->UndoUpdDevice(device->getID());
			device->CopyFrom(*devMgr->m_SlaveProperty.m_ShowDev);
			FreshDeviceMap();
		}
	}
}

void CDeviceMapView::OnDevDelete()
{
	if(CProjectMgr::GetMe().SayWatch())		return;
	std::shared_ptr<CDeviceOne> selDev = GetSelectDev();
	if(!selDev)			return;
	CString text;
	if(selDev->m_ltChildID.size() > 0)
		text = _T("移除此设备，其从设备将一并移除，是否确认移除？");
	else
		text = _T("是否确认移除此设备？");
	if(IDYES != MessageBox(text, _T(""), MB_YESNO | MB_ICONINFORMATION))
		return;

	CDeviceMapDoc* pDoc = GetDocument();
	CDevMgr::GetMe().UndoDelDevice(selDev->getID());
	pDoc->SetUndoEnd();
	CDevMgr::GetMe().CheckAndConnect();
	FreshDeviceMap();
}

//!< 找到被选中的设备，参数表示是否要获得工程设备，默认只获得工程设备
std::shared_ptr<CDeviceOne> CDeviceMapView::GetSelectDev(bool bProj /* = true */)
{
	CDevMgr* devMgr = &CDevMgr::GetMe();
	std::shared_ptr<CDeviceOne> device, empty;
	for (auto device : devMgr->m_vtDevice){
		if(!device)					continue;
		if(bProj && !device->IsProj())	continue;
		if(device->IsSelect())		return device;
	}
	return empty;
}

//!< 获得左顶点那个设备
std::shared_ptr<CDeviceOne> CDeviceMapView::GetLTDev()
{
	std::shared_ptr<CDeviceOne> pLTDev;
	CDevMgr* devMgr = &CDevMgr::GetMe();
	for (std::shared_ptr<CDeviceOne> device : devMgr->m_vtDevice)
	{
		if(!device)								continue;
		if(device->getParentID() != UINT(-1))	continue;
		if(!pLTDev)			{pLTDev = device;	continue;}
		if(device->GetLTPt().x < pLTDev->GetLTPt().x)
			pLTDev = device;
	}
	return pLTDev;
}

//!< 选择上面的设备
void CDeviceMapView::OnSelectUp()
{
	CDevMgr* devMgr = &CDevMgr::GetMe();
	std::shared_ptr<CDeviceOne> selDev = GetSelectDev(false);
	std::shared_ptr<CDeviceOne> newSelDev;
	if(!selDev){
		return;
	}
	else
	{
		std::shared_ptr<CDeviceOne> leftDev;
		for (std::shared_ptr<CDeviceOne> device : devMgr->m_vtDevice)
		{
			if(!device)										continue;		//!< 不能为空
			if(device == selDev)							continue;		//!< 不能是自己
			if(!device->IsShow())							continue;		//!< 不画的不找
			if(device->GetLTPt().x != selDev->GetLTPt().x)	continue;		//!< 横坐标必须相同
			if(device->GetLTPt().y >= selDev->GetLTPt().y)	continue;		//!< 纵坐标必须小于当前选择的设备
			if(!newSelDev)			{newSelDev = device;	continue;}
			if(device->GetLTPt().y > newSelDev->GetLTPt().y)				//!< 取满足条件的纵坐标最大的设备
				newSelDev = device;
		}
	}
	if(!newSelDev)		return;
	ClearSelect();
	newSelDev->SetSelect(true);
	ShowDeviceAt(newSelDev->getID());
	//Invalidate(FALSE);
}

//!< 选择下面的设备
void CDeviceMapView::OnSelectDown()
{
	CDevMgr* devMgr = &CDevMgr::GetMe();
	std::shared_ptr<CDeviceOne> selDev = GetSelectDev(false);
	std::shared_ptr<CDeviceOne> newSelDev;
	if(!selDev){
		newSelDev = GetLTDev();
	}
	else
	{
		std::shared_ptr<CDeviceOne> leftDev;
		for (std::shared_ptr<CDeviceOne> device : devMgr->m_vtDevice)
		{
			if(!device)										continue;		//!< 不能为空
			if(device == selDev)							continue;		//!< 不能是自己
			if(!device->IsShow())							continue;		//!< 不画的不找
			if(device->GetLTPt().x != selDev->GetLTPt().x)	continue;		//!< 横坐标必须相同
			if(device->GetLTPt().y <= selDev->GetLTPt().y)	continue;		//!< 纵坐标必须大于当前选择的设备
			if(!newSelDev)			{newSelDev = device;	continue;}
			if(device->GetLTPt().y < newSelDev->GetLTPt().y)				//!< 取满足条件的纵坐标最小的设备
				newSelDev = device;
		}
	}
	if(!newSelDev)		return;
	ClearSelect();
	newSelDev->SetSelect(true);
	ShowDeviceAt(newSelDev->getID());
	//Invalidate(FALSE);
}

//!< 选择左面的设备
void CDeviceMapView::OnSelectLeft()
{
	CDevMgr* devMgr = &CDevMgr::GetMe();
	std::shared_ptr<CDeviceOne> selDev = GetSelectDev(false);
	std::shared_ptr<CDeviceOne> newSelDev;
	if(!selDev){
		return;
	}
	else
	{
		std::shared_ptr<CDeviceOne> leftDev;
		for (std::shared_ptr<CDeviceOne> device : devMgr->m_vtDevice)
		{
			if(!device)										continue;		//!< 不能为空
			if(device == selDev)							continue;		//!< 不能是自己
			if(!device->IsShow())							continue;		//!< 不画的不找
			if(device->GetLTPt().y != selDev->GetLTPt().y)	continue;		//!< 纵坐标必须相同
			if(device->GetLTPt().x >= selDev->GetLTPt().x)	continue;		//!< 横坐标必须小于当前选择的设备
			if(!newSelDev)			{newSelDev = device;	continue;}
			if(device->GetLTPt().x > newSelDev->GetLTPt().x)				//!< 取满足条件的横坐标最大的设备
				newSelDev = device;
		}
	}
	if(!newSelDev)		return;
	ClearSelect();
	newSelDev->SetSelect(true);
	ShowDeviceAt(newSelDev->getID());
	//Invalidate(FALSE);
}

//!< 选择右面的设备
void CDeviceMapView::OnSelectRight()
{
	CDevMgr* devMgr = &CDevMgr::GetMe();
	std::shared_ptr<CDeviceOne> selDev = GetSelectDev(false);
	std::shared_ptr<CDeviceOne> newSelDev;
	if(!selDev){
		newSelDev = GetLTDev();
	}
	else
	{
		std::shared_ptr<CDeviceOne> leftDev;
		for (std::shared_ptr<CDeviceOne> device : devMgr->m_vtDevice)
		{
			if(!device)										continue;		//!< 不能为空
			if(device == selDev)							continue;		//!< 不能是自己
			if(!device->IsShow())							continue;		//!< 不画的不找
			if(device->GetLTPt().y != selDev->GetLTPt().y)	continue;		//!< 纵坐标必须相同
			if(device->GetLTPt().x <= selDev->GetLTPt().x)	continue;		//!< 横坐标必须大于当前选择的设备
			if(!newSelDev)			{newSelDev = device;	continue;}
			if(device->GetLTPt().x < newSelDev->GetLTPt().x)				//!< 取满足条件的横坐标最小的设备
				newSelDev = device;
		}
	}
	if(!newSelDev)		return;
	ClearSelect();
	newSelDev->SetSelect(true);
	ShowDeviceAt(newSelDev->getID());
	//Invalidate(FALSE);
}

void CDeviceMapView::OnDevParainfo()
{
	std::shared_ptr<CDeviceOne> device = GetSelectDev(false);
	if(!device)		return;
	device->LoadXml();
	CDevMgr* devMgr = &CDevMgr::GetMe();
	Dialog::CBehaviorParamDlg dlg;
	dlg.DoModal(device->getID());
}

//!< 变量表导出
void CDeviceMapView::OnDevOut()
{
	std::shared_ptr<CDeviceOne> device = GetSelectDev();
	if(!device)		return;

	CString extendName = _T("*.dcfg");
	CString extendNames = _T("设备配置文件 (") + extendName + _T(")|") + extendName + _T("||");
	CFileDialog openDlg(FALSE, (LPCTSTR)extendName, _T(""), OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, (LPCTSTR)extendNames);
	if(IDOK == openDlg.DoModal()){
		CString path = CGbl::GetPathFromFilePathName(openDlg.GetPathName());
		CString title = openDlg.GetFileTitle();
		device->DevOut(path, title);
	}
}

//!< 变量表导入
void CDeviceMapView::OnDevIn()
{
	if(CProjectMgr::GetMe().SayWatch())		return;
	std::shared_ptr<CDeviceOne> device = GetSelectDev();
	if(!device)		return;

	CString extendName = _T("*.dcfg");
	CString extendNames = _T("设备配置文件 (") + extendName + _T(")|") + extendName + _T("||");
	CFileDialog openDlg(TRUE, (LPCTSTR)extendName, _T(""), OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, (LPCTSTR)extendNames);
	if(IDOK == openDlg.DoModal())
		device->DevIn(openDlg.GetPathName(), openDlg.GetFileTitle());
}

DROPEFFECT MVC::Device::CDeviceMapView::OnDragOver(COleDataObject* pDataObject, DWORD dwKeyState, CPoint point)
{
	if(CProjectMgr::GetMe().IsWatch())		return DROPEFFECT_NONE;
	CXTPTaskPanelGroupItem* pItemDrop = (CXTPTaskPanelGroupItem*)CXTPTaskPanelItem::CreateFromOleData(pDataObject);
	if (!pItemDrop){
		m_bDropNewIn = false;
		Invalidate(FALSE);
		return DROPEFFECT_NONE;
	}
	else{
		ASSERT_KINDOF(CXTPTaskPanelGroupItem, pItemDrop);
		m_bDropNewIn = true;
		m_uiDropDevType = pItemDrop->GetID() - 100;
#ifdef _DEBUG
		delete pItemDrop;
#endif
		//!< 看看哪个能被选中
		CClientDC dc(this);
		OnPrepareDC(&dc);
		dc.DPtoLP(&point);					//!< 小坐标转打坐标

		ClearSelect();
		std::shared_ptr<CDeviceOne> pDevice, selDev;
		CDevMgr* devMgr = &CDevMgr::GetMe();
		for (auto pDevice : devMgr->m_vtDevice){
			if(!pDevice)	continue;
			if(pDevice->IsInMyRect(point) > 0){
				pDevice->SetSelect(true);
				selDev = pDevice;
			}
		}
		Invalidate(FALSE);
		if(selDev && !selDev->IsProj())		//!< 不能添加到扫描设备上
			return DROPEFFECT_NONE;
		return DROPEFFECT_COPY;
	}
}

BOOL MVC::Device::CDeviceMapView::OnDrop(COleDataObject* pDataObject, DROPEFFECT dropEffect, CPoint point)
{
	m_bDropNewIn = false;
	Invalidate(FALSE);

	CXTPTaskPanelGroupItem* pItemDrop = (CXTPTaskPanelGroupItem*)CXTPTaskPanelItem::CreateFromOleData(pDataObject);
	if (pItemDrop){
		ASSERT_KINDOF(CXTPTaskPanelGroupItem, pItemDrop);
		std::shared_ptr<CDeviceOne> device = GetSelectDev(false);
		if(!device)					OnAddMainDev(pItemDrop->GetID() - 100);
		else if(device->IsProj())	OnAddSlave(pItemDrop->GetID() - 100);
#ifdef _DEBUG
		delete pItemDrop;
#endif
	}

	return CScrollView::OnDrop(pDataObject, dropEffect, point);
}

DROPEFFECT MVC::Device::CDeviceMapView::OnDragEnter(COleDataObject* pDataObject, DWORD dwKeyState, CPoint point)
{
	if(CProjectMgr::GetMe().IsWatch())		return DROPEFFECT_NONE;
	CXTPTaskPanelGroupItem* pItemDrop = (CXTPTaskPanelGroupItem*)CXTPTaskPanelItem::CreateFromOleData(pDataObject);
	if (!pItemDrop)
	{
		return DROPEFFECT_NONE;
	}
	else
	{
		ASSERT_KINDOF(CXTPTaskPanelGroupItem, pItemDrop);
#ifdef _DEBUG
		delete pItemDrop;
#endif
		return DROPEFFECT_COPY;
	}
	return CScrollView::OnDragEnter(pDataObject, dwKeyState, point);
}

void MVC::Device::CDeviceMapView::OnDragLeave()
{
	m_bDropNewIn = false;
	CScrollView::OnDragLeave();
}

BOOL MVC::Device::CDeviceMapView::PreTranslateMessage(MSG* pMsg)
{
	if(pMsg->message == WM_MOUSEMOVE){
		if(!OnMouseMove(0, pMsg->pt))		m_ToolTip.RemoveTool(this);
		else if(SoftInfo::CSoftInfo::GetMe().getTooltip())	m_ToolTip.RelayEvent(pMsg);
		return TRUE;
	}
	else if(pMsg->message == WM_RBUTTONUP || pMsg->message == WM_KEYDOWN){
		m_ToolTip.RelayEvent(pMsg);
	}
	return CScrollView::PreTranslateMessage(pMsg);
}

//!< wParam表示按下的键盘键（字符串），lParam表示tooltip的ID
LRESULT MVC::Device::CDeviceMapView::OnHyperlinkClicked(WPARAM wParam, LPARAM lParam)
{
	if (wParam)
	{
		CString msg = (LPCTSTR)wParam;
		if(msg == _T("主设备"))
			SoftInfo::CMyHelp::GetMe().ShowHelp(_T("主设备"));
		else
			SoftInfo::CMyHelp::GetMe().ShowHelp(_T("从设备"));
	}
	else
		SoftInfo::CMyHelp::GetMe().ShowHelp(_T("设备拓扑"));
	return TRUE;
}

void MVC::Device::CDeviceMapView::OnUpdateAddMainDev(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(!CProjectMgr::GetMe().IsWatch());
}

void MVC::Device::CDeviceMapView::OnUpdateAddSlave(CCmdUI *pCmdUI)
{
	if(CProjectMgr::GetMe().IsWatch())
		pCmdUI->Enable(FALSE);
	else
	{
		std::shared_ptr<CDeviceOne> dev = GetSelectDev();
		if(!dev || !dev->CanAddSlave(false))
			pCmdUI->Enable(FALSE);
	}
}

void MVC::Device::CDeviceMapView::OnUpdateDevInfo(CCmdUI *pCmdUI)
{
	if(CProjectMgr::GetMe().IsWatch() || !GetSelectDev())
		pCmdUI->Enable(FALSE);
}

void MVC::Device::CDeviceMapView::OnUpdateInterfaceSet(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(!CProjectMgr::GetMe().IsWatch());
}

void MVC::Device::CDeviceMapView::OnUpdateDelete(CCmdUI *pCmdUI)
{
	if(CProjectMgr::GetMe().IsWatch() || !GetSelectDev())
		pCmdUI->Enable(FALSE);
}

void MVC::Device::CDeviceMapView::OnUpdateParainfo(CCmdUI *pCmdUI)
{
	//pCmdUI->Enable(!CProjectMgr::GetMe().IsWatch());
	//pCmdUI->Enable(FALSE);
}

void MVC::Device::CDeviceMapView::OnUpdateDevOut(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(FALSE);
}

void MVC::Device::CDeviceMapView::OnUpdateDevIn(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(FALSE);
}

void MVC::Device::CDeviceMapView::OnUpdateEditCopy(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(!!GetSelectDev());
}

void MVC::Device::CDeviceMapView::OnUpdateEditCopyWithChild(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(!!GetSelectDev());
}

void MVC::Device::CDeviceMapView::OnUpdateEditCut(CCmdUI *pCmdUI)
{
	if(CProjectMgr::GetMe().IsWatch() || !GetSelectDev())
		pCmdUI->Enable(FALSE);
}

void MVC::Device::CDeviceMapView::OnUpdateEditPaste(CCmdUI *pCmdUI)
{
	if(CProjectMgr::GetMe().IsWatch() || CDevMgr::GetMe().m_ltDevClipBoard.empty())
	pCmdUI->Enable(FALSE);
}

void MVC::Device::CDeviceMapView::OnZoom(UINT index)
{
	if(index == ID_ZOOM80)			SoftInfo::CSoftInfo::GetMe().setZoomSize(80);
	else if(index == ID_ZOOM100)	SoftInfo::CSoftInfo::GetMe().setZoomSize(100);
	else if(index == ID_ZOOM150)	SoftInfo::CSoftInfo::GetMe().setZoomSize(150);
	else if(index == ID_ZOOM200)	SoftInfo::CSoftInfo::GetMe().setZoomSize(200);
	FreshDeviceMap();
}

void MVC::Device::CDeviceMapView::OnEditCopy()
{
	std::shared_ptr<CDeviceOne> selDev = GetSelectDev();
	if(!selDev)		return;
	CDevMgr::GetMe().OnDevCopy(selDev->getID());
}

void MVC::Device::CDeviceMapView::OnEditCopyWithChild()
{
	std::shared_ptr<CDeviceOne> selDev = GetSelectDev();
	if(!selDev)		return;
	CDevMgr::GetMe().OnDevCopyWithChild(selDev->getID());
}

void MVC::Device::CDeviceMapView::OnEditCut()
{
	if(CProjectMgr::GetMe().SayWatch())	return;
	std::shared_ptr<CDeviceOne> selDev = GetSelectDev();
	CDeviceMapDoc* pDoc = GetDocument();
	if(!selDev)		return;
	CDevMgr::GetMe().OnDevCut(pDoc, selDev->getID());
	FreshDeviceMap();
}

void MVC::Device::CDeviceMapView::OnEditPaste()
{
	if(CProjectMgr::GetMe().SayWatch())	return;
	std::shared_ptr<CDeviceOne> selDev = GetSelectDev();
	CDeviceMapDoc* pDoc = GetDocument();
	if(!selDev)
		CDevMgr::GetMe().OnDevPaste(pDoc, UINT(-1));
	else
		CDevMgr::GetMe().OnDevPaste(pDoc, selDev->getID());
	FreshDeviceMap();
}

void MVC::Device::CDeviceMapView::OnEditUndo()
{
	if(CProjectMgr::GetMe().SayWatch())	return;
	GetDocument()->OnUndo();
}

void MVC::Device::CDeviceMapView::OnEditRedo()
{
	if(CProjectMgr::GetMe().SayWatch())	return;
	GetDocument()->OnRedo();
}

void MVC::Device::CDeviceMapView::OnUpdateEditUndo(CCmdUI *pCmdUI)
{
	if(CProjectMgr::GetMe().IsWatch() || GetDocument()->IsUndoEmpty())
	pCmdUI->Enable(FALSE);
}

void MVC::Device::CDeviceMapView::OnUpdateEditRedo(CCmdUI *pCmdUI)
{
	if(CProjectMgr::GetMe().IsWatch() || GetDocument()->IsRedoEmpty())
	pCmdUI->Enable(FALSE);
}

BOOL MVC::Device::CDeviceMapView::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	if(nFlags & MK_CONTROL){
		UINT size = SoftInfo::CSoftInfo::GetMe().getZoomSize();
		if(zDelta > 0)	SoftInfo::CSoftInfo::GetMe().setZoomSize(size + 20);
		else			SoftInfo::CSoftInfo::GetMe().setZoomSize(size - 20);
		if(size != SoftInfo::CSoftInfo::GetMe().getZoomSize())
			FreshDeviceMap();
	}
	return CScrollView::OnMouseWheel(nFlags, zDelta, pt);
}

void CDeviceMapView::OnScanSet()
{
	MVC::Device::CDevMgr::GetMe().SetScanInfo();
}

void CDeviceMapView::OnScanStart()
{
	((CMainFrame *)g_App.GetMainWnd())->OnScanStart();
}

void CDeviceMapView::OnScanStop()
{
	((CMainFrame *)g_App.GetMainWnd())->OnScanStop();
}

void MVC::Device::CDeviceMapView::OnFreshState()
{
	if(!CProjectMgr::GetMe().IsWatch())		return;
	std::shared_ptr<CDeviceOne> dev = GetSelectDev();
	if(dev)
	{
		if(dev->GetOnLineInf() != -1)		return;
		Servers::DXP::CServerCtrl::GetMe().SendFreshDev((int)dev->getID());
	}
	else
	{
		Servers::DXP::CServerCtrl::GetMe().SendFreshDev(-1);
	}
}

BOOL MVC::Device::CDeviceMapView::OnHelpInfo(HELPINFO* pHelpInfo)
{
	SoftInfo::CMyHelp* pHelp = &SoftInfo::CMyHelp::GetMe();
	std::shared_ptr<CDeviceOne> device = GetSelectDev(false);
	if(!device)							pHelp->ShowHelp(_T("设备拓扑"));
	else if(!device->IsProj())			pHelp->ShowHelp(_T("将扫描设备加载到工程中"));
	else if(device->getLevel() == 0)	pHelp->ShowHelp(_T("主设备"));
	else								pHelp->ShowHelp(_T("从设备"));
	return CScrollView::OnHelpInfo(pHelpInfo);
}

void MVC::Device::CDeviceMapView::OnProgram()
{
	std::shared_ptr<CDeviceOne> device = GetSelectDev();
	if (!device)									return;
	if (!device->GetXmlInfo())						return;
	if (!device->GetXmlInfo()->IsProgrammable())	return;
	device->OnProgram();
}

void MVC::Device::CDeviceMapView::OnUpdateProgram(CCmdUI *pCmdUI)
{
	std::shared_ptr<CDeviceOne> device = GetSelectDev();
	if(!device)
		pCmdUI->Enable(FALSE);
	else if (!device->GetXmlInfo() || !device->GetXmlInfo()->IsProgrammable())
		pCmdUI->Enable(FALSE);
	else
		pCmdUI->Enable(TRUE);
}
