#pragma once


// CSoftSetServerDlg 对话框
namespace SoftInfo{
class CSoftSetServerDlg : public CDialog
{
	DECLARE_DYNAMIC(CSoftSetServerDlg)
public:
	UINT m_uiDataFreshTime;		// 数据刷新周期

public:
	CSoftSetServerDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CSoftSetServerDlg();

// 对话框数据
	enum { IDD = IDD_SOFT_SET_SERVER };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	virtual BOOL DestroyWindow();
	afx_msg BOOL OnHelpInfo(HELPINFO* pHelpInfo);

protected:
	virtual void OnOK();
};
}