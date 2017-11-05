#pragma once


// CCreatProjectDlg �Ի���
namespace Dialog{
class CCreatProjectDlg : public CDialog
{
	DECLARE_DYNAMIC(CCreatProjectDlg)
private:
	int m_nType;
	CString m_strName;			// ������
//	CString m_strPathAll;		// ����ȫ·����������·��������
	CString m_strPathOnly;		// ����·��������'\'
	CString m_strVersion;		// ���̰汾��,�û��������ƶ�
	CString m_strAuthor;		// ��������
	CString m_strDescript;		// ���̱�ע��Ϣ

	bool m_bCreate;				// �Ƿ�Ϊ�����Ի���

private:
	CCreatProjectDlg();
	virtual ~CCreatProjectDlg();

public:
	static CCreatProjectDlg& GetMe(bool bCreate=true);
	CString GetName(){return m_strName;}					// ��ù�����
	CString GetPath(){return m_strPathOnly + m_strName + _T("\\");}				// ��ù���·����ĩβ����\��
	CString GetAuthor(){return m_strAuthor;}				// ���������
	CString GetDescription(){return m_strDescript;}			// ��ñ�ע��Ϣ
	CString GetVersion(){return m_strVersion;}				// ��ð汾��

	void SetType(int type){ m_nType = type; }
	void SetName(CString name){ m_strName = name; }			// ��ù�����
	void SetPathOnly(CString path){m_strPathOnly = path;}	// ��ù���·����ĩβ����\��
	void SetAuthor(CString author){m_strAuthor = author;}	// ���������
	void SetDescription(CString des){m_strDescript = des;}	// ��ñ�ע��Ϣ
	void SetVersion(CString ver){m_strVersion = ver;}		// ��ð汾��
	void SetDefaultPath(CString path){m_strPathOnly=path;}	// ����Ĭ��·��

// �Ի�������
	enum { IDD = IDD_CREATE_PROJECT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

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