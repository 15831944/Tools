// ReportControl.cpp: implementation of the CReportControl class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "DReport.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
using namespace DReport;
CDReport::CDReport()
{
	m_Object = NULL;
	m_uiRow = UINT(-1);
}

CDReport::~CDReport()
{
}

/////////////////////////////////////////////////////////////////////////////
// Message map

BEGIN_MESSAGE_MAP(CDReport, CXTPReportControl)
	//{{AFX_MSG_MAP(CReportControl)
//	ON_WM_KEYDOWN()
	//}}AFX_MSG_MAP
	ON_WM_LBUTTONUP()
	ON_WM_RBUTTONUP()
	ON_WM_LBUTTONDBLCLK()
END_MESSAGE_MAP()

void CDReport::OnLButtonUp(UINT nFlags, CPoint point)
{
	CXTPReportControl::OnLButtonUp(nFlags, point);
	CXTPReportRow* row = HitTest(point);
	if(!row)		return;
	if(m_Object)	m_Object->OnReportLClick(this, row->GetIndex());
}

void CDReport::OnRButtonUp(UINT nFlags, CPoint point)
{
	CXTPReportControl::OnRButtonUp(nFlags, point);
	CXTPReportRow* row = HitTest(point);
	if(!row)		return;
	if(m_Object)	m_Object->OnReportRClick(this, row->GetIndex(), point);
}

void CDReport::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	CXTPReportControl::OnLButtonDblClk(nFlags, point);
	CXTPReportRow* row = HitTest(point);
	if(!row)		return;
	if(m_Object)	m_Object->OnReportDblClick(this, row->GetIndex());
}

void CDReport::OnSelectionChanged()
{
	CXTPReportRow* row = GetFocusedRow();
	if(!row)						return;
	if(row->GetIndex() == m_uiRow)	return;
	m_uiRow = row->GetIndex();
	if(m_Object)	m_Object->OnReportLClick(this, m_uiRow);
}