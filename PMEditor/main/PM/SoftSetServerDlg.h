#pragma once


// CSoftSetServerDlg �Ի���
namespace SoftInfo{
class CSoftSetServerDlg : public CDialog
{
	DECLARE_DYNAMIC(CSoftSetServerDlg)
public:
	UINT m_uiDataFreshTime;		// ����ˢ������

public:
	CSoftSetServerDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CSoftSetServerDlg();

// �Ի�������
	enum { IDD = IDD_SOFT_SET_SERVER };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	virtual BOOL DestroyWindow();
	afx_msg BOOL OnHelpInfo(HELPINFO* pHelpInfo);

protected:
	virtual void OnOK();
};
}