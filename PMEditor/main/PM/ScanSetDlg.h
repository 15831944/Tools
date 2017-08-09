#pragma once
#include "afxwin.h"
#include "DReport.h"

// CScanSetDlg �Ի���
namespace Dialog{
class CScanSetDlg : public CDialog, public DReport::IDReportOwner
{
	DECLARE_DYNAMIC(CScanSetDlg)

public:
	CScanSetDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CScanSetDlg();

// �Ի�������
	enum { IDD = IDD_SCAN_SET_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
private:
	int m_nDelIndex;					//!< ��Ҫ��ɾ������

public:
	BOOL m_bUseEthernet;				//!< �Ƿ�������̫��ɨ��
	BOOL m_bUseSerial;					//!< �Ƿ���������ɨ��
	BOOL m_bSync;						//!< �Ƿ�ɨ������ͬ��������ʦվ������
	UINT m_nSerialStart;				//!< ��ʼģ���ַ
	UINT m_nSerialEnd;					//!< ��ֹģ���ַ
	DWORD m_uiIPStart;					//!< ɨ����ʼIP��ַ
	DWORD m_uiIPEnd;					//!< ɨ����ֹIP��ַ
	CComboBox m_cbCom;					//!< ����
	CComboBox m_cbBaud;					//!< ������
	CComboBox m_cbParity;				//!< У�鷽ʽ
	CComboBox m_cbDataBits;				//!< ����λ
	CComboBox m_cbStopBits;				//!< ֹͣλ
	CComboBox m_cbScanLevel;			//!< ɨ�輶��

private:
	UINT GetIPTurnOver(UINT ip);		//!< ����IP�����ֵ
	bool OnCheck();						//!< ����
	bool OnApplay();					//!< Ӧ��

public:
	void FreshGrid();
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedScan();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnCbnSelchangeComboCom();
	afx_msg BOOL OnHelpInfo(HELPINFO* pHelpInfo);
};
}