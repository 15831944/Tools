#pragma once

#include <stack>
// CDCCE_HtmlDialog �Ի���
namespace MVC{namespace Start{
class CStartView;
class CDCCE_HtmlDialog : public CDHtmlDialog
{
	DECLARE_DYNCREATE(CDCCE_HtmlDialog)

public:
	CString m_LocalURL;			//!< ��ǰ�����ַ
	CString m_HomeRUL;			//!< ��ҳ��ַ
	BOOL m_IsButtonLink;		//!< �ж��û��Ƿ��ǵ����ť�����ӣ����µ���ҳ��ת
	CStartView *m_pStartPageView;

	stack<CString> m_BackStack;	//!< ���˶�ջ
	stack<CString> m_FrontStack;//!< ǰ����ջ

public:
	CDCCE_HtmlDialog(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDCCE_HtmlDialog();
// ��д
	HRESULT OnButtonOK(IHTMLElement *pElement);
	HRESULT OnButtonCancel(IHTMLElement *pElement);
	void OnNewProj(IHTMLElement *pElement);
	void OnOpenProj(IHTMLElement *pElement);
	void OnOpenProjWith(IHTMLElement *pElement);
	void OnShowHelp(IHTMLElement *pElement);

// �Ի�������
	enum { IDD = IDD_DCCE_HTMLDIALOG, IDH = IDR_HTML_DCCE_HTMLDIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
	DECLARE_DHTML_EVENT_MAP()
public:
	virtual void SetHomeProjItem(CString htmlText);
	virtual void GoBack();
	virtual void GoForward();
	virtual void GoHome();
	virtual void ClearFrontStack();
	virtual void ClearBackStack();
	virtual void OnDocumentComplete(LPDISPATCH pDisp, LPCTSTR szUrl);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
protected:
	virtual void OnOK();
};
}}