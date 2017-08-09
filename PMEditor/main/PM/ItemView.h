#pragma once
#include "ItemGrid.h"

// CItemView 视图
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
	UINT m_FreshTime;					//!< 记录了一下本类刷新数据的时间，如果变了要重新设定Timer，这个为了速度优化
	int m_printPage;
	BOOL m_isPrinting;
	CRect m_drawRect;

public:
	CItemGrid m_ItemGrid;

protected:
	CItemView();						// 动态创建所使用的受保护的构造函数
	virtual ~CItemView();

protected:
	virtual void OnDraw(CDC* pDC);      // 重写以绘制该视图
	virtual void OnInitialUpdate();     // 构造后的第一次
	bool IsActive();					//!< 判断自己是不是激活状态

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
	bool ShowItem(UINT id);				//!< 显示这个编号的变量，如果没有，返回false
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
