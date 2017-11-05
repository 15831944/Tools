#pragma once
#include "Resource.h"

#define CPropertyGridDlgBase CXTResizeDialog
namespace Dialog{
// CPropertyDlg 对话框
class CObjectInProperty;
class CPropertyDlg : public CPropertyGridDlgBase
{
//	DECLARE_DYNAMIC(CPropertyDlg)
private:
	CStatic m_GridHolder;
	CXTPPropertyGrid m_PropertyGrid;
	HICON m_hIcon;
	CObjectInProperty* m_Object;						// 要显示的内容
	CRect m_Rect;										// 记录自己的大小，以便内容调整

protected:
	CPropertyDlg(CWnd* pParent = NULL);					// 标准构造函数
	virtual ~CPropertyDlg();
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

public:
// 对话框数据
	enum { IDD = IDD_PROPERTY_DLG };

	DECLARE_MESSAGE_MAP()
protected:
	virtual void OnOK();
	virtual void OnCancel();

public:
	static CPropertyDlg& GetMe();
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedApplay();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg LRESULT OnGridNotify(WPARAM, LPARAM);
	afx_msg BOOL OnHelpInfo(HELPINFO* pHelpInfo);
	virtual INT_PTR DoModal(CObjectInProperty& object);	// 参数是需要显示的内容
	virtual BOOL DestroyWindow();
};
}
