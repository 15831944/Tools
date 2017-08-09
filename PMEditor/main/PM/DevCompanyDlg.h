#pragma once
#include "afxcmn.h"
#include "DTreeCtrl.h"

// CDevCompanyDlg 对话框
namespace Dialog{
class CDevCompanyDlg : public CDialog, public Tool::IDTreeOwner
{
	DECLARE_DYNAMIC(CDevCompanyDlg)
private:
	UINT m_uiDevID;						//!< 说选择的设备ID，如果没有这是-1

public:
	CString m_strCompany;
	CString m_strDevType;
	CString m_strDevDescription;
	Tool::CDTreeCtrl m_TreeCtrl;

public:
	CDevCompanyDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDevCompanyDlg();

	virtual void OnTreeDblClick(CTreeCtrl*, HTREEITEM){;}
	virtual void OnTreeLClick(CTreeCtrl*, HTREEITEM);
	virtual void OnTreeRClick(CTreeCtrl*, HTREEITEM,CPoint);
	virtual void OnTreeKeyDown(CTreeCtrl*, HTREEITEM,UINT);

	UINT GetDevID(){return m_uiDevID;}

// 对话框数据
	enum { IDD = IDD_DEVICE_COMPANY_SHOW };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	afx_msg BOOL OnHelpInfo(HELPINFO* pHelpInfo);
};
}