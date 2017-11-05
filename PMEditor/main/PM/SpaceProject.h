#pragma once

#include "DTreeCtrl.h"
namespace Space{
class CSpaceProject : public CWnd, public Tool::IDTreeOwner
{
public:
	CSpaceProject(void);
	~CSpaceProject(void);

public:
	void UpdateTreeView();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	DECLARE_MESSAGE_MAP()
	virtual void OnTreeDblClick(CTreeCtrl*, HTREEITEM);
	virtual void OnTreeLClick(CTreeCtrl*, HTREEITEM);
	virtual void OnTreeRClick(CTreeCtrl*, HTREEITEM,CPoint);
	virtual void OnTreeKeyDown(CTreeCtrl*, HTREEITEM,UINT);

private:
	Tool::CDTreeCtrl m_ProjectTree;					//!< ������
	CXTPToolBar m_wndToolBar;						//!< �������Ϸ���С������
	afx_msg void OnProjInfo();
	afx_msg void OnProjSave();
	afx_msg void OnAddItem();
	afx_msg void OnUpdateProjSave(CCmdUI *pCmdUI);
	afx_msg void OnUpdateAddItem(CCmdUI *pCmdUI);
	afx_msg BOOL OnHelpInfo(HELPINFO* pHelpInfo);
};
}