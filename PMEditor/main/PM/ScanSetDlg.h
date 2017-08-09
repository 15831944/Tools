#pragma once
#include "afxwin.h"
#include "DReport.h"

// CScanSetDlg 对话框
namespace Dialog{
class CScanSetDlg : public CDialog, public DReport::IDReportOwner
{
	DECLARE_DYNAMIC(CScanSetDlg)

public:
	CScanSetDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CScanSetDlg();

// 对话框数据
	enum { IDD = IDD_SCAN_SET_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
private:
	int m_nDelIndex;					//!< 需要被删除的行

public:
	BOOL m_bUseEthernet;				//!< 是否启用以太网扫描
	BOOL m_bUseSerial;					//!< 是否启动串口扫描
	BOOL m_bSync;						//!< 是否将扫描配置同步到工程师站配置中
	UINT m_nSerialStart;				//!< 起始模块地址
	UINT m_nSerialEnd;					//!< 终止模块地址
	DWORD m_uiIPStart;					//!< 扫描起始IP地址
	DWORD m_uiIPEnd;					//!< 扫描终止IP地址
	CComboBox m_cbCom;					//!< 串口
	CComboBox m_cbBaud;					//!< 波特率
	CComboBox m_cbParity;				//!< 校验方式
	CComboBox m_cbDataBits;				//!< 数据位
	CComboBox m_cbStopBits;				//!< 停止位
	CComboBox m_cbScanLevel;			//!< 扫描级数

private:
	UINT GetIPTurnOver(UINT ip);		//!< 返回IP的逆端值
	bool OnCheck();						//!< 检验
	bool OnApplay();					//!< 应用

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