#pragma once
#include "afxcmn.h"
#include "DReport.h"
#include "ScriptObject.h"

// CScriptEditDlg �Ի���
namespace MVC{namespace Item{class CItem;}}
namespace Dialog{
typedef struct tagTextMemory
{
	CString text;	//!< �ı�
	long begin;		//!< ������ʼλ��
	long end;		//!< ������ֹλ��
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

// �Ի�������
	enum { IDD = IDD_SCRIPT_DLG };
private:
	std::stack<STextMemory> m_strUndo;
	std::stack<STextMemory> m_strRedo;
	CRichEditCtrl m_ScriptTextCtrl;
	STextMemory m_OldScript;							//!< ��¼ԭ����
	UINT m_NowEditType;									//!< ��ǰ�������ͣ�0������1�����У�2�ظ��У�3��ʼ��

public:
	CScriptObject m_ScriptMgr;
	DReport::CDReport m_ItemCtrl;
	CImageList m_ImgList;
	CString m_strScriptText;							//!< �ű�����
	CString m_strItemName;								//!< ��������
	CString m_strDescription;							//!< ����ע��
	CComboBox m_cbItemGroup;							//!< ɸѡ-������
	CComboBox m_cbItemDevice;							//!< ɸѡ-�豸

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	void ReplaceSel(CString text, UINT backSelLen = 0, UINT selId = 0);	//!< ��ű������������
	void ScriptClear();									//!< ��սű�����
	void ScriptMore();									//!< ��������
	void SetUndoRedoState();							//!< �����ݸı�ʱ���жϲ�����һ�³������ظ���������ť��״̬
	void ScriptUndo();									//!< ����
	void ScriptRedo();									//!< �ظ�
	bool CheckScript(CString strScript, CString& strError);			//!< �������Ľű�����ȷ����������ȷ��񣬺ʹ�����Ϣ
	bool OnExcute(CString strScript, CString& strError);			//!< ִ�нű�

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