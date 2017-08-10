// PMHtmlDialog.cpp : 实现文件
//

#include "stdafx.h"
#include "PMApp.h"
#include "MyHelp.h"
#include "Gbl.h"
#include "MainFrm.h"
#include "ProjectMgr.h"
#include "Project.h"
#include "PMHtmlDialog.h"
#include "StartView.h"


// CPMHtmlDialog 对话框
using namespace MVC;
using namespace Start;
IMPLEMENT_DYNCREATE(CPMHtmlDialog, CDHtmlDialog)

CPMHtmlDialog::CPMHtmlDialog(CWnd* pParent /*=NULL*/)
	: CDHtmlDialog(CPMHtmlDialog::IDD, CPMHtmlDialog::IDH, pParent)
{
	m_pStartPageView=NULL;
	m_LocalURL=m_HomeRUL="";//!< 当前网络地址, 主页地址
	m_IsButtonLink=TRUE;	//!< 判断用户是否是点击按钮超链接，导致的网页跳转
	m_pStartPageView=(CStartView *)pParent;
}

CPMHtmlDialog::~CPMHtmlDialog()
{
	ClearBackStack();
	ClearFrontStack();
	g_App.SetStartPage(NULL);
}

void CPMHtmlDialog::DoDataExchange(CDataExchange* pDX)
{
	CDHtmlDialog::DoDataExchange(pDX);
}

BOOL CPMHtmlDialog::OnInitDialog()
{
	SetHostFlags(DOCHOSTUIFLAG_NO3DBORDER);
	CDHtmlDialog::OnInitDialog();
	g_App.SetStartPage(this);

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

BEGIN_MESSAGE_MAP(CPMHtmlDialog, CDHtmlDialog)
END_MESSAGE_MAP()

BEGIN_DHTML_EVENT_MAP(CPMHtmlDialog)
	DHTML_EVENT_ONCLICK(_T("ButtonOK"), OnButtonOK)
	DHTML_EVENT_ONCLICK(_T("ButtonCancel"), OnButtonCancel)
	DHTML_EVENT_ONCLICK(_T("NewProjBtn"),OnNewProj)
	DHTML_EVENT_ONCLICK(_T("OpenProjBtn"),OnOpenProj)
	DHTML_EVENT_ONCLICK(_T("ProjRULList"),OnOpenProjWith)
	DHTML_EVENT_ONCLICK(_T("StartProjDiv"),OnOpenProjWith)
	DHTML_EVENT_ONCLICK(_T("OpenHelpBtn"),OnShowHelp)
END_DHTML_EVENT_MAP()


// CPMHtmlDialog 消息处理程序

HRESULT CPMHtmlDialog::OnButtonOK(IHTMLElement* /*pElement*/)
{
	OnOK();
	return S_OK;  // 除非将焦点设置到控件，否则返回 TRUE
}

HRESULT CPMHtmlDialog::OnButtonCancel(IHTMLElement* /*pElement*/)
{
	OnCancel();
	return S_OK;  // 除非将焦点设置到控件，否则返回 TRUE
}

//通过按钮打开 新建工程窗口
void CPMHtmlDialog::OnNewProj(IHTMLElement *pElement)
{
	((CMainFrame *)g_App.GetMainWnd())->OnProjNew();
	if(g_App.GetStartPage())	g_App.GetStartPage()->UpdateWindow();
}

//通过按钮打开 打开工程窗口
void CPMHtmlDialog::OnOpenProj(IHTMLElement *pElement)
{
	((CMainFrame *)g_App.GetMainWnd())->OnProjOpen();
	if(g_App.GetStartPage())	g_App.GetStartPage()->UpdateWindow();
}

//通过超级连接打开新工程
void CPMHtmlDialog::OnOpenProjWith(IHTMLElement *pElement)
{
	BSTR bstr;
	VARIANT values;
	if(pElement != NULL){
		bstr = CString("hrefs").AllocSysString();
		pElement->getAttribute(bstr, 0, &values);
	}
	if(values.vt != 1 && values.puiVal != NULL){
 		CString fileName, fileNameCpl, oldProjFile;			// fileNameCpl只是为做文件比较之用
		fileName = fileNameCpl = CString(values.bstrVal);
		if(CProjectMgr::GetMe().GetProj())	oldProjFile = CProjectMgr::GetMe().GetProj()->GetWholePathName();
		if(oldProjFile.MakeUpper() == fileNameCpl.MakeUpper())
			return;
		CProjectMgr::GetMe().OpenProject(fileName);
	}
	pElement=NULL;
	if(g_App.GetStartPage()){				//!< 刷一下自己，以免变白屏
		g_App.GetStartPage()->SetFocus();
		g_App.GetStartPage()->UpdateWindow();
	}	
	return;
}

//打开帮助文档
void CPMHtmlDialog::OnShowHelp(IHTMLElement *pElement)
{
	SoftInfo::CMyHelp::GetMe().ShowHelp(_T("目录"));
}

void CPMHtmlDialog::OnDocumentComplete(LPDISPATCH pDisp, LPCTSTR szUrl)
{
	if(m_LocalURL==""){		//如果是最开始的页面，不用处理什么
		m_LocalURL = szUrl;
		return;
	}
	if(((CString)szUrl).Find("#")!=-1){
		return;
	}
	if(!m_IsButtonLink){	//用户点击超链接，造成的网页跳转
		m_BackStack.push(m_LocalURL);
		m_LocalURL = szUrl;
		ClearFrontStack();
	}
	else{					//用户点击按钮链接，造成的网页跳转
		m_IsButtonLink=FALSE;
		m_LocalURL=szUrl;
	}
	CDHtmlDialog::OnDocumentComplete(pDisp, szUrl);
	// TODO: 在此添加专用代码和/或调用基类
}

void CPMHtmlDialog::ClearBackStack()
{
	while(!m_BackStack.empty())
		m_BackStack.pop();
}

void CPMHtmlDialog::ClearFrontStack()
{
	while(!m_FrontStack.empty())
		m_FrontStack.pop();
}

void CPMHtmlDialog::GoBack()
{
	GetCurrentUrl(m_LocalURL);
	Navigate((LPCSTR)m_BackStack.top());
	m_IsButtonLink=TRUE;
	m_FrontStack.push(m_LocalURL);
	m_BackStack.pop();
}

void CPMHtmlDialog::GoForward()
{
	GetCurrentUrl(m_LocalURL);
	Navigate((LPCSTR)m_FrontStack.top());
	m_IsButtonLink=TRUE;
	m_BackStack.push(m_LocalURL);
	m_FrontStack.pop();
}

void CPMHtmlDialog::GoHome()
{
	GetCurrentUrl(m_LocalURL);
	m_BackStack.push(m_LocalURL);
	ClearFrontStack();
	Navigate((LPCTSTR)m_HomeRUL);
}

void CPMHtmlDialog::SetHomeProjItem(CString htmlText)
{
	IHTMLElement *pElement=NULL;
	GetElement("StartProjDiv",&pElement);
	BSTR bstr;
	BSTR bstrHtml,bstrText;
	if(pElement!=NULL){
		bstr=htmlText.AllocSysString();
		pElement->get_innerHTML(&bstrHtml);
		pElement->get_innerText(&bstrText);
		pElement->put_innerHTML(bstr);
	}
}

BOOL CPMHtmlDialog::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类
	switch(pMsg->message){
		case WM_RBUTTONDOWN:
		case WM_RBUTTONUP:
		case WM_RBUTTONDBLCLK:
			return TRUE;
		default:
			return CDHtmlDialog::PreTranslateMessage(pMsg);
	}
	return CDHtmlDialog::PreTranslateMessage(pMsg);
}

void MVC::Start::CPMHtmlDialog::OnOK()
{
	// TODO: 在此添加专用代码和/或调用基类

//	CDHtmlDialog::OnOK();
}
