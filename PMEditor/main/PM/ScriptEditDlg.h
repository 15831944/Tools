#pragma once
#include "afxcmn.h"
#include "DReport.h"
#include "ScriptObject.h"

// CScriptEditDlg 对话框
namespace MVC{namespace Item{class CItem;}}
namespace Dialog{
typedef struct tagTextMemory
{
	CString text;	//!< 文本
	long begin;		//!< 光标的起始位置
	long end;		//!< 光标的终止位置
	tagTextMemory& operator = (tagTextMemory& item)
	{
		text = item.text;
		begin = item.begin;
		end = item.end;
		return *this;
	}
}STextMemory;

namespace ReportItem{
//////////////////////////////////////////////////////////////////////////
// Customized record item, used for displaying read/unread icons.
class CItemRecordIcon : public CXTPReportRecordItem
{
	DECLARE_SERIAL(CItemRecordIcon)
public:
	CItemRecordIcon(BOOL bMem = TRUE);

	// Provides custom group captions depending on the Memory/IO value.
	virtual CString GetGroupCaption(CXTPReportColumn* pColumn);

	// Provides custom group values comparison based on Memory/IO value, instead of based on captions.
	virtual int CompareGroupCaption(CXTPReportColumn* pColumn, CXTPReportRecordItem* pItem);

	// Updates record item icon index depending on Memory/IO value.
	void UpdateIcon();

	// Provides custom records comparison by this item based on read/unread value, instead of based on captions.
	int Compare(CXTPReportColumn* pColumn, CXTPReportRecordItem* pItem);

	virtual void DoPropExchange(CXTPPropExchange* pPX);
public:
	BOOL m_bMem;	// TRUE for Memory, FALSE for IO.
};


class CItemRecord : public CXTPReportRecord
{
	DECLARE_SERIAL(CItemRecord)
public:
	UINT m_ID;

	// Construct record object using empty values on each field
	CItemRecord();

	// Construct record object from detailed values on each field
	CItemRecord(std::shared_ptr<MVC::Item::CItem> item);

	// Clean up internal objects
	virtual ~CItemRecord();

	// Overridden callback method, where we can customize any drawing item metrics.
	virtual void GetItemMetrics(XTP_REPORTRECORDITEM_DRAWARGS* pDrawArgs, XTP_REPORTRECORDITEM_METRICS* pItemMetrics);
};
}

class CScriptEditDlg : public CXTResizeDialog, public DReport::IDReportOwner
{
	DECLARE_DYNAMIC(CScriptEditDlg)
public:
	enum{
		COL_ICON,
		COL_NAME,
		COL_ID,
		COL_VALTYPE,
		COL_SRCTYPE,
		COL_DESCRIPT
	};

public:
	CScriptEditDlg(CString strScript, CString strName, CString strDescription);
	virtual ~CScriptEditDlg();

// 对话框数据
	enum { IDD = IDD_SCRIPT_DLG };
private:
	std::stack<STextMemory> m_strUndo;
	std::stack<STextMemory> m_strRedo;
	CRichEditCtrl m_ScriptTextCtrl;
	STextMemory m_OldScript;							//!< 记录原来的
	UINT m_NowEditType;									//!< 当前操作类型，0正常，1撤销中，2重复中，3初始化

public:
	CScriptObject m_ScriptMgr;
	DReport::CDReport m_ItemCtrl;
	CImageList m_ImgList;
	CString m_strScriptText;							//!< 脚本内容
	CString m_strItemName;								//!< 变量名称
	CString m_strDescription;							//!< 变量注释
	CComboBox m_cbItemGroup;							//!< 筛选-变量组
	CComboBox m_cbItemDevice;							//!< 筛选-设备

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	void ReplaceSel(CString text, UINT backSelLen = 0, UINT selId = 0);	//!< 向脚本框中添加数据
	void ScriptClear();									//!< 清空脚本内容
	void ScriptMore();									//!< 更多内容
	void SetUndoRedoState();							//!< 当数据改变时，判断并设置一下撤销和重复这两个按钮的状态
	void ScriptUndo();									//!< 撤销
	void ScriptRedo();									//!< 重复
	bool CheckScript(CString strScript, CString& strError);			//!< 检查这个的脚本的正确定，返回正确与否，和错误信息
	bool OnExcute(CString strScript, CString& strError);			//!< 执行脚本

public:
	virtual void OnReportDblClick(DReport::CDReport* report, UINT row);

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnClickScriptBtn(WPARAM wParam);
	afx_msg void OnBnClickedScriptCheck();
	afx_msg void OnEnChangeScriptText();
	afx_msg BOOL OnHelpInfo(HELPINFO* pHelpInfo);
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};
}