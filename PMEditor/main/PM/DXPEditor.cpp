// DXPEditor.cpp : 定义应用程序的类行为。
//
#include "stdafx.h"
#include "DXPEditor.h"
#include "MyHelp.h"
#include "ProjGuideDlg.h"
#include "MainFrm.h"
#include "AboutDlg.h"
#include "ProjectMgr.h"
#include "XmlMgr.h"
#include "SoftInfo.h"
#include "ServerCtrl.h"
#include "Hmi.h"

#include "StartDoc.h"
#include "StartView.h"
#include "StartFrm.h"
#include "DCCE_HtmlDialog.h"

#include "ItemDoc.h"
#include "ItemView.h"
#include "ItemFrame.h"

#include "DeviceMapDoc.h"
#include "DeviceMapView.h"
#include "DeviceMapFrm.h"

#include "CamDoc.h"
#include "CamView.h"
#include "CamFrame.h"

#include "DCCE_Splash.h"
#include "CreatProjectDlg.h"
#include "ServerCommer.h"
#include "ServerCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CDXPEditorApp
BEGIN_MESSAGE_MAP(CDXPEditorApp, CWinApp)
	ON_COMMAND(ID_APP_ABOUT, OnAppAbout)
	ON_COMMAND(ID_FILE_PRINT_SETUP, CWinApp::OnFilePrintSetup)
	ON_COMMAND_RANGE(ID_FILE_MRU_FIRST, ID_FILE_MRU_LAST, MRUFileHandler)
END_MESSAGE_MAP()

// 唯一的一个 CDXPEditorApp 对象
CDXPEditorApp g_App;

// CDXPEditorApp 构造
CDXPEditorApp::CDXPEditorApp()
{
	m_StartPage = NULL;				/**< 当前的起始页Dialog指针 */
	// 将所有重要的初始化放置在 InitInstance 中
}

// CDXPEditorApp 初始化
BOOL CDXPEditorApp::InitInstance()
{
	// 如果一个运行在 Windows XP 上的应用程序清单指定要
	// 使用 ComCtl32.dll 版本 6 或更高版本来启用可视化方式，
	//则需要 InitCommonControls()。否则，将无法创建窗口。
#ifndef _DEBUG
	CDCCE_Splash::Init(5);
	CDCCE_Splash::StepOne(_T(""));//正在初始化..."));
#endif

	InitCommonControls();
	CWinApp::InitInstance();
	AfxInitRichEdit2();
	AfxEnableControlContainer();

	// 标准初始化
	SetRegistryKey(_T("应用程序向导生成的本地应用程序"));
	LoadStdProfileSettings(6);  // 加载标准 INI 文件选项(包括 MRU)

	m_pMainWnd = NULL;

	// 注册应用程序的文档模板。文档模板
	// 将用作文档、框架窗口和视图之间的连接
 	m_pStartDocMgr = new CMultiDocTemplate(IDR_MAINMENU,
		RUNTIME_CLASS(MVC::Start::CStartDoc),
		RUNTIME_CLASS(MVC::Start::CStartFrm), // 自定义 MDI 子框架
		RUNTIME_CLASS(MVC::Start::CStartView));
	if (!m_pStartDocMgr)	return FALSE;
	AddDocTemplate(m_pStartDocMgr);

	m_pItemDocMgr = new CMultiDocTemplate(IDR_MAINFRAME,
		RUNTIME_CLASS(MVC::Item::CItemDoc),
		RUNTIME_CLASS(MVC::Item::CItemFrame),
		RUNTIME_CLASS(MVC::Item::CItemView));
	if (!m_pItemDocMgr)		{CDCCE_Splash::Hide();	return FALSE;}
	AddDocTemplate(m_pItemDocMgr);

	m_pDeviceDocMgr = new CMultiDocTemplate(IDR_MAINFRAME,
		RUNTIME_CLASS(MVC::Device::CDeviceMapDoc),
		RUNTIME_CLASS(MVC::Device::CDeviceMapFrm),
		RUNTIME_CLASS(MVC::Device::CDeviceMapView));
	if(!m_pDeviceDocMgr)	{CDCCE_Splash::Hide();	return FALSE;}
	AddDocTemplate(m_pDeviceDocMgr);

	m_pCamDocMgr = new CMultiDocTemplate(IDR_MAINFRAME,
		RUNTIME_CLASS(MVC::Camera::CCamDoc),
		RUNTIME_CLASS(MVC::Camera::CCamFrame),
		RUNTIME_CLASS(MVC::Camera::CCamView));
	if(!m_pCamDocMgr)	{CDCCE_Splash::Hide();	return FALSE;}
	AddDocTemplate(m_pCamDocMgr);

	RegisterShellFileTypes();

	if(CDCCE_Splash::Visible())
	{
		Sleep(100);
		CDCCE_Splash::StepOne(_T(""));
	}

	//!< 装载描述信息
	m_XmlMgr = boost::shared_ptr<XmlInfo::CXmlMgr>(new XmlInfo::CXmlMgr);
	if(!m_XmlMgr->OpenXml()){
		CDCCE_Splash::Hide();
		AfxMessageBox("解析描述文件失败！");
		return FALSE;
	}
	if(CDCCE_Splash::Visible())
	{
		Sleep(300);
		CDCCE_Splash::StepOne(_T(""));
	}

	//!< 初始化全局变量
	SoftInfo::CSoftInfo::GetMe().LoadFile();

	//!< 初始化权限
	Servers::DXP::CServerCtrl::GetMe();				//!< 先得一次,以免在线程中得的时候造成错误

	// 创建主 MDI 框架窗口
	CMainFrame* pMainFrame = new CMainFrame;
	if (!pMainFrame || !pMainFrame->LoadFrame(IDR_MAINFRAME))
		return FALSE;
	m_pMainWnd = pMainFrame;

	pMainFrame->ShowWindow(SW_SHOWMAXIMIZED);

//	CCommandLineInfo cmdInfo;
//	ParseCommandLine(cmdInfo);
//	if (!ProcessShellCommand(cmdInfo))
	pMainFrame->UpdateWindow();

	//!< 打开起始页
//	ShowStartPage();

	//!< 初始化通信服务器
	Servers::DXP::CServerCtrl::GetMe().InitServerMgr(m_pMainWnd);

	//!< 初始化HMI通信器
	Servers::HMI::CHmi::GetMe().InitHmiMgr(m_pMainWnd);

	//!< 初始化帮助文档
	SoftInfo::CMyHelp::GetMe().InitInfo();

	if(CDCCE_Splash::Visible())
	{
		CDCCE_Splash::StepOne(_T(""));
		Sleep(1000);
		CDCCE_Splash::Hide();
	}

	CString cmdLine = m_lpCmdLine;
	if(!cmdLine.IsEmpty())
	{
		ReadCmdLine(cmdLine);
	}
	return TRUE;
}

int CDXPEditorApp::ExitInstance()
{
	// TODO: Add your specialized code here and/or call the base class
//	CoUninitialize();
	return CWinApp::ExitInstance();
}

void CDXPEditorApp::AddToRecentFileList(LPCTSTR lpszPathName)
{
	//! 只有工程文件才能添加到最近打开列表中
	int len = ::_tcslen(lpszPathName);
	LPCTSTR pExt = lpszPathName;
	if(len >= 4 && _tcscmp(pExt + len - 4, _T(".dsl")) == 0)
	{
		CWinApp::AddToRecentFileList(lpszPathName);
	}
}

void CDXPEditorApp::DeleteRecentFile(LPCTSTR lpszPathName)
{
	int count = m_pRecentFileList->GetSize();
	for(int i = 0; i < count; ++i)
	{
		if((*m_pRecentFileList)[i] != lpszPathName)
			continue;
		m_pRecentFileList->Remove(i);
		return;
	}
}

//!< 打开起始页
void CDXPEditorApp::ShowStartPage()
{
	if(m_StartPage)		return;
	m_pStartDocMgr->OpenDocumentFile(NULL);
}

void CDXPEditorApp::MRUFileHandler(UINT i)
{
	ASSERT_VALID(this);
	ASSERT(m_pRecentFileList != NULL);
	ASSERT(i >= ID_FILE_MRU_FILE1);
	ASSERT(i < ID_FILE_MRU_FILE1 + (UINT)m_pRecentFileList->GetSize());

	CString strName, strCurDir, strMessage;
	int nIndex = i - ID_FILE_MRU_FILE1;

	ASSERT((*m_pRecentFileList)[nIndex].GetLength() != 0);

	bool bResault = CProjectMgr::GetMe().OpenProject((*m_pRecentFileList)[nIndex]);
}

// 用于运行对话框的应用程序命令
void CDXPEditorApp::OnAppAbout()
{
	Dialog::CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

BOOL CDXPEditorApp::PreTranslateMessage(MSG* pMsg)
{
	return CWinApp::PreTranslateMessage(pMsg);
}

//!< 软件注册
void CDXPEditorApp::RegistSoftware()
{
	Servers::DXP::CServerCtrl::GetMe().RegistSoft();
}

//!< 解析命令行
void CDXPEditorApp::ReadCmdLine(CString cmdLine)
{
	// 去掉字符串两边的"号
	if(cmdLine.Right(1) == _T("\""))	cmdLine = cmdLine.Left(cmdLine.GetLength() - 1);
	if(cmdLine.Left(1) == _T("\""))		cmdLine = cmdLine.Right(cmdLine.GetLength() - 1);

	// 获取后4个字符,用于判断扩展名
	CString strExp = cmdLine.Right(4);
	strExp.MakeUpper();

	if(strExp == _T(".DSL") && m_pMainWnd)
	{
		((CMainFrame*)m_pMainWnd)->WaitPower2OpenProject(cmdLine);
	}
}

