#pragma once
#include "afxwin.h"


// CCloneItemDlg �Ի���
class CProcessDlg;
namespace MVC{namespace Item{class CItem;}}
namespace Dialog{
class CCloneItemDlg : public CDialog
{
	DECLARE_DYNAMIC(CCloneItemDlg)
public:
	UINT m_uiCloneID;			// ����ID��
	CString m_strCloneName;		// ��������
	UINT m_uiCloneNum;			// ��¡����
	BOOL m_bIsSeries;			// ����Ƿ�����
	bool m_bShowHelp;			// �Ƿ���ʾ������Ϣ
	UINT m_uiBaseNum;			// ����β�����
	UINT m_uiBaseFlt;			// ���Ƹ����λ
	UINT m_uiAddrUnit;			// IO������ַ���
	int m_nRadioType;			// 0β�������1�����λ

	CProcessDlg *m_pProcessDlg;	// ������ʾ���ȵĶԻ���

	std::list<UINT> m_ltItemID;				// ����¡�����ı�����ID����

private:
	CCloneItemDlg(CWnd* pParent = NULL);	// ��׼���캯��
	virtual ~CCloneItemDlg();

public:
	static CCloneItemDlg& GetMe();

// �Ի�������
	enum { IDD = IDD_CLONE_ITEM_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

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