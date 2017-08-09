#pragma once


// CProjEncryptDlg �Ի���
namespace Dialog{
class CProjEncryptDlg : public CDialog
{
public:
	enum { IDD = IDD_ENCRYPT_SET};

	DECLARE_DYNAMIC(CProjEncryptDlg)
private:
	int m_nType;				//!< �Ի�����ʾ�����͡��ڹ��캯����ͨ�����������ֵ��	0������������		1���޸�����		2��ȡ������
	CString m_strEncryptOld;	//!< ������s
	CString m_strEncrypt;		//!< ����
	CString m_strEncryptAgain;	//!< ȷ������
	CString m_strEncryptTitle;	//!< ����ı���

public:
	CString GetEncryptText(){return m_strEncrypt;}		//!< ��ü��ܵ�����

private:
	void OnOKNewEncrypt();		//!< �ж���������ĺϷ���
	void OnOKModifyEncrypt();	//!< �ж��޸�����ĺϷ���
	void OnOKDelEncrypt();		//!< �ж�ȡ������ĺϷ���

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	CProjEncryptDlg(int nType, CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CProjEncryptDlg();
	virtual BOOL OnInitDialog();		//!< ����������������ͣ����ܡ��޸����롢ȡ�����룩����ʾ�����ء��޸Ľ����ϵĿؼ�
	afx_msg void OnBnClickedOk();		//!< ����������������ͣ����ܡ��޸����롢ȡ�����룩�����ò�ͬ�ķ������д���
	afx_msg BOOL OnHelpInfo(HELPINFO* pHelpInfo);
};
}