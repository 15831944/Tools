#pragma once


//!< 重命名 对话框
namespace Dialog{
class CReNameDlg : public CDialog
{
	DECLARE_DYNAMIC(CReNameDlg)

public:
	CReNameDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CReNameDlg();

// 对话框数据
	enum { IDD = IDD_RENAME_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
private:
	CString m_strOldName;	//!< 原名称
	CString m_strNewName;	//!< 新名称

public:
	CString GetNewName(){return m_strNewName;}

public:
	void SetOldName(CString str);			//!< 设定老名称
	void SetDefaultName(CString str);		//!< 设定默认值
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
	afx_msg BOOL OnHelpInfo(HELPINFO* pHelpInfo);
};
}