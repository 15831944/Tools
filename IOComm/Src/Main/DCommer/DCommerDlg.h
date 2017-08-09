// DCommerDlg.h : 头文件
//

#pragma once

// CDCommerDlg 对话框
namespace Comm{class IComm;class ICommListener;}
class CDCommerDlg : public CDialog, public Comm::ICommListener
{
// 对话框数据
	enum { IDD = IDD_DCOMMER_DIALOG };

private:
	UINT m_uiComState;		//!< 当前执行的状态，0正常状态，1正在循环发送
	UINT m_uiXunHuanCount;	//!< 当前循环到第几个了
	int m_nID;				//!< 输出报文的ID号

	UINT m_uiCountSend;		//!< 发送的数量
	UINT m_uiCountRcv;		//!< 接收的数量
	UINT m_uiCountBad;		//!< 错误数量

public:
	CEdit m_CtrlRev;
	UINT m_uiOverTime;		//!< 超时时间
	int m_nRadioHex;		//!< 0表示16进制显示，1表示ASC显示
	int m_nSendType;		//!< 0发一次，1循环发
	UINT m_uiXunHuanNum;	//!< 循环表示循环多少次
	UINT m_uiDelay;			//!< 延迟多少时间，单位毫秒
	BOOL m_bWatch;			//!< 是否监听
	CString m_strSend;		//!< 发送的数据
	CString m_strReceive;	//!< 接收的数据
	CComboBox m_cbCom;		//!< COM口
	CComboBox m_cbBaud;		//!< 波特率
	CComboBox m_cbCheck;	//!< 校验方式
	CComboBox m_cbData;		//!< 数据位
	CComboBox m_cbStop;		//!< 校验位
	byte m_Data[64000];		//!< 发送的数据
	int m_nLen;				//!< 发送的数据长度
// 	boost::shared_ptr<Comm::IComm> m_Comm;
// 	boost::shared_ptr<Comm::IComm> m_Serial;
// 	boost::shared_ptr<Comm::IComm> m_UDP;
// 	boost::shared_ptr<Comm::IComm> m_UDPWatch;
	Comm::IComm* m_Comm;
	Comm::IComm* m_Serial;
	Comm::IComm* m_UDP;
	Comm::IComm* m_UDPWatch;
	Comm::IComm* m_TCPClient;
	int m_nPort;			//!< 端口号
	int m_nWatchPort;		//!< 监听端口号
	DWORD m_uiIP;			//!< IP地址
	int m_nCommType;		//!< 通信类型，0串口，1UDP，2TCP

private:
	bool SetData();			//!< 将字符串解析成要发送的报文
	void ClearRev();
	void SendXunHuan();
	bool BindSerial();
	void InitCommType();	//!< 当通信类型改变时重新初始化
	void CheckOutput();		//!< 输出限制

// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	CDCommerDlg(CWnd* pParent = NULL);	// 标准构造函数
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
	afx_msg void OnBnClickedCheckWatch();		//!< 选中之后直接启动监控
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnBnClickedRadioNet1();
	virtual void OnReceive(Comm::IComm* commer, byte* data, int len, CString info = _T(""));
	virtual void OnOverTime(Comm::IComm* commer);
};
