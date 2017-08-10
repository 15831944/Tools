#pragma once
#include "afxcmn.h"
#include "DTreeCtrl.h"

// CSoftSetDlg 对话框
namespace SoftInfo{
class CSoftSetDlg : public CDialog, public Tool::IDTreeOwner
{
	DECLARE_DYNAMIC(CSoftSetDlg)
	typedef struct tagPageOne{
		std::shared_ptr<CDialog> m_Dialog;
		CString m_strName;
	}SPageOne;

public:
	std::vector<std::shared_ptr<SPageOne> > m_vtPage;
	std::shared_ptr<SPageOne> m_culPage;
	Tool::CDTreeCtrl m_PageTree;

public:
	CSoftSetDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CSoftSetDlg();
	template<class Page>
	void AddPage(Page* page, CString name, int nImageID);
	void ShowPage(std::shared_ptr<SPageOne> page);

// 对话框数据
	enum { IDD = IDD_SOFT_SET_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL DestroyWindow();
	virtual BOOL OnInitDialog();
	virtual void OnTreeDblClick(CTreeCtrl*, HTREEITEM){;}
	virtual void OnTreeRClick(CTreeCtrl*, HTREEITEM,CPoint){;}
	virtual void OnTreeLClick(CTreeCtrl*, HTREEITEM);
	virtual void OnTreeKeyDown(CTreeCtrl*, HTREEITEM,UINT);
	afx_msg void OnBnClickedOk();
	afx_msg BOOL OnHelpInfo(HELPINFO* pHelpInfo);
	LRESULT OnRevMsgOK(WPARAM wParam, LPARAM lParam);
};
}