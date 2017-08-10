#pragma once

//#include "PMHtmlDialog.h"

// CStartView ��ͼ
namespace MVC{namespace Start{
class CPMHtmlDialog;
class CStartView : public CScrollView
{
	DECLARE_DYNCREATE(CStartView)

public:
	std::shared_ptr<CPMHtmlDialog> m_HtmlDlg;

protected:
	BOOL m_isShowHomePage;

protected:
	CStartView();           // ��̬������ʹ�õ��ܱ����Ĺ��캯��
	virtual ~CStartView();

public:
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	virtual void OnDraw(CDC* pDC);      // ��д�Ի��ƴ���ͼ
	virtual void OnInitialUpdate();     // �����ĵ�һ��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnTbPageback();
	afx_msg void OnTbPagefront();
	afx_msg void OnTbPagehome();
	afx_msg void OnUpdateTbPageback(CCmdUI *pCmdUI);
	afx_msg void OnUpdateTbPagefront(CCmdUI *pCmdUI);
	afx_msg void OnUpdateTbPagehome(CCmdUI *pCmdUI);
	virtual BOOL PreTranslateMessage(MSG* pMsg);

public:
	afx_msg void OnPaint();
};
}
}
