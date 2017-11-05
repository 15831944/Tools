#pragma once


// CWriteItemDlg �Ի���
namespace Dialog{
class CWriteItemDlg : public CDialog
{
	DECLARE_DYNAMIC(CWriteItemDlg)
public:
	UINT m_uiID;							// �������
	CString m_strName;						// ��������
	CString m_strType;						// ��������
	CString m_strValueNew;					// ������ֵ
	BOOL m_bWriteIO;						// �Ƿ�д�빤��ֵ

private:
	CWriteItemDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CWriteItemDlg();

public:
	static CWriteItemDlg& GetMe();
// �Ի�������
	enum { IDD = IDD_WRITE_ITEM_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	virtual INT_PTR DoModal(UINT id);
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
	afx_msg BOOL OnHelpInfo(HELPINFO* pHelpInfo);
};
}