// ItemGrid.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "DXPEditor.h"
#include "MainFrm.h"
#include "Gbl.h"
#include "MyHelp.h"
#include "ProjectMgr.h"
#include "SoftInfo.h"

#include "CamGrid.h"
#include "CamView.h"
#include "CamDoc.h"
#include "CamMgr.h"
#include "Camera.h"
#include "PropertyCam.h"

#include "PropertyDlg.h"

#include <afxdisp.h>
#include <time.h>
#include <locale.h>

const CString NEW_ITEM = _T("�½�����ͷ...");
//const CString VAL_TYPE[] = {_T("λ����"), _T("�з����ֽ�"), _T("�޷����ֽ�"), _T("�з��ŵ���"), _T("�޷��ŵ���"),
//_T("�з���˫��"), _T("�޷���˫��"), _T("�����ȸ���")/*, _T("˫���ȸ���")*/};

CString m_strCamSortCol = _T("");
std::list<UINT> ltShowCamID;

// CCamGrid
using namespace MVC;
using namespace Camera;

BEGIN_MESSAGE_MAP(CCamGrid,CUGCtrl)
	//{{AFX_MSG_MAP(CFindGrid)
	// NOTE - the ClassWizard will add and remove mapping macros here.
	//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG_MAP
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()

CCamGrid::CCamGrid()
:m_bWatch(false)
{
	//UGXPThemes::UseThemes(false);		//!< ȥ���κ����⣬���һ��ã��ٶ�Ҳ�ܿ��
	UGXPThemes::UseThemes(false);
	UGXPThemes::SetGridStyle(Style1);
	m_ItemFont.CreateFont(13, 0, 0, 0, 400, 0, 0, 0, 0, 0, 0, 0, 0, _T("����"));
	m_HeadFont.CreateFont(13, 0, 0, 0, 900, 0, 0, 0, 0, 0, 0, 0, 0, _T("����"));
}

CCamGrid::~CCamGrid()
{
	UGXPThemes::CleanUp();
}

BOOL MVC::Camera::CCamGrid::OnEraseBkgnd(CDC* pDC)
{
	return TRUE;
}

void CCamGrid::OnSetup()
{
	ClearAllBitmaps();
	AddBitmap(IDB_BMP_CAMERA);
	AddBitmap(IDB_BMP_CAMERA);
	AddBitmap(IDB_BMP_ADD);
	InitAll();
}

void CCamGrid::InitAll()
{
	CUGCell cell;

	//!< ���õ�Ԫ��ĸ�ʽ
	GetGridDefault(&cell);
	cell.SetAlignment(UG_ALIGNRIGHT | UG_ALIGNVCENTER);
	SetGridDefault(&cell);

	//!< �����ֶα���ĸ�ʽ
	GetHeadingDefault(&cell);
	cell.SetAlignment(UG_ALIGNCENTER | UG_ALIGNVCENTER);
	cell.SetFont(&m_HeadFont);
	SetHeadingDefault(&cell);

	//!< ���ñ��ĸ�ʽ
	EnableExcelBorders(FALSE);						//!< ȡ����ʾ�ı����
	SetDoubleBufferMode(TRUE);						//!< ˫����
	SetHighlightRow(TRUE, FALSE);					//!< ��������ѡ��
	SetMultiSelectMode(TRUE);						//!< ���ÿɶ�ѡ
	PrintSetOption(UG_PRINT_TOPHEADING, TRUE);		//!< ��ӡ��Ҫ����ͷ
	SetCurrentCellMode(2);
	SetVScrollMode (UG_SCROLLTRACKING);
	SetUniformRowHeight(TRUE);

	SetDefFont(&m_ItemFont);
	SoftInfo::CSoftInfo* csi = &SoftInfo::CSoftInfo::GetMe();
	int colCount = 0;
	for(int i = 0; i < (int)csi->m_vtCamCol.size(); ++i){
		if(csi->m_vtCamCol[i].bShow)
			++colCount;
	}
	if(colCount == 0)	colCount = 1;
	SetNumberCols(colCount);

	m_iArrowIndex = AddCellType(&m_sortArrow);
	SetRowHeight(0, 20);
	SetSH_Width(10);

	//!< ��ʼ����
	InitCol();

	m_bSortType = TRUE;
}

void CCamGrid::InitCol()
{
	CUGCell cell;
	int i = 0, index = 0;
	SoftInfo::CSoftInfo* csi = &SoftInfo::CSoftInfo::GetMe();
	GetCell(0, -1, &cell);
	for(i = 0; i < (int)csi->m_vtCamCol.size(); ++i){	//!< ���������ֶ���
		if(!csi->m_vtCamCol[i].bShow)	continue;
		cell.SetText(csi->m_vtCamCol[i].name);			//!< �����ֶε�����
		cell.SetAlignment(UG_ALIGNCENTER);				//!< ���������ʾ
		m_mpColName[index] = csi->m_vtCamCol[i].name;	//!< ��¼�е�����
		SetColWidth(index, csi->m_vtCamCol[i].width);	//!< �����е�Ĭ�Ͽ��
		SetCell(index++, -1, &cell);
	}
	LockColumns(1);										//!< ��ס��һ���У�������Զ��ʾ
}

bool CCamGrid::InitItemOne(UINT id, UINT row)
{
	boost::shared_ptr<CCamera> item = CCamMgr::GetMe().GetCam(id);
	if(!item)	return false;
	CComVariant cvr;
	CUGCell cell;
	SoftInfo::CSoftInfo* csi = &SoftInfo::CSoftInfo::GetMe();

	if(!item){
		CString error;
		error.Format("���Ϊ %d ������ͷ������",id);
		MessageBox(error, _T("��ʾ"), MB_OK | MB_ICONINFORMATION);
		return false;
	}
	CString colName;
	CString text;
	COLORREF color = GetSysColor(COLOR_WINDOW);
	int colCount = GetNumberCols();
	for(int i = 0; i < colCount; ++i)
	{
		colName = m_mpColName[i];
		GetCell(i, row, &cell);
		if(csi->m_vtCamCol[COL_ID].name == colName)								//!< ���
		{
			cell.SetNumber(item->GetID());
			cell.SetBitmap(GetBitmap(0));
			SetMyCell(i, row, &cell, id);
		}
		else if(csi->m_vtCamCol[COL_TAG].name == colName)						//!< ˵��
		{
			cell.SetText(item->GetTag());
			SetMyCell(i, row, &cell, id);
		}
		else if(csi->m_vtCamCol[COL_RECORD].name == colName)					//!< �Ƿ�¼��
		{
			cell.SetText(item->IsRecord() ? _T("��") : _T("��"));
			SetMyCell(i, row, &cell, id);
		}
// 		else if(csi->m_vtCamCol[COL_AUTOSAVE].name == colName)					//!< �Ƿ��Զ�����
// 		{
// 			cell.SetText(item->IsAutoSave() ? _T("��") : _T("��"));
// 			SetMyCell(i, row, &cell, id);
// 		}
		else if(csi->m_vtCamCol[COL_PATH].name == colName)						//!< ����·��
		{
			if(item->IsRecord())		cell.SetText(item->GetFilePath());
			else						cell.SetText(_T(""));
			SetMyCell(i, row, &cell, id);
		}
		else if(csi->m_vtCamCol[COL_ENCODE].name == colName)					//!< �����ʽ
		{
			if(item->IsRecord())		cell.SetText(item->GetEncodeType());
			else						cell.SetText(_T(""));
			SetMyCell(i, row, &cell, id);
		}
		else if(csi->m_vtCamCol[COL_DISTINGUISH].name == colName)				//!< ¼�Ʒֱ���
		{
			if(item->IsRecord())		cell.SetText(item->GetDistinguish());
			else						cell.SetText(_T(""));
			SetMyCell(i, row, &cell, id);
		}
		else if(csi->m_vtCamCol[COL_FPS].name == colName)						//!< ¼��֡����
		{
			if(item->IsRecord())		cell.SetText(item->GetFPS());
			else						cell.SetText(_T(""));
			SetMyCell(i, row, &cell, id);
		}
		else if(csi->m_vtCamCol[COL_BROAD].name == colName)						//!< �Ƿ���������㲥
		{
			cell.SetText(item->IsBroadCast() ? _T("��") : _T("��"));
			SetMyCell(i, row, &cell, id);
		}
		else if(csi->m_vtCamCol[COL_PORT].name == colName)						//!< �㲥�˿ں�,8080
		{
			if(item->IsBroadCast())		cell.SetNumber(item->GetPort());
			else						cell.SetText(_T(""));
			SetMyCell(i, row, &cell, id);
		}
		else if(csi->m_vtCamCol[COL_MAXCLIENT].name == colName)					//!< ���������
		{
			if(item->IsBroadCast())		cell.SetNumber(item->GetMaxClient());
			else						cell.SetText(_T(""));
			SetMyCell(i, row, &cell, id);
		}
		else if(csi->m_vtCamCol[COL_BROADENCODE].name == colName)				//!< �㲥��Ƶ�����ʽ,MWV2
		{
			if(item->IsBroadCast())		cell.SetText(item->GetBroadEncodeType());
			else						cell.SetText(_T(""));
			SetMyCell(i, row, &cell, id);
		}
		else if(csi->m_vtCamCol[COL_BROADDISTINGUISH].name == colName)			//!< �㲥�ֱ���
		{
			if(item->IsBroadCast())		cell.SetText(item->GetBroadDistinguish());
			else						cell.SetText(_T(""));
			SetMyCell(i, row, &cell, id);
		}
		else if(csi->m_vtCamCol[COL_BROADFPS].name == colName)					//!< �㲥֡����
		{
			if(item->IsBroadCast())		cell.SetText(item->GetBroadFPS());
			else						cell.SetText(_T(""));
			SetMyCell(i, row, &cell, id);
		}
	}
	return true;
}

void CCamGrid::SetMyCell(int nCol, long lRow, CUGCell* cell, UINT id)
{
	cell->SetParam(id);
	cell->SetBackColor(::GetSysColor(COLOR_WINDOW));
	cell->SetHBackColor(::GetSysColor(COLOR_HIGHLIGHT));
	SetCell(nCol, lRow, cell);
}

void CCamGrid::InitAddRow()
{
	int row = GetNumberRows() - 1;
	CUGCell cell;
	JoinCells(0, row, GetNumberCols() - 1, row);
	GetCell(0, row, &cell);
	cell.SetParam(-1);
	cell.SetText(NEW_ITEM);
	cell.SetAlignment(UG_ALIGNLEFT);
	cell.SetBitmap(GetBitmap(2));
	SetCell(0, row, &cell);
}

//!< ɾ��������
void CCamGrid::DeleteRowAll()
{
	UnJoinCells(0,GetNumberRows() - 1);
	SetNumberRows(0);
}

//!< ɾ��������
void CCamGrid::DeleteColAll()
{
	SetNumberCols(0);
}

//!< �༭������ͳһ����
void CCamGrid::OnEditEnd()
{
	CCamMgr::GetMe().SetEditEnd();
	SetFocus();
	RedrawAll();
}

//!< ������ı��⣬��������
void CCamGrid::OnTH_LClicked(int iCol, long lRow, int iUpdn, RECT* pRect, POINT* pPoint, BOOL bProcessed)
{
	if(iUpdn == 0)						return;

	QuickSetCellType(m_iSortCol, -1, 0);
	if(iCol == m_iSortCol)		m_bSortType = !m_bSortType;
	else						m_bSortType = TRUE;
	m_iSortCol = iCol;
	m_strCamSortCol = m_mpColName[iCol];
	SortGrid();
}

//!< ˫�����
void CCamGrid::OnDClicked(int col,long row,RECT *rect,POINT *point,BOOL processed)
{
	CUGCtrl::OnDClicked(col, row, rect, point, processed);
	if(row < 0)		return;										//!< û�����ݣ��˳�
	CUGCell cell;
	GetCell(0, row, &cell);
	UINT itemID = cell.GetParam();
	CCamView* pView = (CCamView *)GetParent();

	if(row == GetNumberRows() - 1){
		pView->OnItemAdd();
		SelectRowAt(GetNumberRows() - 1);
	}
	else if(!CProjectMgr::GetMe().IsWatch())
		pView->OnItemEdit();
	SetFocus();
}

//!< �༭������������
void CCamGrid::EditItem(boost::shared_ptr<CCamera> item)
{
	if(!item)					return;
	CPropertyCamera* pp = &CCamMgr::GetMe().m_ItemProperty;
	pp->SetType(false);
	pp->SetItem(item.get());
	Dialog::CPropertyDlg* dlg = &Dialog::CPropertyDlg::GetMe();
	dlg->DoModal(*pp);
	SetFocus();
}

//!< ͳһ�༭������������
// void CCamGrid::EditItems(std::list<boost::shared_ptr<CCamera> > ltItem)
// {
// 	if(ltItem.empty())			{return;}
// 	if(ltItem.size() == 1)		{ItemEdit();		return;}
// 
// 	CPropertyItems* pp = &CCameraMgr::GetMe().m_ItemPropertys;
// 	pp->SetItemList(ltItem);
// 	Dialog::CPropertyDlg* dlg = &Dialog::CPropertyDlg::GetMe();
// 
// 	dlg->DoModal(*pp);
// }

//!< �����±���
boost::shared_ptr<CCamera> CCamGrid::AddNewItem()
{
	MVC::Camera::CCamMgr* camMgr = &MVC::Camera::CCamMgr::GetMe();
	boost::shared_ptr<CCamera> empty;
	MVC::Camera::CPropertyCamera* pp = &MVC::Camera::CCamMgr::GetMe().m_ItemProperty;
	pp->SetType(true);

	Dialog::CPropertyDlg* dlg = &Dialog::CPropertyDlg::GetMe();
	if(dlg->DoModal(*pp) == IDOK){
		return camMgr->GetCam(pp->m_NewItem->GetID());
	}
	return empty;
}

//!< ����������������������Ǹ����Ļ�����Ҫ���⴦��
void CCamGrid::OnLClicked(int col,long row,int updn,RECT *rect,POINT *point,BOOL processed)
{
	if(updn == 1)		return;		//!< ��갴�²���
	int sCol, eCol;
	long sRow, eRow;
	m_GI->m_multiSelect->GetCurrentBlock(&sCol, &sRow, &eCol, &eRow);
	if(eRow < 0 || eRow >= GetNumberRows() - 1)	return;		//!< ������һ��Ҳ��Ч

	CRect rects;
	GetRangeRect(sCol, sRow, eCol - 1, eRow, rects);
	int iRight = rects.right;
	int iBottom = rects.bottom;
	rects = CRect(iRight - 10, iBottom - 10, iRight + 4, iBottom + 1);
	if(rects.PtInRect(*point))
	{
		ReleaseCapture();
		CUGCell cell;
		GetCell(0, eRow, &cell);
		boost::shared_ptr<CCamera> item = CCamMgr::GetMe().GetCam(cell.GetParam());
		ASSERT(item);
		CCamView* pView = (CCamView *)GetParent();
		if(!pView)					return;
		//pView->OnClone();
	}
}

//!< �Ҽ����updn��TRUE��ʾdown����FALSE��ʾup
void CCamGrid::OnRClicked(int col,long row,int updn,RECT *rect,POINT *point,BOOL processed)
{
	SetFocus();
	if(updn == FALSE){
		CMenu popMenu;
		ClientToScreen(point);
		CRect rect;
		GetCellRect(0, -1, &rect);
		point->x += GetSH_Width();
		point->y += rect.Height();	//!< ����Ȼû�аѱ߿��������ߣ��ɶ�
		popMenu.LoadMenu(IDR_RBUTTON_MENU);
		CMenu* menu =popMenu.GetSubMenu(9);
		//!< ��������ط����Ҽ�����ֲ�ͬ�Ĳ˵���
		CCamMgr* camMgr = &CCamMgr::GetMe();
		if(!CProjectMgr::GetMe().IsWatch()){				//!< δ���
		}
		else{												//!< ���
			menu->EnableMenuItem(ID_CAM_ADD, MF_DISABLED|MF_BYCOMMAND);
			menu->EnableMenuItem(ID_CAM_EDIT, MF_DISABLED|MF_BYCOMMAND);
			menu->EnableMenuItem(ID_CAM_DELETE, MF_DISABLED|MF_BYCOMMAND);
			menu->EnableMenuItem(ID_ITEM_UP, MF_DISABLED|MF_BYCOMMAND);
			menu->EnableMenuItem(ID_ITEM_DOWN, MF_DISABLED|MF_BYCOMMAND);
			menu->EnableMenuItem(ID_CAM_EDIT_ALL, MF_DISABLED|MF_BYCOMMAND);
			menu->EnableMenuItem(ID_EDIT_CUT, MF_DISABLED|MF_BYCOMMAND);
			menu->EnableMenuItem(ID_EDIT_PASTE, MF_DISABLED|MF_BYCOMMAND);
		}
		if(row == -1 || row >= GetNumberRows() - 1){		//!< �����һ��
			menu->EnableMenuItem(ID_EDIT_COPY, MF_DISABLED|MF_BYCOMMAND);
			menu->EnableMenuItem(ID_EDIT_CUT, MF_DISABLED|MF_BYCOMMAND);
			menu->EnableMenuItem(ID_ITEM_EDIT, MF_DISABLED|MF_BYCOMMAND);
			menu->EnableMenuItem(ID_VKDELETE, MF_DISABLED|MF_BYCOMMAND);
			menu->EnableMenuItem(ID_ITEM_UP, MF_DISABLED|MF_BYCOMMAND);
			menu->EnableMenuItem(ID_ITEM_DOWN, MF_DISABLED|MF_BYCOMMAND);
			menu->EnableMenuItem(ID_CAM_EDIT_ALL, MF_DISABLED|MF_BYCOMMAND);
		}
		if(camMgr->m_ltItemClipBoard.empty()){				//!< ��������û����
			menu->EnableMenuItem(ID_EDIT_PASTE, MF_DISABLED|MF_BYCOMMAND);
		}
		if(GetNumberRows() > 1 && row == 0 ){				//!< ���һ�б���
			menu->EnableMenuItem(ID_ITEM_UP, MF_DISABLED|MF_BYCOMMAND);
		}
		if(GetNumberRows() > 1 && row == GetNumberRows() - 2){	//!< �����һ�б���
			menu->EnableMenuItem(ID_ITEM_DOWN, MF_DISABLED|MF_BYCOMMAND);
		}
		//!< ��¼��ѡ�еı����������ѡ�ˣ����ܿ�¡
		std::list<long> ltSelectRow;
		for(long row = GetNumberRows() - 2; row >= 0; --row){
			if(IsSelected(0, row, 0))		ltSelectRow.push_back(row);
		}
		if(ltSelectRow.size() > 1){
			menu->EnableMenuItem(ID_CAM_EDIT, MF_DISABLED|MF_BYCOMMAND);
			menu->EnableMenuItem(ID_ITEM_UP, MF_DISABLED|MF_BYCOMMAND);
			menu->EnableMenuItem(ID_ITEM_DOWN, MF_DISABLED|MF_BYCOMMAND);
		}
		menu->EnableMenuItem(ID_PROJ_RENAME, MF_DISABLED|MF_BYCOMMAND);
		CXTPCommandBars::TrackPopupMenu(menu, TPM_LEFTALIGN|TPM_NONOTIFY, point->x, point->y, this);
	}
}

void CCamGrid::OnKeyDown(UINT *vcKey,BOOL processed)
{
	CUGCtrl::OnKeyDown(vcKey, processed);
	long cul_row = GetCurrentRow();
	if(*vcKey == VK_RETURN){
		CRect rc(0,0,0,0);
		CPoint point(0,0);
		OnDClicked(0, cul_row, &rc, &point, processed);
	}
	else if(*vcKey == VK_DELETE){
		CCamView* pView = (CCamView *)GetParent();
		if(pView)	pView->OnVkdelete();
	}
	else if(*vcKey == 0x41){			//!< �����A
		if(GetAsyncKeyState(VK_CONTROL)){
			CCamView* pView = (CCamView *)GetParent();
			if(pView)	pView->OnSelectAll();
		}
	}
	else if(*vcKey == VK_F1){
		SoftInfo::CMyHelp::GetMe().ShowHelp(_T("��DView����������ͷ"));
	}
}

//!< ����ƶ��¼�
void MVC::Camera::CCamGrid::OnMouseMove(int col,long row,POINT *point,UINT nFlags,BOOL processed)
{
	if(nFlags == 0)		//!< 0��ͨ�ƶ���1�����������ƶ���2�Ҽ��ƶ���16�����ƶ�
	{
		int sCol, eCol;
		long sRow, eRow;
		m_GI->m_multiSelect->GetCurrentBlock(&sCol, &sRow, &eCol, &eRow);
		if(eRow < 0 || eRow >= GetNumberRows() - 1)	return;		//!< ������һ��Ҳ��Ч
		CRect rect;
		GetRangeRect(sCol, sRow, eCol - 1, eRow, rect);
		int iRight = rect.right;
		int iBottom = rect.bottom;
		rect = CRect(iRight - 10, iBottom - 10, iRight + 4, iBottom + 1);
		if(rect.PtInRect(*point))		::SetCursor(::LoadCursor(NULL, IDC_CROSS));
		else							::SetCursor(::LoadCursor(NULL, IDC_ARROW));
	}
}

//!< �����Ҽ��˵�����Ϣ��iItem�ǲ˵����ID��lRow��iCol���Ҽ���λ�ã�����ֵ��
void CCamGrid::OnMenuCommand(int iCol, long lRow, int iSection, int iItem)
{
	UNREFERENCED_PARAMETER(iCol);
	UNREFERENCED_PARAMETER(lRow);
	UNREFERENCED_PARAMETER(iSection);
	CRect rect(0,0,0,0);
	CPoint point(0,0);
	int sortID = 0;
	CCamView* pView = (CCamView *)GetParent();
	if(!pView)			return;
	switch(iItem)
	{
		case MENU_FIND:			FindDialog();			break;		//!< ����
		case ID_CAM_ADD:		pView->OnItemAdd();		break;		//!< ��ӱ���
		case ID_CAM_EDIT:		pView->OnItemEdit();	break;		//!< �༭����
		case ID_CAM_DELETE:		pView->OnVkdelete();	break;		//!< �����Ƴ�
		case ID_EDIT_COPY:		pView->OnEditCopy();	break;		//!< ��������
		case ID_EDIT_CUT:		pView->OnEditCut();		break;		//!< ��������
		case ID_EDIT_PASTE:		pView->OnEditPaste();	break;		//!< ����ճ��
		case ID_ITEM_UP:		pView->OnItemUp();		break;		//!< ��������
		case ID_ITEM_DOWN:		pView->OnItemDown();	break;		//!< ��������
		case ID_SELECT_ALL:		pView->OnSelectAll();	break;		//!< ȫѡ
	}
	SetFocus();
}

///////////////////////////////////////////////////////////////////////////////
int CCamGrid::OnMenuStart(int iCol, long lRow, int iSection)
{
	UNREFERENCED_PARAMETER(iCol);
	UNREFERENCED_PARAMETER(lRow);
	if(iSection == UG_GRID)		return TRUE;
	else						return FALSE;
}

//!< �Ը�����ID�ı�����������
bool CCamGrid::SortItem(UINT& lth, UINT& rth)
{
	bool bSort;
	CCamMgr* camMgr = &CCamMgr::GetMe();
	CCamera *lItem, *rItem;
	lItem = camMgr->GetCam(lth).get();
	rItem = camMgr->GetCam(rth).get();
	CString txtL, txtR;						//!< ������Щ�᲻��ʾ���ַ���,��Ҫ�����������
	int nL, nR;								//!< ������Щ�᲻��ʾ������,��Ҫ�����������
	SoftInfo::CSoftInfo* csi = &SoftInfo::CSoftInfo::GetMe();

	//!< �ַ����Ƚ�
	if(m_strCamSortCol == csi->m_vtCamCol[COL_TAG].name)
	{
		bSort = lItem->GetTag().Compare(rItem->GetTag()) <= 0 ? true : false;
	}
	else if(m_strCamSortCol == csi->m_vtCamCol[COL_PATH].name)
	{
		bSort = lItem->GetFilePath().Compare(rItem->GetFilePath()) <= 0 ? true : false;
	}
	else if(m_strCamSortCol == csi->m_vtCamCol[COL_ENCODE].name)
	{
		if(lItem->IsRecord())				txtL = lItem->GetEncodeType();
		else								txtL = _T("");
		if(rItem->IsRecord())				txtR = rItem->GetEncodeType();
		else								txtR = _T("");
		bSort = txtL.Compare(txtR) <= 0 ? true : false;
	}
	else if(m_strCamSortCol == csi->m_vtCamCol[COL_DISTINGUISH].name)
	{
		if(lItem->IsRecord())				txtL = lItem->GetDistinguish();
		else								txtL = _T("");
		if(rItem->IsRecord())				txtR = rItem->GetDistinguish();
		else								txtR = _T("");
		bSort = txtL.Compare(txtR) <= 0 ? true : false;
	}
	else if(m_strCamSortCol == csi->m_vtCamCol[COL_FPS].name)
	{
		if(lItem->IsRecord())				txtL = lItem->GetFPS();
		else								txtL = _T("");
		if(rItem->IsRecord())				txtR = rItem->GetFPS();
		else								txtR = _T("");
		bSort = txtL.Compare(txtR) <= 0 ? true : false;
	}
	else if(m_strCamSortCol == csi->m_vtCamCol[COL_BROADENCODE].name)
	{
		if(lItem->IsBroadCast())			txtL = lItem->GetBroadEncodeType();
		else								txtL = _T("");
		if(rItem->IsBroadCast())			txtR = rItem->GetBroadEncodeType();
		else								txtR = _T("");
		bSort = txtL.Compare(txtR) <= 0 ? true : false;
	}
	else if(m_strCamSortCol == csi->m_vtCamCol[COL_BROADDISTINGUISH].name)
	{
		if(lItem->IsBroadCast())			txtL = lItem->GetBroadDistinguish();
		else								txtL = _T("");
		if(rItem->IsBroadCast())			txtR = rItem->GetBroadDistinguish();
		else								txtR = _T("");
		bSort = txtL.Compare(txtR) <= 0 ? true : false;
	}
	else if(m_strCamSortCol == csi->m_vtCamCol[COL_BROADFPS].name)
	{
		if(lItem->IsBroadCast())			txtL = lItem->GetBroadFPS();
		else								txtL = _T("");
		if(rItem->IsBroadCast())			txtR = rItem->GetBroadFPS();
		else								txtR = _T("");
		bSort = txtL.Compare(txtR) <= 0 ? true : false;
	}

	//!< ��ֵ�Ƚ�
	else if(m_strCamSortCol == csi->m_vtCamCol[COL_ID].name)
	{
		bSort = lItem->GetID() <= rItem->GetID();
	}
	else if(m_strCamSortCol == csi->m_vtCamCol[COL_RECORD].name)
	{
		bSort = lItem->IsRecord() <= rItem->IsRecord();
	}
// 	else if(m_strCamSortCol == csi->m_vtCamCol[COL_AUTOSAVE].name)
// 	{
// 		bSort = lItem->IsAutoSave() <= rItem->IsAutoSave();
// 	}
	else if(m_strCamSortCol == csi->m_vtCamCol[COL_BROAD].name)
	{
		bSort = lItem->IsBroadCast() <= rItem->IsBroadCast();
	}
	else if(m_strCamSortCol == csi->m_vtCamCol[COL_PORT].name)
	{
		if(lItem->IsBroadCast())			nL = lItem->GetPort();
		else								nL = 0;
		if(rItem->IsBroadCast())			nR = rItem->GetPort();
		else								nR = 0;
		bSort = nL <= nR;
	}
	else if(m_strCamSortCol == csi->m_vtCamCol[COL_MAXCLIENT].name)
	{
		if(lItem->IsBroadCast())			nL = lItem->GetMaxClient();
		else								nL = 0;
		if(rItem->IsBroadCast())			nR = rItem->GetMaxClient();
		else								nR = 0;
		bSort = nL <= nR;
	}
//	if(!__bSortType)	bSort = !bSort;
	return !bSort;
}

//!< ���򣬱Ƚ�������Ԫ���ڵ����ݵĴ�С
int CCamGrid::OnSortEvaluate(CUGCell* pCell1, CUGCell* pCell2, int iFlags)
{
	//!< ������ô�ţ��½�����һ����������ڱ��ĩβ
	CString text1=(CString)pCell1->GetText();
	CString text2=(CString)pCell2->GetText();
	if(text1 == _T("�½�����ͷ...") || text1=="")				return 1;
	else if(text2 == _T("�½�����ͷ...") || text2=="")		return -1;

	//!< ����ǴӴ�С���У��Ͱ���������Ԫ�񻥻��˾ͺ���
	if( iFlags & UG_SORT_DESCENDING ){
		CUGCell *ptr = pCell1;
		pCell1 = pCell2;
		pCell2 = ptr;
	}
	COleDateTime date1, date2;
	int retVal = 0;

	CString colName = m_mpColName[m_iSortCol];
	switch( pCell1->GetDataType() )
	{
	case UGCELLDATA_NUMBER:
	case UGCELLDATA_BOOL:
	case UGCELLDATA_CURRENCY:
		if(pCell1->GetNumber() < pCell2->GetNumber())		retVal = -1;
		else												retVal = 1;
		break;

	default:
		retVal = _tcscmp( pCell1->GetText( ), pCell2->GetText());
	}
	return retVal;
}

void CCamGrid::ShowItem(std::list<UINT> ltItemID)
{
	EnableUpdate(FALSE);
	DeleteRowAll();

	SetNumberRows((int)ltItemID.size() + 1);
	int cul_row = 0;
	if(m_bSortType)
	{
		std::list<UINT>::iterator iter;
		for(iter = ltItemID.begin(); iter != ltItemID.end(); ++iter){
			InitItemOne(*iter, cul_row++);
		}
	}
	else
	{
		std::list<UINT>::reverse_iterator iter;
		for(iter = ltItemID.rbegin(); iter != ltItemID.rend(); ++iter){
			InitItemOne(*iter, cul_row++);
		}
	}

	InitAddRow();
	EnableUpdate(TRUE);
	SetFocus();
}

void CCamGrid::RedrawGrid()
{
	CCamMgr* camMgr = &CCamMgr::GetMe();
	std::list<UINT> ltItemID;
	boost::shared_ptr<CCamera> item;
	foreach(item, camMgr->m_vtCam){
		if(item)				ltItemID.push_back(item->GetID());
	}
	ShowItem(ltItemID);
}

void MVC::Camera::CCamGrid::SortGrid()
{
	if(m_bSortType){
//		SortBy(m_iSortCol, UG_SORT_ASCENDING);				//!< Ĭ���������
		QuickSetCellType(m_iSortCol, -1, m_iArrowIndex);	//!< Ĭ����ʾ����ġ����Ǻš���λ��
		QuickSetCellTypeEx(m_iSortCol, -1, UGCT_SORTARROWDOWN);
	}
	else{
//		SortBy(m_iSortCol, UG_SORT_DESCENDING );
		QuickSetCellType(m_iSortCol, -1, m_iArrowIndex );
		QuickSetCellTypeEx(m_iSortCol, -1, UGCT_SORTARROWUP );
	}

//	std::list<UINT> ltShowCamID;
	ltShowCamID.clear();
	CCamMgr* camMgr = &CCamMgr::GetMe();
	boost::shared_ptr<CCamera> item;
	foreach(item, camMgr->m_vtCam){
		if(item)
			ltShowCamID.push_back(item->GetID());
	}
	ltShowCamID.sort(SortItem);
	ShowItem(ltShowCamID);
// 	RedrawAll();
}

void MVC::Camera::CCamGrid::ItemUp()
{
	std::list<long> ltSelectRow;
	for(long row = GetNumberRows() - 2; row >= 0; --row){
		if(IsSelected(0, row, 0))		ltSelectRow.push_back(row);
	}
	if(ltSelectRow.size() > 1)						return;
	int row = GetCurrentRow();
	if(row < 1 || row >= GetNumberRows() - 1)		return;
	CUGCell cell1,cell2;
	GetCell(0, row, &cell1);
	GetCell(0, row - 1, &cell2);
	boost::shared_ptr<CCamera> item1 = CCamMgr::GetMe().GetCam(cell1.GetParam());
	boost::shared_ptr<CCamera> item2 = CCamMgr::GetMe().GetCam(cell2.GetParam());
	ASSERT(item1 && item2);

	//!< ���ԭʼ�ı������ݣ����Ҫѹ����ջ
	boost::shared_ptr<SCamUndo> undo1 = boost::shared_ptr<SCamUndo>(new SCamUndo(CGbl::UNDO_TYPE_UPD, item1, 0));
	boost::shared_ptr<SCamUndo> undo2 = boost::shared_ptr<SCamUndo>(new SCamUndo(CGbl::UNDO_TYPE_UPD, item2, 0));

	//!< ��������������
	CCamMgr::GetMe().ExChangeItem(item1->GetID(), item2->GetID());
	InitItemOne(item2->GetID(), row);
	InitItemOne(item1->GetID(), row - 1);

	//!< ��ӳ�������
	CCamDoc* pDoc = (CCamDoc *)((CCamView *)GetParent())->GetDocument();
	pDoc->AddUndoMember(undo1);
	pDoc->AddUndoMember(undo2);
	pDoc->SetUndoEnd();

	SelectRowAt(row - 1);										//!< ѡ���½�������һ��
	RedrawAll();
}

void MVC::Camera::CCamGrid::ItemDown()
{
	std::list<long> ltSelectRow;
	for(long row = GetNumberRows() - 2; row >= 0; --row){
		if(IsSelected(0, row, 0))		ltSelectRow.push_back(row);
	}
	if(ltSelectRow.size() > 1)						return;
	int row = GetCurrentRow();
	if(row < 0 || row >= GetNumberRows() - 2)		return;
	CUGCell cell1,cell2;
	GetCell(0, row, &cell1);
	GetCell(0, row + 1, &cell2);
	boost::shared_ptr<CCamera> item1 = CCamMgr::GetMe().GetCam(cell1.GetParam());
	boost::shared_ptr<CCamera> item2 = CCamMgr::GetMe().GetCam(cell2.GetParam());
	ASSERT(item1 && item2);

	//!< ���ԭʼ�ı������ݣ����Ҫѹ����ջ
	boost::shared_ptr<SCamUndo> undo1 = boost::shared_ptr<SCamUndo>(new SCamUndo(CGbl::UNDO_TYPE_UPD, item1, 0));
	boost::shared_ptr<SCamUndo> undo2 = boost::shared_ptr<SCamUndo>(new SCamUndo(CGbl::UNDO_TYPE_UPD, item2, 0));

	//!< ��������������
	CCamMgr::GetMe().ExChangeItem(item1->GetID(), item2->GetID());
	InitItemOne(item2->GetID(), row);
	InitItemOne(item1->GetID(), row + 1);

	//!< ��ӳ�������
	CCamDoc* pDoc = (CCamDoc *)((CCamView *)GetParent())->GetDocument();
	pDoc->AddUndoMember(undo1);
	pDoc->AddUndoMember(undo2);
	pDoc->SetUndoEnd();

	SelectRowAt(row + 1);										//!< ѡ���½�������һ��
	RedrawAll();
}

//!< ͳ��һ��Ҫɾ���ı�����������ʾ�Ƿ�Ҫѯ��һ���û�������ʱ��Ҫɾ�����Ͳ���Ҫ֪ͨ�û�
void MVC::Camera::CCamGrid::ItemRemove(bool bAsk /* = true */)
{
	std::list<long> ltSelectRow;
	//!< ��¼��ѡ�е��кţ��Ӻ���ǰ������Ϊɾ��ʱҪ�Ӻ���ǰɾ
	for(long row = GetNumberRows() - 2; row >= 0; --row)
		if(IsSelected(0, row, 0))
			ltSelectRow.push_back(row);

	if(ltSelectRow.empty())				return;
	if(bAsk)
	{
		CString text;
		text.Format("ȷ��Ҫ�Ƴ��� %d ��������",(int)ltSelectRow.size());
		if(IDYES != MessageBox(text, _T("�Ƴ���ʾ"), MB_YESNO)){
			SetFocus();
			return;
		}
	}

	CCamMgr* camMgr = &CCamMgr::GetMe();
	CUGCell cell;
	boost::shared_ptr<CCamera> item;
	std::list<boost::shared_ptr<CCamera> > ltSelectItem;

	//!< ͳ�Ʊ�ѡ�еı���
	foreach(long row, ltSelectRow)
	{
		GetCell(0, row, &cell);
		item = camMgr->GetCam(cell.GetParam());
		ASSERT(item);
		ltSelectItem.push_back(item);
	}

	ItemRemove(ltSelectItem);				//!< ��ʼɾ��
	m_GI->m_multiSelect->ClearAll();		//!< �������ѡ�����
	SetFocus();

	//!< ��ӳ�������
	CCamDoc* pDoc = (CCamDoc *)((CCamView *)GetParent())->GetDocument();
	boost::shared_ptr<SCamUndo> undo;
	foreach(item, ltSelectItem)
	{
		undo = boost::shared_ptr<SCamUndo>(new SCamUndo);
		undo->m_Item = item;
		ASSERT(!camMgr->FindItem(item->GetID()));
		undo->m_uiEditType = CGbl::UNDO_TYPE_DEL;
		pDoc->AddUndoMember(undo);
	}
	pDoc->SetUndoEnd();

	return;
}

//!< ɾ����Щ����
void MVC::Camera::CCamGrid::ItemRemove(std::list<boost::shared_ptr<CCamera> > ltItem)
{
	CCamMgr* camMgr = &CCamMgr::GetMe();
	boost::shared_ptr<CCamera> item;
	foreach(item, ltItem)
	{
		camMgr->DeleteItem(item->GetID());
	}
}

//!< ��������
void MVC::Camera::CCamGrid::ItemCopy()
{
	CCamMgr* camMgr = &CCamMgr::GetMe();
	int numRow = GetNumberRows() - 1;
	std::list<UINT> ltItemID;
	CUGCell cell;
	boost::shared_ptr<CCamera> srCCamera, newItem;
	for(UINT i = 0; i < numRow; ++i)
	{
		if(IsSelected(0, i))
		{
			GetCell(0, i, &cell);
			ltItemID.push_back(cell.GetParam());
		}
	}
	if(ltItemID.size() <= 0)			return;
	camMgr->m_ltItemClipBoard.clear();
	foreach(UINT id, ltItemID)
	{
		srCCamera = camMgr->GetCam(id);
		ASSERT(srCCamera);
		newItem = boost::shared_ptr<CCamera>(new CCamera());
		*newItem = *srCCamera;
		camMgr->m_ltItemClipBoard.push_back(newItem);
	}
}

//!< ��������
void MVC::Camera::CCamGrid::ItemCut()
{
	ItemCopy();			//!< �ȿ�������ɾ��
	ItemRemove(false);
}

//!< ����ճ��
void MVC::Camera::CCamGrid::ItemPaste()
{
	CCamMgr* camMgr = &CCamMgr::GetMe();
	if(camMgr->m_ltItemClipBoard.empty())						return;
	boost::shared_ptr<CCamera> item, srCCamera;
	CCamDoc* pDoc = (CCamDoc *)((CCamView *)GetParent())->GetDocument();

	//!< ����ֱ�ӰѼ�������ı���ճ���������У�����Ҫ����һ�ݹ�ȥ
	foreach(srCCamera, camMgr->m_ltItemClipBoard)
	{
		item = boost::shared_ptr<CCamera>(new CCamera());
		*item = *srCCamera;
		if(item->IsBroadCast())
			item->SetPort(camMgr->GetDifferentPort(8080));
		if(!camMgr->AddItem(item))
			return;			// �������޾Ͳ�����

		boost::shared_ptr<SCamUndo> undo = boost::shared_ptr<SCamUndo>(new SCamUndo);
		undo->m_bEnd = false;
		undo->m_uiEditType = CGbl::UNDO_TYPE_ADD;
		undo->m_Item = item;
		pDoc->AddUndoMember(undo);

		SetFocus();
	}
	pDoc->SetUndoEnd();
	//!< ���ڿ�ʼճ��
//	CCamDoc* pDoc = (CCamDoc *)((CCamView *)GetParent())->GetDocument();
//	Dialog::CCameraInConfigDlg* dlg = &Dialog::CCameraInConfigDlg::GetMe();
//	if(IDOK != dlg->DoModal(Dialog::CCameraInConfigDlg::Paste, GetGroupID()))	return;
//	pDoc->ItemInAdvanceOptions(ltItem);
// 	foreach(item, ltItem)
// 		if(item->getSrcType() == CCamera::SRC_TYPE_IO)
// 			item->getSrcInfo()->setDevID(devID);
// 	if(dlg->m_nSameNameType == 0)			pDoc->ItemInRenameItem(ltItem, dlg->m_uiGroupID);
// 	else if(dlg->m_nSameNameType == 1)		pDoc->ItemInDelFileItem(ltItem, dlg->m_uiGroupID);
// 	else if(dlg->m_nSameNameType == 2)		pDoc->ItemInDelMgrItem(ltItem, dlg->m_uiGroupID);
// 	else if(dlg->m_nSameNameType == 3)		pDoc->ItemInRenameStrItem(ltItem, dlg->m_strItemSameName, dlg->m_uiGroupID);
//	pDoc->SetUndoEnd();
}

void MVC::Camera::CCamGrid::SelectRowAt(long row)
{
	if(row < 0)									row = 0;
	if(row >= GetNumberRows())					row = GetNumberRows() - 1;
	m_GI->m_multiSelect->ClearAll();
	AdjustComponentSizes();
	m_GI->m_multiSelect->OnLClick(0, row, 0);
	GotoRow(row);
	if(m_GI->m_dragRow == row)		m_GI->m_dragRow = row - 1;
	GotoRow(row);		//!< ��ʾ2��û���⣬��ʾ1��ż������ֲ���ʾ�����⣬��������������
}

void MVC::Camera::CCamGrid::OnDrawFocusRect(CDC *dc,RECT *rect)
{
	if(CProjectMgr::GetMe().IsWatch())	return;		//!< ���ʱ�����ڿ�
	CPen pen, wPen;
	int dcID = dc->SaveDC();

	int startCol, endCol;
	long startRow, endRow, topRow, bottomRow;
	m_GI->m_multiSelect->GetCurrentBlock(&startCol, &startRow, &endCol, &endRow);
	topRow = GetTopRow();
	bottomRow = GetBottomRow();
	if((endRow < topRow) || (startRow > bottomRow))		return;
	GetRangeRect(startCol, startRow, endCol - 1, endRow, rect);
	m_CUGGrid->m_drawHint.AddHint(startCol - 1, startRow - 1, endCol + 1, endRow + 1);
	int right = rect->right;
	int top = rect->top;
	int left = rect->left;
	int bottom = rect->bottom;

	//!< ���������
	dc->SelectObject(GetStockObject(BLACK_PEN));
	dc->MoveTo(rect->left, rect->top - 1);
	dc->LineTo(rect->right + 1, rect->top - 1);
	dc->LineTo(rect->right + 1, rect->bottom + 1);
	dc->LineTo(rect->left, rect->bottom + 1);
	dc->LineTo(rect->left, rect->top);
	dc->LineTo(rect->right, rect->top);
	dc->LineTo(rect->right, rect->bottom);
	dc->LineTo(rect->left + 1, rect->bottom);
	dc->LineTo(rect->left + 1, rect->top + 1);
	dc->LineTo(rect->right - 1, rect->top + 1);
	dc->LineTo(rect->right - 1, rect->bottom - 1);
	dc->LineTo(rect->left + 2, rect->bottom - 1);
	dc->LineTo(rect->left + 2, rect->top + 1);

	//!< �����½ǵĺڸ�
	dc->SelectObject(GetStockObject(BLACK_PEN));
	dc->MoveTo(rect->right - 2, rect->bottom - 2);
	dc->LineTo(rect->right + 2, rect->bottom - 2);
	dc->LineTo(rect->right + 2, rect->bottom + 2);
	dc->LineTo(rect->right - 3, rect->bottom + 2);

	//!< ������
	dc->SelectObject(GetStockObject(WHITE_PEN));
	dc->MoveTo(rect->right - 1, rect->bottom - 3);
	dc->LineTo(rect->right + 2, rect->bottom - 3);
	dc->MoveTo(rect->right - 3, rect->bottom - 2);
	dc->LineTo(rect->right - 3, rect->bottom + 2);

	//!< ��ǳ��ɫ���м���
	pen.CreatePen(PS_SOLID,1,RGB(120,120,120));
	dc->SelectObject(pen);
	dc->MoveTo(rect->right + 2, rect->bottom);
	dc->LineTo(rect->left + 1, rect->bottom);
	dc->LineTo(rect->left + 1, rect->top);
	dc->LineTo(rect->right, rect->top);
	dc->LineTo(rect->right, rect->bottom + 3);
}

void MVC::Camera::CCamGrid::SelectAllItem()
{
	if(GetNumberRows() < 2)		return;
	SelectRowAt(0);
	m_GI->m_multiSelect->ClearAll();
	m_GI->m_multiSelect->StartBlock(0, 0);
	m_GI->m_multiSelect->EndBlock(GetNumberCols() - 1, GetNumberRows() - 2);
	RedrawAll();
}

//!< �޸ı���
void MVC::Camera::CCamGrid::ItemEdit()
{
	long row = GetCurrentRow();
	CUGCell cell;
	GetCell(0, row, &cell);
	boost::shared_ptr<CCamera> item = CCamMgr::GetMe().GetCam(cell.GetParam());//itemName);
	if(!item)					return;

	//SelectRowAt(row);									//!< �޸ĵ�ǰѡ�еı���������������ȡ��ѡ��
	//RedrawAll();
	boost::shared_ptr<CCamera> oldItem = boost::shared_ptr<CCamera>(new CCamera());
	*oldItem = *item;
	EditItem(item);
	//SetFocus();
	//RedrawAll();
	if(*oldItem == *item)		return;					//!< ��ȱ�ʾû��

	//!< ��ӳ�������
	boost::shared_ptr<SCamUndo> undo = boost::shared_ptr<SCamUndo>(new SCamUndo);
	undo->m_Item = oldItem;
	undo->m_uiEditType = CGbl::UNDO_TYPE_UPD;
	undo->m_bEnd = true;
	CCamDoc *pDoc = (CCamDoc *)((CCamView *)GetParent())->GetDocument();
	pDoc->AddUndoMember(undo);
	SelectRowAt(row);									//!< �޸ĵ�ǰѡ�еı���������������ȡ��ѡ��
	SetFocus();
	RedrawAll();
}

//!< ͳһ�޸ı���������
// void MVC::Item::CCamGrid::ItemEditAll()
// {
// 	CUGCell cell;
// 	boost::shared_ptr<CCamera> item;
// 	std::list<boost::shared_ptr<CCamera> > ltItem;		//!< ���б�ѡ�еı���
// 	std::vector<boost::shared_ptr<CCamera> > vtItem;	//!< ���б�ѡ�еı���
// 	std::vector<UINT> vtRow;							//!< ��ѡ�е���
// 	CCamMgr* camMgr = &CCamMgr::GetMe();
// 	UINT numRow = GetNumberRows() - 1;
// 
// 	for(UINT i = 0; i < numRow; ++i)
// 	{
// 		if(IsSelected(0, i))
// 		{
// 			GetCell(0, i, &cell);
// 			item = itemMgr->GetItem(cell.GetParam());
// 			ASSERT(item);
// 			ltItem.push_back(item);
// 			vtItem.push_back(item);
// 			vtRow.push_back(i);
// 		}
// 	}
// 
// 	//!< ����һ��ԭ�еı���
// 	std::vector<boost::shared_ptr<CCamera> > vtOldItem;
// 	boost::shared_ptr<CCamera> oldItem;
// 	foreach(item, vtItem)
// 	{
// 		if(!item)			continue;
// 		oldItem = boost::shared_ptr<CCamera>(new CCamera(item->getName()));
// 		*oldItem = *item;
// 		vtOldItem.push_back(oldItem);
// 	}
// 
// 	//!< ��ʼ�༭����
// 	EditItems(ltItem);
// 
// 	boost::shared_ptr<SItemUndo> undo;
// 	CCamDoc* pDoc = (CCamDoc *)((CCamView *)GetParent())->GetDocument();
// 	UINT size = (UINT)vtItem.size();
// 	for(UINT i = 0; i < size; ++i)
// 	{
// 		if(*vtItem[i] == *vtOldItem[i])		continue;		//!< ����˱���û�иı�Ͳ���ˢ�¸���
// 
// 		//!< ��ӳ�������
// 		undo = boost::shared_ptr<SItemUndo>(new SItemUndo);
// 		undo->m_Item = vtOldItem[i];
// 		undo->m_uiEditType = CGbl::UNDO_TYPE_UPD;
// 		pDoc->AddUndoMember(undo);
// 	}
// 	pDoc->SetUndoEnd();
// 	RedrawAll();
// }

//!< ���û�ҵ�������-1
long CCamGrid::GetItemRow(UINT id)
{
	long rowCount = GetNumberRows() - 1;
	CUGCell cell;
	for(int i = 0; i < rowCount; ++i)
	{
		GetCell(0, i, &cell);
		if(cell.GetParam() == id)	return i;
	}
	return -1;
}

UINT CCamGrid::GetRowItem(long row)
{
	if(row < 0 || row >= GetNumberRows() - 1)	return UINT(-1);
	CUGCell cell;
	GetCell(0, row, &cell);
	return (UINT)cell.GetParam();
}

//!< ˢ����Щ����
void CCamGrid::FreshItemRows(std::list<UINT> ltItemID)
{
	if(ltItemID.empty())								return;
	UINT id, rowID;
	long row, selRow = 0;
	CCamMgr* camMgr = &CCamMgr::GetMe();
	EnableUpdate(FALSE);
	long rowCount = GetNumberRows();
	CUGCell cell;
	std::map<UINT, CString> mpFreshIDExist;
	foreach(id, ltItemID)	mpFreshIDExist[id] = _T("e");

	for(row = rowCount - 2; row >= 0; --row)
	{
		GetCell(0, row, &cell);
		rowID = cell.GetParam();
		if(mpFreshIDExist[rowID] == _T("e"))		//!< ����ҵ���Ҳ����˵��Ҫˢ��
		{
			ltItemID.remove(rowID);					//!< �Ȱ�����ɾ��
			if(camMgr->GetCam(rowID))				InitItemOne(rowID, row);
			else									DeleteRow(row);
			selRow = row;
		}
	}
	foreach(id, ltItemID)							//!< ʣ�µĶ���Ҫ���ӵ�
	{
		if(!camMgr->GetCam(id))					continue;
		row = GetNumberRows() - 1;
		InsertRow(row);
		InitItemOne(id, row);
		selRow = row;
	}
	EnableUpdate(TRUE);
	RedrawAll();
	if(selRow < 0)					selRow = 0;
	if(selRow >= GetNumberRows())	selRow = GetNumberRows() - 1;
	SelectRowAt(selRow);
}

//!< ��ø����ַ���
CString CCamGrid::GetFloatStr(float flt)
{
	UINT width = SoftInfo::CSoftInfo::GetMe().getItemFloatWidth();
	CString strFormat;
	strFormat.Format("%d", width);
	strFormat = _T("\%.") + strFormat + _T("f");
	CString strRet;
	strRet.Format(strFormat, flt);
	return strRet;
}
