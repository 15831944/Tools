// DCommerDlg.h : ͷ�ļ�
//

#pragma once

// CDCommerDlg �Ի���
namespace Comm{class IComm;class ICommListener;}
class CDCommerDlg : public CDialog, public Comm::ICommListener
{
// �Ի�������
	enum { IDD = IDD_DCOMMER_DIALOG };

private:
	UINT m_uiComState;		//!< ��ǰִ�е�״̬��0����״̬��1����ѭ������
	UINT m_uiXunHuanCount;	//!< ��ǰѭ�����ڼ�����
	int m_nID;				//!< ������ĵ�ID��

	UINT m_uiCountSend;		//!< ���͵�����
	UINT m_uiCountRcv;		//!< ���յ�����
	UINT m_uiCountBad;		//!< ��������

public:
	CEdit m_CtrlRev;
	UINT m_uiOverTime;		//!< ��ʱʱ��
	int m_nRadioHex;		//!< 0��ʾ16������ʾ��1��ʾASC��ʾ
	int m_nSendType;		//!< 0��һ�Σ�1ѭ����
	UINT m_uiXunHuanNum;	//!< ѭ����ʾѭ�����ٴ�
	UINT m_uiDelay;			//!< �ӳٶ���ʱ�䣬��λ����
	BOOL m_bWatch;			//!< �Ƿ����
	CString m_strSend;		//!< ���͵�����
	CString m_strReceive;	//!< ���յ�����
	CComboBox m_cbCom;		//!< COM��
	CComboBox m_cbBaud;		//!< ������
	CComboBox m_cbCheck;	//!< У�鷽ʽ
	CComboBox m_cbData;		//!< ����λ
	CComboBox m_cbStop;		//!< У��λ
	byte m_Data[64000];		//!< ���͵�����
	int m_nLen;				//!< ���͵����ݳ���
// 	boost::shared_ptr<Comm::IComm> m_Comm;
// 	boost::shared_ptr<Comm::IComm> m_Serial;
// 	boost::shared_ptr<Comm::IComm> m_UDP;
// 	boost::shared_ptr<Comm::IComm> m_UDPWatch;
	Comm::IComm* m_Comm;
	Comm::IComm* m_Serial;
	Comm::IComm* m_UDP;
	Comm::IComm* m_UDPWatch;
	Comm::IComm* m_TCPClient;
	int m_nPort;			//!< �˿ں�
	int m_nWatchPort;		//!< �����˿ں�
	DWORD m_uiIP;			//!< IP��ַ
	int m_nCommType;		//!< ͨ�����ͣ�0���ڣ�1UDP��2TCP

private:
	bool SetData();			//!< ���ַ���������Ҫ���͵ı���
	void ClearRev();
	void SendXunHuan();
	bool BindSerial();
	void InitCommType();	//!< ��ͨ�����͸ı�ʱ���³�ʼ��
	void CheckOutput();		//!< �������

// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	CDCommerDlg(CWnd* pParent = NULL);	// ��׼���캯��
	virtual ~CDCommerDlg();
	afx_msg void OnBnClickedAbout();
	afx_msg void OnBnClickedBtSend();
	afx_msg void OnBnClickedBtCrc();
	afx_msg void OnBnClickedBtBcc();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedBtCleanSend();
	afx_msg void OnBnClickedBtCleanState();
	afx_msg void OnBnClickedBtClearRev();
	afx_msg void OnBnClickedRadioSendtype();
	afx_msg void OnBnClickedCheckWatch();		//!< ѡ��֮��ֱ���������
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnBnClickedRadioNet1();
	virtual void OnReceive(Comm::IComm* commer, byte* data, int len, CString info = _T(""));
	virtual void OnOverTime(Comm::IComm* commer);
};
