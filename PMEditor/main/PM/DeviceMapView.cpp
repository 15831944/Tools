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
#include "XmlMgr.h"
#include "XmlDevice.h"
#include "DevMgr.h"
#include "DevCompanyDlg.h"
#include "BehaviorParamDlg.h"
#include "DMemDC.h"
#include "DBMgr.h"

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
	ON_COMMAND(ID_EDIT_UNDO, &CDeviceMapView::OnEditUndo)
	ON_COMMAND(ID_EDIT_REDO, &CDeviceMapView::OnEditRedo)
	ON_COMMAND_RANGE(ID_ZOOM80, ID_ZOOM200, &CDeviceMapView::OnZoom)
	ON_COMMAND(ID_H, &CDeviceMapView::OnH)
	ON_COMMAND(ID_V, &CDeviceMapView::OnV)
	ON_COMMAND(ID_PROGRAM, &CDeviceMapView::OnProgram)
	ON_UPDATE_COMMAND_UI(ID_EDIT_UNDO, &CDeviceMapView::OnUpdateEditUndo)
	ON_UPDATE_COMMAND_UI(ID_EDIT_REDO, &CDeviceMapView::OnUpdateEditRedo)
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
	m_bHostLineSelect = IsInHostLineRect(point);	//!< 选中母线

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

	CScrollView::OnMouseMove(nFlags, point);
	return FALSE;
}

void MVC::Device::CDeviceMapView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	switch(nChar)
	{
	//case VK_DELETE:		OnDevDelete();			break;
	//case VK_UP:			OnSelectUp();			break;
	//case VK_DOWN:		OnSelectDown();			break;
	//case VK_LEFT:		OnSelectLeft();			break;
	//case VK_RIGHT:		OnSelectRight();		break;
	//case VK_ESCAPE:		ClearSelect();			Invalidate(FALSE);			break;
	case VK_ADD:		OnMouseWheel(MK_CONTROL, 20, CPoint(0, 0));			break;
	case VK_SUBTRACT:	OnMouseWheel(MK_CONTROL, -20, CPoint(0, 0));		break;
	}
	CScrollView::OnKeyDown(nChar, nRepCnt, nFlags);
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
		else if(m_strInWord.MakeUpper() == _T("FIND")){
			((CMainFrame *)g_App.GetMainWnd())->OnFind();
		}
		else if(m_strInWord.MakeUpper() == _T("HELP")){
			((CMainFrame *)g_App.GetMainWnd())->OnHelpShow();
		}
		else if(m_strInWord.MakeUpper() == _T("DB")){
			Servers::DB::CDBMgr::GetMe().OnSetDB();
		}
#ifdef _DEBUG
		else if(m_strInWord.MakeUpper() == _T("STARTPAGE")){
			g_App.ShowStartPage();
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
		m_strInWord = "";
	}
	CScrollView::OnChar(nChar, nRepCnt, nFlags);
}

void CDeviceMapView::OnH()
{
}

void CDeviceMapView::OnV()
{
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

		Invalidate(FALSE);
//		return DROPEFFECT_NONE;
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

void MVC::Device::CDeviceMapView::OnZoom(UINT index)
{
	if(index == ID_ZOOM80)			SoftInfo::CSoftInfo::GetMe().setZoomSize(80);
	else if(index == ID_ZOOM100)	SoftInfo::CSoftInfo::GetMe().setZoomSize(100);
	else if(index == ID_ZOOM150)	SoftInfo::CSoftInfo::GetMe().setZoomSize(150);
	else if(index == ID_ZOOM200)	SoftInfo::CSoftInfo::GetMe().setZoomSize(200);
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
	}
	return CScrollView::OnMouseWheel(nFlags, zDelta, pt);
}


BOOL MVC::Device::CDeviceMapView::OnHelpInfo(HELPINFO* pHelpInfo)
{
	SoftInfo::CMyHelp* pHelp = &SoftInfo::CMyHelp::GetMe();
	return CScrollView::OnHelpInfo(pHelpInfo);
}

void MVC::Device::CDeviceMapView::OnProgram()
{
}

void MVC::Device::CDeviceMapView::OnUpdateProgram(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(TRUE);
}
