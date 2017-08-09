#pragma once
#define CChildFrameBase CXTPFrameWndBase<CMDIChildWnd>

// CItemFrame ���
namespace MVC{
namespace Item{
class CItemFrame : public CChildFrameBase
{
	DECLARE_DYNCREATE(CItemFrame)
public:
	CXTSplitterWnd m_wndSplitter;

protected:
	CItemFrame();           // ��̬������ʹ�õ��ܱ����Ĺ��캯��
	virtual ~CItemFrame();

protected:
	virtual BOOL OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext);
	virtual BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult);
	DECLARE_MESSAGE_MAP()

public:
	virtual BOOL DestroyWindow();

public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg void OnClose();
	afx_msg void OnMDIActivate(BOOL bActivate, CWnd* pActivateWnd, CWnd* pDeactivateWnd);
public:
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};
}
}
