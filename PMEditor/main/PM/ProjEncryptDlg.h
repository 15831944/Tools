#pragma once


// CProjEncryptDlg 对话框
namespace Dialog{
class CProjEncryptDlg : public CDialog
{
public:
	enum { IDD = IDD_ENCRYPT_SET};

	DECLARE_DYNAMIC(CProjEncryptDlg)
private:
	int m_nType;				//!< 对话框显示的类型。在构造函数中通过输入参数赋值。	0：设置新密码		1：修改密码		2：取消密码
	CString m_strEncryptOld;	//!< 旧密码s
	CString m_strEncrypt;		//!< 密码
	CString m_strEncryptAgain;	//!< 确认密码
	CString m_strEncryptTitle;	//!< 密码的标题

public:
	CString GetEncryptText(){return m_strEncrypt;}		//!< 获得加密的密码

private:
	void OnOKNewEncrypt();		//!< 判断设置密码的合法性
	void OnOKModifyEncrypt();	//!< 判断修改密码的合法性
	void OnOKDelEncrypt();		//!< 判断取消密码的合法性

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CProjEncryptDlg(int nType, CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CProjEncryptDlg();
	virtual BOOL OnInitDialog();		//!< 根据设置密码的类型（加密、修改密码、取消密码），显示、隐藏、修改界面上的控件
	afx_msg void OnBnClickedOk();		//!< 根据设置密码的类型（加密、修改密码、取消密码），调用不同的方法进行处理
	afx_msg BOOL OnHelpInfo(HELPINFO* pHelpInfo);
};
}