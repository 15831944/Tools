// StartPageFrame.cpp : 实现文件
//

#include "stdafx.h"
#include "PMApp.h"
#include "Gbl.h"
#include "StartFrm.h"


// CStartFrm
using namespace MVC;
using namespace Start;
IMPLEMENT_DYNCREATE(CStartFrm, CMDIChildWnd)

CStartFrm::CStartFrm()
{
}

CStartFrm::~CStartFrm()
{
}


BEGIN_MESSAGE_MAP(CStartFrm, CMDIChildWnd)
	ON_WM_MDIACTIVATE()
	ON_WM_CLOSE()
END_MESSAGE_MAP()


// CStartFrm 消息处理程序

BOOL CStartFrm::CreateEx(DWORD dwExStyle, LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, LPVOID lpParam)
{
	return CMDIChildWnd::CreateEx(dwExStyle, lpszClassName, lpszWindowName, dwStyle, rect, pParentWnd, nID, lpParam);
}


void CStartFrm::OnClose()
{
	CMDIChildWnd::OnClose();
}

BOOL CStartFrm::LoadFrame(UINT nIDResource, DWORD dwDefaultStyle , CWnd* pParentWnd , CCreateContext* pContext)
{
	// TODO: 在此添加专用代码和/或调用基类

	CMDIChildWnd::LoadFrame(nIDResource, dwDefaultStyle, pParentWnd, pContext);

	CImageList img;
	if(!m_wndReBar.Create(this))
		return -1;

	if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP
		| CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC))
		return -1;      

	//设置按钮宽度和长度
	m_wndToolBar.GetToolBarCtrl().SetButtonWidth(20, 20);
	//设置 热 位图
	img.Create(20, 20, ILC_COLOR8|ILC_MASK,2,2);
	img.SetBkColor(::GetSysColor(COLOR_BTNFACE));
	img.Add(AfxGetApp()->LoadIcon(IDI_PAGEHOME));//0收
	img.Add(AfxGetApp()->LoadIcon(IDI_PAGEBACK));//0收
	img.Add(AfxGetApp()->LoadIcon(IDI_PAGEFRONT));//0收
	m_wndToolBar.GetToolBarCtrl().SetHotImageList(&img);
	img.Detach();	

	//设置 冷 位图
	img.Create(20, 20, ILC_COLOR8|ILC_MASK, 2,2);
	img.SetBkColor(::GetSysColor(COLOR_BTNFACE));
	img.Add(AfxGetApp()->LoadIcon(IDI_PAGEHOME));//0收
	img.Add(AfxGetApp()->LoadIcon(IDI_PAGEBACK));//0收
	img.Add(AfxGetApp()->LoadIcon(IDI_PAGEFRONT));//0收
	m_wndToolBar.GetToolBarCtrl().SetImageList(&img);
	img.Detach();

	//改变属性
	m_wndToolBar.ModifyStyle(0, TBSTYLE_FLAT |CBRS_TOOLTIPS | TBSTYLE_TRANSPARENT|TBBS_CHECKBOX );
	m_wndToolBar.SetButtons(NULL, 3);
	m_wndToolBar.SetButtonInfo(0, ID_TB_PAGEHOME, TBSTYLE_BUTTON, 0);
	m_wndToolBar.SetButtonInfo(1, ID_TB_PAGEBACK, TBSTYLE_BUTTON, 1);
	m_wndToolBar.SetButtonInfo(2, ID_TB_PAGEFRONT, TBSTYLE_BUTTON, 2);

	CRect rectToolBar;
	m_wndToolBar.GetItemRect(0, &rectToolBar);			//得到按钮大小
	m_wndToolBar.SetSizes(CSize(20,20), CSize(20,20));	//设置按钮大小
	m_wndReBar.AddBar(&m_wndToolBar);					//在Rebar中加入ToolBar

	REBARBANDINFO rbbi;				//改变一些属性
	rbbi.cbSize = sizeof(rbbi);		//必须填
	rbbi.fMask = RBBIM_CHILDSIZE | RBBIM_IDEALSIZE | RBBIM_SIZE|RBBIM_BACKGROUND;
	rbbi.cxMinChild = rectToolBar.Width();	//工具条的宽度
	rbbi.cyMinChild = rectToolBar.Height();	//高度

	//加背景位图，注意rbbi.fMask中RBBIM_BACKGROUND标志
//	rbbi.hbmBack = LoadBitmap(::AfxGetInstanceHandle(), MAKEINTRESOURCE(IDB_COLOR_TOOLBAR));//IDB_MENUBITMAP2));
	rbbi.cx = rbbi.cxIdeal = rectToolBar.Width() * 10;
	m_wndReBar.GetReBarCtrl().SetBandInfo(0, &rbbi);

	return TRUE;
}

void CStartFrm::ActivateFrame(int nCmdShow)
{
	CMDIChildWnd::ActivateFrame(nCmdShow);
}

void CStartFrm::OnMDIActivate(BOOL bActivate, CWnd* pActivateWnd, CWnd* pDeactivateWnd)
{
	CDocument *pDoc = GetActiveDocument();
	if(pDoc)		pDoc->UpdateAllViews(NULL);
	CMDIChildWnd::OnMDIActivate(bActivate, pActivateWnd, pDeactivateWnd);
}

