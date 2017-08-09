// DCCE_HtmlDialog.cpp : 实现文件
//

#include "stdafx.h"
#include "DXPEditor.h"
#include "MyHelp.h"
#include "Gbl.h"
#include "MainFrm.h"
#include "ProjectMgr.h"
#include "Project.h"
#include "DCCE_HtmlDialog.h"
#include "StartView.h"


// CDCCE_HtmlDialog 对话框
using namespace MVC;
using namespace Start;
IMPLEMENT_DYNCREATE(CDCCE_HtmlDialog, CDHtmlDialog)

CDCCE_HtmlDialog::CDCCE_HtmlDialog(CWnd* pParent /*=NULL*/)
	: CDHtmlDialog(CDCCE_HtmlDialog::IDD, CDCCE_HtmlDialog::IDH, pParent)
{
	m_pStartPageView=NULL;
	m_LocalURL=m_HomeRUL="";//!< 当前网络地址, 主页地址
	m_IsButtonLink=TRUE;	//!< 判断用户是否是点击按钮超链接，导致的网页跳转
	m_pStartPageView=(CStartView *)pParent;
}

CDCCE_HtmlDialog::~CDCCE_HtmlDialog()
{
	ClearBackStack();
	ClearFrontStack();
	g_App.SetStartPage(NULL);
}

void CDCCE_HtmlDialog::DoDataExchange(CDataExchange* pDX)
{
	CDHtmlDialog::DoDataExchange(pDX);
}

BOOL CDCCE_HtmlDialog::OnInitDialog()
{
	SetHostFlags(DOCHOSTUIFLAG_NO3DBORDER);
	CDHtmlDialog::OnInitDialog();
	g_App.SetStartPage(this);

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

BEGIN_MESSAGE_MAP(CDCCE_HtmlDialog, CDHtmlDialog)
END_MESSAGE_MAP()

BEGIN_DHTML_EVENT_MAP(CDCCE_HtmlDialog)
	DHTML_EVENT_ONCLICK(_T("ButtonOK"), OnButtonOK)
	DHTML_EVENT_ONCLICK(_T("ButtonCancel"), OnButtonCancel)
	DHTML_EVENT_ONCLICK(_T("NewProjBtn"),OnNewProj)
	DHTML_EVENT_ONCLICK(_T("OpenProjBtn"),OnOpenProj)
	DHTML_EVENT_ONCLICK(_T("ProjRULList"),OnOpenProjWith)
	DHTML_EVENT_ONCLICK(_T("StartProjDiv"),OnOpenProjWith)
	DHTML_EVENT_ONCLICK(_T("OpenHelpBtn"),OnShowHelp)
END_DHTML_EVENT_MAP()


// CDCCE_HtmlDialog 消息处理程序

HRESULT CDCCE_HtmlDialog::OnButtonOK(IHTMLElement* /*pElement*/)
{
	OnOK();
	return S_OK;  // 除非将焦点设置到控件，否则返回 TRUE
}

HRESULT CDCCE_HtmlDialog::OnButtonCancel(IHTMLElement* /*pElement*/)
{
	OnCancel();
	return S_OK;  // 除非将焦点设置到控件，否则返回 TRUE
}

//通过按钮打开 新建工程窗口
void CDCCE_HtmlDialog::OnNewProj(IHTMLElement *pElement)
{
	((CMainFrame *)g_App.GetMainWnd())->OnProjNew();
	if(g_App.GetStartPage())	g_App.GetStartPage()->UpdateWindow();
}

//通过按钮打开 打开工程窗口
void CDCCE_HtmlDialog::OnOpenProj(IHTMLElement *pElement)
{
	((CMainFrame *)g_App.GetMainWnd())->OnProjOpen();
	if(g_App.GetStartPage())	g_App.GetStartPage()->UpdateWindow();
}

//通过超级连接打开新工程
void CDCCE_HtmlDialog::OnOpenProjWith(IHTMLElement *pElement)
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
void CDCCE_HtmlDialog::OnShowHelp(IHTMLElement *pElement)
{
	SoftInfo::CMyHelp::GetMe().ShowHelp(_T("目录"));
}

void CDCCE_HtmlDialog::OnDocumentComplete(LPDISPATCH pDisp, LPCTSTR szUrl)
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

void CDCCE_HtmlDialog::ClearBackStack()
{
	while(!m_BackStack.empty())
		m_BackStack.pop();
}

void CDCCE_HtmlDialog::ClearFrontStack()
{
	while(!m_FrontStack.empty())
		m_FrontStack.pop();
}

void CDCCE_HtmlDialog::GoBack()
{
	GetCurrentUrl(m_LocalURL);
	Navigate((LPCSTR)m_BackStack.top());
	m_IsButtonLink=TRUE;
	m_FrontStack.push(m_LocalURL);
	m_BackStack.pop();
}

void CDCCE_HtmlDialog::GoForward()
{
	GetCurrentUrl(m_LocalURL);
	Navigate((LPCSTR)m_FrontStack.top());
	m_IsButtonLink=TRUE;
	m_BackStack.push(m_LocalURL);
	m_FrontStack.pop();
}

void CDCCE_HtmlDialog::GoHome()
{
	GetCurrentUrl(m_LocalURL);
	m_BackStack.push(m_LocalURL);
	ClearFrontStack();
	Navigate((LPCTSTR)m_HomeRUL);
}

void CDCCE_HtmlDialog::SetHomeProjItem(CString htmlText)
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

BOOL CDCCE_HtmlDialog::PreTranslateMessage(MSG* pMsg)
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

void MVC::Start::CDCCE_HtmlDialog::OnOK()
{
	// TODO: 在此添加专用代码和/或调用基类

//	CDHtmlDialog::OnOK();
}
