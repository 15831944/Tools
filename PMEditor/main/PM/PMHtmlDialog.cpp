// PMHtmlDialog.cpp : ʵ���ļ�
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


// CPMHtmlDialog �Ի���
using namespace MVC;
using namespace Start;
IMPLEMENT_DYNCREATE(CPMHtmlDialog, CDHtmlDialog)

CPMHtmlDialog::CPMHtmlDialog(CWnd* pParent /*=NULL*/)
	: CDHtmlDialog(CPMHtmlDialog::IDD, CPMHtmlDialog::IDH, pParent)
{
	m_pStartPageView=NULL;
	m_LocalURL=m_HomeRUL="";//!< ��ǰ�����ַ, ��ҳ��ַ
	m_IsButtonLink=TRUE;	//!< �ж��û��Ƿ��ǵ����ť�����ӣ����µ���ҳ��ת
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

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
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


// CPMHtmlDialog ��Ϣ�������

HRESULT CPMHtmlDialog::OnButtonOK(IHTMLElement* /*pElement*/)
{
	OnOK();
	return S_OK;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

HRESULT CPMHtmlDialog::OnButtonCancel(IHTMLElement* /*pElement*/)
{
	OnCancel();
	return S_OK;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

//ͨ����ť�� �½����̴���
void CPMHtmlDialog::OnNewProj(IHTMLElement *pElement)
{
	((CMainFrame *)g_App.GetMainWnd())->OnProjNew();
	if(g_App.GetStartPage())	g_App.GetStartPage()->UpdateWindow();
}

//ͨ����ť�� �򿪹��̴���
void CPMHtmlDialog::OnOpenProj(IHTMLElement *pElement)
{
	((CMainFrame *)g_App.GetMainWnd())->OnProjOpen();
	if(g_App.GetStartPage())	g_App.GetStartPage()->UpdateWindow();
}

//ͨ���������Ӵ��¹���
void CPMHtmlDialog::OnOpenProjWith(IHTMLElement *pElement)
{
	BSTR bstr;
	VARIANT values;
	if(pElement != NULL){
		bstr = CString("hrefs").AllocSysString();
		pElement->getAttribute(bstr, 0, &values);
	}
	if(values.vt != 1 && values.puiVal != NULL){
 		CString fileName, fileNameCpl, oldProjFile;			// fileNameCplֻ��Ϊ���ļ��Ƚ�֮��
		fileName = fileNameCpl = CString(values.bstrVal);
		if(CProjectMgr::GetMe().GetProj())	oldProjFile = CProjectMgr::GetMe().GetProj()->GetWholePathName();
		if(oldProjFile.MakeUpper() == fileNameCpl.MakeUpper())
			return;
		CProjectMgr::GetMe().OpenProject(fileName);
	}
	pElement=NULL;
	if(g_App.GetStartPage()){				//!< ˢһ���Լ�����������
		g_App.GetStartPage()->SetFocus();
		g_App.GetStartPage()->UpdateWindow();
	}	
	return;
}

//�򿪰����ĵ�
void CPMHtmlDialog::OnShowHelp(IHTMLElement *pElement)
{
	SoftInfo::CMyHelp::GetMe().ShowHelp(_T("Ŀ¼"));
}

void CPMHtmlDialog::OnDocumentComplete(LPDISPATCH pDisp, LPCTSTR szUrl)
{
	if(m_LocalURL==""){		//������ʼ��ҳ�棬���ô���ʲô
		m_LocalURL = szUrl;
		return;
	}
	if(((CString)szUrl).Find("#")!=-1){
		return;
	}
	if(!m_IsButtonLink){	//�û���������ӣ���ɵ���ҳ��ת
		m_BackStack.push(m_LocalURL);
		m_LocalURL = szUrl;
		ClearFrontStack();
	}
	else{					//�û������ť���ӣ���ɵ���ҳ��ת
		m_IsButtonLink=FALSE;
		m_LocalURL=szUrl;
	}
	CDHtmlDialog::OnDocumentComplete(pDisp, szUrl);
	// TODO: �ڴ����ר�ô����/����û���
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
	// TODO: �ڴ����ר�ô����/����û���
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
	// TODO: �ڴ����ר�ô����/����û���

//	CDHtmlDialog::OnOK();
}
