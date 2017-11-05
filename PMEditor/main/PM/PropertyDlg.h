#pragma once
#include "Resource.h"

#define CPropertyGridDlgBase CXTResizeDialog
namespace Dialog{
// CPropertyDlg �Ի���
class CObjectInProperty;
class CPropertyDlg : public CPropertyGridDlgBase
{
//	DECLARE_DYNAMIC(CPropertyDlg)
private:
	CStatic m_GridHolder;
	CXTPPropertyGrid m_PropertyGrid;
	HICON m_hIcon;
	CObjectInProperty* m_Object;						// Ҫ��ʾ������
	CRect m_Rect;										// ��¼�Լ��Ĵ�С���Ա����ݵ���

protected:
	CPropertyDlg(CWnd* pParent = NULL);					// ��׼���캯��
	virtual ~CPropertyDlg();
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

public:
// �Ի�������
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
	virtual INT_PTR DoModal(CObjectInProperty& object);	// ��������Ҫ��ʾ������
	virtual BOOL DestroyWindow();
};
}
