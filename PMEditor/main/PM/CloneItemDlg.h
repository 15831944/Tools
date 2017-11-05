#pragma once
#include "afxwin.h"


// CCloneItemDlg 对话框
class CProcessDlg;
namespace MVC{namespace Item{class CItem;}}
namespace Dialog{
class CCloneItemDlg : public CDialog
{
	DECLARE_DYNAMIC(CCloneItemDlg)
public:
	UINT m_uiCloneID;			// 变量ID号
	CString m_strCloneName;		// 变量名称
	UINT m_uiCloneNum;			// 克隆数量
	BOOL m_bIsSeries;			// 编号是否连续
	bool m_bShowHelp;			// 是否显示帮助信息
	UINT m_uiBaseNum;			// 名称尾数间隔
	UINT m_uiBaseFlt;			// 名称浮点进位
	UINT m_uiAddrUnit;			// IO变量地址间隔
	int m_nRadioType;			// 0尾数间隔，1浮点进位

	CProcessDlg *m_pProcessDlg;	// 用于显示进度的对话框

	std::list<UINT> m_ltItemID;				// 被克隆出来的变量的ID链表

private:
	CCloneItemDlg(CWnd* pParent = NULL);	// 标准构造函数
	virtual ~CCloneItemDlg();

public:
	static CCloneItemDlg& GetMe();

// 对话框数据
	enum { IDD = IDD_CLONE_ITEM_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedOk();
//	afx_msg void OnBnClickedShowCloneinfo();
	afx_msg void OnBnClickedRadioNumber();
	afx_msg void OnBnClickedRadioFloat();
	afx_msg BOOL OnHelpInfo(HELPINFO* pHelpInfo);

	virtual BOOL OnInitDialog();
	virtual INT_PTR DoModal(UINT id);
};
}