#pragma once

#include "DTreeCtrl.h"

namespace Space{
class CSpaceItem: public CWnd, public Tool::IDTreeOwner
{
private:
	CXTPToolBar m_wndToolBar;						//!< pane�Ϸ���С������
	BOOL m_bShowItemGroup;							//!< ��������ʾ����ʽ��TRUE��ʾ����֣�FALSE��ʾ���豸��

	Tool::CDTreeCtrl m_GroupTree;					//!< ��������
	UINT m_uiCulGroupID;							//!< ��ǰ������ı�����
	HTREEITEM m_AddNew;								//!< ����ڵ㡰����±����顱

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