// DXPEditor.cpp : ����Ӧ�ó��������Ϊ��
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

// Ψһ��һ�� CDXPEditorApp ����
CDXPEditorApp g_App;

// CDXPEditorApp ����
CDXPEditorApp::CDXPEditorApp()
{
	m_StartPage = NULL;				/**< ��ǰ����ʼҳDialogָ�� */
	// ��������Ҫ�ĳ�ʼ�������� InitInstance ��
}

// CDXPEditorApp ��ʼ��
BOOL CDXPEditorApp::InitInstance()
{
	// ���һ�������� Windows XP �ϵ�Ӧ�ó����嵥ָ��Ҫ
	// ʹ�� ComCtl32.dll �汾 6 ����߰汾�����ÿ��ӻ���ʽ��
	//����Ҫ InitCommonControls()�����򣬽��޷��������ڡ�
#ifndef _DEBUG
	CDCCE_Splash::Init(5);
	CDCCE_Splash::StepOne(_T(""));//���ڳ�ʼ��..."));
#endif

	InitCommonControls();
	CWinApp::InitInstance();
	AfxInitRichEdit2();
	AfxEnableControlContainer();

	// ��׼��ʼ��
	SetRegistryKey(_T("Ӧ�ó��������ɵı���Ӧ�ó���"));
	LoadStdProfileSettings(6);  // ���ر�׼ INI �ļ�ѡ��(���� MRU)

	m_pMainWnd = NULL;

	// ע��Ӧ�ó�����ĵ�ģ�塣�ĵ�ģ��
	// �������ĵ�����ܴ��ں���ͼ֮�������
 	m_pStartDocMgr = new CMultiDocTemplate(IDR_MAINMENU,
		RUNTIME_CLASS(MVC::Start::CStartDoc),
		RUNTIME_CLASS(MVC::Start::CStartFrm), // �Զ��� MDI �ӿ��
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

	//!< װ��������Ϣ
	m_XmlMgr = boost::shared_ptr<XmlInfo::CXmlMgr>(new XmlInfo::CXmlMgr);
	if(!m_XmlMgr->OpenXml()){
		CDCCE_Splash::Hide();
		AfxMessageBox("���������ļ�ʧ�ܣ�");
		return FALSE;
	}
	if(CDCCE_Splash::Visible())
	{
		Sleep(300);
		CDCCE_Splash::StepOne(_T(""));
	}

	//!< ��ʼ��ȫ�ֱ���
	SoftInfo::CSoftInfo::GetMe().LoadFile();

	//!< ��ʼ��Ȩ��
	Servers::DXP::CServerCtrl::GetMe();				//!< �ȵ�һ��,�������߳��еõ�ʱ����ɴ���

	// ������ MDI ��ܴ���
	CMainFrame* pMainFrame = new CMainFrame;
	if (!pMainFrame || !pMainFrame->LoadFrame(IDR_MAINFRAME))
		return FALSE;
	m_pMainWnd = pMainFrame;

	pMainFrame->ShowWindow(SW_SHOWMAXIMIZED);

//	CCommandLineInfo cmdInfo;
//	ParseCommandLine(cmdInfo);
//	if (!ProcessShellCommand(cmdInfo))
	pMainFrame->UpdateWindow();

	//!< ����ʼҳ
//	ShowStartPage();

	//!< ��ʼ��ͨ�ŷ�����
	Servers::DXP::CServerCtrl::GetMe().InitServerMgr(m_pMainWnd);

	//!< ��ʼ��HMIͨ����
	Servers::HMI::CHmi::GetMe().InitHmiMgr(m_pMainWnd);

	//!< ��ʼ�������ĵ�
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
	//! ֻ�й����ļ�������ӵ�������б���
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

//!< ����ʼҳ
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

// �������жԻ����Ӧ�ó�������
void CDXPEditorApp::OnAppAbout()
{
	Dialog::CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

BOOL CDXPEditorApp::PreTranslateMessage(MSG* pMsg)
{
	return CWinApp::PreTranslateMessage(pMsg);
}

//!< ���ע��
void CDXPEditorApp::RegistSoftware()
{
	Servers::DXP::CServerCtrl::GetMe().RegistSoft();
}

//!< ����������
void CDXPEditorApp::ReadCmdLine(CString cmdLine)
{
	// ȥ���ַ������ߵ�"��
	if(cmdLine.Right(1) == _T("\""))	cmdLine = cmdLine.Left(cmdLine.GetLength() - 1);
	if(cmdLine.Left(1) == _T("\""))		cmdLine = cmdLine.Right(cmdLine.GetLength() - 1);

	// ��ȡ��4���ַ�,�����ж���չ��
	CString strExp = cmdLine.Right(4);
	strExp.MakeUpper();

	if(strExp == _T(".DSL") && m_pMainWnd)
	{
		((CMainFrame*)m_pMainWnd)->WaitPower2OpenProject(cmdLine);
	}
}

