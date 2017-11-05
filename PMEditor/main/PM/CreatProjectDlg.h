#pragma once


// CCreatProjectDlg 对话框
namespace Dialog{
class CCreatProjectDlg : public CDialog
{
	DECLARE_DYNAMIC(CCreatProjectDlg)
private:
	int m_nType;
	CString m_strName;			// 工程名
//	CString m_strPathAll;		// 工程全路径名，包括路径和名称
	CString m_strPathOnly;		// 工程路径，最后带'\'
	CString m_strVersion;		// 工程版本号,用户可随意制定
	CString m_strAuthor;		// 工程作者
	CString m_strDescript;		// 工程备注信息

	bool m_bCreate;				// 是否为创建对话框

private:
	CCreatProjectDlg();
	virtual ~CCreatProjectDlg();

public:
	static CCreatProjectDlg& GetMe(bool bCreate=true);
	CString GetName(){return m_strName;}					// 获得工程名
	CString GetPath(){return m_strPathOnly + m_strName + _T("\\");}				// 获得工程路径，末尾带‘\’
	CString GetAuthor(){return m_strAuthor;}				// 获得作者名
	CString GetDescription(){return m_strDescript;}			// 获得备注信息
	CString GetVersion(){return m_strVersion;}				// 获得版本号

	void SetType(int type){ m_nType = type; }
	void SetName(CString name){ m_strName = name; }			// 获得工程名
	void SetPathOnly(CString path){m_strPathOnly = path;}	// 获得工程路径，末尾带‘\’
	void SetAuthor(CString author){m_strAuthor = author;}	// 获得作者名
	void SetDescription(CString des){m_strDescript = des;}	// 获得备注信息
	void SetVersion(CString ver){m_strVersion = ver;}		// 获得版本号
	void SetDefaultPath(CString path){m_strPathOnly=path;}	// 设置默认路径

// 对话框数据
	enum { IDD = IDD_CREATE_PROJECT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedFindPath();
	afx_msg void OnChangeProjName();
	afx_msg BOOL OnHelpInfo(HELPINFO* pHelpInfo);
	virtual BOOL OnInitDialog();
	virtual BOOL DestroyWindow();
};
}