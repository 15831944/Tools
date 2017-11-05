#pragma once


// CWriteItemDlg 对话框
namespace Dialog{
class CWriteItemDlg : public CDialog
{
	DECLARE_DYNAMIC(CWriteItemDlg)
public:
	UINT m_uiID;							// 变量编号
	CString m_strName;						// 变量名称
	CString m_strType;						// 变量类型
	CString m_strValueNew;					// 变量新值
	BOOL m_bWriteIO;						// 是否写入工程值

private:
	CWriteItemDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CWriteItemDlg();

public:
	static CWriteItemDlg& GetMe();
// 对话框数据
	enum { IDD = IDD_WRITE_ITEM_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual INT_PTR DoModal(UINT id);
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
	afx_msg BOOL OnHelpInfo(HELPINFO* pHelpInfo);
};
}