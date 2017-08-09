#pragma once
#include "XTooltip.h"
// CDeviceMapView ��ͼ

namespace MVC{
namespace Device{
class CDeviceMapDoc;
class CDeviceOne;
class CDeviceMapView : public CScrollView
{
	DECLARE_DYNCREATE(CDeviceMapView)
private:
	CString m_strInWord;			//!< ��¼���̵�����������ʵ�֮��
	CPen m_Pen;
	CFont m_Font;
	bool m_bHostLineSelect;			//!< Ŀǰ�Ƿ񱻸�����
	COleDropTarget m_DropTarget;	//!< Ϊ�˼���view��Drop����
	bool m_bDropNewIn;				//!< ��ǰ�Ƿ���drag״̬��������豸���Ƿ�Ҫ��DropͼƬ
	UINT m_uiDropDevType;			//!< ��ǰ����drag״̬�µ��豸����
	bool m_bDropOldIn;				//!< �ƶ��豸�ı�ʾ��ֻ�����ʱ��Ч���ƶ��Ժ��Ҫ��m_DropDevice��
	Tool::CXTooltip m_ToolTip;		//!< ������ʾ����

	CString m_strHeader;			//!< ��������
	CString m_strBody;				//!< ��������
	CString m_strFooter;			//!< �ײ��ı�
	CString m_strHtml;				//!< �������ı�

protected:
	CDeviceMapView();				//!< ��̬������ʹ�õ��ܱ����Ĺ��캯��
	virtual ~CDeviceMapView();

public:
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	virtual void OnDraw(CDC* pDC);			//!< ��д�Ի��Ƹ���ͼ
	virtual void OnDrawHostLine(CDC* pDC);	//!< ��ĸ��
	virtual void OnInitialUpdate();			//!< �����ĵ�һ��
	virtual void OnDrawInit(CDC* pDC);		//!< ����ʼ��״̬
	virtual void OnUpdate(CView* /*pSender*/, LPARAM /*lHint*/, CObject* /*pHint*/);

	bool IsInHostLineRect(CPoint point);
	void ClearSelect();						//!< ȡ�������豸��ѡ��״̬
	bool IfDevCanLink();					//!< ������ǰ����ק���豸�뱻ѡ�е��豸�Ƿ�������
	boost::shared_ptr<CDeviceOne> GetSelectDev(bool bProj = true);	//!< �ҵ���ѡ�е��豸��������ʾ�Ƿ�Ҫ�����豸
	boost::shared_ptr<CDeviceOne> GetLTDev();			//!< ����󶥵��Ǹ��豸
	virtual void OnSelectUp();				//!< ѡ��������豸
	virtual void OnSelectDown();			//!< ѡ��������豸
	virtual void OnSelectLeft();			//!< ѡ��������豸
	virtual void OnSelectRight();			//!< ѡ��������豸

public:
	void FreshDeviceMap();					//!< ���»�һ���豸����
	void OnAddDev();						//!< ����豸�����û�б�ѡ�е�����Ӹ��豸��������Ӵ��豸
	void OnAddMainDev(UINT devID);			//!< ������豸�������Ѿ��ƶ����ĸ��豸
	void OnAddSlave(UINT devID);			//!< ��Ӵ��豸�������Ѿ��ƶ����ĸ��豸
	void OnAddScan(UINT devID);				//!< ������Ϊ�����豸
	void ShowDeviceAt(UINT devID);			//!< ѡ������豸������ʾ����豸
	void ShowPoint(CPoint pt);				//!< ��ʾ�����

	DECLARE_MESSAGE_MAP()
public:
	CDeviceMapDoc* GetDocument();
	afx_msg void OnPaint();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnDestroy();
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnAddMainDev();
	afx_msg void OnAddSlave();
	afx_msg void OnDevinterfaceSet();
	afx_msg void OnDevDelete();
	afx_msg void OnDevParainfo();
	afx_msg void OnDevOut();
	afx_msg void OnDevIn();
	afx_msg void OnEditCopy();
	afx_msg void OnEditCopyWithChild();
	afx_msg void OnEditCut();
	afx_msg void OnEditPaste();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg LRESULT OnHyperlinkClicked(WPARAM wParam, LPARAM lParam);
	afx_msg BOOL OnMouseMove(UINT nFlags, CPoint point);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnInterfaceSet();
	afx_msg void OnZoom(UINT index);
	afx_msg void OnEditUndo();
	afx_msg void OnEditRedo();
	afx_msg void OnAddScan();
	afx_msg void OnScanSet();
	afx_msg void OnScanStart();
	afx_msg void OnScanStop();
	afx_msg void OnH();
	afx_msg void OnV();
	afx_msg void OnAddScanInf2Proj();
	afx_msg void OnFreshState();
	afx_msg BOOL OnHelpInfo(HELPINFO* pHelpInfo);

	afx_msg void OnUpdateAddMainDev(CCmdUI *pCmdUI);
	afx_msg void OnUpdateAddSlave(CCmdUI *pCmdUI);
	afx_msg void OnUpdateDevInfo(CCmdUI *pCmdUI);
	afx_msg void OnUpdateInterfaceSet(CCmdUI *pCmdUI);
	afx_msg void OnUpdateDelete(CCmdUI *pCmdUI);
	afx_msg void OnUpdateParainfo(CCmdUI *pCmdUI);
	afx_msg void OnUpdateDevOut(CCmdUI *pCmdUI);
	afx_msg void OnUpdateDevIn(CCmdUI *pCmdUI);
	afx_msg void OnUpdateEditCopy(CCmdUI *pCmdUI);
	afx_msg void OnUpdateEditCopyWithChild(CCmdUI *pCmdUI);
	afx_msg void OnUpdateEditCut(CCmdUI *pCmdUI);
	afx_msg void OnUpdateEditPaste(CCmdUI *pCmdUI);
	afx_msg void OnUpdateEditUndo(CCmdUI *pCmdUI);
	afx_msg void OnUpdateEditRedo(CCmdUI *pCmdUI);

	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual DROPEFFECT OnDragOver(COleDataObject* pDataObject, DWORD dwKeyState, CPoint point);
	virtual BOOL OnDrop(COleDataObject* pDataObject, DROPEFFECT dropEffect, CPoint point);
	virtual DROPEFFECT OnDragEnter(COleDataObject* pDataObject, DWORD dwKeyState, CPoint point);
	virtual void OnDragLeave();

protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
public:
	afx_msg void OnProgram();
public:
	afx_msg void OnUpdateProgram(CCmdUI *pCmdUI);
};
}
}
