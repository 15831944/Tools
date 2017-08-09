#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

namespace Space{
//////////////////////////////////////////////////////////////////////////
// Customized record item, used for displaying read/unread icons.
class COutputIcon : public CXTPReportRecordItem
{
	DECLARE_SERIAL(COutputIcon)
public:
	COutputIcon();					//!< 0-Information, 1-warning, 2-error
	COutputIcon(UINT type);			//!< 0-Information, 1-warning, 2-error

	virtual CString GetGroupCaption(CXTPReportColumn* pColumn);			// Provides custom group captions depending on the type value.
	virtual int CompareGroupCaption(CXTPReportColumn* pColumn, CXTPReportRecordItem* pItem);	// Provides custom group values comparison based on type value, instead of based on captions.
	void UpdateIcon();				// Updates record item icon index depending on type value.
	int Compare(CXTPReportColumn* pColumn, CXTPReportRecordItem* pItem);// Provides custom records comparison by this item based on read/unread value, instead of based on captions.

public:
	UINT m_uiType;					//!< 0-Information, 1-warning, 2-error, 3-noicon
};

//////////////////////////////////////////////////////////////////////////
// COutputRecord
class COutputRecord : public CXTPReportRecord
{
	DECLARE_SERIAL(COutputRecord)
public:
	UINT m_uiType;
	CString m_strType;
	CString m_strAddr;
	CString m_strOut;
	COutputIcon* m_pIcon;
	COutputRecord();
	COutputRecord(UINT type, CString strType, CString strAddr, CString strOut);
	virtual ~COutputRecord();
	virtual void GetItemMetrics(XTP_REPORTRECORDITEM_DRAWARGS* pDrawArgs, XTP_REPORTRECORDITEM_METRICS* pItemMetrics);
};

/////////////////////////////////////////////////////////////////////////////
// COutputPane window
class CSpaceOutput : public CXTPOfficeBorder<CXTPReportControl>
{
private:
	CImageList m_ImgList;
	bool m_bLock;
	bool m_bLockUpdate;

public:
	enum{
		Information,
		Warning,
		Error,
		Normal
	};
	CSpaceOutput();
	virtual ~CSpaceOutput();

// Implementation
public:
	void OnSelectAll();
	void Clear();
	void PrintOut(CString str, bool twice = true);
	void PrintOut(UINT type, CString strType, CString strAddr, CString strOut);
	void LockUpdate(bool bLock){m_bLockUpdate = bLock;}

private:
	void ShowItem(CString str);						//!< 从本文中找到对应的变量，并显示出来
	void ShowDevice(CString str);					//!< 从本文中找到对应的设备，并显示出来

protected:
	//{{AFX_MSG(COutputPane)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnOutputClear();
	afx_msg void OnOutputLock();
	afx_msg BOOL OnHelpInfo(HELPINFO* pHelpInfo);
};
}