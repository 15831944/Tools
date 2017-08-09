#pragma once


// CStartFrm ���
namespace MVC{namespace Start{
class CStartFrm : public CMDIChildWnd
{
	DECLARE_DYNCREATE(CStartFrm)
public:
	CReBar			m_wndReBar;        //Rebar��
#undef CToolBar
	CToolBar		m_wndToolBar;
protected:
	CStartFrm();           // ��̬������ʹ�õ��ܱ����Ĺ��캯��
	virtual ~CStartFrm();

protected:
	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL CreateEx(DWORD dwExStyle, LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, LPVOID lpParam = NULL);
	virtual BOOL LoadFrame(UINT nIDResource, DWORD dwDefaultStyle = WS_OVERLAPPEDWINDOW | FWS_ADDTOTITLE, CWnd* pParentWnd = NULL, CCreateContext* pContext = NULL);
	virtual void ActivateFrame(int nCmdShow = -1);
	afx_msg void OnMDIActivate(BOOL bActivate, CWnd* pActivateWnd, CWnd* pDeactivateWnd);
	afx_msg void OnClose();
};
}}