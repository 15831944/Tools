// MainFrm.cpp : CMainFrame 类的实现
//
#include "stdafx.h"
#include "PMApp.h"
#include "Gbl.h"
#include "SoftInfo.h"
#include "MyHelp.h"
#include "MainFrm.h"
#include "ProjectMgr.h"
#include "Compiler.h"
#include "ProjectMgr.h"
#include "Project.h"
#include "SoftSetDlg.h"
#include "FindDlg.h"

#include "ServerCtrl.h"
#include "Hmi.h"

#include "ItemMgr.h"
#include "ItemDoc.h"
#include "ItemView.h"

#include "DevMgr.h"
#include "ScanMgr.h"
#include "DeviceOne.h"
#include "DeviceMapDoc.h"
#include "DeviceMapView.h"
#include "DBMgr.h"
#include "ConfigMgr.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

const CString TITLE = _T("DView编辑器");
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
	ON_COMMAND(ID_COMPILE_CHECK, &CMainFrame::OnCompileCheck)
	ON_COMMAND(ID_SERVER_RUN, &CMainFrame::OnServerRun)
	ON_COMMAND(ID_SERVER_STOP, &CMainFrame::OnServerStop)
	ON_COMMAND(ID_ADD_DEVICE, &CMainFrame::OnAddDevice)
	ON_COMMAND(ID_ADD_ITEM, &CMainFrame::OnAddItem)
	ON_COMMAND(ID_RENAME, &CMainFrame::OnRename)
	ON_COMMAND(ID_PROJ_INFO, &CMainFrame::OnProjInfo)
	ON_COMMAND(ID_SOFT_SET, &CMainFrame::OnSoftSet)
	ON_COMMAND(ID_FIND, &CMainFrame::OnFind)
	ON_COMMAND(ID_VIEW_PROJECT, &CMainFrame::OnShowViewProject)
	ON_COMMAND(ID_VIEW_ITEM, &CMainFrame::OnShowViewItem)
	ON_COMMAND(ID_VIEW_DEVICE, &CMainFrame::OnShowViewDevice)
	ON_COMMAND(ID_VIEW_OUTPUT, &CMainFrame::OnShowViewOutput)
	ON_COMMAND(ID_SCAN_SET, &CMainFrame::OnScanSet)
	ON_COMMAND(ID_SCAN_START, &CMainFrame::OnScanStart)
	ON_COMMAND(ID_SCANANDSHOW, &CMainFrame::OnScanandshow)
	ON_COMMAND(ID_SCAN_STOP, &CMainFrame::OnScanStop)
	ON_COMMAND(ID_HMI_START, &CMainFrame::OnHmiStart)
	ON_COMMAND(ID_HELP_FILE, &CMainFrame::OnHelpShow)
	ON_UPDATE_COMMAND_UI(ID_PROJ_CLOSE, &CMainFrame::OnUpdateWithProj)
	ON_UPDATE_COMMAND_UI(ID_PROJ_SAVE, &CMainFrame::OnUpdateWithProj)
	ON_UPDATE_COMMAND_UI(ID_PROJ_BACKUP, &CMainFrame::OnUpdateWithProj)
	ON_UPDATE_COMMAND_UI(ID_RENAME, &CMainFrame::OnUpdateWithProj)
	ON_UPDATE_COMMAND_UI(ID_ADD_DEVICE, &CMainFrame::OnUpdateWithProj)
	ON_UPDATE_COMMAND_UI(ID_ADD_ITEM, &CMainFrame::OnUpdateWithProj)
	ON_UPDATE_COMMAND_UI(ID_COMPILE_CHECK, &CMainFrame::OnUpdateWithProj)
	ON_UPDATE_COMMAND_UI(ID_PROJ_INFO, &CMainFrame::OnUpdateWithProj)
	ON_UPDATE_COMMAND_UI(ID_FIND, &CMainFrame::OnUpdateWithProj)
	ON_UPDATE_COMMAND_UI(ID_SCAN_SET, &CMainFrame::OnUpdateWithProj)
	ON_UPDATE_COMMAND_UI(ID_HMI_START, &CMainFrame::OnUpdateWithProj)
	ON_UPDATE_COMMAND_UI(ID_SERVER_RUN, &CMainFrame::OnUpdateServerRun)
	ON_UPDATE_COMMAND_UI(ID_SERVER_STOP, &CMainFrame::OnUpdateServerStop)
	ON_UPDATE_COMMAND_UI(ID_SCAN_START, &CMainFrame::OnUpdateScanStart)
	ON_UPDATE_COMMAND_UI(ID_SCAN_STOP, &CMainFrame::OnUpdateScanStop)
	ON_COMMAND(ID_REGIST, &CMainFrame::OnRegist)
	ON_COMMAND(ID_STARTPAGE, &CMainFrame::OnStartpage)
END_MESSAGE_MAP()

BEGIN_EVENTSINK_MAP(CMainFrame, CMDIFrameWnd)
	ON_EVENT(CMainFrame, IDS_PMCLIENT, 1, OnDataReady, VTS_I4)
	ON_EVENT(CMainFrame, IDS_PMCLIENT, 2, OnVariableAlarm, VTS_I4 VTS_I4 VTS_PVARIANT)
	ON_EVENT(CMainFrame, IDS_PMCLIENT, 3, OnVariableLag, VTS_I4 VTS_I4 VTS_PVARIANT)
	ON_EVENT(CMainFrame, IDS_PMCLIENT, 4, OnLoadOver, VTS_I4 VTS_I4)
	ON_EVENT(CMainFrame, IDS_PMCLIENT, 5, OnDeviceStatus, VTS_I4 VTS_I4 VTS_I4)
	ON_EVENT(CMainFrame, IDS_PMCLIENT, 6, OnBehavior, VTS_I4 VTS_I4 VTS_VARIANT VTS_I4)
	ON_EVENT(CMainFrame, IDS_PMCLIENT, 7, OnScan, VTS_VARIANT VTS_I4)
	ON_EVENT(CMainFrame, IDS_PMCLIENT, 8, OnScanBehavior, VTS_I4 VTS_I4 VTS_I4 VTS_I4 VTS_PI4 VTS_VARIANT VTS_I4)
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
:m_bLimitTimer(false)							//!< 是否限时定时器开启
{
}

CMainFrame::~CMainFrame()
{
}

//!< 设置窗口样式
void CMainFrame::SetTheme(int index)
{
	switch(index)
	{
	case 0:		XTPPaintManager()->SetTheme(xtpThemeOffice2000);		break;
	case 1:		XTPPaintManager()->SetTheme(xtpThemeOfficeXP);			break;
	case 2:		XTPPaintManager()->SetTheme(xtpThemeOffice2003);		break;
	case 3:		XTPPaintManager()->SetTheme(xtpThemeNativeWinXP);		break;
	case 4:		XTPPaintManager()->SetTheme(xtpThemeWhidbey);			break;
	case 5:		XTPPaintManager()->SetTheme(xtpThemeOffice2007);		break;
	case 6:		XTPPaintManager()->SetTheme(xtpThemeRibbon);			break;
	case 7:		XTPPaintManager()->SetTheme(xtpThemeVisualStudio2008);	break;
	case 8:		XTPPaintManager()->SetTheme(xtpThemeCustom);			break;
	}
}

//!< 设置Pane样式
void CMainFrame::SetPaneTheme(int index)
{
	switch(index)
	{
	case 0:		m_paneManager.SetTheme(xtpPaneThemeDefault);			break;
	case 1:		m_paneManager.SetTheme(xtpPaneThemeOffice);				break;
	case 2:		m_paneManager.SetTheme(xtpPaneThemeOffice2003);			break;
	case 3:		m_paneManager.SetTheme(xtpPaneThemeNativeWinXP);		break;
	case 4:		m_paneManager.SetTheme(xtpPaneThemeGrippered);			break;
	case 5:		m_paneManager.SetTheme(xtpPaneThemeVisio);				break;
	case 6:		m_paneManager.SetTheme(xtpPaneThemeWhidbey);			break;
	case 7:		m_paneManager.SetTheme(xtpPaneThemeShortcutBar2003);	break;
	case 8:		m_paneManager.SetTheme(xtpPaneThemeExplorer);			break;
	case 9:		m_paneManager.SetTheme(xtpPaneThemeVisualStudio2005);	break;
	case 10:	m_paneManager.SetTheme(xtpPaneThemeOffice2007);			break;
	case 11:	m_paneManager.SetTheme(xtpPaneThemeWord2007);			break;
	case 12:	m_paneManager.SetTheme(xtpPaneThemeOutlook2007);		break;
	case 13:	m_paneManager.SetTheme(xtpPaneThemeCustom);;			break;
	}
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	// 初始化 OLE 库
	CoInitialize(NULL);
	if (!AfxOleInit()){AfxMessageBox(IDP_OLE_INIT_FAILED);		return FALSE;}
	if (CMDIFrameWnd::OnCreate(lpCreateStruct) == -1)	return -1;
	if (!CreateToolBar())								return -1;

	//!< 添加状态栏
	if (!m_wndStatusBar.Create(this) || !m_wndStatusBar.SetIndicators(indicators, sizeof(indicators)/sizeof(UINT)))		return -1;

	m_wndStatusBar.GetPane(0)->SetBeginGroup(FALSE);
	FillToStatusBar();
	SetTheme(7);											//!< 设置窗口的风格
	m_paneManager.InstallDockingPanes(this);				//!< Add pane
	SetPaneTheme(9);										//!< 设置每个pane的风格

	CXTPDockingPane* paneDeviceView = m_paneManager.CreatePane(ID_VIEW_DEVICE, CRect(0, 0, 150, 140), xtpPaneDockRight);
	CXTPDockingPane* paneOutputView = m_paneManager.CreatePane(ID_VIEW_OUTPUT, CRect(0, 0, 150, 200), xtpPaneDockBottom);
	CXTPDockingPane* paneProjView = m_paneManager.CreatePane(ID_VIEW_PROJECT, CRect(0, 0, 230, 140), xtpPaneDockLeft);
	CXTPDockingPane* paneItemView = m_paneManager.CreatePane(ID_VIEW_ITEM, CRect(0, 0, 210, 140), xtpPaneDockLeft);
	paneDeviceView->Hide();

	paneDeviceView->SetTitle("设备视图\n设备视图");			//!< 在这里设Title是因为CreatePane有语言不兼容的bug
	paneOutputView->SetTitle("输出栏\n输出栏");
	paneProjView->SetTitle("工程视图\n工程视图");
	paneItemView->SetTitle("变量视图\n变量视图");

	m_paneManager.AttachPane(paneItemView, paneProjView);	//!< 将两个pane合并在一起
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

	//!< 初始化服务通信器
	m_SevCommer.Create(NULL, WS_VISIBLE, CRect(10, 10, 20, 20), this, IDS_PMCLIENT);

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
		else if(pPane->GetID() == ID_VIEW_DEVICE){
			if (!m_SpaceDevice.GetSafeHwnd())	CreateSpaceDevice();
			pPane->Attach(&m_SpaceDevice);
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
	if(m_SevCommer.GetSafeHwnd())
		m_SevCommer.DataInit(SoftInfo::CSoftInfo::GetMe().getFreshDataTime());
}

//!< 启动或停止时间限制
void CMainFrame::SetTimeLimit(bool bLimit)
{
	if(m_bLimitTimer == bLimit)			return;
	m_bLimitTimer = bLimit;
	if(bLimit)		SetTimer(TIME_LIMIT, 1000, NULL);
	else			{KillTimer(TIME_LIMIT);		SetTitle(TITLE);}
}

//!< 得带获得权限信息后再打开工程
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
	if (!InitCommandBars())				return false;	//!< 创建CXTPCommandBars对象
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

bool CMainFrame::CreateSpaceDevice()
{
	return m_SpaceDevice.CreateSpace();
}

void CMainFrame::FillToStatusBar()
{
	//AddLogo
	CXTPStatusBarPane* pPane1=m_wndStatusBar.AddIndicator(ID_INDICATOR_LOGO,0);
	pPane1->SetText(_T("DView编辑器 "));// + CGbl::GetMe().getVersionStr() + _T("    "));
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
	//!< 无论关闭与否，先把软件配置保存了
	SoftInfo::CSoftInfo::GetMe().SaveFile();
	CProjectMgr* projMgr = &CProjectMgr::GetMe();
	if(!projMgr->GetProj() || 
		(!projMgr->IsModify() &&
		!projMgr->GetProj()->IsHmiModify() &&
		Servers::Config::CConfigMgr::GetMe().GetConfigOpenCount() == 0))
	{
		int nResult = MessageBox(_T("是否确定退出程序？"), _T("提示"), MB_YESNO | MB_ICONINFORMATION);
		if(IDNO == nResult)						return;
		Servers::HMI::CHmi::GetMe().SetClose(nResult == IDYES);
		if(projMgr->GetProj())					ProjClose();
	}
	else
	{
		CString str = _T("是否保存当前工程？\r\n");
		str = str + _T("\r\n 是:保存所有工程包括HMI页面信息和Config程序信息，并退出");
		str = str + _T("\r\n 否:不保存所有工程包括HMI页面信息和Config程序信息，并退出");
		str = str + _T("\r\n 取消:不退出");
		int nResult = MessageBox(str, _T("提示"), MB_YESNOCANCEL | MB_ICONINFORMATION);
		if(IDYES == nResult)					OnProjSave();
		else if(IDCANCEL == nResult)			return;
		Servers::HMI::CHmi::GetMe().SetClose(nResult == IDYES);
		ProjClose();
	}
	SaveCommandBars(_T("CommandBars"));
// 	CComVariant cvr;
// 	m_SevCommer.Execute(7, cvr, 0, 0, 0);			//!< 清空Client中的内存
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
	if(CProjectMgr::GetMe().SaveProject())
		CGbl::PrintOut(_T("保存成功！"));
	else
		MessageBox(_T("保存失败！"), _T("提示"), MB_ICONEXCLAMATION);
}

void CMainFrame::OnProjBackUp()
{
	CProjectMgr::GetMe().BackUpProject();
}

void CMainFrame::OnProjClose()
{
	AskAndProjClose();
}

//!< 询问并关闭工程
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

//!< 无声无息的关闭工程
void CMainFrame::ProjClose()
{
	if(CProjectMgr::GetMe().IsWatch()){
//		MVC::Item::CItemMgr::GetMe().SetWatch(false);
//		MVC::Device::CDevMgr::GetMe().SetWatch(false);
		CProjectMgr::GetMe().SetWatch(false);
		KillTimer(TIME_WATCH);
		CComVariant cvr;
		m_SevCommer.Execute(7, cvr, 0, 0, 0);			//!< 清空Client中的内存
	}
	CProjectMgr::GetMe().CloseProject();
}

void CMainFrame::UpdateTreeView()
{
	m_SpaceProject.UpdateTreeView();
	m_SpaceItem.UpdateTreeView();
}

//!< 启动编译
void CMainFrame::CompileRun(bool bRunServer /* = false */)
{
	//!< 编译之前的准备工作
	std::shared_ptr<CProject> proj = CProjectMgr::GetMe().GetProj();
	if(!proj)		return;
	CProjectMgr::GetMe().SaveProject();
	CGbl::PrintClear();

	//!< 开始编译
	if(bRunServer)		Servers::Compile::CCompiler::GetMe().AddRunObj(&Servers::DXP::CServerCtrl::GetMe());
	Servers::Compile::CCompiler::GetMe().CompileProj();
}

//!< 启动编译扫描，编译成功后要启动扫描
void CMainFrame::CompileScan()
{
	std::shared_ptr<CProject> proj = CProjectMgr::GetMe().GetProj();
	if(!proj)		return;
	//!< 编译前要清空输出栏
	CGbl::PrintClear();

	//!< 开始编译
	Servers::Compile::CCompiler::GetMe().CompileScan();
}

//!< 检查
void CMainFrame::OnCompileCheck()
{
	CompileRun(false);
}

//!< 启动服务器运行
void CMainFrame::OnServerRun()
{
	if(!CProjectMgr::GetMe().GetProj())	return;
	int state = Servers::DXP::CServerCtrl::GetMe().GetState();
	if(state == 1)			return;										//!< 正在运行中,不能操作
	else if( state == 2)	Servers::DXP::CServerCtrl::GetMe().OnRun();	//!< 处于暂停中,只能直接启动
	else					CompileRun(true);							//!< 停止状态,启动编译运行
}

//!< 启动服务器停止
void CMainFrame::OnServerStop()
{
	Servers::DXP::CServerCtrl::GetMe().OnStopSev();
}

//!< 接收被启动程序传回来的数据
BOOL CMainFrame::OnCopyData(CWnd* pWnd, COPYDATASTRUCT* pCopyDataStruct)
{
	if(pCopyDataStruct)
	{
	/*	CString str;
		str.Format("%d",(UINT)pCopyDataStruct->dwData);
		CGbl::GetMe().PrintOut(str);*/
		if(PRINT == (UINT)pCopyDataStruct->dwData)
		{
			char *data = new char[pCopyDataStruct->cbData + 1];
			memcpy(data, pCopyDataStruct->lpData, pCopyDataStruct->cbData);
			data[pCopyDataStruct->cbData] = 0;
			CString msg = data;
			CGbl::PrintOut(msg, 0);
			m_paneManager.ShowPane(ID_VIEW_OUTPUT);
			delete[] data;
		}
		else if (SHOW_ITEM == (UINT)pCopyDataStruct->dwData)
		{
			MVC::Item::CItemMgr::GetMe().ShowItem(-1);
			m_paneManager.HidePane(ID_VIEW_OUTPUT);
			m_paneManager.HidePane(ID_VIEW_PROJECT);
			m_paneManager.HidePane(ID_VIEW_ITEM);
			m_paneManager.HidePane(ID_VIEW_DEVICE);
			ShowWindow(SW_SHOWMAXIMIZED);
		}
		else if(Servers::Compile::CCompiler::GetMe().IsINeed((UINT)pCopyDataStruct->dwData))	//!< 如果是编译器的消息
			Servers::Compile::CCompiler::GetMe().OnReceive(pCopyDataStruct);
		else if(Servers::DXP::CServerCtrl::GetMe().IsINeed((UINT)pCopyDataStruct->dwData))
			Servers::DXP::CServerCtrl::GetMe().OnReceive(pCopyDataStruct);
		else if(Servers::HMI::CHmi::GetMe().IsINeed((UINT)pCopyDataStruct->dwData))
			Servers::HMI::CHmi::GetMe().OnReceive(pCopyDataStruct);
		else if(Servers::DB::CDBMgr::GetMe().IsNeed((UINT)pCopyDataStruct->dwData))
			Servers::DB::CDBMgr::GetMe().OnReceive(pCopyDataStruct);
	}
	return CXTPMDIFrameWnd::OnCopyData(pWnd, pCopyDataStruct);
}

void CMainFrame::OnTimer(UINT_PTR nIDEvent)
{
	if(nIDEvent == TIME_WATCH)// && SoftInfo::CSoftInfo::GetMe().getHDevMap() == 1)		//!< 监控的时间
	{
		MVC::Item::CItemMgr::GetMe().ReadItemValue(true);
		MVC::Item::CItemMgr::GetMe().ReadItemValue(false);
	}
	else if(nIDEvent == TIME_SCAN)
	{
		if(Servers::DXP::CServerCtrl::GetMe().GetScanState() == 1){
			CComVariant cvr;
			m_SevCommer.Execute(9, cvr, 0, 0, 0);			//!< 清空Client中的内存
		}
	}
	else if(nIDEvent == Servers::Compile::CCompiler::COMPILE_OVERTIME_ID)
	{
		Servers::Compile::CCompiler::GetMe().OnCompileOverTime();	//!< 编译超时了
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
		//!< 设置Title
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

void CMainFrame::OnUpdateServerRun(CCmdUI *pCmdUI)
{
	OnUpdateWithProj(pCmdUI);
	Servers::DXP::CServerCtrl* server = &Servers::DXP::CServerCtrl::GetMe();
	if(server->GetState() == 1)				pCmdUI->Enable(FALSE);
	if(server->GetScanState() == 1)			pCmdUI->Enable(FALSE);
}

void CMainFrame::OnUpdateServerStop(CCmdUI *pCmdUI)
{
	OnUpdateWithProj(pCmdUI);
	Servers::DXP::CServerCtrl* server = &Servers::DXP::CServerCtrl::GetMe();
	if(server->GetState() != 1)				pCmdUI->Enable(FALSE);
	if(server->GetScanState() == 1)			pCmdUI->Enable(FALSE);
}

void CMainFrame::OnDataReady(long nShakeInterval)
{
	if(!CProjectMgr::GetMe().IsWatch())		return;		//!< 不监控，不处理
}

void CMainFrame::OnVariableAlarm(int nAlarmType,int nID,VARIANT* varValue)
{
	if(!CProjectMgr::GetMe().IsWatch())		return;		//!< 不监控，不处理
	MVC::Item::CItemMgr::GetMe().OnVariableAlarm(nAlarmType, nID, varValue);
}

void CMainFrame::OnVariableLag(int nID,int nDevice,VARIANT* varValue)
{
	if(!CProjectMgr::GetMe().IsWatch())		return;		//!< 不监控，不处理
}

void CMainFrame::OnLoadOver(int nLoadType,int nLoadSize)
{
	if(!CProjectMgr::GetMe().IsWatch())		return;		//!< 不监控，不处理
}

//!< 设备状态改变事件。nDeviceInterface
void CMainFrame::OnDeviceStatus(long nDeviceID,long nDeviceInterface,long nDeviceStatus)
{
	if(!CProjectMgr::GetMe().IsWatch())		return;		//!< 不监控，不处理
	MVC::Device::CDevMgr::GetMe().OnDeviceStatus(nDeviceID, nDeviceInterface, nDeviceStatus);
}

void CMainFrame::OnBehavior(long lBehaviorID, long lDeviceID, VARIANT& varValue, long lResult)
{
	MVC::Device::CDevMgr::GetMe().OnBehavior(lBehaviorID, lDeviceID, varValue, lResult);
}

void CMainFrame::OnScanBehavior(long devType, long infType, long level, long bevID, long* plAddr, VARIANT& varValue, long lResult)
{
	//!< 判断收到的是什么东西
	MVC::Device::CDevMgr* devMgr = &MVC::Device::CDevMgr::GetMe();
	std::shared_ptr<MVC::Device::CDeviceOne> device = devMgr->GetDevice(devType, infType, level, plAddr);
	if(!device)			return;
	OnBehavior(bevID, device->getID(), varValue, 1);
}

void CMainFrame::OnScan(VARIANT &varValue,long lResult)
{
	//!< 为数据赋值
	VARIANT *var = (VARIANT *)varValue.byref;
	char *pValue=NULL;
	HRESULT hr = SafeArrayAccessData(var->parray, (void**)&pValue);
	long Low(0), High(0);								//!< 获取数组长度
	if(SUCCEEDED(hr)){
		hr = SafeArrayGetLBound(var->parray, 1, &Low);	//!< 维数索引从1开始
		hr = SafeArrayGetUBound(var->parray, 1, &High);	//!< 维数索引从1开始
	}
	else{SafeArrayUnaccessData(var->parray);			return;}
	int varCount = High - Low + 1;
	if(varCount < 1){SafeArrayUnaccessData(var->parray);return;}
	SafeArrayUnaccessData(var->parray);
	MVC::Device::CDevMgr::GetMe().m_ScanMgr->OnScanRev(pValue, varCount);
}

void CMainFrame::OnAddDevice()
{
	if(!CProjectMgr::GetMe().GetProj())	return;
	//!< 如果拓扑图没打开，那么打开拓扑图
	MVC::Device::CDevMgr* devMgr = &MVC::Device::CDevMgr::GetMe();
	devMgr->OpenDoc();
	devMgr->m_DevMapDoc->GetView()->OnAddDev();
}

void CMainFrame::OnAddItem()
{
	if(!CProjectMgr::GetMe().GetProj())	return;
	//!< 如果变量表没打开，那么打开变量表一览
	BOOL isMax;
	CFrameWnd* pFrame = MDIGetActive(&isMax);		//!< 找到当前被激活的子窗口
	if(pFrame)
	{
		MVC::Item::CItemDoc* pDoc = (MVC::Item::CItemDoc *)pFrame->GetActiveDocument();
		if(pDoc && pDoc->IsKindOf(RUNTIME_CLASS(MVC::Item::CItemDoc)))
		{
			pDoc->GetView()->OnItemAdd();
			return;
		}
	}
	MVC::Item::CItemMgr* itemMgr = &MVC::Item::CItemMgr::GetMe();
	itemMgr->OpenDoc();
	itemMgr->m_pItemDoc->GetView()->OnItemAdd();
}

//!< 重命名工程名
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

//!< 查找功能
void CMainFrame::OnFind()
{
	Dialog::CFindDlg::GetMe().DoModal();
}

//!< 显示工程试图
void CMainFrame::OnShowViewProject()
{
	m_paneManager.ShowPane(ID_VIEW_PROJECT);
}

//!< 显示变量组视图
void CMainFrame::OnShowViewItem()
{
	m_paneManager.ShowPane(ID_VIEW_ITEM);
}

//!< 显示设备视图
void CMainFrame::OnShowViewDevice()
{
	m_paneManager.ShowPane(ID_VIEW_DEVICE);
}

//!< 显示输出栏视图
void CMainFrame::OnShowViewOutput()
{
	m_paneManager.ShowPane(ID_VIEW_OUTPUT);
}

void CMainFrame::OnHmiStart()
{
	if(!CProjectMgr::GetMe().SaveProject())			return;
	Servers::Compile::CCompiler::GetMe().AddRunObj(&Servers::HMI::CHmi::GetMe());
	Servers::Compile::CCompiler::GetMe().CompileProj();
}

void CMainFrame::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	switch(nChar)
	{
	case VK_F1:		OnHelpShow();		return;
	}
	CXTPMDIFrameWnd::OnKeyDown(nChar, nRepCnt, nFlags);
}

void CMainFrame::OnScanSet()
{
	MVC::Device::CDevMgr::GetMe().OpenDoc();
	MVC::Device::CDevMgr::GetMe().SetScanInfo();
}

void CMainFrame::OnScanStart()
{
	CProjectMgr::GetMe().SaveProject();
	if(CProjectMgr::GetMe().IsScan())
	{
		MVC::Device::CDevMgr::GetMe().SetDevScan(true);
	}
	else
	{
		//!< 如果需要编译开始编译
		std::shared_ptr<MVC::Device::CScanSetInfo> pScanInfo = MVC::Device::CDevMgr::GetMe().GetScanInfo();
//		if(!pScanInfo->IsCompiled())
		if(true)
		{
			CompileScan();
		}
		else
		{	//!< 开始运行
			std::shared_ptr<CProject> proj = CProjectMgr::GetMe().GetProj();
			if(!proj)													return;
			Servers::Compile::CCompiler::GetMe().RunScan();
		}
	}
}

void CMainFrame::OnScanStop()
{
	Servers::DXP::CServerCtrl::GetMe().OnScanStop();
	KillTimer(2);
}

void CMainFrame::OnUpdateScanStart(CCmdUI *pCmdUI)
{
	OnUpdateWithProj(pCmdUI);
	if(CProjectMgr::GetMe().IsScan())	pCmdUI->SetText(_T("刷新扫描数据(&F)"));
	else								pCmdUI->SetText(_T("开始扫描(&R)"));
	Servers::DXP::CServerCtrl* server = &Servers::DXP::CServerCtrl::GetMe();
	if(server->GetState() == 1 || server->GetState() == 2)
		pCmdUI->Enable(FALSE);
}

void CMainFrame::OnUpdateScanStop(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(CProjectMgr::GetMe().IsScan());
	Servers::DXP::CServerCtrl* server = &Servers::DXP::CServerCtrl::GetMe();
	if(server->GetState() == 1 || server->GetState() == 2)
		pCmdUI->Enable(FALSE);
}

void CMainFrame::OnUpdateWithProj(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(CProjectMgr::GetMe().GetProj()?TRUE:FALSE);
}

void CMainFrame::OnScanandshow()
{
	MVC::Device::CDevMgr::GetMe().OpenDoc();
	OnScanStart();
}

void CMainFrame::OnRegist()
{
	g_App.RegistSoftware();
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
	else if(pView->IsKindOf(RUNTIME_CLASS(MVC::Device::CDeviceMapView)))
		SoftInfo::CMyHelp::GetMe().ShowHelp(_T("设备拓扑"));
	else
		SoftInfo::CMyHelp::GetMe().ShowHelp(_T("目录"));
}

