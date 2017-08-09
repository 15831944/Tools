#pragma once
#include "afxcmn.h"
#include "DTreeCtrl.h"

// CAddItemGroupDlg 对话框
namespace Dialog{
	class CAddItemGroupDlg : public CDialog, public Tool::IDTreeOwner
{
	DECLARE_DYNAMIC(CAddItemGroupDlg)
public:
	CString m_strGroupName;
	CString m_strParentName;
	UINT m_uiParentID;

private:
	CString m_strOldGroupName;
	Tool::CDTreeCtrl m_GroupTree;
	bool m_bShowTree;
	HTREEITEM m_CulItem;

	UINT m_uiGroupID;							//!< 如果是修改组信息，这里记录了组的ID号

private:
	CAddItemGroupDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CAddItemGroupDlg();
	void AddGroupChild(HTREEITEM root, UINT parentID);
	void SetParentName(CString name);

public:
	static CAddItemGroupDlg& GetMe();

// 对话框数据
	enum { IDD = IDD_ADD_ITEMGROUP };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnBnClickedBtShowtree();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedOk();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg BOOL OnHelpInfo(HELPINFO* pHelpInfo);

	virtual INT_PTR DoModal(CString parentName = _T("System"), UINT groupID = -1);
	virtual BOOL OnInitDialog();
	virtual void OnTreeDblClick(CTreeCtrl*, HTREEITEM);
	virtual void OnTreeLClick(CTreeCtrl*, HTREEITEM);
	virtual void OnTreeRClick(CTreeCtrl*, HTREEITEM,CPoint);
	virtual void OnTreeKeyDown(CTreeCtrl*, HTREEITEM,UINT);
	};
}