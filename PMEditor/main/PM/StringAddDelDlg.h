#pragma once
#include "afxwin.h"

// CStringAddDelDlg �Ի���
namespace Dialog{
class CStringAddDelDlg : public CDialog
{
	DECLARE_DYNAMIC(CStringAddDelDlg)
public:
	CListBox m_OutList;				//!< ����ӵ��ַ���
	CListBox m_InList;				//!< ����ӵ��ַ���
	std::list<CString> m_ltOut;		//!< ����ӵ��ַ���
	std::list<CString> m_ltIn;		//!< ����ӵ��ַ���
	CString m_strOut;				//!< ����ӵı���
	CString m_strIn;				//!< ����ӵı���
	CString m_strTitle;				//!< ����ı���

	CStringAddDelDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CStringAddDelDlg();

// �Ի�������
	enum { IDD = IDD_SERIALADDDEL_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

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