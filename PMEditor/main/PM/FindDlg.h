#pragma once
#include "afxwin.h"


// CFindDlg 对话框
namespace Dialog{
class CFindDlg : public CDialog
{
	DECLARE_DYNAMIC(CFindDlg)

private:
	CFindDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CFindDlg();

public:
	static CFindDlg& GetMe();

// 对话框数据
	enum { IDD = IDD_FIND_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	void BeginFind();
	void CheckEnabled();

	DECLARE_MESSAGE_MAP()
private:
	std::list<CString> m_ltMatchStr;	//!< 所有用户在查找中输入过的文本
	CString m_strMatch;					//!< 用户上次查找的文本
	std::list<CString> m_ltFindArea;	//!< 用户所能查找的对象
	int m_nAreaSel;						//!< 用户上次查找的对象

public:
	CComboBox m_cbFindText;
	CComboBox m_cbFindArea;
	BOOL m_bMatchUpDown;
	BOOL m_bMatchWhole;
	BOOL m_bUseRegex;					//!< 是否使用正则表达式

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