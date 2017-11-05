#pragma once

#include "DTreeCtrl.h"
// CItemInConfigDlg �Ի���
namespace Dialog{
class CItemInConfigDlg : public CDialog, public Tool::IDTreeOwner
{
	DECLARE_DYNAMIC(CItemInConfigDlg)
public:
	enum{ItemIn = 0,Paste};
	UINT m_uiShowType;					//!< ���Ի������ͣ�0�������룬1����ճ��
	CString m_strTitle;					//!< ���Ի���ı���
	Tool::CDTreeCtrl m_GroupTree;

	BOOL m_bKeep;						//!< ��������
	BOOL m_bKeepItemIn;					//!< ���ֱ������������
	BOOL m_bKeepPaste;					//!< ���ֱ���ճ��������
	BOOL m_bUseAdvanceOptions;			//!< �Ƿ�ʹ�ø߼�ѡ��
	BOOL m_bUseAdvanceOptionsItemIn;	//!< �����Ƿ�ʹ�ø߼�ѡ��
	BOOL m_bUseAdvanceOptionsPaste;		//!< ճ���Ƿ�ʹ�ø߼�ѡ��

	UINT m_uiGroupID;					//!< 
	CString m_strGroupName;

	int m_nSameNameType;				//!< ��������
	int m_nSameNameAddr;				//!< �������������ַ�����λ�ã�0ǰ�棬1����
	CString m_strItemSameName;			//!< ���������ַ���


	BOOL m_bUseBaseID;					//!< �Ƿ�ʹ�ñ�������
	int m_nBaseID;						//!< ���������ӻ���
	int m_nBaseIDSame;					//!< ID�ص�����

	BOOL m_bNameName;					//!< ���� �Ƿ񲹼��ַ���
	CString m_strNameName;				//!< ���� ���ӵ��ַ���
	int m_nNameAddr;					//!< ���� �����ַ�����λ�ã�0ǰ�棬1���棬Tag���ظ�ҲҪ�����ַ���

	BOOL m_bTagName;					//!< ��ǩ �Ƿ񲹼��ַ���
	CString m_strTagName;				//!< ��ǩ ���ӵ��ַ���
	int m_nTagAddr;						//!< ��ǩ �����ַ�����λ�ã�0ǰ�棬1���棬Tag���ظ�ҲҪ�����ַ���

	BOOL m_bDespName;					//!< ��ע �Ƿ񲹼��ַ���
	CString m_strDespName;				//!< ��ע ���ӵ��ַ���
	int m_nDespAddr;					//!< ��ע �����ַ�����λ�ã�0ǰ�棬1���棬Desp���ظ�ҲҪ�����ַ���

private:
	CItemInConfigDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CItemInConfigDlg();

public:
	static CItemInConfigDlg& GetMe();

// �Ի�������
	enum { IDD = IDD_ITEMIN_DLG };

protected:
	void UpdateGroupTree();
	void AddGroupChild(HTREEITEM root, HTREEITEM& selItem, UINT parentID);
	void SetGroupName(CString name);
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	virtual INT_PTR DoModal(UINT type = ItemIn, int groupId = -1);
	virtual BOOL OnInitDialog();
	virtual void OnTreeDblClick(CTreeCtrl*, HTREEITEM);
	virtual void OnTreeLClick(CTreeCtrl*, HTREEITEM);
	virtual void OnTreeRClick(CTreeCtrl*, HTREEITEM,CPoint);
	virtual void OnTreeKeyDown(CTreeCtrl*, HTREEITEM,UINT);
	afx_msg void OnBnClickedAddGroup();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedRadioItemname1();
	afx_msg void OnBnClickedRadioItemname2();
	afx_msg void OnBnClickedRadioItemname3();
	afx_msg void OnBnClickedRadioItemname4();
	afx_msg void OnBnClickAdvanceOptions();
	afx_msg void OnBnClickBaseID();
	afx_msg void OnBnClickNameStr();
	afx_msg void OnBnClickTagStr();
	afx_msg void OnBnClickDespStr();
	afx_msg BOOL OnHelpInfo(HELPINFO* pHelpInfo);
};
}