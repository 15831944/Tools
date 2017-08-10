// ItemGrid.cpp : 实现文件
//

#include "stdafx.h"
#include "DXPEditor.h"
#include "MyHelp.h"
#include "ProjectMgr.h"
#include "Gbl.h"
#include "SoftInfo.h"
//#include "Power.h"
#include "MainFrm.h"
#include "ItemGrid.h"
#include "ItemView.h"
#include "ItemDoc.h"
#include "ItemGroup.h"
#include "ItemMgr.h"
#include "Item.h"
#include "ItemProperty.h"
#include "SourceProperty.h"
#include "AlarmProperty.h"

#include "CloneItemDlg.h"
#include "PropertyDlg.h"
#include "WriteItemDlg.h"
#include "ItemInConfigDlg.h"

#include "DevMgr.h"
#include "DeviceOne.h"

#include <afxdisp.h>
#include <time.h>
#include <locale.h>

const CString NEW_ITEM = _T("新建变量...");
const CString VAL_TYPE[] = {_T("位变量"), _T("有符号字节"), _T("无符号字节"), _T("有符号单字"), _T("无符号单字"),
_T("有符号双字"), _T("无符号双字"), _T("单精度浮点")/*, _T("双精度浮点")*/};

CString m_strSortCol = _T("");
std::list<UINT> ltShowItemID;

// CItemGrid
using namespace MVC;
using namespace Item;

BEGIN_MESSAGE_MAP(CItemGrid,CUGCtrl)
	//{{AFX_MSG_MAP(CFindGrid)
	// NOTE - the ClassWizard will add and remove mapping macros here.
	//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG_MAP
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()

CItemGrid::CItemGrid()
:m_uiCulGroupID(UINT(-1))
,m_uiShowType(3)
,m_nProjCol(-1)
,m_nIOCol(-1)
,m_bWatch(false)
,m_bHoldToClone(false)					//!< 通过鼠标按下进行多重复制的标志
,m_nHoldToCloneID(-1)					//!< 等待克隆的变量ID
,m_nHoldToCloneRow(-1)					//!< 等待克隆的变量在第几行
,m_nHoleToCloneCurCount(0)				//!< 等待克隆的变量的当前数量
{
	//UGXPThemes::UseThemes(false);		//!< 去掉任何主题，简洁一点好，速度也能快点
	UGXPThemes::UseThemes(false);
	UGXPThemes::SetGridStyle(Style1);
	m_mpValType[_T("位变量")] = 0;
	m_mpValType[_T("有符号字节")] = 1;
	m_mpValType[_T("无符号字节")] = 2;
	m_mpValType[_T("有符号单字")] = 3;
	m_mpValType[_T("无符号单字")] = 4;
	m_mpValType[_T("有符号双字")] = 5;
	m_mpValType[_T("无符号双字")] = 6;
	m_mpValType[_T("单精度浮点")] = 7;
	m_ItemFont.CreateFont(13, 0, 0, 0, 400, 0, 0, 0, 0, 0, 0, 0, 0, _T("宋体"));
	m_HeadFont.CreateFont(13, 0, 0, 0, 900, 0, 0, 0, 0, 0, 0, 0, 0, _T("宋体"));
}

CItemGrid::~CItemGrid()
{
	UGXPThemes::CleanUp();
}

BOOL MVC::Item::CItemGrid::OnEraseBkgnd(CDC* pDC)
{
	return TRUE;
}

void CItemGrid::OnSetup()
{
	ClearAllBitmaps();
	AddBitmap(IDB_BMP_ITEMIO);
	AddBitmap(IDB_BMP_ITEMMEMORY);
	AddBitmap(IDB_BMP_ADD);
	InitAll();
}

void CItemGrid::InitAll()
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
	for(int i = 0; i < (int)csi->m_vtColInfo.size(); ++i){
		if(csi->m_vtColInfo[i].bShow)
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

void CItemGrid::InitCol()
{
	CUGCell cell;
	int i = 0, index = 0;
	SoftInfo::CSoftInfo* csi = &SoftInfo::CSoftInfo::GetMe();
	GetCell(0, -1, &cell);
	for(i = 0; i < (int)csi->m_vtColInfo.size(); ++i){	//!< 遍历所有字段栏
		if (!csi->m_vtColInfo[i].bShow)	continue;
		if (i == COL_MODBUS485 && 
			!csi->IsModbus485())		continue;
		if (i == COL_VALUE)		m_nProjCol = index;		//!< 记住工程值所在的列
		if (i == COL_IOVALUE)	m_nIOCol = index;		//!< 记住IO值所在的列
		cell.SetText(csi->m_vtColInfo[i].name);			//!< 设置字段的名称
		cell.SetAlignment(UG_ALIGNCENTER);				//!< 设置左侧显示
		m_mpColName[index] = csi->m_vtColInfo[i].name;	//!< 记录列的名字
		SetColWidth(index, csi->m_vtColInfo[i].width);	//!< 设置列的默认宽度
		SetCell(index++, -1, &cell);
	}
	LockColumns(1);										//!< 锁住第一个列，让它永远显示
}

bool CItemGrid::InitItemOne(UINT id, UINT row)
{
	std::shared_ptr<CItem> item = CItemMgr::GetMe().GetItem(id);
	if(!item)	return false;
	std::shared_ptr<CPropertySource> src = item->getSrcInfo();
	std::shared_ptr<CPropertyAlarm> alarm = item->getAlarmInfo();
	MVC::Device::CDevMgr* devMgr = &MVC::Device::CDevMgr::GetMe();
	std::shared_ptr<MVC::Device::CDeviceOne> device;
	bool bIsIO = false;
	if(item->getSrcType() == CItem::SRC_TYPE_IO){	device = devMgr->GetDevice(src->getDeviceID()); bIsIO = true;}
	CComVariant cvr;
	CUGCell cell;
	SoftInfo::CSoftInfo* csi = &SoftInfo::CSoftInfo::GetMe();

	if(!item){
		CString error;
		error.Format("%d",id);
		AfxMessageBox("编号为 "+error+" 的变量不存在");
		return false;
	}
	CString colName;
	CString text;
	COLORREF color = GetSysColor(COLOR_WINDOW);
	int colCount = GetNumberCols();
	for(int i = 0; i < colCount; ++i)
	{
		colName = m_mpColName[i];
		bool bAlarm = item->IsAlarm();
		GetCell(i, row, &cell);
		if(csi->m_vtColInfo[COL_NAME].name == colName)							//!< 名称
		{
			cell.SetText(item->getName());
			cell.SetAlignment(UG_ALIGNLEFT | UG_ALIGNVCENTER);
			if(bIsIO)		cell.SetBitmap(GetBitmap(0));
			else			cell.SetBitmap(GetBitmap(1));
			SetMyCell(i, row, &cell, id, bAlarm);
		}
		else if(csi->m_vtColInfo[COL_TAG].name == colName)						//!< 标签
		{
			cell.SetText(item->getTag());
			SetMyCell(i, row, &cell, id, bAlarm);
		}
		else if(csi->m_vtColInfo[COL_ID].name == colName)						//!< 编号
		{
			cell.SetNumber(item->getID());
			SetMyCell(i, row, &cell, id, bAlarm);
		}
		else if(csi->m_vtColInfo[COL_TYPE].name == colName)						//!< 数值类型
		{
			cell.SetText(VAL_TYPE[item->getValType() % 8]);
			SetMyCell(i, row, &cell, id, bAlarm);
		}
		else if(csi->m_vtColInfo[COL_IOTYPE].name == colName)					//!< IO值类型
		{
			if(bIsIO)							cell.SetText(VAL_TYPE[src->getIOType() % 8]);
			else								cell.SetText(_T(""));
			SetMyCell(i, row, &cell, id, bAlarm);
		}
		else if(csi->m_vtColInfo[COL_SRC].name == colName)						//!< 数据源类型
		{
			if(bIsIO)							cell.SetText(_T("I/O变量"));
			else								cell.SetText(_T("内存变量"));
			SetMyCell(i, row, &cell, id, bAlarm);
		}
		else if(csi->m_vtColInfo[COL_ACCESS].name == colName)					//!< 访问权限
		{
			if(item->getAccessRight() == 0)		text = _T("可读可写");
			else if(item->getAccessRight() == 1)text = _T("只读");
			else								text = _T("只写");
			cell.SetText(text);
			SetMyCell(i, row, &cell, id, bAlarm);
		}
		else if(csi->m_vtColInfo[COL_DESCRIPT].name == colName)					//!< 备注信息
		{
			cell.SetText(item->getDescription());
			SetMyCell(i, row, &cell, id, bAlarm);
		}
		else if(csi->m_vtColInfo[COL_VALUE].name == colName)					//!< 工程值
		{
			//CComVariant cvrProj = item->getValue();
			//if(cvrProj.vt == VT_BOOL)									{text = cvrProj.boolVal?_T("1"):_T("0");}
			//else if(cvrProj.vt == VT_R4)								{text = GetFloatStr(cvrProj.fltVal);}
			//else
			//{
			//	int temp=cvrProj.vt;
			//	CGbl::ChangeVariantType(cvrProj, VT_I8);
			//	LONGLONG llValue = cvrProj.llVal;
			//	if (csi->isShowHex())
			//	{
			//		text.Format("0x%.8X", llValue);
			//	}
			//	else
			//	{
			//		if(temp==VT_I1||temp==VT_I2||temp==VT_I4)
			//			text.Format("%d", llValue);
			//		else if(temp==VT_UI1||temp==VT_UI2||temp==VT_UI4)
			//			text.Format("%u",llValue);
			//		else
			//			text.Format("%d",llValue);
			//	}
			//}
			text=item->getStrValue(csi->isShowHex());
			if(text.GetAt(0) == '.')									text = _T("0") + text;
			else if(text.GetAt(0) == '-' && text.GetAt(1) == '.')		text = _T("-0") + text.Right(text.GetLength() - 1);
			cell.SetText(text);
			SetMyCell(i, row, &cell, id, bAlarm);
		}
		else if(csi->m_vtColInfo[COL_GROUP].name == colName)					//!< 变量组
		{
			std::shared_ptr<CItemGroup> myGroup = CItemMgr::GetMe().GetGroup(item->getMyGroupID());
			ASSERT(myGroup);
			cell.SetText(myGroup->getName());
			SetMyCell(i, row, &cell, id, bAlarm);
		}
		else if(csi->m_vtColInfo[COL_CONVERT].name == colName)					//!< 转换
		{
			text = _T("");
			if(bIsIO && src->getConvertType() != 0)								//!< IO变量，并且启动转换
			{
				CComVariant minProj = src->getProjMin();
				CComVariant maxProj = src->getProjMax();
				CComVariant minIO = src->getIOMin();
				CComVariant maxIO = src->getIOMax();
				minProj.ChangeType(VT_BSTR);
				maxProj.ChangeType(VT_BSTR);
				minIO.ChangeType(VT_BSTR);
				maxIO.ChangeType(VT_BSTR);

				if(src->getConvertType() == 1)		text = _T("线性(");
				else if(src->getConvertType() == 2)	text = _T("平方根(");
				else								text = _T("其它(");
				text = text + CString(minProj.bstrVal) + _T(",") + (CString)maxProj.bstrVal +
					_T(",") + CString(minIO.bstrVal) + _T(",") + (CString)maxIO.bstrVal + _T(")");
			}
			cell.SetText(text);
			SetMyCell(i, row, &cell, id, bAlarm);
		}
		else if(csi->m_vtColInfo[COL_IOVALUE].name == colName)					//!< IO值
		{
			if(bIsIO){
//				CComVariant ioVar = item->getIOValue();
////				cvr = item->getIOValue();
//				if(ioVar.vt == VT_BOOL)	{text = ioVar.boolVal?_T("1"):_T("0");}
//				else if(ioVar.vt == VT_R4){text = GetFloatStr(ioVar.fltVal);}
//				else
//				{
//					int temp=ioVar.vt;
//					CGbl::ChangeVariantType(ioVar, VT_I8);
//					LONGLONG llValue = ioVar.llVal;
//					if (csi->isShowHex())
//					{
//						text.Format("0x%.8X", llValue);
//					}
//					else
//					{
//						if(temp==VT_I1||temp==VT_I2||temp==VT_I4)
//							text.Format("%d", llValue);
//						else if(temp==VT_UI1||temp==VT_UI2||temp==VT_UI4)
//							text.Format("%u", llValue);
//						else
//							text.Format("%d", llValue);
//					}
//				}
				text=item->getStrIOValue(csi->isShowHex());
				if(text.GetAt(0) == '.')								text = _T("0") + text;
				else if(text.GetAt(0) == '-' && text.GetAt(1) == '.')	text = _T("-0") + text.Right(text.GetLength() - 1);
			}
			else														text = _T("");
			cell.SetText(text);
			SetMyCell(i, row, &cell, id, bAlarm);
		}
		else if(csi->m_vtColInfo[COL_DESCRIPT].name == colName)					//!< 备注
		{
			cell.SetText(item->getDescription());
			SetMyCell(i, row, &cell, id, bAlarm);
		}
		else if(csi->m_vtColInfo[COL_SCRIPT].name == colName)					//!< 脚本
		{
			if(bIsIO)						cell.SetText(_T(""));
			else							cell.SetText(src->getScriptText());
			SetMyCell(i, row, &cell, id, bAlarm);
		}
		else if(csi->m_vtColInfo[COL_DEVICE].name == colName)					//!< 设备
		{
			if(device)						cell.SetText(device->getName());
			else							cell.SetText(_T(""));
			SetMyCell(i, row, &cell, id, bAlarm);
		}
		else if(csi->m_vtColInfo[COL_ADDR].name == colName)						//!< 地址
		{
			if(device)						cell.SetText(src->GetAreaString());
			else							cell.SetText(_T(""));
			SetMyCell(i, row, &cell, id, bAlarm);
		}
		else if(csi->m_vtColInfo[COL_FRESHTIME].name == colName)				//!< 刷新时间
		{
			cell.SetNumber(src->getFreshTime());
			SetMyCell(i, row, &cell, id, bAlarm);
		}
		else if(csi->m_vtColInfo[COL_BIT].name == colName)						//!< 位报警
		{
			if(item->getValType() != 0)		text = _T("");
			else							text = alarm->GetBitAlarmInfo(false);
			cell.SetText(text);
			SetMyCell(i, row, &cell, id, bAlarm);
		}
		else if(csi->m_vtColInfo[COL_LOLO].name == colName)						//!< 下下限
		{
			if(alarm->getLoloActive())		cell.SetNumber(alarm->getLoloValue());
			else							cell.SetText(_T(""));
			SetMyCell(i, row, &cell, id, bAlarm);
		}
		else if(csi->m_vtColInfo[COL_LOW].name == colName)						//!< 下限
		{
			if(alarm->getLowActive())		cell.SetNumber(alarm->getLowValue());
			else							cell.SetText(_T(""));
			SetMyCell(i, row, &cell, id, bAlarm);
		}
		else if(csi->m_vtColInfo[COL_HIGH].name == colName)						//!< 上限
		{
			if(alarm->getHighActive())		cell.SetNumber(alarm->getHighValue());
			else							cell.SetText(_T(""));
			SetMyCell(i, row, &cell, id, bAlarm);
		}
		else if(csi->m_vtColInfo[COL_HIHI].name == colName)						//!< 上上限
		{
			if(alarm->getHihiActive())		cell.SetNumber(alarm->getHihiValue());
			else							cell.SetText(_T(""));
			SetMyCell(i, row, &cell, id, bAlarm);
		}
		else if(csi->m_vtColInfo[COL_AIM].name == colName)						//!< 目标
		{
			if(alarm->getAimActive())		cell.SetNumber(alarm->getAimValue());
			else							cell.SetText(_T(""));
			SetMyCell(i, row, &cell, id, bAlarm);
		}
		else if(csi->m_vtColInfo[COL_SHIFT].name == colName)					//!< 变化率
		{
			if(alarm->getShiftActive())		cell.SetNumber(alarm->getShiftDelta());
			else							cell.SetText(_T(""));
			SetMyCell(i, row, &cell, id, bAlarm);
		}
		else if(csi->m_vtColInfo[COL_MODBUS485].name == colName)				//!< Modbus485
		{
			if(item->getModbus485())		cell.SetText(_T("1"));
			else							cell.SetText(_T(""));
			SetMyCell(i, row, &cell, id, bAlarm);
		}
		else if (csi->m_vtColInfo[COL_REV_DB].name == colName)					//!< 保留历史数据
		{
			if (item->getReservDB())		cell.SetText(_T("1"));
			else							cell.SetText(_T(""));
			SetMyCell(i, row, &cell, id, bAlarm);
		}
	}
	return true;
}

void CItemGrid::SetMyCell(int nCol, long lRow, CUGCell* cell, UINT id, bool bAlarm /* = fasle */)
{
	cell->SetParam(id);
	if(bAlarm){
		if(CProjectMgr::GetMe().IsWatch()){			// 害怕编译器将这句先于bAlarm执行,所以将这两个本应该合在一起的语句分开,确保效率
			COLORREF color1 = RGB(255,50,0);
			COLORREF color2 = RGB(255,0,255);
			cell->SetBackColor(color1);
			cell->SetHBackColor(color2);
		}
	}
	else{
		cell->SetBackColor(::GetSysColor(COLOR_WINDOW));
		cell->SetHBackColor(::GetSysColor(COLOR_HIGHLIGHT));
	}
	SetCell(nCol, lRow, cell);
}

//!< 刷新变量值
void CItemGrid::InitItemValue(UINT id, UINT row, CUGCell* cell)
{
	std::shared_ptr<CItem> item = CItemMgr::GetMe().GetItem(id);
	if(!item || !cell)		return;
//	CUGCell cell;
	CString text;
	COLORREF bkColor, hbkColor;
	if(item->IsAlarm())		{bkColor = RGB(255,50,0);				hbkColor = RGB(255,0,255);}
	else					{bkColor = ::GetSysColor(COLOR_WINDOW);	hbkColor = ::GetSysColor(COLOR_HIGHLIGHT);}
	SoftInfo::CSoftInfo *csi = &SoftInfo::CSoftInfo::GetMe();

	//!< 工程值
	if(m_nProjCol >= 0)
	{
//		GetCell(m_nProjCol, row, &cell);
	/*	CComVariant cvr = item->getValue();
		if(cvr.vt == VT_BOOL)		text = cvr.boolVal?_T("1"):_T("0");
		else if(cvr.vt == VT_R4)	text = GetFloatStr(cvr.fltVal);
		else
		{
			int temp=cvr.vt;
			CGbl::ChangeVariantType(cvr, VT_I8);
			LONGLONG llValue = cvr.llVal;
			if (csi->isShowHex())
			{
				text.Format("0x%.8X", llValue);
			}
			else
			{
				if(temp==VT_I1||temp==VT_I2||temp==VT_I4)
					text.Format("%d", llValue);
				else if(temp==VT_UI1||temp==VT_UI2||temp==VT_UI4)
					text.Format("%u", llValue);
				else
					text.Format("%d", llValue);
			}
		}*/
		text=item->getStrValue(csi->isShowHex());
		if(text.GetAt(0) == '.')		text = _T("0") + text;
		else if(text.GetAt(0) == '-' && text.GetAt(1) == '.'){
			text = _T("-0") + text.Right(text.GetLength() - 1);
		}
		cell->SetText(text);
		cell->SetBackColor(bkColor);
		cell->SetHBackColor(hbkColor);
//		cell.SetAlignment(UG_ALIGNRIGHT | UG_ALIGNVCENTER);
		SetCell(m_nProjCol, row, cell);
	}

	//!< IO值
	if(m_nIOCol >= 0)
	{
	/*	CComVariant cvr = item->getIOValue();
		if(cvr.vt == VT_BOOL)		text = cvr.boolVal?_T("1"):_T("0");
		else if(cvr.vt == VT_R4)	text = GetFloatStr(cvr.fltVal);
		else
		{
			int temp=cvr.vt;
			CGbl::ChangeVariantType(cvr, VT_I8);
			LONGLONG llValue = cvr.llVal;
			if (csi->isShowHex())
			{
				text.Format("0x%.8X", llValue);
			}
			else
			{
				if(temp==VT_I1||temp==VT_I2||temp==VT_I4)
					text.Format("%d", llValue);
				else if(temp==VT_UI1||temp==VT_UI2||temp==VT_UI4)
					text.Format("%u", llValue);
				else
					text.Format("%d", llValue);
			}
		}*/
	     text=item->getStrIOValue(csi->isShowHex());
		if(text.GetAt(0) == '.')		text = _T("0") + text;
		else if(text.GetAt(0) == '-' && text.GetAt(1) == '.'){
			text = _T("-0") + text.Right(text.GetLength() - 1);
		}
		if(item->getSrcType() == CItem::SRC_TYPE_MEM)		text = _T("");
		cell->SetText(text);
		cell->SetBackColor(bkColor);
		cell->SetHBackColor(hbkColor);
//		cell.SetAlignment(UG_ALIGNRIGHT | UG_ALIGNVCENTER);
		SetCell(m_nIOCol, row, cell);
	}
}

void CItemGrid::InitAddRow()
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
void CItemGrid::DeleteRowAll()
{
	UnJoinCells(0,GetNumberRows() - 1);
	SetNumberRows(0);
}

//!< 删除所有列
void CItemGrid::DeleteColAll()
{
	SetNumberCols(0);
}

//!< 刷新一遍变量数值
void CItemGrid::FreshData()
{
	std::vector<std::shared_ptr<CItem> > vtRowItem;		//!< 下表是行号，内容是每行所对应的变量
	int rowCount = GetNumberRows() - 1;						//!< 仅表示变量的行数
	if(rowCount <= 0)					return;				//!< 
	if(m_nProjCol < 0 && m_nIOCol < 0)	return;				//!< 

	CUGCell cell, cellValue;
	CItemMgr* itemMgr = &CItemMgr::GetMe();
	std::shared_ptr<CItem> item;
	if(m_nProjCol > 0)					GetCell(m_nProjCol, 0, &cellValue);
	else								GetCell(m_nIOCol, 0, &cellValue);
	for(int i = 0; i < rowCount; ++i)
	{
		GetCell(0, i, &cell);
		item = itemMgr->GetItem(cell.GetParam());
		if(!item)			return;
		InitItemValue(cell.GetParam(), i, &cellValue);
	}
	RedrawCol(m_nProjCol);
	RedrawCol(m_nIOCol);

	//!< 获得当前页内所有显示的变量
// 	CUGCell cell;
// 	CItemMgr* itemMgr = &CItemMgr::GetMe();
// 	std::shared_ptr<CItem> item;
// 	for(int i = 0; i < (int)GetNumberRows() - 1; ++i)
// 	{
// 		GetCell(0, i, &cell);
// 		item = itemMgr->GetItem(cell.GetParam());
// 		if(!item)	continue;
// 		ASSERT(item);
// 		vtRowItem.push_back(item);
// 	}
// 
// 	//!< 判断哪些值被改变了
// 	bool bNeedFresh = false;
// 	for(long row = 0; row < (int)vtRowItem.size(); ++row)
// 	{
// 		//!< 刷新那些值被改变的
// 		if(vtRowItem[row]->IsValueChange() || vtRowItem[row]->IsIOValueChange()){
// 			bNeedFresh = true;
// 			InitItemValue(vtRowItem[row]->getID(), row);		//!< 只修改值的那两个格
// 		}
// 	}
// 
// 	if(bNeedFresh){				//!< 如果有值被改变了，则需要刷一下
// 		if(m_nProjCol >= 0)		RedrawCol(m_nProjCol);
// 		if(m_nIOCol >= 0)		RedrawCol(m_nIOCol);
// 	}
}

//!< 编辑结束，统一处理
void CItemGrid::OnEditEnd()
{
	CItemMgr::GetMe().SetEditEnd();
	SetFocus();
	RedrawAll();
}

//!< 点击表格的标题，重新排序
void CItemGrid::OnTH_LClicked(int iCol, long lRow, int iUpdn, RECT* pRect, POINT* pPoint, BOOL bProcessed )
{
	if(iUpdn == 0)						return;

	QuickSetCellType(m_iSortCol, -1, 0);
	if(iCol == m_iSortCol)		m_bSortType = !m_bSortType;
	else						m_bSortType = TRUE;
	m_iSortCol = iCol;
	m_strSortCol = m_mpColName[iCol];
	SortGrid();
}

//!< 双击表格
void CItemGrid::OnDClicked(int col,long row,RECT *rect,POINT *point,BOOL processed)
{
	CUGCtrl::OnDClicked(col, row, rect, point, processed);
	if(row < 0)		return;										//!< 没有内容，退出
	CUGCell cell;
	GetCell(0, row, &cell);
	UINT itemID = cell.GetParam();
	CItemView* pView = (CItemView *)GetParent();

// 	if(CProjectMgr::GetMe().IsWatch())
// 	{
// 		WriteValue();											//!< 这里证明了是要修改变量的监控值
// 	}
// 	else
// 	{
// 		if(row == GetNumberRows() - 1){							//!< 这里证明了是要新增变量元素
// 			pView->OnItemAdd();
// 			SelectRowAt(GetNumberRows() - 1);
// 		}else{	pView->OnItemEdit();}							//!< 这里证明了是要修改某个变量元素
// 	}
	if(row == GetNumberRows() - 1){
		pView->OnItemAdd();
		SelectRowAt(GetNumberRows() - 1);
	}
	else if(CProjectMgr::GetMe().IsWatch())	WriteValue();		//!< 这里证明了是要修改变量的监控值
	else		pView->OnItemEdit();
	SetFocus();
}

//!< 写变量的实时值
void CItemGrid::WriteValue()
{
	if(!CProjectMgr::GetMe().IsWatch())			return;
	long row = GetCurrentRow();
	if(row < 0 || row >= GetNumberRows() - 1)		return;
	CUGCell cell;
	GetCell(0, GetCurrentRow(), &cell);
	UINT itemID = cell.GetParam();
	CItemMgr* itemMgr = &CItemMgr::GetMe();
	std::shared_ptr<CItem> item = itemMgr->GetItem(itemID);
	if(!item)		return;
	Dialog::CWriteItemDlg::GetMe().DoModal(item->getID());
}

//!< 编辑变量基本属性
void CItemGrid::EditItem(std::shared_ptr<CItem> item)
{
	if(!item)					return;
	CPropertyItem* pp = &CItemMgr::GetMe().m_ItemProperty;
	pp->SetType(false);
	pp->SetItem(item.get());
	Dialog::CPropertyDlg* dlg = &Dialog::CPropertyDlg::GetMe();
	dlg->DoModal(*pp);
	SetFocus();
}

//!< 统一编辑变量基本属性
void CItemGrid::EditItems(std::list<std::shared_ptr<CItem> > ltItem)
{
	if(ltItem.empty())			{return;}
	if(ltItem.size() == 1)		{ItemEdit();		return;}

	CPropertyItems* pp = &CItemMgr::GetMe().m_ItemPropertys;
	pp->SetItemList(ltItem);
	Dialog::CPropertyDlg* dlg = &Dialog::CPropertyDlg::GetMe();

	dlg->DoModal(*pp);
}

//!< 增加新变量
std::shared_ptr<CItem> CItemGrid::AddNewItem()
{
	MVC::Item::CItemMgr* itemMgr = &MVC::Item::CItemMgr::GetMe();
	std::shared_ptr<CItem> empty;
	//!< 看看还能不能加了变量
	int count = MAX_ITEM_COUNT;
	count = count << 1;
	if(itemMgr->GetItemSize() >= (count >> 1))
	{
		CString strError;
		strError.Format("数量超过了限制 %d 个，无法再添加变量！", (count >> 1));
		MessageBox(strError, _T("警告"), MB_OK | MB_ICONWARNING);
		return empty;
	}
	MVC::Item::CPropertyItem* pp = &MVC::Item::CItemMgr::GetMe().m_ItemProperty;
	pp->SetType(true);
	if(m_uiShowType == 3)			pp->SetGroupID(m_uiCulGroupID);
//	else							pp->SetGroupID(0);
	Dialog::CPropertyDlg* dlg = &Dialog::CPropertyDlg::GetMe();
	if(dlg->DoModal(*pp) == IDOK){
		return itemMgr->GetItem(pp->m_NewItem->getID());
	}
	return empty;
}

//!< 变量克隆
void CItemGrid::ItemClone()
{
	UINT id = GetRowItem(GetCurrentRow());
	CItemMgr* itemMgr = &CItemMgr::GetMe();
	std::shared_ptr<CItem> item = itemMgr->GetItem(id);
	if(!item)							return;
	Dialog::CCloneItemDlg* dlg = &Dialog::CCloneItemDlg::GetMe();
	if(item->getSrcType() == CItem::SRC_TYPE_IO && item->getSrcInfo()->getIOType() == 0)
		dlg->m_nRadioType = 1;
	else
		dlg->m_nRadioType = 0;
	if(IDOK != dlg->DoModal(id))		return;
	//RedrawGrid();
	SetFocus();
	RedrawAll();                                                                                                      

	//!< 添加撤销对象
	CItemDoc* pDoc = (CItemDoc *)((CItemView *)GetParent())->GetDocument();
	std::shared_ptr<SItemUndo> undo;
	for (UINT id : dlg->m_ltItemID)
	{
		item = itemMgr->GetItem(id);
		ASSERT(item);
		undo = std::shared_ptr<SItemUndo>(new SItemUndo);
		undo->m_Item = item;
		undo->m_uiEditType = CGbl::UNDO_TYPE_ADD;
		pDoc->AddUndoMember(undo);
	}
	pDoc->SetUndoEnd();
}

//!< 鼠标左键按下
void MVC::Item::CItemGrid::OnLButtonDown(int col, long row, RECT *rect, POINT *point, BOOL processed)
{
	// 看看要不要克隆
	long colLhs, colRhs, rowLhs, rowRhs;
	m_GI->m_multiSelect->GetCurrentBlock((int*)&colLhs, &rowLhs, (int*)&colRhs, &rowRhs);
	if(rowRhs < 0 || rowRhs >= GetNumberRows() - 1)	return;		//!< 点最后一行无效

	CRect rects;
	GetRangeRect(colLhs, rowRhs, colLhs, rowRhs, rects);
	int iRight = rects.right;
	int iBottom = rects.bottom;
	rects = CRect(iRight - 10, iBottom - 10, iRight + 4, iBottom + 1);
	if(rects.PtInRect(*point))
	{
		CUGCell cell;
		GetCell(0, rowRhs, &cell);
		int nID = cell.GetParam();
		if (CItemMgr::GetMe().GetItem(nID) && !CProjectMgr::GetMe().IsWatch())
		{
			//SetCapture();
			::SetCursor(::LoadCursor(NULL, IDC_CROSS));
			m_nHoldToCloneID = nID;
			m_bHoldToClone = true;
			m_nHoldToCloneRow = rowRhs;
			m_nHoleToCloneCurCount = 0;		//!< 等待克隆的变量的当前数量
		}
	}
}

//!< 鼠标左键抬起
void MVC::Item::CItemGrid::OnLButtonUp(int col, long row, RECT *rect, POINT *point, BOOL processed)
{
	// 看看要不要克隆
	if (!m_bHoldToClone)			return;
	MVC::Item::CItemMgr* itemMgr = &MVC::Item::CItemMgr::GetMe();

	m_bHoldToClone = false;
	::SetCursor(::LoadCursor(NULL, IDC_ARROW));
	std::shared_ptr<CItem> item = itemMgr->GetItem(m_nHoldToCloneID);
	if (!item)						return;
	if (m_nHoleToCloneCurCount == 0)return;

	bool bAdd = m_nHoleToCloneCurCount > 0 ? true : false;		// 看看是增加还是递减

	// 开始增加吧
	int maxID = MAX_ITEM_COUNT;
	std::shared_ptr<MVC::Item::CItem> newItem;
	SoftInfo::CSoftInfo* csi = &SoftInfo::CSoftInfo::GetMe();
	int nCount = m_nHoleToCloneCurCount;						// 要克隆多少个
	if (nCount < 0)	nCount = 0 - nCount;
	std::list<UINT> ltAddID;									// 记录被创建的指令,一会需要压撤销栈的
	int nBit = item->getValType() == 0 ? 1 : 0;					// nType=0整数，1小数
	int nAddrUnit = item->getValType() >= 5 ? 2 : 1;			// 大于等于5的都是双字变量
	for(int i = 0; i < nCount; ++i)
	{
// 这里还有两项工作:
// 		1. 增加方向向上的增加指令克隆功能
// 		2. 我是不是应该先弹个框问问是否克隆的其它配置
		newItem = std::shared_ptr<MVC::Item::CItem>(new MVC::Item::CItem(_T("")));
		if (bAdd)
		{
			if(!item->OnCloneMe(*newItem, i + 1, nAddrUnit, nBit))		continue;			//!< 如果此项没克隆成功，就克隆下一条
		}
		else
		{
			if(!item->OnCloneMe(*newItem, -1 - i, nAddrUnit, nBit))		continue;			//!< 如果此项没克隆成功，就克隆下一条
		}
		if(!itemMgr->AddItem(newItem, maxID, item->getMyGroupID())){
			break;	//!< 如果超界就不再继续遍历了
		}
		else{
			ltAddID.push_back(newItem->getID());
		}
	}
// 	SoftInfo::CSoftInfo::GetMe().setCloneNumber(baseNum);
// 	SoftInfo::CSoftInfo::GetMe().setCloneFloat(baseFlt);

	//!< 添加撤销对象
	CItemDoc* pDoc = (CItemDoc *)((CItemView *)GetParent())->GetDocument();
	std::shared_ptr<SItemUndo> undo;
	for (UINT id : ltAddID)
	{
		item = itemMgr->GetItem(id);
		ASSERT(item);
		undo = std::shared_ptr<SItemUndo>(new SItemUndo);
		undo->m_Item = item;
		undo->m_uiEditType = CGbl::UNDO_TYPE_ADD;
		pDoc->AddUndoMember(undo);
	}
	pDoc->SetUndoEnd();
	RedrawAll();
}

//!< 鼠标左键点击，如果点击的是那个大点的话，需要特殊处理
void MVC::Item::CItemGrid::OnLClicked(int col,long row,int updn,RECT *rect,POINT *point,BOOL processed)
{
	if(updn == 1)		//!< 鼠标按下
	{
		OnLButtonDown(col, row, rect, point, processed);
	}
	else
	{
		OnLButtonUp(col, row, rect, point, processed);
	}
}

//!< 右键表格，updn是TRUE表示down，是FALSE表示up
void MVC::Item::CItemGrid::OnRClicked(int col,long row,int updn,RECT *rect,POINT *point,BOOL processed)
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
		CMenu* menu =popMenu.GetSubMenu(0);
		//!< 对于特殊地方的右键会出现不同的菜单项
		CItemMgr* itemMgr = &CItemMgr::GetMe();
		if(!CProjectMgr::GetMe().IsWatch()){				//!< 未监控
			menu->EnableMenuItem(ID_ITEM_WRITE, MF_GRAYED);
		}
		else{												//!< 监控
			menu->EnableMenuItem(ID_ITEM_ADD, MF_DISABLED|MF_BYCOMMAND);
			menu->EnableMenuItem(ID_ITEM_EDIT, MF_DISABLED|MF_BYCOMMAND);
			menu->EnableMenuItem(ID_ITEM_DELETE, MF_DISABLED|MF_BYCOMMAND);
			menu->EnableMenuItem(ID_ITEM_UP, MF_DISABLED|MF_BYCOMMAND);
			menu->EnableMenuItem(ID_ITEM_DOWN, MF_DISABLED|MF_BYCOMMAND);
			menu->EnableMenuItem(ID_ITEM_CLONE, MF_DISABLED|MF_BYCOMMAND);
			menu->EnableMenuItem(ID_ITEM_EDIT_ALL, MF_DISABLED|MF_BYCOMMAND);
			menu->EnableMenuItem(ID_ITEM_IN, MF_DISABLED|MF_BYCOMMAND);
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
			menu->EnableMenuItem(ID_ITEM_CLONE, MF_DISABLED|MF_BYCOMMAND);
			menu->EnableMenuItem(ID_ITEM_EDIT_ALL, MF_DISABLED|MF_BYCOMMAND);
		}
		if(itemMgr->m_ltItemClipBoard.empty()){				//!< 剪贴板里没东西
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
			menu->EnableMenuItem(ID_ITEM_EDIT, MF_DISABLED|MF_BYCOMMAND);
			menu->EnableMenuItem(ID_ITEM_CLONE, MF_DISABLED|MF_BYCOMMAND);
			menu->EnableMenuItem(ID_ITEM_UP, MF_DISABLED|MF_BYCOMMAND);
			menu->EnableMenuItem(ID_ITEM_DOWN, MF_DISABLED|MF_BYCOMMAND);
		}
		menu->EnableMenuItem(ID_PROJ_RENAME, MF_DISABLED|MF_BYCOMMAND);
		menu->CheckMenuItem(ID_SHOW_HEX, SoftInfo::CSoftInfo::GetMe().isShowHex()?MF_CHECKED:MF_UNCHECKED);
		CXTPCommandBars::TrackPopupMenu(menu, TPM_LEFTALIGN|TPM_NONOTIFY, point->x, point->y, this);
	}
}

void MVC::Item::CItemGrid::OnKeyDown(UINT *vcKey,BOOL processed)
{
	CUGCtrl::OnKeyDown(vcKey, processed);
	long cul_row = GetCurrentRow();
	if(*vcKey == VK_RETURN){
		CRect rc(0,0,0,0);
		CPoint point(0,0);
		OnDClicked(0, cul_row, &rc, &point, processed);
	}
	else if(*vcKey == VK_DELETE){
		CItemView* pView = (CItemView *)GetParent();
		if(pView)	pView->OnVkdelete();
	}
	else if(*vcKey == 0x41){			//!< 输入的A
		if(GetAsyncKeyState(VK_CONTROL)){
			CItemView* pView = (CItemView *)GetParent();
			if(pView)	pView->OnSelectAll();
		}
	}
	else if(*vcKey == VK_F1){
		SoftInfo::CMyHelp::GetMe().ShowHelp(_T("变量"));
	}
}

//!< 鼠标移动事件
void MVC::Item::CItemGrid::OnMouseMove(int col,long row,POINT *point,UINT nFlags,BOOL processed)
{
	if(nFlags == 0)		//!< 0普通移动，1按下鼠标左键移动，2右键移动，16滚轮移动
	{
		int sCol, eCol;
		long sRow, eRow;
		m_GI->m_multiSelect->GetCurrentBlock(&sCol, &sRow, &eCol, &eRow);
		if(eRow < 0 || eRow >= GetNumberRows() - 1)	return;		//!< 点击最后一行也无效
		CRect rect;
		GetRangeRect(sCol, sRow, sCol, eRow, rect);
		int iRight = rect.right;
		int iBottom = rect.bottom;
		rect = CRect(iRight - 10, iBottom - 10, iRight + 4, iBottom);
		if(rect.PtInRect(*point) && !CProjectMgr::GetMe().IsWatch())
			::SetCursor(::LoadCursor(NULL, IDC_CROSS));
		else
			::SetCursor(::LoadCursor(NULL, IDC_ARROW));
	}
	else if (nFlags == 1)
	{
		if (m_bHoldToClone)
		{
			m_nHoleToCloneCurCount = row - m_nHoldToCloneRow;
			if (m_nHoleToCloneCurCount < 0)
				m_nHoleToCloneCurCount = 0;
		}
	}
}

//!< 鼠标按住移动超过屏幕的事件
void MVC::Item::CItemGrid::OnMouseMoveOver(int col,long row,BOOL bUp)
{
	if (m_bHoldToClone)
	{
		if (row <= 0 && bUp)
			m_nHoleToCloneCurCount--;
		else if (row >= (GetNumberRows() - 1) && !bUp)
			m_nHoleToCloneCurCount++;
		if (m_nHoleToCloneCurCount < 0)
			m_nHoleToCloneCurCount = 0;
		RedrawAll();
	}
}

//!< 接收右键菜单的消息，iItem是菜单项的ID，lRow和iCol是右键的位置（行列值）
void MVC::Item::CItemGrid::OnMenuCommand(int iCol, long lRow, int iSection, int iItem)
{
	UNREFERENCED_PARAMETER(iCol);
	UNREFERENCED_PARAMETER(lRow);
	UNREFERENCED_PARAMETER(iSection);
	CRect rect(0,0,0,0);
	CPoint point(0,0);
	int sortID = 0;
	CItemView* pView = (CItemView *)GetParent();
	if(!pView)			return;
	switch(iItem)
	{
		case MENU_FIND:			FindDialog();			break;		//!< 查找
		case ID_ITEM_ADD:		pView->OnItemAdd();		break;		//!< 添加变量
		case ID_ITEM_EDIT:		pView->OnItemEdit();	break;		//!< 编辑变量
		case ID_ITEM_DELETE:	pView->OnVkdelete();	break;		//!< 变量移除
		case ID_EDIT_COPY:		pView->OnEditCopy();	break;		//!< 变量拷贝
		case ID_EDIT_CUT:		pView->OnEditCut();		break;		//!< 变量剪切
		case ID_EDIT_PASTE:		pView->OnEditPaste();	break;		//!< 变量粘贴
		case ID_ITEM_EDIT_ALL:	pView->OnItemEditAll();	break;		//!< 变量统一修改
		case ID_ITEM_IN:		pView->OnItemIn();		break;		//!< 变量导入
		case ID_ITEM_UP:		pView->OnItemUp();		break;		//!< 变量上移
		case ID_ITEM_DOWN:		pView->OnItemDown();	break;		//!< 变量下移
		case ID_ITEM_CLONE:		pView->OnClone();		break;		//!< 变量克隆
		case ID_ITEM_OUT:		pView->OnItemOut();		break;		//!< 导出所有的
		case ID_CURRENT_OUT:	pView->OnCurrentOut();	break;		//!< 导出当前的
		case ID_SELECT_OUT:		pView->OnSelectOut();	break;		//!< 导出选中的
		case ID_SELECT_ALL:		pView->OnSelectAll();	break;		//!< 全选
		case ID_ITEM_WRITE:		WriteValue();			break;		//!< 写入变量值
		case ID_SHOW_HEX:		pView->OnShowHex();		break;		//!< 显示十六进制
	}
	SetFocus();
}

///////////////////////////////////////////////////////////////////////////////
int CItemGrid::OnMenuStart(int iCol, long lRow, int iSection)
{
	UNREFERENCED_PARAMETER(iCol);
	UNREFERENCED_PARAMETER(lRow);
	if(iSection == UG_GRID)		return TRUE;
	else						return FALSE;
}

//!< 对该两个ID的变量进行排序
bool CItemGrid::SortItem(UINT& lth, UINT& rth)
{
	bool bSort = true;
	CItemMgr* itemMgr = &CItemMgr::GetMe();
	CItem *lItem, *rItem;
	lItem = itemMgr->GetItem(lth).get();
	rItem = itemMgr->GetItem(rth).get();
	SoftInfo::CSoftInfo* csi = &SoftInfo::CSoftInfo::GetMe();
	//!< 字符串比较
	if(m_strSortCol == csi->m_vtColInfo[COL_NAME].name)
	{
		bSort = lItem->getName().Compare(rItem->getName()) <= 0 ? true : false;
	}
	else if(m_strSortCol == csi->m_vtColInfo[COL_TAG].name)
	{
		bSort = lItem->getTag().Compare(rItem->getTag()) <= 0 ? true : false;
	}
	else if(m_strSortCol == csi->m_vtColInfo[COL_DESCRIPT].name)
	{
		bSort = lItem->getDescription().Compare(rItem->getDescription()) <= 0 ? true : false;
	}
	else if(m_strSortCol == csi->m_vtColInfo[COL_SCRIPT].name)
	{
		int ltype = lItem->getSrcType(), rtype = rItem->getSrcType();
		if(ltype == CItem::SRC_TYPE_MEM && rtype == CItem::SRC_TYPE_IO)			bSort = true;
		else if(ltype == CItem::SRC_TYPE_IO && rtype == CItem::SRC_TYPE_MEM)	bSort = false;
		else	bSort = lItem->getSrcInfo()->getScriptText().Compare(rItem->getSrcInfo()->getScriptText()) <= 0?true:false;
	}
	//!< 列表比较
	else if(m_strSortCol == csi->m_vtColInfo[COL_SRC].name)
	{
		bSort = lItem->getSrcType() <= rItem->getSrcType();
	} 
	else if(m_strSortCol == csi->m_vtColInfo[COL_ACCESS].name)
	{
		bSort = lItem->getAccessRight() <= rItem->getAccessRight();
	}
	else if(m_strSortCol == csi->m_vtColInfo[COL_TYPE].name)
	{
		bSort = lItem->getValType() <= rItem->getValType();
	}
	else if(m_strSortCol == csi->m_vtColInfo[COL_IOTYPE].name)
	{
		int ltype = lItem->getSrcType(), rtype = rItem->getSrcType();
		if(ltype == CItem::SRC_TYPE_MEM && rtype == CItem::SRC_TYPE_IO)			bSort = true;
		else if(ltype == CItem::SRC_TYPE_IO && rtype == CItem::SRC_TYPE_MEM)	bSort = false;
		else	bSort = lItem->getSrcInfo()->getIOType() <= rItem->getSrcInfo()->getIOType();
	}
	else if(m_strSortCol == csi->m_vtColInfo[COL_GROUP].name)
	{
		CString textL = itemMgr->GetGroup(lItem->getMyGroupID())->getName();
		CString textR = itemMgr->GetGroup(rItem->getMyGroupID())->getName();
		bSort = textL.Compare(textR) <= 0 ? true : false;
	}
	else if(m_strSortCol == csi->m_vtColInfo[COL_CONVERT].name)
	{
		int ltype = lItem->getSrcType(), rtype = rItem->getSrcType();
		if(ltype == CItem::SRC_TYPE_MEM && rtype == CItem::SRC_TYPE_IO)			bSort = true;
		else if(ltype == CItem::SRC_TYPE_IO && rtype == CItem::SRC_TYPE_MEM)	bSort = false;
		else	bSort = lItem->getSrcInfo()->getConvertType() <= rItem->getSrcInfo()->getConvertType();
	}
	else if(m_strSortCol == csi->m_vtColInfo[COL_DEVICE].name)
	{
		int ltype = lItem->getSrcType(), rtype = rItem->getSrcType();
		if(ltype == CItem::SRC_TYPE_MEM && rtype == CItem::SRC_TYPE_IO)			bSort = true;
		else if(ltype == CItem::SRC_TYPE_IO && rtype == CItem::SRC_TYPE_MEM)	bSort = false;
		else	bSort = lItem->getSrcInfo()->getDeviceID() <= rItem->getSrcInfo()->getDeviceID();
	}
	else if(m_strSortCol == csi->m_vtColInfo[COL_ADDR].name)
	{
		int ltype = lItem->getSrcType(), rtype = rItem->getSrcType();
		if(ltype == CItem::SRC_TYPE_MEM && rtype == CItem::SRC_TYPE_IO)			bSort = true;
		else if(ltype == CItem::SRC_TYPE_IO && rtype == CItem::SRC_TYPE_MEM)	bSort = false;
		else	bSort = lItem->getSrcInfo()->GetAreaString().Compare(rItem->getSrcInfo()->GetAreaString()) <= 0?true:false;
	}
	//!< 数值比较
	else if(m_strSortCol == csi->m_vtColInfo[COL_ID].name)
	{
		bSort = lItem->getID() <= rItem->getID();
	}
	else if(m_strSortCol == csi->m_vtColInfo[COL_VALUE].name)
	{
		CComVariant lVar = lItem->getValue(), rVar = rItem->getValue();
		if(!CGbl::ChangeVariantType(lVar, VT_R8))								bSort = true;
		else if(!CGbl::ChangeVariantType(rVar, VT_R8))							bSort = false;
		else	bSort = lVar.dblVal <= rVar.dblVal;
	}
	else if(m_strSortCol == csi->m_vtColInfo[COL_IOVALUE].name)
	{
		int ltype = lItem->getSrcType(), rtype = rItem->getSrcType();
		if(ltype == CItem::SRC_TYPE_MEM && rtype == CItem::SRC_TYPE_IO)			bSort = true;
		else if(ltype == CItem::SRC_TYPE_IO && rtype == CItem::SRC_TYPE_MEM)	bSort = false;
		else
		{
			CComVariant lVar = lItem->getIOValue(), rVar = rItem->getIOValue();
			if(!CGbl::ChangeVariantType(lVar, VT_R8))							bSort = true;
			else if(!CGbl::ChangeVariantType(rVar, VT_R8))						bSort = false;
			else	bSort = lVar.dblVal <= rVar.dblVal;
		}
	}
	else if(m_strSortCol == csi->m_vtColInfo[COL_FRESHTIME].name)
	{
		bSort = lItem->getSrcInfo()->getFreshTime() <= rItem->getSrcInfo()->getFreshTime();
	}
	else if(m_strSortCol == csi->m_vtColInfo[COL_BIT].name)
	{
		if(lItem->getValType() != 0)											bSort = true;
		else if(rItem->getValType() != 0)										bSort = false;
		else if(!lItem->getAlarmInfo()->getLoloActive())						bSort = true;
		else if(!rItem->getAlarmInfo()->getLoloActive())						bSort = false;
		else	bSort = lItem->getAlarmInfo()->getBitAlarmType() <= rItem->getAlarmInfo()->getBitAlarmType();
	}
	else if(m_strSortCol == csi->m_vtColInfo[COL_LOLO].name)
	{
		if(!lItem->getAlarmInfo()->getLoloActive())								bSort = true;
		else if(!rItem->getAlarmInfo()->getLoloActive())						bSort = false;
		else	bSort = lItem->getAlarmInfo()->getLoloValue() <= rItem->getAlarmInfo()->getLoloValue();
	}
	else if(m_strSortCol == csi->m_vtColInfo[COL_LOW].name)
	{
		if(!lItem->getAlarmInfo()->getLowActive())								bSort = true;
		else if(!rItem->getAlarmInfo()->getLowActive())						bSort = false;
		else	bSort = lItem->getAlarmInfo()->getLowValue() <= rItem->getAlarmInfo()->getLowValue();
	}
	else if(m_strSortCol == csi->m_vtColInfo[COL_HIGH].name)
	{
		if(!lItem->getAlarmInfo()->getHighActive())								bSort = true;
		else if(!rItem->getAlarmInfo()->getHighActive())						bSort = false;
		else	bSort = lItem->getAlarmInfo()->getHighValue() <= rItem->getAlarmInfo()->getHighValue();
	}
	else if(m_strSortCol == csi->m_vtColInfo[COL_HIHI].name)
	{
		if(!lItem->getAlarmInfo()->getHihiActive())								bSort = true;
		else if(!rItem->getAlarmInfo()->getHihiActive())						bSort = false;
		else	bSort = lItem->getAlarmInfo()->getHihiValue() <= rItem->getAlarmInfo()->getHihiValue();
	}
	else if(m_strSortCol == csi->m_vtColInfo[COL_AIM].name)
	{
		if(!lItem->getAlarmInfo()->getAimActive())								bSort = true;
		else if(!rItem->getAlarmInfo()->getAimActive())							bSort = false;
		else	bSort = lItem->getAlarmInfo()->getAimValue() <= rItem->getAlarmInfo()->getAimValue();
	}
	else if(m_strSortCol == csi->m_vtColInfo[COL_SHIFT].name)
	{
		if(!lItem->getAlarmInfo()->getShiftActive())							bSort = true;
		else if(!rItem->getAlarmInfo()->getShiftActive())						bSort = false;
		else	bSort = lItem->getAlarmInfo()->getShiftDelta() <= rItem->getAlarmInfo()->getShiftDelta();
	}
	else if(m_strSortCol == csi->m_vtColInfo[COL_MODBUS485].name)
	{
		if(!lItem->getModbus485())												bSort = true;
		else if(!rItem->getModbus485())											bSort = false;
		else																	bSort = bSort;
	}
	else if (m_strSortCol == csi->m_vtColInfo[COL_REV_DB].name)
	{
		if (!lItem->getReservDB())												bSort = true;
		else if (!rItem->getReservDB())											bSort = false;
		else																	bSort = bSort;
	}
//	if(!__bSortType)	bSort = !bSort;
	return !bSort;
}

//!< 排序，比较两个单元格内的数据的大小
int CItemGrid::OnSortEvaluate(CUGCell* pCell1, CUGCell* pCell2, int iFlags)
{
	//!< 无论怎么排，新建变量一栏都必须放在表格末尾
	CString text1=(CString)pCell1->GetText();
	CString text2=(CString)pCell2->GetText();
	if(text1 == _T("新建变量...") || text1=="")				return 1;
	else if(text2 == _T("新建变量...") || text2=="")		return -1;

	//!< 如果是从大到小排列，就把这两个单元格互换了就好了
	if( iFlags & UG_SORT_DESCENDING ){
		CUGCell *ptr = pCell1;
		pCell1 = pCell2;
		pCell2 = ptr;
	}
	COleDateTime date1, date2;
	int retVal = 0;

	CString colName = m_mpColName[m_iSortCol];
	if(SoftInfo::CSoftInfo::GetMe().m_vtColInfo[COL_TYPE].name == colName)			//!< 类型比较
	{
		retVal = m_mpValType[(CString)pCell1->GetText()] - m_mpValType[(CString)pCell2->GetText()];
	}
	else														//!< 其它比较
	{
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
	}
	return retVal;
}

void CItemGrid::ShowItem(std::list<UINT> ltItemID)
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

void CItemGrid::RedrawGrid()
{
	CItemMgr* itemMgr = &CItemMgr::GetMe();
	if(m_uiShowType == 0)			//!< 所有变量
	{
		std::list<UINT> ltItemID;
		std::shared_ptr<CItem> item;
		for (auto item : itemMgr->m_vtItem){
			if(item)				ltItemID.push_back(item->getID());
		}
		ShowItem(ltItemID);
	}
	else if(m_uiShowType == 3)		//!< 变量组里的变量
	{
		std::shared_ptr<CItemGroup> group = itemMgr->GetGroup(m_uiCulGroupID);
		ShowItem(group->getMyItem());
	}
}

void MVC::Item::CItemGrid::SortGrid()
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

//	std::list<UINT> ltShowItemID;
	ltShowItemID.clear();
	CItemMgr* itemMgr = &CItemMgr::GetMe();
	if(m_uiShowType == 0)			//!< 所有变量
	{
		for (auto item : itemMgr->m_vtItem){
			if(item)
				ltShowItemID.push_back(item->getID());
		}
	}
	else if(m_uiShowType == 3)		//!< 变量组里的变量
	{
		std::shared_ptr<CItemGroup> group = itemMgr->GetGroup(m_uiCulGroupID);
		ltShowItemID = group->getMyItem();
	}
	ltShowItemID.sort(SortItem);
	ShowItem(ltShowItemID);
// 	RedrawAll();
}

void MVC::Item::CItemGrid::ItemUp()
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
	std::shared_ptr<CItem> item1 = CItemMgr::GetMe().GetItem(cell1.GetParam());
	std::shared_ptr<CItem> item2 = CItemMgr::GetMe().GetItem(cell2.GetParam());
	ASSERT(item1 && item2);

	//!< 获得原始的变量数据，后边要压撤销栈
	std::shared_ptr<SItemUndo> undo1 = std::shared_ptr<SItemUndo>(new SItemUndo(CGbl::UNDO_TYPE_UPD, item1, 0));
	std::shared_ptr<SItemUndo> undo2 = std::shared_ptr<SItemUndo>(new SItemUndo(CGbl::UNDO_TYPE_UPD, item2, 0));

	//!< 交换这两个变量
	CItemMgr::GetMe().ExChangeItem(item1->getID(), item2->getID());
	InitItemOne(item2->getID(), row);
	InitItemOne(item1->getID(), row - 1);

	//!< 添加撤销对象
	CItemDoc* pDoc = (CItemDoc *)((CItemView *)GetParent())->GetDocument();
	pDoc->AddUndoMember(undo1);
	pDoc->AddUndoMember(undo2);
	pDoc->SetUndoEnd();

	SelectRowAt(row - 1);										//!< 选中新建变量这一项
	RedrawAll();
}

void MVC::Item::CItemGrid::ItemDown()
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
	std::shared_ptr<CItem> item1 = CItemMgr::GetMe().GetItem(cell1.GetParam());
	std::shared_ptr<CItem> item2 = CItemMgr::GetMe().GetItem(cell2.GetParam());
	ASSERT(item1 && item2);

	//!< 获得原始的变量数据，后边要压撤销栈
	std::shared_ptr<SItemUndo> undo1 = std::shared_ptr<SItemUndo>(new SItemUndo(CGbl::UNDO_TYPE_UPD, item1, 0));
	std::shared_ptr<SItemUndo> undo2 = std::shared_ptr<SItemUndo>(new SItemUndo(CGbl::UNDO_TYPE_UPD, item2, 0));

	//!< 交换这两个变量
	CItemMgr::GetMe().ExChangeItem(item1->getID(), item2->getID());
	InitItemOne(item2->getID(), row);
	InitItemOne(item1->getID(), row + 1);

	//!< 添加撤销对象
	CItemDoc* pDoc = (CItemDoc *)((CItemView *)GetParent())->GetDocument();
	pDoc->AddUndoMember(undo1);
	pDoc->AddUndoMember(undo2);
	pDoc->SetUndoEnd();

	SelectRowAt(row + 1);										//!< 选中新建变量这一项
	RedrawAll();
}

//!< 统计一下要删除的变量，参数表示是否要询问一下用户，剪切时候要删除，就不需要通知用户
void MVC::Item::CItemGrid::ItemRemove(bool bAsk /* = true */)
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

	CItemMgr* itemMgr = &CItemMgr::GetMe();
	CUGCell cell;
	std::shared_ptr<CItem> item;
	std::list<std::shared_ptr<CItem> > ltSelectItem;

	//!< 统计被选中的变量
	for (long row : ltSelectRow)
	{
		GetCell(0, row, &cell);
		item = itemMgr->GetItem(cell.GetParam());
		ASSERT(item);
		ltSelectItem.push_back(item);
	}

	ItemRemove(ltSelectItem);				//!< 开始删除
	m_GI->m_multiSelect->ClearAll();		//!< 清空所有选择的行
	SetFocus();

	//!< 添加撤销对象
	CItemDoc* pDoc = (CItemDoc *)((CItemView *)GetParent())->GetDocument();
	std::shared_ptr<SItemUndo> undo;
	for (auto item : ltSelectItem)
	{
		undo = std::shared_ptr<SItemUndo>(new SItemUndo);
		undo->m_Item = item;
		undo->m_uiEditType = CGbl::UNDO_TYPE_DEL;
		undo->m_uiEditInfo = m_uiCulGroupID;
		pDoc->AddUndoMember(undo);
	}
	pDoc->SetUndoEnd();

	return;
}

//!< 删除这些变量
void MVC::Item::CItemGrid::ItemRemove(std::list<std::shared_ptr<CItem> > ltItem)
{
	CItemMgr* itemMgr = &CItemMgr::GetMe();
	for (auto item : ltItem)
	{
		if (item)
			itemMgr->DeleteItem(item->getID());
//		if(m_uiShowType == 3)		itemMgr->RemoveItem(item->getID(), m_uiCulGroupID);		//!< 分组显示中，不能删除组中的变量
//		else						itemMgr->DeleteItem(item->getID());						//!< 如果不是分组显示，则把这个变量彻底删除
	}
}

//!< 变量拷贝
void MVC::Item::CItemGrid::ItemCopy()
{
	CItemMgr* itemMgr = &CItemMgr::GetMe();
	int numRow = GetNumberRows() - 1;
	std::list<UINT> ltItemID;
	CUGCell cell;
	std::shared_ptr<CItem> srcItem, newItem;
	for(UINT i = 0; i < numRow; ++i)
	{
		if(IsSelected(0, i))
		{
			GetCell(0, i, &cell);
			ltItemID.push_back(cell.GetParam());
		}
	}
	if(ltItemID.size() <= 0)			return;
	itemMgr->m_ltItemClipBoard.clear();
	for (UINT id : ltItemID)
	{
		srcItem = itemMgr->GetItem(id);
		ASSERT(srcItem);
		newItem = std::shared_ptr<CItem>(new CItem(_T("")));
		*newItem = *srcItem;
		itemMgr->m_ltItemClipBoard.push_back(newItem);
	}
}

//!< 变量剪切
void MVC::Item::CItemGrid::ItemCut()
{
	ItemCopy();			//!< 先拷贝，再删除
	ItemRemove(false);
}

//!< 变量粘贴
void MVC::Item::CItemGrid::ItemPaste()
{
	CItemMgr* itemMgr = &CItemMgr::GetMe();
	if(itemMgr->m_ltItemClipBoard.empty())						return;
	CString name;
	std::shared_ptr<CItem> item, srcItem;
	std::list<std::shared_ptr<CItem> > ltItem;
	//!< 不能直接把剪贴板里的变量粘到变量表中，而是要拷贝一份过去
	for (auto srcItem : itemMgr->m_ltItemClipBoard)
	{
		item = std::shared_ptr<CItem>(new CItem(_T("")));
		*item = *srcItem;
		ltItem.push_back(item);
	}
	//!< 现在开始粘贴
	CItemDoc* pDoc = (CItemDoc *)((CItemView *)GetParent())->GetDocument();
	Dialog::CItemInConfigDlg* dlg = &Dialog::CItemInConfigDlg::GetMe();
	if(IDOK != dlg->DoModal(Dialog::CItemInConfigDlg::Paste, GetGroupID()))	return;
	pDoc->ItemInAdvanceOptions(ltItem);
	int devID = dlg->GetDeviceID();
	for (auto item : ltItem)
		if(item->getSrcType() == CItem::SRC_TYPE_IO)
			item->getSrcInfo()->setDevID(devID);
	if(dlg->m_nSameNameType == 0)			pDoc->ItemInRenameItem(ltItem, dlg->m_uiGroupID);
	else if(dlg->m_nSameNameType == 1)		pDoc->ItemInDelFileItem(ltItem, dlg->m_uiGroupID);
	else if(dlg->m_nSameNameType == 2)		pDoc->ItemInDelMgrItem(ltItem, dlg->m_uiGroupID);
	else if(dlg->m_nSameNameType == 3)		pDoc->ItemInRenameStrItem(ltItem, dlg->m_strItemSameName, dlg->m_uiGroupID);
	pDoc->SetUndoEnd();
}

void MVC::Item::CItemGrid::SelectRowAt(long row)
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

//!< 导出所有变量
void MVC::Item::CItemGrid::OutItemAll()
{
	CString outText;
	CItemMgr* mgr = &CItemMgr::GetMe();
	if(mgr->GetItemSize() <= 0)		return;
	outText.Format("是否要导出 所有 变量？(%d 项)",mgr->GetItemSize());
	if(IDYES == AfxMessageBox(outText, MB_YESNO | MB_ICONINFORMATION))
		mgr->OutItem(mgr->GetItemIDAll());
}

//!< 导出当前所有变量
void MVC::Item::CItemGrid::OutItemCurrent()
{
	CString outText;
	CItemMgr* mgr = &CItemMgr::GetMe();
	if(GetNumberRows() <= 1)			return;
	outText.Format("是否要导出 当前显示 的所有变量？(%d 项)", GetNumberRows() - 1);
	if(IDYES == AfxMessageBox(outText, MB_YESNO | MB_ICONINFORMATION))
	{
		if(m_uiShowType == 0)			mgr->OutItem(mgr->GetItemIDAll());
		else if(m_uiShowType == 1)		mgr->OutItem(mgr->GetItemIDMem());
		else if(m_uiShowType == 2)		mgr->OutItem(mgr->GetItemIDIO());
		else if(m_uiShowType == 3)		mgr->OutItem(mgr->GetGroup(m_uiCulGroupID)->getMyItem());
	}
}

//!< 导出所有选中变量
void MVC::Item::CItemGrid::OutItemSelect()
{
	CString outText;
	UINT numRow = GetNumberRows() - 1;
	std::list<UINT> ltItem;
	CUGCell cell;
	std::shared_ptr<CItem> item;
	CItemMgr* itemMgr = &CItemMgr::GetMe();
	for(UINT i = 0; i < numRow; ++i)
	{
		if(IsSelected(0, i))
		{
			GetCell(0, i, &cell);
			item = itemMgr->GetItem(cell.GetParam());
			ASSERT(item);
			ltItem.push_back(item->getID());
		}
	}
	if(ltItem.size() <= 0)			return;
	outText.Format("是否要导出 当前选中 的所有变量？(%d 项)", (UINT)ltItem.size());
	if(IDYES == AfxMessageBox(outText, MB_YESNO | MB_ICONINFORMATION))
		CItemMgr::GetMe().OutItem(ltItem);
}

//!< 重绘选中框，使其更醒目
void MVC::Item::CItemGrid::OnDrawFocusBackRect(CDC *dc)
{
	long startCol, endCol, startRow, endRow, topRow, bottomRow;
	m_GI->m_multiSelect->GetCurrentBlock((int *)&startCol, &startRow, (int *)&endCol, &endRow);
	topRow = GetTopRow();	bottomRow = GetBottomRow();
	if((endRow < topRow) || (startRow > bottomRow))		return;
	if (m_bHoldToClone)		startRow = endRow = m_nHoldToCloneRow;

	CRect rect;
	GetRangeRect(startCol, startRow, endCol - 1, endRow, rect);
	m_CUGGrid->m_drawHint.AddHint(startCol - 1, startRow - 1, endCol + 1, endRow + 1);

	int right = rect.right;
	int top = rect.top;
	int left = rect.left;
	int bottom = rect.bottom;

	CRect firstColRect;
	GetRangeRect(startCol, endRow, startCol, endRow, &firstColRect);
	int firstColRight = firstColRect.right;
	int firstColTop = firstColRect.top;
	int firstColLeft = firstColRect.left;
	int firstColBottom = firstColRect.bottom;

	//!< 画方框黑线
	dc->SelectObject(GetStockObject(BLACK_PEN));
	dc->MoveTo(firstColRight - 4, bottom);
	dc->LineTo(left, bottom);
	dc->LineTo(left, top - 1);
	dc->LineTo(right + 1, top - 1);
	dc->LineTo(right + 1, bottom);
	dc->LineTo(firstColRight + 4, bottom);

	dc->MoveTo(firstColRight - 4, bottom - 2);
	dc->LineTo(left + 2, bottom - 2);
	dc->LineTo(left + 2, top + 1);
	dc->LineTo(right - 1, top + 1);
	dc->LineTo(right - 1, bottom - 2);
	dc->LineTo(firstColRight + 4, bottom - 2);

	//!< 画黑格
	dc->SelectObject(GetStockObject(BLACK_PEN));
	dc->MoveTo(firstColRight - 2, bottom - 3);
	dc->LineTo(firstColRight + 2, bottom - 3);
	dc->LineTo(firstColRight + 2, bottom + 1);
	dc->LineTo(firstColRight - 2, bottom + 1);
	dc->LineTo(firstColRight - 2, bottom - 3);

	dc->MoveTo(firstColRight - 1, bottom - 2);
	dc->LineTo(firstColRight + 1, bottom - 2);
	dc->LineTo(firstColRight + 1, bottom);
	dc->LineTo(firstColRight - 1, bottom);
	dc->LineTo(firstColRight - 1, bottom - 2);

	//!< 画白线
	dc->SelectObject(GetStockObject(WHITE_PEN));
	dc->MoveTo(firstColRight - 3, bottom);
	dc->LineTo(firstColRight - 3, bottom - 4);
	dc->LineTo(firstColRight + 3, bottom - 4);
	dc->LineTo(firstColRight + 3, bottom);

	//!< 画浅灰色的中间线
	CPen pen;
	pen.CreatePen(PS_SOLID,1,RGB(120,120,120));
	dc->SelectObject(pen);
	dc->MoveTo(left + 1, bottom - 1);
	dc->LineTo(left + 1, top);
	dc->LineTo(right, top);
	dc->LineTo(right, bottom - 1);
	dc->LineTo(left + 1, bottom - 1);

	dc->MoveTo(firstColRight, bottom - 3);
	dc->LineTo(firstColRight, bottom + 1);
}

//!< 重绘选中框，使其更醒目
void MVC::Item::CItemGrid::OnDrawFocusHodeText(CDC *dc)
{
	if (!m_bHoldToClone)				return;
	if (m_nHoleToCloneCurCount <= 0)	return;

	long startCol, endCol, startRow, endRow, topRow, bottomRow;
	m_GI->m_multiSelect->GetCurrentBlock((int *)&startCol, &startRow, (int *)&endCol, &endRow);
	topRow = GetTopRow();	bottomRow = GetBottomRow();
	if((endRow < topRow) || (startRow > bottomRow))		return;

	// 画文本
	CPen penHold;
	penHold.CreatePen(PS_SOLID, 1, RGB(255, 0, 0));
	dc->SelectObject(penHold);
	int nCulRow = endRow;
	if (m_nHoleToCloneCurCount < 0)	nCulRow = startRow;
	CRect rectCulRowFirstCol;
	GetRangeRect(startCol, nCulRow, startCol, nCulRow, &rectCulRowFirstCol);
	CString str;
	str.Format("%d个", abs(m_nHoleToCloneCurCount));
	dc->TextOut(rectCulRowFirstCol.right + 8, rectCulRowFirstCol.top + 6, str);

	// 画框选的小边框
	CPen penRect;
	penRect.CreatePen(PS_INSIDEFRAME, 3, 0x257DF4);
	dc->SelectObject(penRect);
	CRect rect;
	GetRangeRect(startCol, startRow, startCol, endRow, &rect);

	dc->MoveTo(rect.left, rect.top);
	dc->LineTo(rect.right, rect.top);
	dc->LineTo(rect.right, rect.bottom);
	dc->LineTo(rect.left, rect.bottom);
	dc->LineTo(rect.left, rect.top);
}

void MVC::Item::CItemGrid::OnDrawFocusRect(CDC *dc,RECT *rect)
{
	if(CProjectMgr::GetMe().IsWatch())	return;		//!< 监控时不画黑框
	int dcID = dc->SaveDC();
	OnDrawFocusBackRect(dc);
	if (m_bHoldToClone)
		OnDrawFocusHodeText(dc);
}

//!< 画背景，其实是我用来画水印的，不过由于效果不理想，现在已经弃用
void MVC::Item::CItemGrid::OnDrawBackBitmap(CDC *dc)
{
	return;
	CRect rt;
	GetClientRect(&rt);
	OnDrawBackBitmap(dc, rt.left, rt.top, rt.right, rt.bottom);
}

//!< 打印时画水印，目前还完全没加上，因为打印出来的效果不理想
//!< 可能是因为平时显示和打印时的尺寸单位不一致造成的，平时显示的单位的像素，打印时可能是毫米或者磅、英寸等单位
void MVC::Item::CItemGrid::OnDrawBackBitmap(CDC *dc, int l ,int t, int r, int b)
{
/*	if(!SoftInfo::CSoftInfo::GetMe().isItemWaterMark())		return;
	BITMAP bm; 
	CBitmap bitmap;
	CDC dcMem;
	CRect rt = CRect(l, t, r, b);
	CRect rect;
	GetClientRect(&rect);
	float zoomWidth = rt.Width() / rect.Width();
	float zoomHeight = rt.Height() / rect.Height();
	if(zoomWidth < 0.5)		zoomWidth = 0.5;
	if(zoomHeight < 0.5)	zoomHeight = 0.5;
	bitmap.LoadBitmap(IDB_BMP_LOGO);

	VERIFY(bitmap.GetObject(sizeof(BITMAP), (LPVOID)&bm));
	dcMem.CreateCompatibleDC(NULL);
	CBitmap* pOldBitmap = dcMem.SelectObject(&bitmap);

	//	dc->StretchBlt(0,0,rt.Width(),rt.Height(),&dcMem,0,0,bm.bmWidth,bm.bmHeight,SRCCOPY);

	for (register int nX = rt.left; nX < rt.right; nX += (bm.bmWidth * zoomWidth))
		for (register int nY = rt.top; nY < rt.bottom; nY += (bm.bmHeight * zoomHeight))
			dc->StretchBlt(nX, nY, bm.bmWidth * zoomWidth, bm.bmHeight * zoomHeight, &dcMem, 0, 0, bm.bmWidth, bm.bmHeight, SRCAND);
//			dc->BitBlt(nX, nY, bm.bmWidth * zoomWidth, bm.bmHeight * zoomHeight, &dcMem, 0, 0, SRCAND);
*/}

void MVC::Item::CItemGrid::SelectAllItem()
{
	if(GetNumberRows() < 2)		return;
	SelectRowAt(0);
	m_GI->m_multiSelect->ClearAll();
	m_GI->m_multiSelect->StartBlock(0, 0);
	m_GI->m_multiSelect->EndBlock(GetNumberCols() - 1, GetNumberRows() - 2);
	RedrawAll();
}

//!< 修改变量
void MVC::Item::CItemGrid::ItemEdit()
{
	long row = GetCurrentRow();
	CUGCell cell;
	GetCell(0, row, &cell);
	std::shared_ptr<CItem> item = CItemMgr::GetMe().GetItem(cell.GetParam());//itemName);
	if(!item)					return;

	//SelectRowAt(row);									//!< 修改当前选中的变量，其他变量都取消选中
	//RedrawAll();
	std::shared_ptr<CItem> oldItem = std::shared_ptr<CItem>(new CItem(item->getName()));
	*oldItem = *item;
	EditItem(item);
	//SetFocus();
	//RedrawAll();
	if(*oldItem == *item)		return;					//!< 相等表示没改
	CItemMgr::GetMe().m_mpItem[oldItem->getName()].reset();
	CItemMgr::GetMe().m_mpItem[item->getName()] = item;

	//!< 添加撤销对象
	std::shared_ptr<SItemUndo> undo = std::shared_ptr<SItemUndo>(new SItemUndo);
	undo->m_Item = oldItem;
	undo->m_uiEditType = CGbl::UNDO_TYPE_UPD;
	undo->m_bEnd = true;
	CItemDoc *pDoc = (CItemDoc *)((CItemView *)GetParent())->GetDocument();
	pDoc->AddUndoMember(undo);
	SelectRowAt(row);									//!< 修改当前选中的变量，其他变量都取消选中
	SetFocus();
	RedrawAll();
}

//!< 统一修改变量的属性
void MVC::Item::CItemGrid::ItemEditAll()
{
	CUGCell cell;
	std::shared_ptr<CItem> item;
	std::list<std::shared_ptr<CItem> > ltItem;		//!< 所有被选中的变量
	std::vector<std::shared_ptr<CItem> > vtItem;		//!< 所有被选中的变量
	std::vector<UINT> vtRow;							//!< 被选中的行
	CItemMgr* itemMgr = &CItemMgr::GetMe();
	UINT numRow = GetNumberRows() - 1;

	for(UINT i = 0; i < numRow; ++i)
	{
		if(IsSelected(0, i)){
			GetCell(0, i, &cell);
			item = itemMgr->GetItem(cell.GetParam());
			ASSERT(item);
			ltItem.push_back(item);
			vtItem.push_back(item);
			vtRow.push_back(i);
		}
	}

	//!< 备份一下原有的变量
	std::vector<std::shared_ptr<CItem> > vtOldItem;
	std::shared_ptr<CItem> oldItem;
	for (auto item : vtItem)
	{
		if(!item)			continue;
		oldItem = std::shared_ptr<CItem>(new CItem(item->getName()));
		*oldItem = *item;
		vtOldItem.push_back(oldItem);
	}

	//!< 开始编辑所有
	EditItems(ltItem);

	std::shared_ptr<SItemUndo> undo;
	CItemDoc* pDoc = (CItemDoc *)((CItemView *)GetParent())->GetDocument();
	UINT size = (UINT)vtItem.size();
	for(UINT i = 0; i < size; ++i)
	{
		if(*vtItem[i] == *vtOldItem[i])		continue;		//!< 如果此变量没有改变就不用刷新该行

		//!< 添加撤销对象
		undo = std::shared_ptr<SItemUndo>(new SItemUndo);
		undo->m_Item = vtOldItem[i];
		undo->m_uiEditType = CGbl::UNDO_TYPE_UPD;
		pDoc->AddUndoMember(undo);
	}
	pDoc->SetUndoEnd();
	RedrawAll();
}

//!< 如果没找到，返回-1
long CItemGrid::GetItemRow(UINT id)
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

UINT CItemGrid::GetRowItem(long row)
{
	if(row < 0 || row >= GetNumberRows() - 1)	return UINT(-1);
	CUGCell cell;
	GetCell(0, row, &cell);
	return (UINT)cell.GetParam();
}

//!< 刷新这些变量
void CItemGrid::FreshItemRows(std::list<UINT> ltItemID)
{
	if(ltItemID.empty())								return;
	UINT id, rowID;
	long row, selRow = 0;
	CItemMgr* itemMgr = &CItemMgr::GetMe();
	EnableUpdate(FALSE);
	long rowCount = GetNumberRows();
	CUGCell cell;
	std::map<UINT, CString> mpFreshIDExist;
	for (UINT id : ltItemID)	mpFreshIDExist[id] = _T("e");

	if(m_uiShowType == 0)
	{
		for(row = rowCount - 2; row >= 0; --row)
		{
			GetCell(0, row, &cell);
			rowID = cell.GetParam();
			if(mpFreshIDExist[rowID] == _T("e"))		//!< 如果找到，也就是说需要刷新
			{
				ltItemID.remove(rowID);					//!< 先把这项删了
				if(itemMgr->GetItem(rowID))				InitItemOne(rowID, row);
				else									DeleteRow(row);
				selRow = row;
			}
		}
		for (auto id : ltItemID)							//!< 剩下的都是要增加的
		{
			if(!itemMgr->GetItem(id))					continue;
			row = GetNumberRows() - 1;
			InsertRow(row);
			InitItemOne(id, row);
			selRow = row;
		}
	}
	else if(m_uiShowType == 3)
	{
		std::shared_ptr<CItemGroup> group = itemMgr->GetGroup(m_uiCulGroupID);
		for(row = rowCount - 2; row >= 0; --row)
		{
			GetCell(0, row, &cell);
			rowID = cell.GetParam();
			if(mpFreshIDExist[rowID] == _T("e"))						//!< 如果找到，也就是说需要刷新
			{
				ltItemID.remove(rowID);									//!< 先把这项删了
				std::shared_ptr<CItem> item = itemMgr->GetItem(rowID);
				if(!item || item->getMyGroupID() !=	group->getID())		DeleteRow(row);
				else													InitItemOne(rowID, row);
				selRow = row;
			}
		}
		for (auto id : ltItemID)											//!< 剩下的都是要增加的
		{
			if(!itemMgr->GetItem(id))									continue;
			if(itemMgr->GetItem(id)->getMyGroupID() != m_uiCulGroupID)	continue;
			row = GetNumberRows() - 1;
			InsertRow(row);
			InitItemOne(id, row);
			selRow = row;
		}
	}
	EnableUpdate(TRUE);
	RedrawAll();
	if(selRow < 0)					selRow = 0;
	if(selRow >= GetNumberRows())	selRow = GetNumberRows() - 1;
	SelectRowAt(selRow);
}

//!< 获得浮点字符串
CString CItemGrid::GetFloatStr(float flt)
{
	UINT width = SoftInfo::CSoftInfo::GetMe().getItemFloatWidth();
	CString strFormat;
	strFormat.Format("%d", width);
	strFormat = _T("\%.") + strFormat + _T("f");
	CString strRet;
	strRet.Format(strFormat, flt);
	return strRet;
}
