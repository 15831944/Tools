// ItemGrid.cpp : 实现文件
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

const CString NEW_ITEM = _T("新建摄像头...");
//const CString VAL_TYPE[] = {_T("位变量"), _T("有符号字节"), _T("无符号字节"), _T("有符号单字"), _T("无符号单字"),
//_T("有符号双字"), _T("无符号双字"), _T("单精度浮点")/*, _T("双精度浮点")*/};

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
	//UGXPThemes::UseThemes(false);		//!< 去掉任何主题，简洁一点好，速度也能快点
	UGXPThemes::UseThemes(false);
	UGXPThemes::SetGridStyle(Style1);
	m_ItemFont.CreateFont(13, 0, 0, 0, 400, 0, 0, 0, 0, 0, 0, 0, 0, _T("宋体"));
	m_HeadFont.CreateFont(13, 0, 0, 0, 900, 0, 0, 0, 0, 0, 0, 0, 0, _T("宋体"));
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

	//!< 设置单元格的格式
	GetGridDefault(&cell);
	cell.SetAlignment(UG_ALIGNRIGHT | UG_ALIGNVCENTER);
	SetGridDefault(&cell);

	//!< 设置字段标题的格式
	GetHeadingDefault(&cell);
	cell.SetAlignment(UG_ALIGNCENTER | UG_ALIGNVCENTER);
	cell.SetFont(&m_HeadFont);
	SetHeadingDefault(&cell);

	//!< 设置表格的格式
	EnableExcelBorders(FALSE);						//!< 取消显示的表格线
	SetDoubleBufferMode(TRUE);						//!< 双缓冲
	SetHighlightRow(TRUE, FALSE);					//!< 设置整行选中
	SetMultiSelectMode(TRUE);						//!< 设置可多选
	PrintSetOption(UG_PRINT_TOPHEADING, TRUE);		//!< 打印需要打列头
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

	//!< 初始化列
	InitCol();

	m_bSortType = TRUE;
}

void CCamGrid::InitCol()
{
	CUGCell cell;
	int i = 0, index = 0;
	SoftInfo::CSoftInfo* csi = &SoftInfo::CSoftInfo::GetMe();
	GetCell(0, -1, &cell);
	for(i = 0; i < (int)csi->m_vtCamCol.size(); ++i){	//!< 遍历所有字段栏
		if(!csi->m_vtCamCol[i].bShow)	continue;
		cell.SetText(csi->m_vtCamCol[i].name);			//!< 设置字段的名称
		cell.SetAlignment(UG_ALIGNCENTER);				//!< 设置左侧显示
		m_mpColName[index] = csi->m_vtCamCol[i].name;	//!< 记录列的名字
		SetColWidth(index, csi->m_vtCamCol[i].width);	//!< 设置列的默认宽度
		SetCell(index++, -1, &cell);
	}
	LockColumns(1);										//!< 锁住第一个列，让它永远显示
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
		error.Format("编号为 %d 的摄像头不存在",id);
		MessageBox(error, _T("提示"), MB_OK | MB_ICONINFORMATION);
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
		if(csi->m_vtCamCol[COL_ID].name == colName)								//!< 编号
		{
			cell.SetNumber(item->GetID());
			cell.SetBitmap(GetBitmap(0));
			SetMyCell(i, row, &cell, id);
		}
		else if(csi->m_vtCamCol[COL_TAG].name == colName)						//!< 说明
		{
			cell.SetText(item->GetTag());
			SetMyCell(i, row, &cell, id);
		}
		else if(csi->m_vtCamCol[COL_RECORD].name == colName)					//!< 是否录制
		{
			cell.SetText(item->IsRecord() ? _T("是") : _T("否"));
			SetMyCell(i, row, &cell, id);
		}
// 		else if(csi->m_vtCamCol[COL_AUTOSAVE].name == colName)					//!< 是否自动保存
// 		{
// 			cell.SetText(item->IsAutoSave() ? _T("是") : _T("否"));
// 			SetMyCell(i, row, &cell, id);
// 		}
		else if(csi->m_vtCamCol[COL_PATH].name == colName)						//!< 保存路径
		{
			if(item->IsRecord())		cell.SetText(item->GetFilePath());
			else						cell.SetText(_T(""));
			SetMyCell(i, row, &cell, id);
		}
		else if(csi->m_vtCamCol[COL_ENCODE].name == colName)					//!< 编码格式
		{
			if(item->IsRecord())		cell.SetText(item->GetEncodeType());
			else						cell.SetText(_T(""));
			SetMyCell(i, row, &cell, id);
		}
		else if(csi->m_vtCamCol[COL_DISTINGUISH].name == colName)				//!< 录制分辨率
		{
			if(item->IsRecord())		cell.SetText(item->GetDistinguish());
			else						cell.SetText(_T(""));
			SetMyCell(i, row, &cell, id);
		}
		else if(csi->m_vtCamCol[COL_FPS].name == colName)						//!< 录制帧速率
		{
			if(item->IsRecord())		cell.SetText(item->GetFPS());
			else						cell.SetText(_T(""));
			SetMyCell(i, row, &cell, id);
		}
		else if(csi->m_vtCamCol[COL_BROAD].name == colName)						//!< 是否启动网络广播
		{
			cell.SetText(item->IsBroadCast() ? _T("是") : _T("否"));
			SetMyCell(i, row, &cell, id);
		}
		else if(csi->m_vtCamCol[COL_PORT].name == colName)						//!< 广播端口号,8080
		{
			if(item->IsBroadCast())		cell.SetNumber(item->GetPort());
			else						cell.SetText(_T(""));
			SetMyCell(i, row, &cell, id);
		}
		else if(csi->m_vtCamCol[COL_MAXCLIENT].name == colName)					//!< 最大连接数
		{
			if(item->IsBroadCast())		cell.SetNumber(item->GetMaxClient());
			else						cell.SetText(_T(""));
			SetMyCell(i, row, &cell, id);
		}
		else if(csi->m_vtCamCol[COL_BROADENCODE].name == colName)				//!< 广播视频编码格式,MWV2
		{
			if(item->IsBroadCast())		cell.SetText(item->GetBroadEncodeType());
			else						cell.SetText(_T(""));
			SetMyCell(i, row, &cell, id);
		}
		else if(csi->m_vtCamCol[COL_BROADDISTINGUISH].name == colName)			//!< 广播分辨率
		{
			if(item->IsBroadCast())		cell.SetText(item->GetBroadDistinguish());
			else						cell.SetText(_T(""));
			SetMyCell(i, row, &cell, id);
		}
		else if(csi->m_vtCamCol[COL_BROADFPS].name == colName)					//!< 广播帧速率
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

//!< 删除所有行
void CCamGrid::DeleteRowAll()
{
	UnJoinCells(0,GetNumberRows() - 1);
	SetNumberRows(0);
}

//!< 删除所有列
void CCamGrid::DeleteColAll()
{
	SetNumberCols(0);
}

//!< 编辑结束，统一处理
void CCamGrid::OnEditEnd()
{
	CCamMgr::GetMe().SetEditEnd();
	SetFocus();
	RedrawAll();
}

//!< 点击表格的标题，重新排序
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

//!< 双击表格
void CCamGrid::OnDClicked(int col,long row,RECT *rect,POINT *point,BOOL processed)
{
	CUGCtrl::OnDClicked(col, row, rect, point, processed);
	if(row < 0)		return;										//!< 没有内容，退出
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

//!< 编辑变量基本属性
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

//!< 统一编辑变量基本属性
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

//!< 增加新变量
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

//!< 鼠标左键点击，如果点击的是那个大点的话，需要特殊处理
void CCamGrid::OnLClicked(int col,long row,int updn,RECT *rect,POINT *point,BOOL processed)
{
	if(updn == 1)		return;		//!< 鼠标按下不管
	int sCol, eCol;
	long sRow, eRow;
	m_GI->m_multiSelect->GetCurrentBlock(&sCol, &sRow, &eCol, &eRow);
	if(eRow < 0 || eRow >= GetNumberRows() - 1)	return;		//!< 点击最后一行也无效

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

//!< 右键表格，updn是TRUE表示down，是FALSE表示up
void CCamGrid::OnRClicked(int col,long row,int updn,RECT *rect,POINT *point,BOOL processed)
{
	SetFocus();
	if(updn == FALSE){
		CMenu popMenu;
		ClientToScreen(point);
		CRect rect;
		GetCellRect(0, -1, &rect);
		point->x += GetSH_Width();
		point->y += rect.Height();	//!< 它居然没有把边框给算在里边，可恶！
		popMenu.LoadMenu(IDR_RBUTTON_MENU);
		CMenu* menu =popMenu.GetSubMenu(9);
		//!< 对于特殊地方的右键会出现不同的菜单项
		CCamMgr* camMgr = &CCamMgr::GetMe();
		if(!CProjectMgr::GetMe().IsWatch()){				//!< 未监控
		}
		else{												//!< 监控
			menu->EnableMenuItem(ID_CAM_ADD, MF_DISABLED|MF_BYCOMMAND);
			menu->EnableMenuItem(ID_CAM_EDIT, MF_DISABLED|MF_BYCOMMAND);
			menu->EnableMenuItem(ID_CAM_DELETE, MF_DISABLED|MF_BYCOMMAND);
			menu->EnableMenuItem(ID_ITEM_UP, MF_DISABLED|MF_BYCOMMAND);
			menu->EnableMenuItem(ID_ITEM_DOWN, MF_DISABLED|MF_BYCOMMAND);
			menu->EnableMenuItem(ID_CAM_EDIT_ALL, MF_DISABLED|MF_BYCOMMAND);
			menu->EnableMenuItem(ID_EDIT_CUT, MF_DISABLED|MF_BYCOMMAND);
			menu->EnableMenuItem(ID_EDIT_PASTE, MF_DISABLED|MF_BYCOMMAND);
		}
		if(row == -1 || row >= GetNumberRows() - 1){		//!< 点最后一行
			menu->EnableMenuItem(ID_EDIT_COPY, MF_DISABLED|MF_BYCOMMAND);
			menu->EnableMenuItem(ID_EDIT_CUT, MF_DISABLED|MF_BYCOMMAND);
			menu->EnableMenuItem(ID_ITEM_EDIT, MF_DISABLED|MF_BYCOMMAND);
			menu->EnableMenuItem(ID_VKDELETE, MF_DISABLED|MF_BYCOMMAND);
			menu->EnableMenuItem(ID_ITEM_UP, MF_DISABLED|MF_BYCOMMAND);
			menu->EnableMenuItem(ID_ITEM_DOWN, MF_DISABLED|MF_BYCOMMAND);
			menu->EnableMenuItem(ID_CAM_EDIT_ALL, MF_DISABLED|MF_BYCOMMAND);
		}
		if(camMgr->m_ltItemClipBoard.empty()){				//!< 剪贴板里没东西
			menu->EnableMenuItem(ID_EDIT_PASTE, MF_DISABLED|MF_BYCOMMAND);
		}
		if(GetNumberRows() > 1 && row == 0 ){				//!< 点第一行变量
			menu->EnableMenuItem(ID_ITEM_UP, MF_DISABLED|MF_BYCOMMAND);
		}
		if(GetNumberRows() > 1 && row == GetNumberRows() - 2){	//!< 点最后一行变量
			menu->EnableMenuItem(ID_ITEM_DOWN, MF_DISABLED|MF_BYCOMMAND);
		}
		//!< 记录被选中的变量，如果多选了，不能克隆
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
	else if(*vcKey == 0x41){			//!< 输入的A
		if(GetAsyncKeyState(VK_CONTROL)){
			CCamView* pView = (CCamView *)GetParent();
			if(pView)	pView->OnSelectAll();
		}
	}
	else if(*vcKey == VK_F1){
		SoftInfo::CMyHelp::GetMe().ShowHelp(_T("在DView中配置摄像头"));
	}
}

//!< 鼠标移动事件
void MVC::Camera::CCamGrid::OnMouseMove(int col,long row,POINT *point,UINT nFlags,BOOL processed)
{
	if(nFlags == 0)		//!< 0普通移动，1按下鼠标左键移动，2右键移动，16滚轮移动
	{
		int sCol, eCol;
		long sRow, eRow;
		m_GI->m_multiSelect->GetCurrentBlock(&sCol, &sRow, &eCol, &eRow);
		if(eRow < 0 || eRow >= GetNumberRows() - 1)	return;		//!< 点击最后一行也无效
		CRect rect;
		GetRangeRect(sCol, sRow, eCol - 1, eRow, rect);
		int iRight = rect.right;
		int iBottom = rect.bottom;
		rect = CRect(iRight - 10, iBottom - 10, iRight + 4, iBottom + 1);
		if(rect.PtInRect(*point))		::SetCursor(::LoadCursor(NULL, IDC_CROSS));
		else							::SetCursor(::LoadCursor(NULL, IDC_ARROW));
	}
}

//!< 接收右键菜单的消息，iItem是菜单项的ID，lRow和iCol是右键的位置（行列值）
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
		case MENU_FIND:			FindDialog();			break;		//!< 查找
		case ID_CAM_ADD:		pView->OnItemAdd();		break;		//!< 添加变量
		case ID_CAM_EDIT:		pView->OnItemEdit();	break;		//!< 编辑变量
		case ID_CAM_DELETE:		pView->OnVkdelete();	break;		//!< 变量移除
		case ID_EDIT_COPY:		pView->OnEditCopy();	break;		//!< 变量拷贝
		case ID_EDIT_CUT:		pView->OnEditCut();		break;		//!< 变量剪切
		case ID_EDIT_PASTE:		pView->OnEditPaste();	break;		//!< 变量粘贴
		case ID_ITEM_UP:		pView->OnItemUp();		break;		//!< 变量上移
		case ID_ITEM_DOWN:		pView->OnItemDown();	break;		//!< 变量下移
		case ID_SELECT_ALL:		pView->OnSelectAll();	break;		//!< 全选
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

//!< 对该两个ID的变量进行排序
bool CCamGrid::SortItem(UINT& lth, UINT& rth)
{
	bool bSort;
	CCamMgr* camMgr = &CCamMgr::GetMe();
	CCamera *lItem, *rItem;
	lItem = camMgr->GetCam(lth).get();
	rItem = camMgr->GetCam(rth).get();
	CString txtL, txtR;						//!< 对于有些会不显示的字符串,需要用这个来处理
	int nL, nR;								//!< 对于有些会不显示的数字,需要用这个来处理
	SoftInfo::CSoftInfo* csi = &SoftInfo::CSoftInfo::GetMe();

	//!< 字符串比较
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

	//!< 数值比较
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

//!< 排序，比较两个单元格内的数据的大小
int CCamGrid::OnSortEvaluate(CUGCell* pCell1, CUGCell* pCell2, int iFlags)
{
	//!< 无论怎么排，新建变量一栏都必须放在表格末尾
	CString text1=(CString)pCell1->GetText();
	CString text2=(CString)pCell2->GetText();
	if(text1 == _T("新建摄像头...") || text1=="")				return 1;
	else if(text2 == _T("新建摄像头...") || text2=="")		return -1;

	//!< 如果是从大到小排列，就把这两个单元格互换了就好了
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
//		SortBy(m_iSortCol, UG_SORT_ASCENDING);				//!< 默认排序的列
		QuickSetCellType(m_iSortCol, -1, m_iArrowIndex);	//!< 默认显示排序的‘三角号’的位置
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

	//!< 获得原始的变量数据，后边要压撤销栈
	boost::shared_ptr<SCamUndo> undo1 = boost::shared_ptr<SCamUndo>(new SCamUndo(CGbl::UNDO_TYPE_UPD, item1, 0));
	boost::shared_ptr<SCamUndo> undo2 = boost::shared_ptr<SCamUndo>(new SCamUndo(CGbl::UNDO_TYPE_UPD, item2, 0));

	//!< 交换这两个变量
	CCamMgr::GetMe().ExChangeItem(item1->GetID(), item2->GetID());
	InitItemOne(item2->GetID(), row);
	InitItemOne(item1->GetID(), row - 1);

	//!< 添加撤销对象
	CCamDoc* pDoc = (CCamDoc *)((CCamView *)GetParent())->GetDocument();
	pDoc->AddUndoMember(undo1);
	pDoc->AddUndoMember(undo2);
	pDoc->SetUndoEnd();

	SelectRowAt(row - 1);										//!< 选中新建变量这一项
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

	//!< 获得原始的变量数据，后边要压撤销栈
	boost::shared_ptr<SCamUndo> undo1 = boost::shared_ptr<SCamUndo>(new SCamUndo(CGbl::UNDO_TYPE_UPD, item1, 0));
	boost::shared_ptr<SCamUndo> undo2 = boost::shared_ptr<SCamUndo>(new SCamUndo(CGbl::UNDO_TYPE_UPD, item2, 0));

	//!< 交换这两个变量
	CCamMgr::GetMe().ExChangeItem(item1->GetID(), item2->GetID());
	InitItemOne(item2->GetID(), row);
	InitItemOne(item1->GetID(), row + 1);

	//!< 添加撤销对象
	CCamDoc* pDoc = (CCamDoc *)((CCamView *)GetParent())->GetDocument();
	pDoc->AddUndoMember(undo1);
	pDoc->AddUndoMember(undo2);
	pDoc->SetUndoEnd();

	SelectRowAt(row + 1);										//!< 选中新建变量这一项
	RedrawAll();
}

//!< 统计一下要删除的变量，参数表示是否要询问一下用户，剪切时候要删除，就不需要通知用户
void MVC::Camera::CCamGrid::ItemRemove(bool bAsk /* = true */)
{
	std::list<long> ltSelectRow;
	//!< 记录被选中的行号，从后往前数，因为删除时要从后往前删
	for(long row = GetNumberRows() - 2; row >= 0; --row)
		if(IsSelected(0, row, 0))
			ltSelectRow.push_back(row);

	if(ltSelectRow.empty())				return;
	if(bAsk)
	{
		CString text;
		text.Format("确定要移除此 %d 项数据吗？",(int)ltSelectRow.size());
		if(IDYES != MessageBox(text, _T("移除提示"), MB_YESNO)){
			SetFocus();
			return;
		}
	}

	CCamMgr* camMgr = &CCamMgr::GetMe();
	CUGCell cell;
	boost::shared_ptr<CCamera> item;
	std::list<boost::shared_ptr<CCamera> > ltSelectItem;

	//!< 统计被选中的变量
	foreach(long row, ltSelectRow)
	{
		GetCell(0, row, &cell);
		item = camMgr->GetCam(cell.GetParam());
		ASSERT(item);
		ltSelectItem.push_back(item);
	}

	ItemRemove(ltSelectItem);				//!< 开始删除
	m_GI->m_multiSelect->ClearAll();		//!< 清空所有选择的行
	SetFocus();

	//!< 添加撤销对象
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

//!< 删除这些变量
void MVC::Camera::CCamGrid::ItemRemove(std::list<boost::shared_ptr<CCamera> > ltItem)
{
	CCamMgr* camMgr = &CCamMgr::GetMe();
	boost::shared_ptr<CCamera> item;
	foreach(item, ltItem)
	{
		camMgr->DeleteItem(item->GetID());
	}
}

//!< 变量拷贝
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

//!< 变量剪切
void MVC::Camera::CCamGrid::ItemCut()
{
	ItemCopy();			//!< 先拷贝，再删除
	ItemRemove(false);
}

//!< 变量粘贴
void MVC::Camera::CCamGrid::ItemPaste()
{
	CCamMgr* camMgr = &CCamMgr::GetMe();
	if(camMgr->m_ltItemClipBoard.empty())						return;
	boost::shared_ptr<CCamera> item, srCCamera;
	CCamDoc* pDoc = (CCamDoc *)((CCamView *)GetParent())->GetDocument();

	//!< 不能直接把剪贴板里的变量粘到变量表中，而是要拷贝一份过去
	foreach(srCCamera, camMgr->m_ltItemClipBoard)
	{
		item = boost::shared_ptr<CCamera>(new CCamera());
		*item = *srCCamera;
		if(item->IsBroadCast())
			item->SetPort(camMgr->GetDifferentPort(8080));
		if(!camMgr->AddItem(item))
			return;			// 超过上限就不添了

		boost::shared_ptr<SCamUndo> undo = boost::shared_ptr<SCamUndo>(new SCamUndo);
		undo->m_bEnd = false;
		undo->m_uiEditType = CGbl::UNDO_TYPE_ADD;
		undo->m_Item = item;
		pDoc->AddUndoMember(undo);

		SetFocus();
	}
	pDoc->SetUndoEnd();
	//!< 现在开始粘贴
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
	GotoRow(row);		//!< 显示2次没问题，显示1次偶尔会出现不显示的问题，界面库计算有问题
}

void MVC::Camera::CCamGrid::OnDrawFocusRect(CDC *dc,RECT *rect)
{
	if(CProjectMgr::GetMe().IsWatch())	return;		//!< 监控时不画黑框
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

	//!< 画方框黑线
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

	//!< 画右下角的黑格
	dc->SelectObject(GetStockObject(BLACK_PEN));
	dc->MoveTo(rect->right - 2, rect->bottom - 2);
	dc->LineTo(rect->right + 2, rect->bottom - 2);
	dc->LineTo(rect->right + 2, rect->bottom + 2);
	dc->LineTo(rect->right - 3, rect->bottom + 2);

	//!< 画白线
	dc->SelectObject(GetStockObject(WHITE_PEN));
	dc->MoveTo(rect->right - 1, rect->bottom - 3);
	dc->LineTo(rect->right + 2, rect->bottom - 3);
	dc->MoveTo(rect->right - 3, rect->bottom - 2);
	dc->LineTo(rect->right - 3, rect->bottom + 2);

	//!< 画浅灰色的中间线
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

//!< 修改变量
void MVC::Camera::CCamGrid::ItemEdit()
{
	long row = GetCurrentRow();
	CUGCell cell;
	GetCell(0, row, &cell);
	boost::shared_ptr<CCamera> item = CCamMgr::GetMe().GetCam(cell.GetParam());//itemName);
	if(!item)					return;

	//SelectRowAt(row);									//!< 修改当前选中的变量，其他变量都取消选中
	//RedrawAll();
	boost::shared_ptr<CCamera> oldItem = boost::shared_ptr<CCamera>(new CCamera());
	*oldItem = *item;
	EditItem(item);
	//SetFocus();
	//RedrawAll();
	if(*oldItem == *item)		return;					//!< 相等表示没改

	//!< 添加撤销对象
	boost::shared_ptr<SCamUndo> undo = boost::shared_ptr<SCamUndo>(new SCamUndo);
	undo->m_Item = oldItem;
	undo->m_uiEditType = CGbl::UNDO_TYPE_UPD;
	undo->m_bEnd = true;
	CCamDoc *pDoc = (CCamDoc *)((CCamView *)GetParent())->GetDocument();
	pDoc->AddUndoMember(undo);
	SelectRowAt(row);									//!< 修改当前选中的变量，其他变量都取消选中
	SetFocus();
	RedrawAll();
}

//!< 统一修改变量的属性
// void MVC::Item::CCamGrid::ItemEditAll()
// {
// 	CUGCell cell;
// 	boost::shared_ptr<CCamera> item;
// 	std::list<boost::shared_ptr<CCamera> > ltItem;		//!< 所有被选中的变量
// 	std::vector<boost::shared_ptr<CCamera> > vtItem;	//!< 所有被选中的变量
// 	std::vector<UINT> vtRow;							//!< 被选中的行
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
// 	//!< 备份一下原有的变量
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
// 	//!< 开始编辑所有
// 	EditItems(ltItem);
// 
// 	boost::shared_ptr<SItemUndo> undo;
// 	CCamDoc* pDoc = (CCamDoc *)((CCamView *)GetParent())->GetDocument();
// 	UINT size = (UINT)vtItem.size();
// 	for(UINT i = 0; i < size; ++i)
// 	{
// 		if(*vtItem[i] == *vtOldItem[i])		continue;		//!< 如果此变量没有改变就不用刷新该行
// 
// 		//!< 添加撤销对象
// 		undo = boost::shared_ptr<SItemUndo>(new SItemUndo);
// 		undo->m_Item = vtOldItem[i];
// 		undo->m_uiEditType = CGbl::UNDO_TYPE_UPD;
// 		pDoc->AddUndoMember(undo);
// 	}
// 	pDoc->SetUndoEnd();
// 	RedrawAll();
// }

//!< 如果没找到，返回-1
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

//!< 刷新这些变量
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
		if(mpFreshIDExist[rowID] == _T("e"))		//!< 如果找到，也就是说需要刷新
		{
			ltItemID.remove(rowID);					//!< 先把这项删了
			if(camMgr->GetCam(rowID))				InitItemOne(rowID, row);
			else									DeleteRow(row);
			selRow = row;
		}
	}
	foreach(id, ltItemID)							//!< 剩下的都是要增加的
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

//!< 获得浮点字符串
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
