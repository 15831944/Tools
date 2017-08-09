#pragma once
#include "DReport.h"

// CProjGuideDlg 对话框
namespace Dialog{
namespace ReportProj{
	//////////////////////////////////////////////////////////////////////////
	// Customized record item, used for displaying read/unread icons.
	class CProjRecordIcon : public CXTPReportRecordItem
	{
		DECLARE_SERIAL(CProjRecordIcon)
	public:
		CProjRecordIcon();

		// Provides custom group captions depending on the Memory/IO value.
		virtual CString GetGroupCaption(CXTPReportColumn* pColumn);

		// Provides custom group values comparison based on Memory/IO value, instead of based on captions.
		virtual int CompareGroupCaption(CXTPReportColumn* pColumn, CXTPReportRecordItem* pItem);

		// Updates record item icon index depending on Memory/IO value.
		void UpdateIcon();

		virtual void DoPropExchange(CXTPPropExchange* pPX);
	};

	class CProjRecord : public CXTPReportRecord
	{
		DECLARE_SERIAL(CProjRecord)
	public:
		UINT m_uiID;
		CString m_strProjName;

		// Construct record object using empty values on each field
		CProjRecord();

		// Construct record object from detailed values on each field
		CProjRecord(UINT i);

		// Clean up internal objects
		virtual ~CProjRecord();

		// Overridden callback method, where we can customize any drawing item metrics.
		virtual void GetItemMetrics(XTP_REPORTRECORDITEM_DRAWARGS* pDrawArgs, XTP_REPORTRECORDITEM_METRICS* pItemMetrics);
	};
}

#define CDialogSampleDlgBase CXTPDialogBase<CXTResizeDialog>
class CProjGuideDlg : public CDialogSampleDlgBase, public DReport::IDReportOwner
{
	DECLARE_DYNAMIC(CProjGuideDlg)
public:
	DReport::CDReport m_ProjCtrl;
	CImageList m_ImgList;
	CXTPToolBar* m_ToolBar;

private:
	HICON m_hIcon;

public:
	CProjGuideDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CProjGuideDlg();

// 对话框数据
	enum { IDD = IDD_PROJ_GUIDE };

protected:
	void RepositionTooBar();
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual void InitData();
	virtual void FreshToolBar();			//!< 刷新所有toolbar的状态

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg void OnProjNew();
	afx_msg void OnProjDel();
	afx_msg void OnProjOpen();
	afx_msg void OnProjBackup();
	afx_msg void OnIDIOServer();
	afx_msg void OnHMI();
	afx_msg void OnRun();
	afx_msg void OnHelp();
};
}
