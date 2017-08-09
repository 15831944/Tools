#pragma once
#include "afxwin.h"


// CFindDlg �Ի���
namespace Dialog{
class CFindDlg : public CDialog
{
	DECLARE_DYNAMIC(CFindDlg)

private:
	CFindDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CFindDlg();

public:
	static CFindDlg& GetMe();

// �Ի�������
	enum { IDD = IDD_FIND_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	void BeginFind();
	void CheckEnabled();

	DECLARE_MESSAGE_MAP()
private:
	std::list<CString> m_ltMatchStr;	//!< �����û��ڲ�������������ı�
	CString m_strMatch;					//!< �û��ϴβ��ҵ��ı�
	std::list<CString> m_ltFindArea;	//!< �û����ܲ��ҵĶ���
	int m_nAreaSel;						//!< �û��ϴβ��ҵĶ���

public:
	CComboBox m_cbFindText;
	CComboBox m_cbFindArea;
	BOOL m_bMatchUpDown;
	BOOL m_bMatchWhole;
	BOOL m_bUseRegex;					//!< �Ƿ�ʹ��������ʽ

// 	CXTNoFlickerWnd <CButton> m_CK_Item_ID;
// 	CXTNoFlickerWnd <CButton> m_CK_Item_Name;
// 	CXTNoFlickerWnd <CButton> m_CK_Item_Tag;
// 	CXTNoFlickerWnd <CButton> m_CK_Item_Desp;
// 	CXTNoFlickerWnd <CButton> m_CK_Item_Src;
// 	CXTNoFlickerWnd <CButton> m_CK_Item_Access;
// 	CXTNoFlickerWnd <CButton> m_CK_Item_ProjType;
// 	CXTNoFlickerWnd <CButton> m_CK_Item_IOType;
// 	CXTNoFlickerWnd <CButton> m_CK_Item_Convert;
// 	CXTNoFlickerWnd <CButton> m_CK_Item_Script;
// 	CXTNoFlickerWnd <CButton> m_CK_Item_Device;
// 	CXTNoFlickerWnd <CButton> m_CK_Item_Addr;
// 	CXTNoFlickerWnd <CButton> m_CK_Item_FreshTime;
// 	CXTNoFlickerWnd <CButton> m_CK_Item_AlarmOnOff;
// 	CXTNoFlickerWnd <CButton> m_CK_Item_AlarmLimit;
// 	CXTNoFlickerWnd <CButton> m_CK_Item_AlarmAim;
// 	CXTNoFlickerWnd <CButton> m_CK_Item_AlarmShift;
// 
// 	CXTNoFlickerWnd <CButton> m_CK_Dev_ID;
// 	CXTNoFlickerWnd <CButton> m_CK_Dev_Name;
// 	CXTNoFlickerWnd <CButton> m_CK_Dev_Tag;
// 	CXTNoFlickerWnd <CButton> m_CK_Dev_Type;
// 	CXTNoFlickerWnd <CButton> m_CK_Dev_Inf;

public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedRegex();
	virtual BOOL OnInitDialog();
	afx_msg BOOL OnHelpInfo(HELPINFO* pHelpInfo);
};
}