#pragma once
#include "ItemGrid.h"

// CItemView ��ͼ
namespace MVC{
namespace Item{
class CItemView : public CScrollView
{
	DECLARE_DYNCREATE(CItemView)
public:
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif
private:
	UINT m_FreshTime;					//!< ��¼��һ�±���ˢ�����ݵ�ʱ�䣬�������Ҫ�����趨Timer�����Ϊ���ٶ��Ż�
	int m_printPage;
	BOOL m_isPrinting;
	CRect m_drawRect;

public:
	CItemGrid m_ItemGrid;

protected:
	CItemView();						// ��̬������ʹ�õ��ܱ����Ĺ��캯��
	virtual ~CItemView();

protected:
	virtual void OnDraw(CDC* pDC);      // ��д�Ի��Ƹ���ͼ
	virtual void OnInitialUpdate();     // �����ĵ�һ��
	bool IsActive();					//!< �ж��Լ��ǲ��Ǽ���״̬

	DECLARE_MESSAGE_MAP()
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnEditUndo();
	afx_msg void OnEditRedo();
	afx_msg void OnUpdateEditRedo(CCmdUI *pCmdUI);
	afx_msg void OnUpdateEditUndo(CCmdUI *pCmdUI);

public:
	bool ShowItem(UINT id);				//!< ��ʾ�����ŵı��������û�У�����false
	afx_msg void OnSelectAll();
	afx_msg void OnItemIn();
	afx_msg void OnItemOut();
	afx_msg void OnCurrentOut();
	afx_msg void OnSelectOut();
	afx_msg void OnClone();
	afx_msg void OnItemAdd();
	afx_msg void OnItemEdit();
	afx_msg void OnItemUp();
	afx_msg void OnItemDown();
	afx_msg void OnItemEditAll();
	afx_msg void OnVkdelete();
	afx_msg void OnEditCopy();
	afx_msg void OnEditCut();
	afx_msg void OnEditPaste();
//	afx_msg void OnItemDelete();
	afx_msg void OnItemWrite();
	afx_msg void OnShowHex();
	afx_msg BOOL OnHelpInfo(HELPINFO* pHelpInfo);
	afx_msg void OnUpdateItemWrite(CCmdUI *pCmdUI);
	afx_msg void OnUpdateItemAdd(CCmdUI *pCmdUI);
	afx_msg void OnUpdateItemEdit(CCmdUI *pCmdUI);
	afx_msg void OnUpdateItemEditAll(CCmdUI *pCmdUI);
	afx_msg void OnUpdateItemDelete(CCmdUI *pCmdUI);
	afx_msg void OnUpdateItemUp(CCmdUI *pCmdUI);
	afx_msg void OnUpdateItemDown(CCmdUI *pCmdUI);
	afx_msg void OnUpdateItemIn(CCmdUI *pCmdUI);
	afx_msg void OnUpdateItemClone(CCmdUI *pCmdUI);
	afx_msg void OnUpdateShowHex(CCmdUI *pCmdUI);

protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnPrint(CDC* pDC, CPrintInfo* pInfo);
};
}
}
