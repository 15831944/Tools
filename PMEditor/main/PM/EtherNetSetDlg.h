#pragma once
#include "PropertyEthernet.h"
#include "DTreeCtrl.h"
#include "afxcmn.h"

// CEtherNetSetDlg 对话框
namespace Dialog{
class CEtherNetSetDlg : public CXTResizePropertyPage, public Tool::IDTreeOwner
{
	DECLARE_DYNAMIC(CEtherNetSetDlg)
public:
	Tool::CDTreeCtrl m_TreeCtrl;
	CStatic m_GridHolder;					//!< 串口表格的载体
	CXTPPropertyGrid m_PropertyGrid;		//!< 显示串口属性的表格
	MVC::Device::InterfaceSet::CPropertyEthernet m_Object;	//!< 串口的属性对象

public:
	CEtherNetSetDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CEtherNetSetDlg();
	virtual void OnTreeDblClick(CTreeCtrl*, HTREEITEM){;}
	virtual void OnTreeLClick(CTreeCtrl*, HTREEITEM);
	virtual void OnTreeRClick(CTreeCtrl*, HTREEITEM,CPoint);
	virtual void OnTreeKeyDown(CTreeCtrl*, HTREEITEM,UINT);

// 对话框数据
	enum { IDD = IDD_ETHERNET_SET };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
	afx_msg LRESULT OnGridNotify(WPARAM, LPARAM);
	virtual BOOL OnInitDialog();
public:
	afx_msg void OnBnClickedAddEthernet();
	afx_msg void OnTvnSelchangedEthernetTree(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg BOOL OnHelpInfo(HELPINFO* pHelpInfo);
};
}