#pragma once
#include "afxwin.h"
#include "afxcmn.h"


// CNewProjectDlg 对话框

class CNewProjectDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CNewProjectDlg)

public:
	static CNewProjectDlg& GetMe();
	int GetType() const { return m_nProjectType; }
	CString GetName() const { return m_strProjectName; }
	CString GetPath() const { return m_strProjectPath + m_strProjectName + _T("\\"); }
	CString GetAuthor() const { return m_strProjectAuthor; }
	CString GetVersion() const { return m_strProjectVersion; }
	CString GetDescription() const { return m_strProjectDescription; }

	void SetDefaultPath(CString strPath) {
		m_strProjectPath = strPath;
	}

private:
	CNewProjectDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CNewProjectDlg();

// 对话框数据
	enum { IDD = IDD_NEW_PROJECT_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()

private:
	int m_nProjectType;
	CString m_strProjectName;
	CString m_strProjectPath;
	CString m_strProjectAuthor;
	CString m_strProjectVersion;
	CString m_strProjectDescription;

	CImageList m_imgList;

private:
	CListCtrl m_ctrlProjectType;

	virtual BOOL OnInitDialog();
	afx_msg void OnLvnItemchangedListProjectType(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMDblclkListProjectType(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedBtnBrowsePath();
	afx_msg void OnBnClickedOk();
	afx_msg BOOL OnHelpInfo(HELPINFO* pHelpInfo);
};
