// MainFrm.cpp : CMainFrame 类的实现
//
#include "stdafx.h"
#include "PMApp.h"
#include "Gbl.h"
#include "SoftInfo.h"
#include "MyHelp.h"
#include "MainFrm.h"
#include "ProjectMgr.h"
#include "ProjectMgr.h"
#include "Project.h"
#include "SoftSetDlg.h"
#include "FindDlg.h"

#include "ItemMgr.h"
#include "ItemDoc.h"
#include "ItemView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

const CString TITLE = _T("PM编辑器");
const int TIME_LIMIT = 110;
int m_nLimitTime = 60 * 120;// / 720;

// CMainFrame
IMPLEMENT_DYNAMIC(CMainFrame, CMDIFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CMDIFrameWnd)
	ON_WM_CREATE()
	ON_WM_CLOSE()
	ON_WM_COPYDATA()
	ON_WM_TIMER()
	ON_WM_KEYDOWN()
	ON_MESSAGE(XTPWM_DOCKINGPANE_NOTIFY, OnDockingPaneNotify)
	ON_MESSAGE(WM_XTP_PRETRANSLATEMOUSEMSG, OnTabbarMouseMsg)
	ON_COMMAND(XTP_ID_CUSTOMIZE, OnCustomize)
	ON_COMMAND(ID_PROJ_NEW, &CMainFrame::OnProjNew)
	ON_COMMAND(ID_PROJ_OPEN, &CMainFrame::OnProjOpen)
	ON_COMMAND(ID_PROJ_SAVE, &CMainFrame::OnProjSave)
	ON_COMMAND(ID_PROJ_BACKUP, &CMainFrame::OnProjBackUp)
	ON_COMMAND(ID_PROJ_CLOSE, &CMainFrame::OnProjClose)
	ON_COMMAND(ID_ADD_ITEM, &CMainFrame::OnAddItem)
	ON_COMMAND(ID_RENAME, &CMainFrame::OnRename)
	ON_COMMAND(ID_PROJ_INFO, &CMainFrame::OnProjInfo)
	ON_COMMAND(ID_SOFT_SET, &CMainFrame::OnSoftSet)
	ON_COMMAND(ID_FIND, &CMainFrame::OnFind)
	ON_COMMAND(ID_VIEW_PROJECT, &CMainFrame::OnShowViewProject)
	ON_COMMAND(ID_VIEW_ITEM, &CMainFrame::OnShowViewItem)
	ON_COMMAND(ID_VIEW_OUTPUT, &CMainFrame::OnShowViewOutput)
	ON_COMMAND(ID_HELP_FILE, &CMainFrame::OnHelpShow)
	ON_UPDATE_COMMAND_UI(ID_PROJ_CLOSE, &CMainFrame::OnUpdateWithProj)
	ON_UPDATE_COMMAND_UI(ID_PROJ_SAVE, &CMainFrame::OnUpdateWithProj)
	ON_UPDATE_COMMAND_UI(ID_PROJ_BACKUP, &CMainFrame::OnUpdateWithProj)
	ON_UPDATE_COMMAND_UI(ID_RENAME, &CMainFrame::OnUpdateWithProj)
	ON_UPDATE_COMMAND_UI(ID_ADD_ITEM, &CMainFrame::OnUpdateWithProj)
	ON_UPDATE_COMMAND_UI(ID_COMPILE_CHECK, &CMainFrame::OnUpdateWithProj)
	ON_UPDATE_COMMAND_UI(ID_PROJ_INFO, &CMainFrame::OnUpdateWithProj)
	ON_UPDATE_COMMAND_UI(ID_FIND, &CMainFrame::OnUpdateWithProj)
	ON_COMMAND(ID_STARTPAGE, &CMainFrame::OnStartpage)
END_MESSAGE_MAP()

BEGIN_EVENTSINK_MAP(CMainFrame, CMDIFrameWnd)
END_EVENTSINK_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // 状态行指示器
};

static UINT uHideCmds[] =   // 隐藏的菜单
{
	ID_FILE_CLOSE,
//	ID_FILE_SAVE,
	ID_FILE_SAVE_AS,
	ID_FILE_PRINT,
	ID_FILE_PRINT_PREVIEW,
	ID_FILE_PRINT_SETUP,
	ID_EDIT_CUT,
	ID_EDIT_COPY,
	ID_EDIT_PASTE,
	ID_EDIT_FIND
};

// CMainFrame 构造/析构
CMainFrame::CMainFrame()
:m_bLimitTimer(false)							// 是否限时定时器开启
{
}

CMainFrame::~CMainFrame()
{
}

// 设置窗口样式
void CMainFrame::SetTheme(int index)
{
	switch(index)
	{
	case 0:		XTPPaintManager()->SetTheme(xtpThemeOffice2000);		break;
	case 1:		XTPPaintManager()->SetTheme(xtpThemeOfficeXP);			break;
	case 2:		XTPPaintManager()->SetTheme(xtpThemeOffice2003);		break;
	case 3:		XTPPaintManager()->SetTheme(xtpThemeNativeWinXP);		break;
	case 4:		XTPPaintManager()->SetTheme(xtpThemeWhidbey);			break;
	case 5:		XTPPaintManager()->SetTheme(xtpThemeResource);			break;	// Office 2007 
	case 6:		XTPPaintManager()->SetTheme(xtpThemeRibbon);			break;
	case 7:		XTPPaintManager()->SetTheme(xtpThemeVisualStudio2008);	break;
	case 8:		XTPPaintManager()->SetTheme(xtpThemeVisualStudio6);		break;
	case 9:		XTPPaintManager()->SetTheme(xtpThemeVisualStudio2010);	break;
	case 10:	XTPPaintManager()->SetTheme(xtpThemeCustom);			break;
	}
}

// 设置Pane样式
void CMainFrame::SetPaneTheme(int index)
{
	switch(index)
	{
	case 0:		m_paneManager.SetTheme(xtpPaneThemeDefault);				break;
	case 1:		m_paneManager.SetTheme(xtpPaneThemeVisualStudio2003);		break;
	case 2:		m_paneManager.SetTheme(xtpPaneThemeOffice2003);				break;
	case 3:		m_paneManager.SetTheme(xtpPaneThemeWinNative);				break;
	case 4:		m_paneManager.SetTheme(xtpPaneThemeGrippered);				break;
	case 5:		m_paneManager.SetTheme(xtpPaneThemeOffice2002Visio);		break;
	case 6:		m_paneManager.SetTheme(xtpPaneThemeVisualStudio2005Beta1);	break;
	case 7:		m_paneManager.SetTheme(xtpPaneThemeOffice2003Outlook);		break;
	case 8:		m_paneManager.SetTheme(xtpPaneThemeWinExplorer);			break;
	case 9:		m_paneManager.SetTheme(xtpPaneThemeVisualStudio2005);		break;
	case 10:	m_paneManager.SetTheme(xtpPaneThemeResource);				break;
	case 11:	m_paneManager.SetTheme(xtpPaneThemeOffice2007Word);			break;
	case 12:	m_paneManager.SetTheme(xtpPaneThemeOffice2007Outlook);		break;
	case 13:	m_paneManager.SetTheme(xtpPaneThemeVisualStudio6);			break;
	case 14:	m_paneManager.SetTheme(xtpPaneThemeVisualStudio2005Beta2);	break;
	case 15:	m_paneManager.SetTheme(xtpPaneThemeVisualStudio2008);		break;
	case 16:	m_paneManager.SetTheme(xtpPaneThemeVisualStudio2010);		break;
	case 17:	m_paneManager.SetTheme(xtpPaneThemeOffice2003Visio);		break;
	case 18:	m_paneManager.SetTheme(xtpPaneThemeOffice2007Visio);		break;
	}
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	// 初始化 OLE 库
	CoInitialize(NULL);
	if (!AfxOleInit()){AfxMessageBox(IDP_OLE_INIT_FAILED);		return FALSE;}
	if (CMDIFrameWnd::OnCreate(lpCreateStruct) == -1)	return -1;
	if (!CreateToolBar())								return -1;

	// 添加状态栏
	if (!m_wndStatusBar.Create(this) || !m_wndStatusBar.SetIndicators(indicators, sizeof(indicators)/sizeof(UINT)))		return -1;

	m_wndStatusBar.GetPane(0)->SetBeginGroup(FALSE);
	FillToStatusBar();
	SetTheme(xtpThemeVisualStudio2010);						// 设置窗口的风格
	//SetTheme(xtpThemeVisualStudio6);
	m_paneManager.InstallDockingPanes(this);				// Add pane
	SetPaneTheme(xtpPaneThemeVisualStudio2010);				// 设置每个pane的风格

	CXTPDockingPane* paneOutputView = m_paneManager.CreatePane(ID_VIEW_OUTPUT, CRect(0, 0, 150, 200), xtpPaneDockBottom);
	CXTPDockingPane* paneProjView = m_paneManager.CreatePane(ID_VIEW_PROJECT, CRect(0, 0, 230, 140), xtpPaneDockLeft);
	CXTPDockingPane* paneItemView = m_paneManager.CreatePane(ID_VIEW_ITEM, CRect(0, 0, 210, 140), xtpPaneDockLeft);

	paneOutputView->SetTitle("输出栏\n输出栏");
	paneProjView->SetTitle("工程视图\n工程视图");
	paneItemView->SetTitle("变量视图\n变量视图");

	m_paneManager.AttachPane(paneItemView, paneProjView);	// 将两个pane合并在一起
	paneProjView->Select();
	m_paneManager.EnableKeyboardNavigate();
	m_paneManager.SetAlphaDockingContext(TRUE);
	//Call this member to specify whether an alpha docking context is used to indicate where a pane can be docked while dragging the pane.
	m_paneManager.SetShowDockingContextStickers(TRUE);
	//Call this member to specify whether Visual Studio 2005 style Docking Context Stickers are used while dragging a docking pane.
	m_paneManager.SetShowContentsWhileDragging(TRUE);
	m_paneManager.SetDefaultPaneOptions(xtpPaneHasMenuButton);
	//Call this method to set default options for each pane
	VERIFY(m_MTIClientWnd.Attach(this, TRUE));
	m_MTIClientWnd.EnableToolTips();
	m_MTIClientWnd.GetPaintManager()->m_bShowIcons = FALSE;
	m_MTIClientWnd.SetNewTabPosition(xtpWorkspaceNewTabLeftMost);
	m_MTIClientWnd.SetFlags(xtpWorkspaceHideArrowsAlways | xtpWorkspaceShowActiveFiles);

	EnableAutomation();
	UpdateData(TRUE);

	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	SetIcon(m_hIcon, TRUE);
	SetTitle(TITLE);

	return 0;
}

LRESULT CMainFrame::OnDockingPaneNotify(WPARAM wParam, LPARAM lParam)
{
	if (wParam == XTP_DPN_SHOWWINDOW)
	{
		CXTPDockingPane* pPane = (CXTPDockingPane*)lParam;

		if(pPane->IsValid())	return 1;
		if(pPane->GetID() == ID_VIEW_TOOLBOX) ;
		else if(pPane->GetID() == ID_VIEW_PROPERTIESWINDOW);
		else if(pPane->GetID() == ID_HELP_DYNAMICHELP);
		else if(pPane->GetID() == ID_VIEW_PROJECT){
			if (!m_SpaceProject.GetSafeHwnd())
				m_SpaceProject.Create(_T("STATIC"), NULL, WS_CHILD|WS_VISIBLE|WS_CLIPCHILDREN|WS_CLIPSIBLINGS, CXTPEmptyRect(), this, 0);
			pPane->Attach(&m_SpaceProject);
		}
		else if(pPane->GetID() == ID_VIEW_ITEM){
			if (!m_SpaceItem.GetSafeHwnd())
				m_SpaceItem.Create(_T("STATIC"), NULL, WS_CHILD|WS_VISIBLE|WS_CLIPCHILDREN|WS_CLIPSIBLINGS, CXTPEmptyRect(), this, 1);
			pPane->Attach(&m_SpaceItem);
		}
		else if(pPane->GetID() == ID_VIEW_OUTPUT){
			if(!m_SpaceOutput.GetSafeHwnd()){
				m_SpaceOutput.Create(WS_CHILD|WS_HSCROLL|WS_VSCROLL|ES_AUTOHSCROLL|ES_MULTILINE|WS_VISIBLE, CXTPEmptyRect(), this, 2);
				m_SpaceOutput.ModifyStyle(ES_AUTOHSCROLL|ES_AUTOVSCROLL, 0);
			}
			pPane->Attach(&m_SpaceOutput);
		}
	}
	return 1;
}

LRESULT CMainFrame::OnTabbarMouseMsg(WPARAM wParam,LPARAM lParam)
{
	if (wParam != WM_RBUTTONDOWN)
		return FALSE;

	CPoint point = CPoint((DWORD)lParam);
	CXTPTabManagerItem* pItem =  m_MTIClientWnd.HitTest(point);

	if (pItem)
	{
		CWnd* pFrame = CWnd::FromHandle(pItem->GetHandle());
		MDIActivate(pFrame);
		m_MTIClientWnd.Refresh();

		CMenu menu;
		VERIFY(menu.LoadMenu(IDR_RBUTTON_MENU));
		CMenu* pPopup = menu.GetSubMenu(5);

		m_MTIClientWnd.WorkspaceToScreen(&point);
		CXTPCommandBars::TrackPopupMenu(pPopup, 0, point.x, point.y, this);
		m_MTIClientWnd.Refresh();
		return TRUE;
	}
	return FALSE;
}

void CMainFrame::InitCommerTime()
{
}

// 启动或停止时间限制
void CMainFrame::SetTimeLimit(bool bLimit)
{
	if(m_bLimitTimer == bLimit)			return;
	m_bLimitTimer = bLimit;
	if(bLimit)		SetTimer(TIME_LIMIT, 1000, NULL);
	else			{KillTimer(TIME_LIMIT);		SetTitle(TITLE);}
}

// 得带获得权限信息后再打开工程
void CMainFrame::WaitPower2OpenProject(CString path)
{
	CProjectMgr::GetMe().OpenProject(path);
}

void CMainFrame::SetTitle(CString title)
{
	if(!GetSafeHwnd())	return;
	SetWindowText(title);
	CXTPMDIFrameWnd::SetTitle(title);
}

bool CMainFrame::CreateToolBar()
{
	if (!InitCommandBars())				return false;	// 创建CXTPCommandBars对象
	CXTPCommandBars* pCommandBars = GetCommandBars();
	CXTPCommandBar* pMenuBar = pCommandBars->SetMenu(_T("Menu Bar"), IDR_MAINFRAME);
	pMenuBar->SetFlags(xtpFlagIgnoreSetMenuMessage);

	CXTPToolBar* pCommandBar = (CXTPToolBar*)pCommandBars->Add(_T("标准工具条"), xtpBarTop);
	if (!pCommandBar || !pCommandBar->LoadToolBar(IDR_MAINFRAME)){
		TRACE0("Failed to create toolbar\n");
		return false;
	}
	pCommandBar->ShowExpandButton(FALSE);
	CXTPToolBar* pWatchBar = (CXTPToolBar*)pCommandBars->Add(_T("监控工具条"), xtpBarTop);
	if (!pWatchBar || !pWatchBar->LoadToolBar(IDR_WATCH)){
		TRACE0("Failed to create toolbar\n");
		return false;
	}
	pWatchBar->ShowExpandButton(FALSE);
	DockRightOf(pWatchBar, pCommandBar);
	return true;
}

bool CMainFrame::CreateStatusBar()
{
	return true;
}

void CMainFrame::FillToStatusBar()
{
	//AddLogo
	CXTPStatusBarPane* pPane1=m_wndStatusBar.AddIndicator(ID_INDICATOR_LOGO,0);
	pPane1->SetText(_T("PM编辑器 "));// + CGbl::GetMe().getVersionStr() + _T("    "));
	pPane1->SetTextColor(0x915f36);
	//CXTPPaintManager::CNonClientMetrics ncm;
	NONCLIENTMETRICS ncm;
	::ZeroMemory(&ncm, sizeof(NONCLIENTMETRICS));
	ncm.cbSize = sizeof(NONCLIENTMETRICS);
	::SystemParametersInfo(SPI_GETNONCLIENTMETRICS, sizeof(NONCLIENTMETRICS), &ncm, 0);
	ncm.lfMenuFont.lfWeight = FW_BOLD;
	pPane1->SetTextFont(&ncm.lfMenuFont);
	pPane1->BestFit();

	//AddProgress()
	if (!m_wndProgCtrl.Create(WS_CHILD|WS_VISIBLE|PBS_SMOOTH,CRect(0,0,0,0), &m_wndStatusBar, 0)){
		TRACE0("Failed to create edit control.\n");
		return;
	}
	CXTPStatusBarPane* pPane2=m_wndStatusBar.AddIndicator(ID_INDICATOR_PROG);
	int nIndex = m_wndStatusBar.CommandToIndex(ID_INDICATOR_PROG);
	ASSERT (nIndex != -1);

	pPane2->SetCaption(pPane2->GetText());
	pPane2->SetText(_T(""));

	m_wndStatusBar.SetPaneWidth(nIndex, 150);
	m_wndStatusBar.SetPaneStyle(nIndex, m_wndStatusBar.GetPaneStyle(nIndex) | SBPS_NOBORDERS);
	m_wndStatusBar.AddControl(&m_wndProgCtrl, ID_INDICATOR_PROG, FALSE);

	//AddSwitchButtons
	static UINT switches[]={ID_INDICATOR_MASTER,};
	CXTPStatusBarSwitchPane* pSwitchPane = (CXTPStatusBarSwitchPane*)m_wndStatusBar.AddIndicator(new CXTPStatusBarSwitchPane(), ID_INDICATOR_STATE);
	pSwitchPane->SetSwitches(switches, sizeof(switches)/sizeof(UINT));

	for (int i = 0; i < sizeof(switches)/sizeof(UINT); i++)
		m_wndStatusBar.GetImageManager()->SetIcon(switches[i], switches[i]);

	pSwitchPane->BestFit();
	if (!m_wndAnimCtrl.Create(WS_CHILD|WS_VISIBLE|ACS_CENTER,CRect(0,0,0,0), &m_wndStatusBar, 0)){
		TRACE0("Failed to create avi control.\n");
		return;
	}
	m_wndAnimCtrl.Open(IDR_HEARTBEAT);
	m_wndAnimCtrl.Play(0, -1, -1);
	CXTPStatusBarPane* pPane3 = m_wndStatusBar.AddIndicator(ID_INDICATOR_ANIM);

	// Initialize the pane info and add the control.
	nIndex = m_wndStatusBar.CommandToIndex(ID_INDICATOR_ANIM);
	ASSERT (nIndex != -1);

	m_wndStatusBar.SetPaneWidth(nIndex, 14);
	m_wndStatusBar.SetPaneStyle(nIndex, m_wndStatusBar.GetPaneStyle(nIndex) | SBPS_POPOUT);
	m_wndStatusBar.AddControl(&m_wndAnimCtrl, ID_INDICATOR_ANIM, FALSE);
}

void CMainFrame::OnCustomize()
{
	// get a pointer to the command bars object.
	CXTPCommandBars* pCommandBars = GetCommandBars();
	if (pCommandBars == NULL)		return;

	// instantiate the customize dialog
	CXTPCustomizeSheet dlg(pCommandBars);

	// add the options page to the customize dialog.
	CXTPCustomizeOptionsPage pageOptions(&dlg);
	dlg.AddPage(&pageOptions);

	// add the commands page to the customize dialog.
	CXTPCustomizeCommandsPage* pPageCommands = dlg.GetCommandsPage();
	pPageCommands->AddCategories(IDR_MAINMENU);

	// initialize the commands page page.
	pPageCommands->InsertAllCommandsCategory();
	pPageCommands->InsertBuiltInMenus(IDR_MAINMENU);
	pPageCommands->InsertNewMenuCategory();

	// display the customize dialog.
	dlg.DoModal();
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CMDIFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	// TODO: 在此处通过修改 CREATESTRUCT cs 来修改窗口类或
	return TRUE;
}

// CMainFrame 诊断
#ifdef _DEBUG
void CMainFrame::AssertValid() const{CMDIFrameWnd::AssertValid();}
void CMainFrame::Dump(CDumpContext& dc) const{CMDIFrameWnd::Dump(dc);}
#endif //_DEBUG

// CMainFrame 消息处理程序
void CMainFrame::OnClose()
{
//Save Regester
	// 无论关闭与否，先把软件配置保存了
	SoftInfo::CSoftInfo::GetMe().SaveFile();
	CProjectMgr* projMgr = &CProjectMgr::GetMe();
	if (!projMgr->GetProj() || 
		!projMgr->IsModify())
	{
		int nResult = MessageBox(_T("是否确定退出程序？"), _T("提示"), MB_YESNO | MB_ICONINFORMATION);
		if(IDNO == nResult)						return;
		if(projMgr->GetProj())					ProjClose();
	}
	else
	{
		CString str = _T("是否保存当前工程？\r\n");
		int nResult = MessageBox(str, _T("提示"), MB_YESNOCANCEL | MB_ICONINFORMATION);
		if(IDYES == nResult)					OnProjSave();
		else if(IDCANCEL == nResult)			return;
		ProjClose();
	}
	SaveCommandBars(_T("CommandBars"));

	CXTPMDIFrameWnd::OnClose();
}

void CMainFrame::OnProjNew()
{
	CProjectMgr::GetMe().NewProject();
}

void CMainFrame::OnProjOpen()
{
	CProjectMgr::GetMe().OpenProject();
}

void CMainFrame::OnProjSave()
{
	//if(CProjectMgr::GetMe().SaveProject())
	//	CGbl::PrintOut(_T("保存成功！"));
	//else
	//	MessageBox(_T("保存失败！"), _T("提示"), MB_ICONEXCLAMATION);
}

void CMainFrame::OnProjBackUp()
{
	CProjectMgr::GetMe().BackUpProject();
}

void CMainFrame::OnProjClose()
{
	AskAndProjClose();
}

// 询问并关闭工程
bool CMainFrame::AskAndProjClose()
{
	if (!CProjectMgr::GetMe().GetProj())		return true;
	if (CProjectMgr::GetMe().IsModify())
	{
		int nResult = MessageBox(_T("是否保存当前工程？"), _T("提示"), MB_YESNOCANCEL | MB_ICONINFORMATION);
		if (IDYES == nResult)				OnProjSave();
		else if (IDCANCEL == nResult)		return false;
	}
	ProjClose();
	return true;
}

// 无声无息的关闭工程
void CMainFrame::ProjClose()
{
	if(CProjectMgr::GetMe().IsWatch()){
		CProjectMgr::GetMe().SetWatch(false);
		KillTimer(TIME_WATCH);
	}
	CProjectMgr::GetMe().CloseProject();
}

void CMainFrame::UpdateTreeView()
{
	m_SpaceProject.UpdateTreeView();
	m_SpaceItem.UpdateTreeView();
}

// 启动编译
void CMainFrame::CompileRun(bool bRunServer /* = false */)
{
}

void CMainFrame::OnTimer(UINT_PTR nIDEvent)
{
	if(nIDEvent == TIME_WATCH)// && SoftInfo::CSoftInfo::GetMe().getHDevMap() == 1)		// 监控的时间
	{
	}
	else if(nIDEvent == TIME_LIMIT)
	{
		if(m_nLimitTime <= 0)
		{
			KillTimer(TIME_LIMIT);
			CProjectMgr* projMgr = &CProjectMgr::GetMe();
			if(projMgr->IsModify())			projMgr->SaveProject();
			MessageBox(_T("2小时运行时间到，程序将自动保存并退出"), _T("提示"), MB_OK);
			ProjClose();
			CXTPMDIFrameWnd::OnClose();
			return;
		}
		m_nLimitTime--;
		// 设置Title
		int n = m_nLimitTime;
		int nHour, nMinute, nSecond;
		nHour = n / 3600;
		nMinute = (n - nHour * 3600) / 60;
		nSecond = n - nHour * 3600 - nMinute * 60;
		CString strHour, strMinute, strSecond, strTitle;
		strHour.Format(_T("%d小时:"), nHour);
		strMinute.Format(_T("%d分钟:"), nMinute);
		strSecond.Format(_T("%d秒钟"), nSecond);
		strTitle = TITLE + _T("  剩余时间: ") + strHour + strMinute + strSecond;
		SetTitle(strTitle);
	}
	CXTPMDIFrameWnd::OnTimer(nIDEvent);
}

void CMainFrame::OnAddItem()
{
	if(!CProjectMgr::GetMe().GetProj())	return;
	// 如果变量表没打开，那么打开变量表一览
	BOOL isMax;
	CFrameWnd* pFrame = MDIGetActive(&isMax);		// 找到当前被激活的子窗口
	if(pFrame)
	{
		MVC::Item::CItemDoc* pDoc = (MVC::Item::CItemDoc *)pFrame->GetActiveDocument();
		if(pDoc && pDoc->IsKindOf(RUNTIME_CLASS(MVC::Item::CItemDoc)))
		{
			pDoc->GetView()->OnItemAdd();
			return;
		}
	}
	//MVC::Item::CItemMgr* itemMgr = &MVC::Item::CItemMgr::GetMe();
	//itemMgr->OpenDoc();
	//itemMgr->m_pItemDoc->GetView()->OnItemAdd();
}

// 重命名工程名
void CMainFrame::OnRename()
{
	CProjectMgr* projMgr = &CProjectMgr::GetMe();
	if(!projMgr->GetProj())	return;
	CString oldPath = projMgr->GetProj()->GetWholePathName();
	if(projMgr->GetProj()->OnReName()){
		m_SpaceProject.UpdateTreeView();
		g_App.DeleteRecentFile(oldPath);
		g_App.AddToRecentFileList(projMgr->GetProj()->GetWholePathName());
	}
}

void CMainFrame::OnProjInfo()
{
	CProjectMgr::GetMe().EditProjInfo();
}

void CMainFrame::OnSoftSet()
{
	SoftInfo::CSoftSetDlg dlg;
	dlg.DoModal();
}

// 查找功能
void CMainFrame::OnFind()
{
	Dialog::CFindDlg::GetMe().DoModal();
}

// 显示工程试图
void CMainFrame::OnShowViewProject()
{
	m_paneManager.ShowPane(ID_VIEW_PROJECT);
}

// 显示变量组视图
void CMainFrame::OnShowViewItem()
{
	m_paneManager.ShowPane(ID_VIEW_ITEM);
}

// 显示输出栏视图
void CMainFrame::OnShowViewOutput()
{
	m_paneManager.ShowPane(ID_VIEW_OUTPUT);
}

void CMainFrame::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	switch(nChar)
	{
	case VK_F1:		OnHelpShow();		return;
	}
	CXTPMDIFrameWnd::OnKeyDown(nChar, nRepCnt, nFlags);
}

void CMainFrame::OnUpdateWithProj(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(CProjectMgr::GetMe().GetProj()?TRUE:FALSE);
}

void CMainFrame::OnStartpage()
{
	g_App.ShowStartPage();
}

void CMainFrame::OnHelpShow()
{
	CView* pView = GetActiveView();
	if(!pView)
		SoftInfo::CMyHelp::GetMe().ShowHelp(_T("目录"));
	else if(pView->IsKindOf(RUNTIME_CLASS(MVC::Item::CItemView)))
		SoftInfo::CMyHelp::GetMe().ShowHelp(_T("变量"));
	else
		SoftInfo::CMyHelp::GetMe().ShowHelp(_T("目录"));
}

