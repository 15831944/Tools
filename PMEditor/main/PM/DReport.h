// ReportControl.h: interface for the CReportControl class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DREPORT_H__7361D7E3_7483_4EAC_BDC2_15CA2BC0F49B__INCLUDED_)
#define AFX_DREPORT_H__7361D7E3_7483_4EAC_BDC2_15CA2BC0F49B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define WM_ADD_RECORD_EX (WM_USER + 1)

namespace DReport{
class CDReport;
struct IDReportOwner
{
public:
	virtual void OnReportDblClick(CDReport* report, UINT row){;}
	virtual void OnReportLClick(CDReport* report, UINT row){;}
	virtual void OnReportRClick(CDReport* report, UINT row, CPoint point){;}
};

class CDReport : public CXTPReportControl
{
private:
	IDReportOwner* m_Object;
	UINT m_uiRow;

public:
	CDReport();
	virtual ~CDReport();
	void SetOwner(IDReportOwner* owner){m_Object = owner;}
	virtual void OnSelectionChanged();
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CReportControl)
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CDReport)
//	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
}
#endif // !defined(AFX_DREPORT_H__7361D7E3_7483_4EAC_BDC2_15CA2BC0F49B__INCLUDED_)
