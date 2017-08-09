#pragma once
#include "afxcmn.h"
#include "afxwin.h"
#include "DTreeCtrl.h"
#include "PropertySerial.h"

// CSerialSetDlg 对话框
namespace MVC{namespace Device{namespace InterfaceSet{class CDSerial;}}}
namespace Dialog{
class CSerialSetDlg : public CXTResizePropertyPage, public Tool::IDTreeOwner
{
	DECLARE_DYNAMIC(CSerialSetDlg)
private:
	bool m_bInit;							//!< 表示数据是否初始化了

public:
	Tool::CDTreeCtrl m_TreeCtrl;			//!< 串口树
	CStatic m_GridHolder;					//!< 串口表格的载体
	CXTPPropertyGrid m_PropertyGrid;		//!< 显示串口属性的表格

	MVC::Device::InterfaceSet::CPropertySerial m_Object;	//!< 串口的属性对象
	std::list<boost::shared_ptr<MVC::Device::InterfaceSet::CDSerial> > m_ltSerial;

public:
	CSerialSetDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CSerialSetDlg();
	void SaveModify();

// 对话框数据
	enum { IDD = IDD_SERIAL_SET };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	void InitSerialList();								//!< 画串口那个list

public:
	boost::shared_ptr<MVC::Device::InterfaceSet::CDSerial> GetSerial(CString name);
	virtual void OnTreeDblClick(CTreeCtrl*, HTREEITEM){;}
	virtual void OnTreeLClick(CTreeCtrl*, HTREEITEM);
	virtual void OnTreeRClick(CTreeCtrl*, HTREEITEM,CPoint);
	virtual void OnTreeKeyDown(CTreeCtrl*, HTREEITEM,UINT);

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedAddSerial();
	afx_msg void OnBnClickedDelSerial();
	afx_msg LRESULT OnGridNotify(WPARAM, LPARAM);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg BOOL OnHelpInfo(HELPINFO* pHelpInfo);
	virtual BOOL OnInitDialog();
};
}