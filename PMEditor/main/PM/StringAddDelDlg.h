#pragma once
#include "afxwin.h"

// CStringAddDelDlg 对话框
namespace Dialog{
class CStringAddDelDlg : public CDialog
{
	DECLARE_DYNAMIC(CStringAddDelDlg)
public:
	CListBox m_OutList;				//!< 可添加的字符串
	CListBox m_InList;				//!< 已添加的字符串
	std::list<CString> m_ltOut;		//!< 可添加的字符串
	std::list<CString> m_ltIn;		//!< 已添加的字符串
	CString m_strOut;				//!< 可添加的标题
	CString m_strIn;				//!< 已添加的标题
	CString m_strTitle;				//!< 窗体的标题

	CStringAddDelDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CStringAddDelDlg();

// 对话框数据
	enum { IDD = IDD_SERIALADDDEL_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	virtual BOOL DestroyWindow();
	afx_msg void OnBnClickedSerialAdd();
	afx_msg void OnBnClickedSerialDel();
	afx_msg void OnBnClickedSerialAddall();
	afx_msg void OnBnClickedSerialDelall();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnLbnDblclkSerialOut();
	afx_msg void OnLbnDblclkSerialIn();
	afx_msg BOOL OnHelpInfo(HELPINFO* pHelpInfo);
};
}