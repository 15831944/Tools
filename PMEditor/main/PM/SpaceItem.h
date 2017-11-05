#pragma once

#include "DTreeCtrl.h"

namespace Space{
class CSpaceItem: public CWnd, public Tool::IDTreeOwner
{
private:
	CXTPToolBar m_wndToolBar;						//!< pane上方的小工具栏
	BOOL m_bShowItemGroup;							//!< 变量树显示排序方式，TRUE表示按组分，FALSE表示按设备分

	Tool::CDTreeCtrl m_GroupTree;					//!< 变量组树
	UINT m_uiCulGroupID;							//!< 当前被激活的变量组
	HTREEITEM m_AddNew;								//!< 特殊节点“添加新变量组”

public:
	CSpaceItem(void);
	~CSpaceItem(void);

public:
	void UpdateTreeView();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnGroupAdd();
	afx_msg void OnGroupEdit();
	afx_msg void OnGroupRemove();
	afx_msg void OnItemGroup();
	afx_msg BOOL OnHelpInfo(HELPINFO* pHelpInfo);

	afx_msg void OnUpdateItemGroup(CCmdUI *pCmdUI);
	DECLARE_MESSAGE_MAP()

private:
	void IntoGroup(UINT groupid);
	void AddGroupChild(HTREEITEM root, UINT parentID);
	void OnTreeDblClick(CTreeCtrl* pTreeCtr, HTREEITEM hItem);
	void OnTreeLClick(CTreeCtrl* pTreeCtr, HTREEITEM hItem);
	void OnTreeRClick(CTreeCtrl* pTreeCtr, HTREEITEM hItem, CPoint point);
	void OnTreeKeyDown(CTreeCtrl* pTreeCtr, HTREEITEM hItem, UINT nChar);
// 	afx_msg void OnProjInfo();
};
}