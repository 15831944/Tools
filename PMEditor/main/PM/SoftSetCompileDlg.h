#pragma once


// CSoftSetCompileDlg �Ի���
namespace SoftInfo{
class CSoftSetCompileDlg : public CDialog
{
	DECLARE_DYNAMIC(CSoftSetCompileDlg)
public:
	BOOL m_bOutPut;				// �Ƿ񼤻������
	BOOL m_bOutPutWarning;		// �Ƿ����������Ϣ

public:
	CSoftSetCompileDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CSoftSetCompileDlg();

// �Ի�������
	enum { IDD = IDD_SOFT_SET_COMPILE};

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	virtual void OnOK();

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	virtual BOOL DestroyWindow();

	afx_msg BOOL OnHelpInfo(HELPINFO* pHelpInfo);
};
}