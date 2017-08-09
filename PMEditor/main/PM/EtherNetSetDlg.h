#pragma once
#include "PropertyEthernet.h"
#include "DTreeCtrl.h"
#include "afxcmn.h"

// CEtherNetSetDlg �Ի���
namespace Dialog{
class CEtherNetSetDlg : public CXTResizePropertyPage, public Tool::IDTreeOwner
{
	DECLARE_DYNAMIC(CEtherNetSetDlg)
public:
	Tool::CDTreeCtrl m_TreeCtrl;
	CStatic m_GridHolder;					//!< ���ڱ�������
	CXTPPropertyGrid m_PropertyGrid;		//!< ��ʾ�������Եı��
	MVC::Device::InterfaceSet::CPropertyEthernet m_Object;	//!< ���ڵ����Զ���

public:
	CEtherNetSetDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CEtherNetSetDlg();
	virtual void OnTreeDblClick(CTreeCtrl*, HTREEITEM){;}
	virtual void OnTreeLClick(CTreeCtrl*, HTREEITEM);
	virtual void OnTreeRClick(CTreeCtrl*, HTREEITEM,CPoint);
	virtual void OnTreeKeyDown(CTreeCtrl*, HTREEITEM,UINT);

// �Ի�������
	enum { IDD = IDD_ETHERNET_SET };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
	afx_msg LRESULT OnGridNotify(WPARAM, LPARAM);
	virtual BOOL OnInitDialog();
public:
	afx_msg void OnBnClickedAddEthernet();
	afx_msg void OnTvnSelchangedEthernetTree(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg BOOL OnHelpInfo(HELPINFO* pHelpInfo);
};
}