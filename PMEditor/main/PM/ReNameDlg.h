#pragma once


//!< ������ �Ի���
namespace Dialog{
class CReNameDlg : public CDialog
{
	DECLARE_DYNAMIC(CReNameDlg)

public:
	CReNameDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CReNameDlg();

// �Ի�������
	enum { IDD = IDD_RENAME_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
private:
	CString m_strOldName;	//!< ԭ����
	CString m_strNewName;	//!< ������

public:
	CString GetNewName(){return m_strNewName;}

public:
	void SetOldName(CString str);			//!< �趨������
	void SetDefaultName(CString str);		//!< �趨Ĭ��ֵ
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
	afx_msg BOOL OnHelpInfo(HELPINFO* pHelpInfo);
};
}