#pragma once


// CSoftSetCompileDlg 对话框
namespace SoftInfo{
class CSoftSetCompileDlg : public CDialog
{
	DECLARE_DYNAMIC(CSoftSetCompileDlg)
public:
	BOOL m_bOutPut;				// 是否激活输出栏
	BOOL m_bOutPutWarning;		// 是否输出警告信息

public:
	CSoftSetCompileDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CSoftSetCompileDlg();

// 对话框数据
	enum { IDD = IDD_SOFT_SET_COMPILE};

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual void OnOK();

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	virtual BOOL DestroyWindow();

	afx_msg BOOL OnHelpInfo(HELPINFO* pHelpInfo);
};
}