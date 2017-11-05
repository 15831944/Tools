#pragma once

#include "DTreeCtrl.h"
// CItemInConfigDlg 对话框
namespace Dialog{
class CItemInConfigDlg : public CDialog, public Tool::IDTreeOwner
{
	DECLARE_DYNAMIC(CItemInConfigDlg)
public:
	enum{ItemIn = 0,Paste};
	UINT m_uiShowType;					//!< 本对话框类型，0变量导入，1变量粘贴
	CString m_strTitle;					//!< 本对话框的标题
	Tool::CDTreeCtrl m_GroupTree;

	BOOL m_bKeep;						//!< 保持配置
	BOOL m_bKeepItemIn;					//!< 保持变量导入的配置
	BOOL m_bKeepPaste;					//!< 保持变量粘贴的配置
	BOOL m_bUseAdvanceOptions;			//!< 是否使用高级选项
	BOOL m_bUseAdvanceOptionsItemIn;	//!< 导入是否使用高级选项
	BOOL m_bUseAdvanceOptionsPaste;		//!< 粘贴是否使用高级选项

	UINT m_uiGroupID;					//!< 
	CString m_strGroupName;

	int m_nSameNameType;				//!< 重名处理
	int m_nSameNameAddr;				//!< 名称重名补加字符串的位置，0前面，1后面
	CString m_strItemSameName;			//!< 重名补加字符串


	BOOL m_bUseBaseID;					//!< 是否使用变量基数
	int m_nBaseID;						//!< 变量的增加基数
	int m_nBaseIDSame;					//!< ID重叠处理

	BOOL m_bNameName;					//!< 名称 是否补加字符串
	CString m_strNameName;				//!< 名称 补加的字符串
	int m_nNameAddr;					//!< 名称 补加字符串的位置，0前面，1后面，Tag不重复也要补加字符串

	BOOL m_bTagName;					//!< 标签 是否补加字符串
	CString m_strTagName;				//!< 标签 补加的字符串
	int m_nTagAddr;						//!< 标签 补加字符串的位置，0前面，1后面，Tag不重复也要补加字符串

	BOOL m_bDespName;					//!< 备注 是否补加字符串
	CString m_strDespName;				//!< 备注 补加的字符串
	int m_nDespAddr;					//!< 备注 补加字符串的位置，0前面，1后面，Desp不重复也要补加字符串

private:
	CItemInConfigDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CItemInConfigDlg();

public:
	static CItemInConfigDlg& GetMe();

// 对话框数据
	enum { IDD = IDD_ITEMIN_DLG };

protected:
	void UpdateGroupTree();
	void AddGroupChild(HTREEITEM root, HTREEITEM& selItem, UINT parentID);
	void SetGroupName(CString name);
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

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