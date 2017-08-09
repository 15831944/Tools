#pragma once
#include "afxcmn.h"
#include "afxwin.h"
#include "DTreeCtrl.h"
#include "PropertySerial.h"

// CSerialSetDlg �Ի���
namespace MVC{namespace Device{namespace InterfaceSet{class CDSerial;}}}
namespace Dialog{
class CSerialSetDlg : public CXTResizePropertyPage, public Tool::IDTreeOwner
{
	DECLARE_DYNAMIC(CSerialSetDlg)
private:
	bool m_bInit;							//!< ��ʾ�����Ƿ��ʼ����

public:
	Tool::CDTreeCtrl m_TreeCtrl;			//!< ������
	CStatic m_GridHolder;					//!< ���ڱ�������
	CXTPPropertyGrid m_PropertyGrid;		//!< ��ʾ�������Եı��

	MVC::Device::InterfaceSet::CPropertySerial m_Object;	//!< ���ڵ����Զ���
	std::list<boost::shared_ptr<MVC::Device::InterfaceSet::CDSerial> > m_ltSerial;

public:
	CSerialSetDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CSerialSetDlg();
	void SaveModify();

// �Ի�������
	enum { IDD = IDD_SERIAL_SET };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	void InitSerialList();								//!< �������Ǹ�list

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