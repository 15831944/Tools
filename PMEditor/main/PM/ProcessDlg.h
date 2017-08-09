#pragma once
#include "afxcmn.h"


// CProcessDlg 对话框
#define WM_PROCESS_RANGE	1024
#define WM_PROCESS_POS		1025
#define WM_PROCESS_DONE		1026
class CProcessDlg : public CDialog
{
	DECLARE_DYNAMIC(CProcessDlg)

public:
	CProcessDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CProcessDlg();

// 对话框数据
	enum { IDD = IDD_PROCESS_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CProgressCtrl m_ProgCtrl;
	CString m_strOut;
	bool m_bReady;	// 是否准备好了
	bool m_bStop;	// 是否点击了中断

	int m_nStart;
	int m_nEnd;
	int m_nPos;

	afx_msg void OnBnClickedCancel();
	afx_msg LONG OnSetRange(WPARAM wParam, LPARAM lParam);
	afx_msg LONG OnSetPos(WPARAM wParam, LPARAM lParam);
	afx_msg LONG OnDone(WPARAM wPara, LPARAM lParam);
	virtual BOOL OnInitDialog();
	//void SetProcessRange(int nStart, int nEnd);
	//void SetProcessPos(int nPos);
};
