#pragma once
#include "CamGrid.h"

// CCamView 视图
namespace MVC{
namespace Camera{
class CCamView : public CScrollView
{
	DECLARE_DYNCREATE(CCamView)
public:
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif
private:
	int m_printPage;
	BOOL m_isPrinting;
	CRect m_drawRect;

public:
	CCamGrid m_ItemGrid;

protected:
	CCamView();						// 动态创建所使用的受保护的构造函数
	virtual ~CCamView();

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
	afx_msg void OnItemAdd();
	afx_msg void OnItemEdit();
	afx_msg void OnItemUp();
	afx_msg void OnItemDown();
//	afx_msg void OnItemEditAll();
	afx_msg void OnVkdelete();
	afx_msg void OnEditCopy();
	afx_msg void OnEditCut();
	afx_msg void OnEditPaste();
	afx_msg BOOL OnHelpInfo(HELPINFO* pHelpInfo);
	afx_msg void OnUpdateItemAdd(CCmdUI *pCmdUI);
	afx_msg void OnUpdateItemEdit(CCmdUI *pCmdUI);
//	afx_msg void OnUpdateItemEditAll(CCmdUI *pCmdUI);
	afx_msg void OnUpdateItemDelete(CCmdUI *pCmdUI);
	afx_msg void OnUpdateItemUp(CCmdUI *pCmdUI);
	afx_msg void OnUpdateItemDown(CCmdUI *pCmdUI);
	afx_msg void OnUpdateItemIn(CCmdUI *pCmdUI);

protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnPrint(CDC* pDC, CPrintInfo* pInfo);
};
}
}
