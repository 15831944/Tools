// OutputPane.cpp : implementation file
//
// This file is a part of the XTREME TOOLKIT PRO MFC class library.
// (c)1998-2008 Codejock Software, All Rights Reserved.
//
// THIS SOURCE FILE IS THE PROPERTY OF CODEJOCK SOFTWARE AND IS NOT TO BE
// RE-DISTRIBUTED BY ANY MEANS WHATSOEVER WITHOUT THE EXPRESSED WRITTEN
// CONSENT OF CODEJOCK SOFTWARE.
//
// THIS SOURCE CODE CAN ONLY BE USED UNDER THE TERMS AND CONDITIONS OUTLINED
// IN THE XTREME TOOLKIT PRO LICENSE AGREEMENT. CODEJOCK SOFTWARE GRANTS TO
// YOU (ONE SOFTWARE DEVELOPER) THE LIMITED RIGHT TO USE THIS SOFTWARE ON A
// SINGLE COMPUTER.
//
// CONTACT INFORMATION:
// support@codejock.com
// http://www.codejock.com
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "PMApp.h"
#include "MyHelp.h"
#include "Gbl.h"
#include "SoftInfo.h"
#include "OutputPane.h"

#include "ItemMgr.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

using namespace Space;

IMPLEMENT_SERIAL(COutputIcon, CXTPReportRecordItem, VERSIONABLE_SCHEMA | _XTP_SCHEMA_CURRENT)
IMPLEMENT_SERIAL(COutputRecord, CXTPReportRecord, VERSIONABLE_SCHEMA | _XTP_SCHEMA_CURRENT)

//////////////////////////////////////////////////////////////////////////
// COutputIcon
COutputIcon::COutputIcon()
{
	m_uiType = CSpaceOutput::Normal;
	UpdateIcon();
}

// 0-Information, 1-warning, 2-error
COutputIcon::COutputIcon(UINT type){
	m_uiType = type;
	if(m_uiType != CSpaceOutput::Information && 
		m_uiType != CSpaceOutput::Warning && 
		m_uiType != CSpaceOutput::Error)
		m_uiType = CSpaceOutput::Normal;
	UpdateIcon();
}

void COutputIcon::UpdateIcon(){
	if(m_uiType == 0)		SetIconIndex(19);
	else if(m_uiType == 1)	SetIconIndex(20);
	else if(m_uiType == 2)	SetIconIndex(21);
	else					SetIconIndex(-1);
}

int COutputIcon::Compare(CXTPReportColumn* /*pColumn*/, CXTPReportRecordItem* pItem){
	return int(m_uiType) - int(((COutputIcon *)pItem)->m_uiType);
}

CString COutputIcon::GetGroupCaption(CXTPReportColumn* /*pColumn*/){
	if(m_uiType == 0)		return _T("Message status: Information");
	else if(m_uiType == 1)	return _T("Message status: Warning");
	else if(m_uiType == 2)	return _T("Message status: Error");
	return _T("Message status: Normal");
}

int COutputIcon::CompareGroupCaption(CXTPReportColumn* pColumn, CXTPReportRecordItem* pItem){
	return GetGroupCaption(pColumn).Compare(pItem->GetGroupCaption(pColumn));
}

//////////////////////////////////////////////////////////////////////////
// CVirtualRecord
COutputRecord::COutputRecord()
{
	m_uiType = CSpaceOutput::Normal;
	m_strType = _T("");
	m_strAddr = _T("");
	m_strOut = _T("");

	AddItem(new CXTPReportRecordItemText(_T("")));
	AddItem(new COutputIcon(m_uiType));
	AddItem(new CXTPReportRecordItemText(m_strOut));
	AddItem(new CXTPReportRecordItemText(m_strType));
	AddItem(new CXTPReportRecordItemText(m_strAddr));
	CString strTime = CGbl::GetCulTime();
	AddItem(new CXTPReportRecordItemText(strTime));
}

COutputRecord::COutputRecord(UINT type, CString strType, CString strAddr, CString strOut)
{
	m_uiType = type;
	m_strType = strType;
	m_strAddr = strAddr;
	m_strOut = strOut;

	AddItem(new CXTPReportRecordItemText(_T("")));
	m_pIcon = (COutputIcon *)AddItem(new COutputIcon(m_uiType));
	AddItem(new CXTPReportRecordItemText(m_strOut));
	AddItem(new CXTPReportRecordItemText(m_strType));
	AddItem(new CXTPReportRecordItemText(m_strAddr));
	CString strTime = CGbl::GetCulTime();
	AddItem(new CXTPReportRecordItemText(strTime));
}

COutputRecord::~COutputRecord()
{
}

void COutputRecord::GetItemMetrics(XTP_REPORTRECORDITEM_DRAWARGS* pDrawArgs, XTP_REPORTRECORDITEM_METRICS* pItemMetrics)
{
	CXTPReportRecord::GetItemMetrics(pDrawArgs, pItemMetrics);
}

/////////////////////////////////////////////////////////////////////////////
// COutputPane
CSpaceOutput::CSpaceOutput()
:m_bLock(false)
,m_bLockUpdate(false)
{
}

CSpaceOutput::~CSpaceOutput()
{
}

//IMPLEMENT_DYNCREATE(CSpaceOutput, CXTPReportControl)
BEGIN_MESSAGE_MAP(CSpaceOutput, CXTPReportControl)
	//{{AFX_MSG_MAP(COutputPane)
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
	ON_WM_LBUTTONDBLCLK()
	ON_WM_RBUTTONUP()
	ON_WM_HELPINFO()
	ON_COMMAND(ID_OUTPUT_CLEAR, &CSpaceOutput::OnOutputClear)
	ON_COMMAND(ID_OUTPUT_LOCK, &CSpaceOutput::OnOutputLock)
END_MESSAGE_MAP()

int CSpaceOutput::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if(CXTPReportControl::OnCreate(lpCreateStruct) == -1)
		return -1;
	ModifyStyleEx(0, WS_EX_STATICEDGE);
	SetFont(XTPPaintManager()->GetRegularFont());
	GetReportHeader()->AllowColumnRemove(FALSE);
	GetPaintManager()->m_strNoItems = _T("");
	CString str = GetFilterText();
	m_ImgList.Create(16, 16, ILC_MASK | ILC_COLOR24, 0, 1);
	CBitmap bmp;
	bmp.LoadBitmap(IDB_FILETREE);
	m_ImgList.Add(&bmp, RGB(0, 128, 128));
	SetImageList(&m_ImgList);

	AddColumn(new CXTPReportColumn(0, _T(""), 1, FALSE));
	AddColumn(new CXTPReportColumn(1, _T(""), 20, FALSE, 22));
	AddColumn(new CXTPReportColumn(2, _T("说明"), 54));
	AddColumn(new CXTPReportColumn(3, _T("类型"), 60, FALSE));
	AddColumn(new CXTPReportColumn(4, _T("地址"), 40, FALSE));
	AddColumn(new CXTPReportColumn(5, _T("时间"), 160, FALSE));
	Populate();

	SetGridColor(::GetSysColor(COLOR_WINDOW));

	return 0;
}

void CSpaceOutput::OnSize(UINT nType, int cx, int cy)
{
	CXTPReportControl::OnSize(nType, cx, cy);
}

void CSpaceOutput::OnSelectAll()
{
}

void CSpaceOutput::Clear()
{
	ResetContent();
	Populate();
}

void CSpaceOutput::PrintOut(CString str, bool twice /* = true */)
{
	if(!twice)
	{
		int nCount = GetRecords()->GetCount();
		COutputRecord* pRecord;
		for(int i = 0; i < nCount; ++i)
		{
			pRecord = (COutputRecord *)GetRecords()->GetAt(i);
			if(pRecord->m_strOut == str)	return;
		}
	}
	PrintOut(Normal, _T(""), _T(""), str);
}

void CSpaceOutput::PrintOut(UINT type, CString strType, CString strAddr, CString strOut)
{
	if(type == CSpaceOutput::Warning && !SoftInfo::CSoftInfo::GetMe().isOutWarning())	return;
	AddRecord(new COutputRecord(type, strType, strAddr, strOut));
	if(!m_bLockUpdate)			Populate();
	if(!m_bLock){
		CXTPReportRows* rows = GetRows();
		SetFocusedRow(rows->GetAt(rows->GetCount() - 1));
	}
}

void CSpaceOutput::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	CXTPOfficeBorder<CXTPReportControl>::OnLButtonDblClk(nFlags, point);
	CXTPReportRow* row = HitTest(point);
	if(!row)							return;
	COutputRecord* pRecord = (COutputRecord *)row->GetRecord();
	if(!pRecord)						return;
	if(pRecord->m_uiType != Information &&
		pRecord->m_uiType != Warning &&
		pRecord->m_uiType != Error)		return;

	if(pRecord->m_strType.Trim() == _T("变量"))
	{
		ShowItem(pRecord->m_strAddr);
	}
}

void CSpaceOutput::OnRButtonUp(UINT nFlags, CPoint point)
{
	CXTPOfficeBorder<CXTPReportControl>::OnRButtonUp(nFlags, point);
	GetCursorPos(&point);
	CMenu popMenu;
	popMenu.LoadMenu(IDR_RBUTTON_MENU);
	CMenu* menu = popMenu.GetSubMenu(6);
	if(m_bLock)		menu->CheckMenuItem(ID_OUTPUT_LOCK, MF_CHECKED);
	else			menu->CheckMenuItem(ID_OUTPUT_LOCK, MF_UNCHECKED);
	CXTPCommandBars::TrackPopupMenu(menu, TPM_LEFTALIGN|TPM_NONOTIFY, point.x, point.y, this);
}

// 从本文中找到对应的变量，并显示出来
void CSpaceOutput::ShowItem(CString strAddr)
{
	if(strAddr.Trim() == _T(""))		return;
	UINT id = (UINT)atoi(strAddr);
	MVC::Item::CItemMgr::GetMe().ShowItem(id);
}

void Space::CSpaceOutput::OnOutputClear()
{
	Clear();
}

void Space::CSpaceOutput::OnOutputLock()
{
	m_bLock = !m_bLock;
}

BOOL Space::CSpaceOutput::OnHelpInfo(HELPINFO* pHelpInfo)
{
	SoftInfo::CMyHelp::GetMe().ShowHelp(_T("输出栏"));
	return CWnd::OnHelpInfo(pHelpInfo);
}