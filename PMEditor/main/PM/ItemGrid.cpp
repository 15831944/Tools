// ItemGrid.cpp : ʵ���ļ�
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

const CString NEW_ITEM = _T("�½�����...");
const CString VAL_TYPE[] = {_T("λ����"), _T("�з����ֽ�"), _T("�޷����ֽ�"), _T("�з��ŵ���"), _T("�޷��ŵ���"),
_T("�з���˫��"), _T("�޷���˫��"), _T("�����ȸ���")/*, _T("˫���ȸ���")*/};

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
,m_bHoldToClone(false)					//!< ͨ����갴�½��ж��ظ��Ƶı�־
,m_nHoldToCloneID(-1)					//!< �ȴ���¡�ı���ID
,m_nHoldToCloneRow(-1)					//!< �ȴ���¡�ı����ڵڼ���
,m_nHoleToCloneCurCount(0)				//!< �ȴ���¡�ı����ĵ�ǰ����
{
	//UGXPThemes::UseThemes(false);		//!< ȥ���κ����⣬���һ��ã��ٶ�Ҳ�ܿ��
	UGXPThemes::UseThemes(false);
	UGXPThemes::SetGridStyle(Style1);
	m_mpValType[_T("λ����")] = 0;
	m_mpValType[_T("�з����ֽ�")] = 1;
	m_mpValType[_T("�޷����ֽ�")] = 2;
	m_mpValType[_T("�з��ŵ���")] = 3;
	m_mpValType[_T("�޷��ŵ���")] = 4;
	m_mpValType[_T("�з���˫��")] = 5;
	m_mpValType[_T("�޷���˫��")] = 6;
	m_mpValType[_T("�����ȸ���")] = 7;
	m_ItemFont.CreateFont(13, 0, 0, 0, 400, 0, 0, 0, 0, 0, 0, 0, 0, _T("����"));
	m_HeadFont.CreateFont(13, 0, 0, 0, 900, 0, 0, 0, 0, 0, 0, 0, 0, _T("����"));
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
	for(int i = 0; i < (int)csi->m_vtColInfo.size(); ++i){
		if(csi->m_vtColInfo[i].bShow)
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

void CItemGrid::InitCol()
{
	CUGCell cell;
	int i = 0, index = 0;
	SoftInfo::CSoftInfo* csi = &SoftInfo::CSoftInfo::GetMe();
	GetCell(0, -1, &cell);
	for(i = 0; i < (int)csi->m_vtColInfo.size(); ++i){	//!< ���������ֶ���
		if (!csi->m_vtColInfo[i].bShow)	continue;
		if (i == COL_MODBUS485 && 
			!csi->IsModbus485())		continue;
		if (i == COL_VALUE)		m_nProjCol = index;		//!< ��ס����ֵ���ڵ���
		if (i == COL_IOVALUE)	m_nIOCol = index;		//!< ��סIOֵ���ڵ���
		cell.SetText(csi->m_vtColInfo[i].name);			//!< �����ֶε�����
		cell.SetAlignment(UG_ALIGNCENTER);				//!< ���������ʾ
		m_mpColName[index] = csi->m_vtColInfo[i].name;	//!< ��¼�е�����
		SetColWidth(index, csi->m_vtColInfo[i].width);	//!< �����е�Ĭ�Ͽ��
		SetCell(index++, -1, &cell);
	}
	LockColumns(1);										//!< ��ס��һ���У�������Զ��ʾ
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
		AfxMessageBox("���Ϊ "+error+" �ı���������");
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
		if(csi->m_vtColInfo[COL_NAME].name == colName)							//!< ����
		{
			cell.SetText(item->getName());
			cell.SetAlignment(UG_ALIGNLEFT | UG_ALIGNVCENTER);
			if(bIsIO)		cell.SetBitmap(GetBitmap(0));
			else			cell.SetBitmap(GetBitmap(1));
			SetMyCell(i, row, &cell, id, bAlarm);
		}
		else if(csi->m_vtColInfo[COL_TAG].name == colName)						//!< ��ǩ
		{
			cell.SetText(item->getTag());
			SetMyCell(i, row, &cell, id, bAlarm);
		}
		else if(csi->m_vtColInfo[COL_ID].name == colName)						//!< ���
		{
			cell.SetNumber(item->getID());
			SetMyCell(i, row, &cell, id, bAlarm);
		}
		else if(csi->m_vtColInfo[COL_TYPE].name == colName)						//!< ��ֵ����
		{
			cell.SetText(VAL_TYPE[item->getValType() % 8]);
			SetMyCell(i, row, &cell, id, bAlarm);
		}
		else if(csi->m_vtColInfo[COL_IOTYPE].name == colName)					//!< IOֵ����
		{
			if(bIsIO)							cell.SetText(VAL_TYPE[src->getIOType() % 8]);
			else								cell.SetText(_T(""));
			SetMyCell(i, row, &cell, id, bAlarm);
		}
		else if(csi->m_vtColInfo[COL_SRC].name == colName)						//!< ����Դ����
		{
			if(bIsIO)							cell.SetText(_T("I/O����"));
			else								cell.SetText(_T("�ڴ����"));
			SetMyCell(i, row, &cell, id, bAlarm);
		}
		else if(csi->m_vtColInfo[COL_ACCESS].name == colName)					//!< ����Ȩ��
		{
			if(item->getAccessRight() == 0)		text = _T("�ɶ���д");
			else if(item->getAccessRight() == 1)text = _T("ֻ��");
			else								text = _T("ֻд");
			cell.SetText(text);
			SetMyCell(i, row, &cell, id, bAlarm);
		}
		else if(csi->m_vtColInfo[COL_DESCRIPT].name == colName)					//!< ��ע��Ϣ
		{
			cell.SetText(item->getDescription());
			SetMyCell(i, row, &cell, id, bAlarm);
		}
		else if(csi->m_vtColInfo[COL_VALUE].name == colName)					//!< ����ֵ
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
		else if(csi->m_vtColInfo[COL_GROUP].name == colName)					//!< ������
		{
			std::shared_ptr<CItemGroup> myGroup = CItemMgr::GetMe().GetGroup(item->getMyGroupID());
			ASSERT(myGroup);
			cell.SetText(myGroup->getName());
			SetMyCell(i, row, &cell, id, bAlarm);
		}
		else if(csi->m_vtColInfo[COL_CONVERT].name == colName)					//!< ת��
		{
			text = _T("");
			if(bIsIO && src->getConvertType() != 0)								//!< IO��������������ת��
			{
				CComVariant minProj = src->getProjMin();
				CComVariant maxProj = src->getProjMax();
				CComVariant minIO = src->getIOMin();
				CComVariant maxIO = src->getIOMax();
				minProj.ChangeType(VT_BSTR);
				maxProj.ChangeType(VT_BSTR);
				minIO.ChangeType(VT_BSTR);
				maxIO.ChangeType(VT_BSTR);

				if(src->getConvertType() == 1)		text = _T("����(");
				else if(src->getConvertType() == 2)	text = _T("ƽ����(");
				else								text = _T("����(");
				text = text + CString(minProj.bstrVal) + _T(",") + (CString)maxProj.bstrVal +
					_T(",") + CString(minIO.bstrVal) + _T(",") + (CString)maxIO.bstrVal + _T(")");
			}
			cell.SetText(text);
			SetMyCell(i, row, &cell, id, bAlarm);
		}
		else if(csi->m_vtColInfo[COL_IOVALUE].name == colName)					//!< IOֵ
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
		else if(csi->m_vtColInfo[COL_DESCRIPT].name == colName)					//!< ��ע
		{
			cell.SetText(item->getDescription());
			SetMyCell(i, row, &cell, id, bAlarm);
		}
		else if(csi->m_vtColInfo[COL_SCRIPT].name == colName)					//!< �ű�
		{
			if(bIsIO)						cell.SetText(_T(""));
			else							cell.SetText(src->getScriptText());
			SetMyCell(i, row, &cell, id, bAlarm);
		}
		else if(csi->m_vtColInfo[COL_DEVICE].name == colName)					//!< �豸
		{
			if(device)						cell.SetText(device->getName());
			else							cell.SetText(_T(""));
			SetMyCell(i, row, &cell, id, bAlarm);
		}
		else if(csi->m_vtColInfo[COL_ADDR].name == colName)						//!< ��ַ
		{
			if(device)						cell.SetText(src->GetAreaString());
			else							cell.SetText(_T(""));
			SetMyCell(i, row, &cell, id, bAlarm);
		}
		else if(csi->m_vtColInfo[COL_FRESHTIME].name == colName)				//!< ˢ��ʱ��
		{
			cell.SetNumber(src->getFreshTime());
			SetMyCell(i, row, &cell, id, bAlarm);
		}
		else if(csi->m_vtColInfo[COL_BIT].name == colName)						//!< λ����
		{
			if(item->getValType() != 0)		text = _T("");
			else							text = alarm->GetBitAlarmInfo(false);
			cell.SetText(text);
			SetMyCell(i, row, &cell, id, bAlarm);
		}
		else if(csi->m_vtColInfo[COL_LOLO].name == colName)						//!< ������
		{
			if(alarm->getLoloActive())		cell.SetNumber(alarm->getLoloValue());
			else							cell.SetText(_T(""));
			SetMyCell(i, row, &cell, id, bAlarm);
		}
		else if(csi->m_vtColInfo[COL_LOW].name == colName)						//!< ����
		{
			if(alarm->getLowActive())		cell.SetNumber(alarm->getLowValue());
			else							cell.SetText(_T(""));
			SetMyCell(i, row, &cell, id, bAlarm);
		}
		else if(csi->m_vtColInfo[COL_HIGH].name == colName)						//!< ����
		{
			if(alarm->getHighActive())		cell.SetNumber(alarm->getHighValue());
			else							cell.SetText(_T(""));
			SetMyCell(i, row, &cell, id, bAlarm);
		}
		else if(csi->m_vtColInfo[COL_HIHI].name == colName)						//!< ������
		{
			if(alarm->getHihiActive())		cell.SetNumber(alarm->getHihiValue());
			else							cell.SetText(_T(""));
			SetMyCell(i, row, &cell, id, bAlarm);
		}
		else if(csi->m_vtColInfo[COL_AIM].name == colName)						//!< Ŀ��
		{
			if(alarm->getAimActive())		cell.SetNumber(alarm->getAimValue());
			else							cell.SetText(_T(""));
			SetMyCell(i, row, &cell, id, bAlarm);
		}
		else if(csi->m_vtColInfo[COL_SHIFT].name == colName)					//!< �仯��
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
		else if (csi->m_vtColInfo[COL_REV_DB].name == colName)					//!< ������ʷ����
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
		if(CProjectMgr::GetMe().IsWatch()){			// ���±��������������bAlarmִ��,���Խ���������Ӧ�ú���һ������ֿ�,ȷ��Ч��
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

//!< ˢ�±���ֵ
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

	//!< ����ֵ
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

	//!< IOֵ
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

//!< ɾ��������
void CItemGrid::DeleteRowAll()
{
	UnJoinCells(0,GetNumberRows() - 1);
	SetNumberRows(0);
}

//!< ɾ��������
void CItemGrid::DeleteColAll()
{
	SetNumberCols(0);
}

//!< ˢ��һ�������ֵ
void CItemGrid::FreshData()
{
	std::vector<std::shared_ptr<CItem> > vtRowItem;		//!< �±����кţ�������ÿ������Ӧ�ı���
	int rowCount = GetNumberRows() - 1;						//!< ����ʾ����������
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

	//!< ��õ�ǰҳ��������ʾ�ı���
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
// 	//!< �ж���Щֵ���ı���
// 	bool bNeedFresh = false;
// 	for(long row = 0; row < (int)vtRowItem.size(); ++row)
// 	{
// 		//!< ˢ����Щֵ���ı��
// 		if(vtRowItem[row]->IsValueChange() || vtRowItem[row]->IsIOValueChange()){
// 			bNeedFresh = true;
// 			InitItemValue(vtRowItem[row]->getID(), row);		//!< ֻ�޸�ֵ����������
// 		}
// 	}
// 
// 	if(bNeedFresh){				//!< �����ֵ���ı��ˣ�����Ҫˢһ��
// 		if(m_nProjCol >= 0)		RedrawCol(m_nProjCol);
// 		if(m_nIOCol >= 0)		RedrawCol(m_nIOCol);
// 	}
}

//!< �༭������ͳһ����
void CItemGrid::OnEditEnd()
{
	CItemMgr::GetMe().SetEditEnd();
	SetFocus();
	RedrawAll();
}

//!< ������ı��⣬��������
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

//!< ˫�����
void CItemGrid::OnDClicked(int col,long row,RECT *rect,POINT *point,BOOL processed)
{
	CUGCtrl::OnDClicked(col, row, rect, point, processed);
	if(row < 0)		return;										//!< û�����ݣ��˳�
	CUGCell cell;
	GetCell(0, row, &cell);
	UINT itemID = cell.GetParam();
	CItemView* pView = (CItemView *)GetParent();

// 	if(CProjectMgr::GetMe().IsWatch())
// 	{
// 		WriteValue();											//!< ����֤������Ҫ�޸ı����ļ��ֵ
// 	}
// 	else
// 	{
// 		if(row == GetNumberRows() - 1){							//!< ����֤������Ҫ��������Ԫ��
// 			pView->OnItemAdd();
// 			SelectRowAt(GetNumberRows() - 1);
// 		}else{	pView->OnItemEdit();}							//!< ����֤������Ҫ�޸�ĳ������Ԫ��
// 	}
	if(row == GetNumberRows() - 1){
		pView->OnItemAdd();
		SelectRowAt(GetNumberRows() - 1);
	}
	else if(CProjectMgr::GetMe().IsWatch())	WriteValue();		//!< ����֤������Ҫ�޸ı����ļ��ֵ
	else		pView->OnItemEdit();
	SetFocus();
}

//!< д������ʵʱֵ
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

//!< �༭������������
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

//!< ͳһ�༭������������
void CItemGrid::EditItems(std::list<std::shared_ptr<CItem> > ltItem)
{
	if(ltItem.empty())			{return;}
	if(ltItem.size() == 1)		{ItemEdit();		return;}

	CPropertyItems* pp = &CItemMgr::GetMe().m_ItemPropertys;
	pp->SetItemList(ltItem);
	Dialog::CPropertyDlg* dlg = &Dialog::CPropertyDlg::GetMe();

	dlg->DoModal(*pp);
}

//!< �����±���
std::shared_ptr<CItem> CItemGrid::AddNewItem()
{
	MVC::Item::CItemMgr* itemMgr = &MVC::Item::CItemMgr::GetMe();
	std::shared_ptr<CItem> empty;
	//!< �������ܲ��ܼ��˱���
	int count = MAX_ITEM_COUNT;
	count = count << 1;
	if(itemMgr->GetItemSize() >= (count >> 1))
	{
		CString strError;
		strError.Format("�������������� %d �����޷�����ӱ�����", (count >> 1));
		MessageBox(strError, _T("����"), MB_OK | MB_ICONWARNING);
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

//!< ������¡
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

	//!< ��ӳ�������
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

//!< ����������
void MVC::Item::CItemGrid::OnLButtonDown(int col, long row, RECT *rect, POINT *point, BOOL processed)
{
	// ����Ҫ��Ҫ��¡
	long colLhs, colRhs, rowLhs, rowRhs;
	m_GI->m_multiSelect->GetCurrentBlock((int*)&colLhs, &rowLhs, (int*)&colRhs, &rowRhs);
	if(rowRhs < 0 || rowRhs >= GetNumberRows() - 1)	return;		//!< �����һ����Ч

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
			m_nHoleToCloneCurCount = 0;		//!< �ȴ���¡�ı����ĵ�ǰ����
		}
	}
}

//!< ������̧��
void MVC::Item::CItemGrid::OnLButtonUp(int col, long row, RECT *rect, POINT *point, BOOL processed)
{
	// ����Ҫ��Ҫ��¡
	if (!m_bHoldToClone)			return;
	MVC::Item::CItemMgr* itemMgr = &MVC::Item::CItemMgr::GetMe();

	m_bHoldToClone = false;
	::SetCursor(::LoadCursor(NULL, IDC_ARROW));
	std::shared_ptr<CItem> item = itemMgr->GetItem(m_nHoldToCloneID);
	if (!item)						return;
	if (m_nHoleToCloneCurCount == 0)return;

	bool bAdd = m_nHoleToCloneCurCount > 0 ? true : false;		// ���������ӻ��ǵݼ�

	// ��ʼ���Ӱ�
	int maxID = MAX_ITEM_COUNT;
	std::shared_ptr<MVC::Item::CItem> newItem;
	SoftInfo::CSoftInfo* csi = &SoftInfo::CSoftInfo::GetMe();
	int nCount = m_nHoleToCloneCurCount;						// Ҫ��¡���ٸ�
	if (nCount < 0)	nCount = 0 - nCount;
	std::list<UINT> ltAddID;									// ��¼��������ָ��,һ����Ҫѹ����ջ��
	int nBit = item->getValType() == 0 ? 1 : 0;					// nType=0������1С��
	int nAddrUnit = item->getValType() >= 5 ? 2 : 1;			// ���ڵ���5�Ķ���˫�ֱ���
	for(int i = 0; i < nCount; ++i)
	{
// ���ﻹ�������:
// 		1. ���ӷ������ϵ�����ָ���¡����
// 		2. ���ǲ���Ӧ���ȵ����������Ƿ��¡����������
		newItem = std::shared_ptr<MVC::Item::CItem>(new MVC::Item::CItem(_T("")));
		if (bAdd)
		{
			if(!item->OnCloneMe(*newItem, i + 1, nAddrUnit, nBit))		continue;			//!< �������û��¡�ɹ����Ϳ�¡��һ��
		}
		else
		{
			if(!item->OnCloneMe(*newItem, -1 - i, nAddrUnit, nBit))		continue;			//!< �������û��¡�ɹ����Ϳ�¡��һ��
		}
		if(!itemMgr->AddItem(newItem, maxID, item->getMyGroupID())){
			break;	//!< �������Ͳ��ټ���������
		}
		else{
			ltAddID.push_back(newItem->getID());
		}
	}
// 	SoftInfo::CSoftInfo::GetMe().setCloneNumber(baseNum);
// 	SoftInfo::CSoftInfo::GetMe().setCloneFloat(baseFlt);

	//!< ��ӳ�������
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

//!< ����������������������Ǹ����Ļ�����Ҫ���⴦��
void MVC::Item::CItemGrid::OnLClicked(int col,long row,int updn,RECT *rect,POINT *point,BOOL processed)
{
	if(updn == 1)		//!< ��갴��
	{
		OnLButtonDown(col, row, rect, point, processed);
	}
	else
	{
		OnLButtonUp(col, row, rect, point, processed);
	}
}

//!< �Ҽ����updn��TRUE��ʾdown����FALSE��ʾup
void MVC::Item::CItemGrid::OnRClicked(int col,long row,int updn,RECT *rect,POINT *point,BOOL processed)
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
		CMenu* menu =popMenu.GetSubMenu(0);
		//!< ��������ط����Ҽ�����ֲ�ͬ�Ĳ˵���
		CItemMgr* itemMgr = &CItemMgr::GetMe();
		if(!CProjectMgr::GetMe().IsWatch()){				//!< δ���
			menu->EnableMenuItem(ID_ITEM_WRITE, MF_GRAYED);
		}
		else{												//!< ���
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
		if(row == -1 || row >= GetNumberRows() - 1){		//!< �����һ��
		menu->EnableMenuItem(ID_EDIT_COPY, MF_DISABLED|MF_BYCOMMAND);
		menu->EnableMenuItem(ID_EDIT_CUT, MF_DISABLED|MF_BYCOMMAND);
			menu->EnableMenuItem(ID_ITEM_EDIT, MF_DISABLED|MF_BYCOMMAND);
			menu->EnableMenuItem(ID_VKDELETE, MF_DISABLED|MF_BYCOMMAND);
			menu->EnableMenuItem(ID_ITEM_UP, MF_DISABLED|MF_BYCOMMAND);
			menu->EnableMenuItem(ID_ITEM_DOWN, MF_DISABLED|MF_BYCOMMAND);
			menu->EnableMenuItem(ID_ITEM_CLONE, MF_DISABLED|MF_BYCOMMAND);
			menu->EnableMenuItem(ID_ITEM_EDIT_ALL, MF_DISABLED|MF_BYCOMMAND);
		}
		if(itemMgr->m_ltItemClipBoard.empty()){				//!< ��������û����
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
	else if(*vcKey == 0x41){			//!< �����A
		if(GetAsyncKeyState(VK_CONTROL)){
			CItemView* pView = (CItemView *)GetParent();
			if(pView)	pView->OnSelectAll();
		}
	}
	else if(*vcKey == VK_F1){
		SoftInfo::CMyHelp::GetMe().ShowHelp(_T("����"));
	}
}

//!< ����ƶ��¼�
void MVC::Item::CItemGrid::OnMouseMove(int col,long row,POINT *point,UINT nFlags,BOOL processed)
{
	if(nFlags == 0)		//!< 0��ͨ�ƶ���1�����������ƶ���2�Ҽ��ƶ���16�����ƶ�
	{
		int sCol, eCol;
		long sRow, eRow;
		m_GI->m_multiSelect->GetCurrentBlock(&sCol, &sRow, &eCol, &eRow);
		if(eRow < 0 || eRow >= GetNumberRows() - 1)	return;		//!< ������һ��Ҳ��Ч
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

//!< ��갴ס�ƶ�������Ļ���¼�
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

//!< �����Ҽ��˵�����Ϣ��iItem�ǲ˵����ID��lRow��iCol���Ҽ���λ�ã�����ֵ��
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
		case MENU_FIND:			FindDialog();			break;		//!< ����
		case ID_ITEM_ADD:		pView->OnItemAdd();		break;		//!< ��ӱ���
		case ID_ITEM_EDIT:		pView->OnItemEdit();	break;		//!< �༭����
		case ID_ITEM_DELETE:	pView->OnVkdelete();	break;		//!< �����Ƴ�
		case ID_EDIT_COPY:		pView->OnEditCopy();	break;		//!< ��������
		case ID_EDIT_CUT:		pView->OnEditCut();		break;		//!< ��������
		case ID_EDIT_PASTE:		pView->OnEditPaste();	break;		//!< ����ճ��
		case ID_ITEM_EDIT_ALL:	pView->OnItemEditAll();	break;		//!< ����ͳһ�޸�
		case ID_ITEM_IN:		pView->OnItemIn();		break;		//!< ��������
		case ID_ITEM_UP:		pView->OnItemUp();		break;		//!< ��������
		case ID_ITEM_DOWN:		pView->OnItemDown();	break;		//!< ��������
		case ID_ITEM_CLONE:		pView->OnClone();		break;		//!< ������¡
		case ID_ITEM_OUT:		pView->OnItemOut();		break;		//!< �������е�
		case ID_CURRENT_OUT:	pView->OnCurrentOut();	break;		//!< ������ǰ��
		case ID_SELECT_OUT:		pView->OnSelectOut();	break;		//!< ����ѡ�е�
		case ID_SELECT_ALL:		pView->OnSelectAll();	break;		//!< ȫѡ
		case ID_ITEM_WRITE:		WriteValue();			break;		//!< д�����ֵ
		case ID_SHOW_HEX:		pView->OnShowHex();		break;		//!< ��ʾʮ������
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

//!< �Ը�����ID�ı�����������
bool CItemGrid::SortItem(UINT& lth, UINT& rth)
{
	bool bSort = true;
	CItemMgr* itemMgr = &CItemMgr::GetMe();
	CItem *lItem, *rItem;
	lItem = itemMgr->GetItem(lth).get();
	rItem = itemMgr->GetItem(rth).get();
	SoftInfo::CSoftInfo* csi = &SoftInfo::CSoftInfo::GetMe();
	//!< �ַ����Ƚ�
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
	//!< �б�Ƚ�
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
	//!< ��ֵ�Ƚ�
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

//!< ���򣬱Ƚ�������Ԫ���ڵ����ݵĴ�С
int CItemGrid::OnSortEvaluate(CUGCell* pCell1, CUGCell* pCell2, int iFlags)
{
	//!< ������ô�ţ��½�����һ����������ڱ��ĩβ
	CString text1=(CString)pCell1->GetText();
	CString text2=(CString)pCell2->GetText();
	if(text1 == _T("�½�����...") || text1=="")				return 1;
	else if(text2 == _T("�½�����...") || text2=="")		return -1;

	//!< ����ǴӴ�С���У��Ͱ���������Ԫ�񻥻��˾ͺ���
	if( iFlags & UG_SORT_DESCENDING ){
		CUGCell *ptr = pCell1;
		pCell1 = pCell2;
		pCell2 = ptr;
	}
	COleDateTime date1, date2;
	int retVal = 0;

	CString colName = m_mpColName[m_iSortCol];
	if(SoftInfo::CSoftInfo::GetMe().m_vtColInfo[COL_TYPE].name == colName)			//!< ���ͱȽ�
	{
		retVal = m_mpValType[(CString)pCell1->GetText()] - m_mpValType[(CString)pCell2->GetText()];
	}
	else														//!< �����Ƚ�
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
	if(m_uiShowType == 0)			//!< ���б���
	{
		std::list<UINT> ltItemID;
		std::shared_ptr<CItem> item;
		for (auto item : itemMgr->m_vtItem){
			if(item)				ltItemID.push_back(item->getID());
		}
		ShowItem(ltItemID);
	}
	else if(m_uiShowType == 3)		//!< ��������ı���
	{
		std::shared_ptr<CItemGroup> group = itemMgr->GetGroup(m_uiCulGroupID);
		ShowItem(group->getMyItem());
	}
}

void MVC::Item::CItemGrid::SortGrid()
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

//	std::list<UINT> ltShowItemID;
	ltShowItemID.clear();
	CItemMgr* itemMgr = &CItemMgr::GetMe();
	if(m_uiShowType == 0)			//!< ���б���
	{
		for (auto item : itemMgr->m_vtItem){
			if(item)
				ltShowItemID.push_back(item->getID());
		}
	}
	else if(m_uiShowType == 3)		//!< ��������ı���
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

	//!< ���ԭʼ�ı������ݣ����Ҫѹ����ջ
	std::shared_ptr<SItemUndo> undo1 = std::shared_ptr<SItemUndo>(new SItemUndo(CGbl::UNDO_TYPE_UPD, item1, 0));
	std::shared_ptr<SItemUndo> undo2 = std::shared_ptr<SItemUndo>(new SItemUndo(CGbl::UNDO_TYPE_UPD, item2, 0));

	//!< ��������������
	CItemMgr::GetMe().ExChangeItem(item1->getID(), item2->getID());
	InitItemOne(item2->getID(), row);
	InitItemOne(item1->getID(), row - 1);

	//!< ��ӳ�������
	CItemDoc* pDoc = (CItemDoc *)((CItemView *)GetParent())->GetDocument();
	pDoc->AddUndoMember(undo1);
	pDoc->AddUndoMember(undo2);
	pDoc->SetUndoEnd();

	SelectRowAt(row - 1);										//!< ѡ���½�������һ��
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

	//!< ���ԭʼ�ı������ݣ����Ҫѹ����ջ
	std::shared_ptr<SItemUndo> undo1 = std::shared_ptr<SItemUndo>(new SItemUndo(CGbl::UNDO_TYPE_UPD, item1, 0));
	std::shared_ptr<SItemUndo> undo2 = std::shared_ptr<SItemUndo>(new SItemUndo(CGbl::UNDO_TYPE_UPD, item2, 0));

	//!< ��������������
	CItemMgr::GetMe().ExChangeItem(item1->getID(), item2->getID());
	InitItemOne(item2->getID(), row);
	InitItemOne(item1->getID(), row + 1);

	//!< ��ӳ�������
	CItemDoc* pDoc = (CItemDoc *)((CItemView *)GetParent())->GetDocument();
	pDoc->AddUndoMember(undo1);
	pDoc->AddUndoMember(undo2);
	pDoc->SetUndoEnd();

	SelectRowAt(row + 1);										//!< ѡ���½�������һ��
	RedrawAll();
}

//!< ͳ��һ��Ҫɾ���ı�����������ʾ�Ƿ�Ҫѯ��һ���û�������ʱ��Ҫɾ�����Ͳ���Ҫ֪ͨ�û�
void MVC::Item::CItemGrid::ItemRemove(bool bAsk /* = true */)
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

	CItemMgr* itemMgr = &CItemMgr::GetMe();
	CUGCell cell;
	std::shared_ptr<CItem> item;
	std::list<std::shared_ptr<CItem> > ltSelectItem;

	//!< ͳ�Ʊ�ѡ�еı���
	for (long row : ltSelectRow)
	{
		GetCell(0, row, &cell);
		item = itemMgr->GetItem(cell.GetParam());
		ASSERT(item);
		ltSelectItem.push_back(item);
	}

	ItemRemove(ltSelectItem);				//!< ��ʼɾ��
	m_GI->m_multiSelect->ClearAll();		//!< �������ѡ�����
	SetFocus();

	//!< ��ӳ�������
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

//!< ɾ����Щ����
void MVC::Item::CItemGrid::ItemRemove(std::list<std::shared_ptr<CItem> > ltItem)
{
	CItemMgr* itemMgr = &CItemMgr::GetMe();
	for (auto item : ltItem)
	{
		if (item)
			itemMgr->DeleteItem(item->getID());
//		if(m_uiShowType == 3)		itemMgr->RemoveItem(item->getID(), m_uiCulGroupID);		//!< ������ʾ�У�����ɾ�����еı���
//		else						itemMgr->DeleteItem(item->getID());						//!< ������Ƿ�����ʾ����������������ɾ��
	}
}

//!< ��������
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

//!< ��������
void MVC::Item::CItemGrid::ItemCut()
{
	ItemCopy();			//!< �ȿ�������ɾ��
	ItemRemove(false);
}

//!< ����ճ��
void MVC::Item::CItemGrid::ItemPaste()
{
	CItemMgr* itemMgr = &CItemMgr::GetMe();
	if(itemMgr->m_ltItemClipBoard.empty())						return;
	CString name;
	std::shared_ptr<CItem> item, srcItem;
	std::list<std::shared_ptr<CItem> > ltItem;
	//!< ����ֱ�ӰѼ�������ı���ճ���������У�����Ҫ����һ�ݹ�ȥ
	for (auto srcItem : itemMgr->m_ltItemClipBoard)
	{
		item = std::shared_ptr<CItem>(new CItem(_T("")));
		*item = *srcItem;
		ltItem.push_back(item);
	}
	//!< ���ڿ�ʼճ��
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
	GotoRow(row);		//!< ��ʾ2��û���⣬��ʾ1��ż������ֲ���ʾ�����⣬��������������
}

//!< �������б���
void MVC::Item::CItemGrid::OutItemAll()
{
	CString outText;
	CItemMgr* mgr = &CItemMgr::GetMe();
	if(mgr->GetItemSize() <= 0)		return;
	outText.Format("�Ƿ�Ҫ���� ���� ������(%d ��)",mgr->GetItemSize());
	if(IDYES == AfxMessageBox(outText, MB_YESNO | MB_ICONINFORMATION))
		mgr->OutItem(mgr->GetItemIDAll());
}

//!< ������ǰ���б���
void MVC::Item::CItemGrid::OutItemCurrent()
{
	CString outText;
	CItemMgr* mgr = &CItemMgr::GetMe();
	if(GetNumberRows() <= 1)			return;
	outText.Format("�Ƿ�Ҫ���� ��ǰ��ʾ �����б�����(%d ��)", GetNumberRows() - 1);
	if(IDYES == AfxMessageBox(outText, MB_YESNO | MB_ICONINFORMATION))
	{
		if(m_uiShowType == 0)			mgr->OutItem(mgr->GetItemIDAll());
		else if(m_uiShowType == 1)		mgr->OutItem(mgr->GetItemIDMem());
		else if(m_uiShowType == 2)		mgr->OutItem(mgr->GetItemIDIO());
		else if(m_uiShowType == 3)		mgr->OutItem(mgr->GetGroup(m_uiCulGroupID)->getMyItem());
	}
}

//!< ��������ѡ�б���
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
	outText.Format("�Ƿ�Ҫ���� ��ǰѡ�� �����б�����(%d ��)", (UINT)ltItem.size());
	if(IDYES == AfxMessageBox(outText, MB_YESNO | MB_ICONINFORMATION))
		CItemMgr::GetMe().OutItem(ltItem);
}

//!< �ػ�ѡ�п�ʹ�����Ŀ
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

	//!< ���������
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

	//!< ���ڸ�
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

	//!< ������
	dc->SelectObject(GetStockObject(WHITE_PEN));
	dc->MoveTo(firstColRight - 3, bottom);
	dc->LineTo(firstColRight - 3, bottom - 4);
	dc->LineTo(firstColRight + 3, bottom - 4);
	dc->LineTo(firstColRight + 3, bottom);

	//!< ��ǳ��ɫ���м���
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

//!< �ػ�ѡ�п�ʹ�����Ŀ
void MVC::Item::CItemGrid::OnDrawFocusHodeText(CDC *dc)
{
	if (!m_bHoldToClone)				return;
	if (m_nHoleToCloneCurCount <= 0)	return;

	long startCol, endCol, startRow, endRow, topRow, bottomRow;
	m_GI->m_multiSelect->GetCurrentBlock((int *)&startCol, &startRow, (int *)&endCol, &endRow);
	topRow = GetTopRow();	bottomRow = GetBottomRow();
	if((endRow < topRow) || (startRow > bottomRow))		return;

	// ���ı�
	CPen penHold;
	penHold.CreatePen(PS_SOLID, 1, RGB(255, 0, 0));
	dc->SelectObject(penHold);
	int nCulRow = endRow;
	if (m_nHoleToCloneCurCount < 0)	nCulRow = startRow;
	CRect rectCulRowFirstCol;
	GetRangeRect(startCol, nCulRow, startCol, nCulRow, &rectCulRowFirstCol);
	CString str;
	str.Format("%d��", abs(m_nHoleToCloneCurCount));
	dc->TextOut(rectCulRowFirstCol.right + 8, rectCulRowFirstCol.top + 6, str);

	// ����ѡ��С�߿�
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
	if(CProjectMgr::GetMe().IsWatch())	return;		//!< ���ʱ�����ڿ�
	int dcID = dc->SaveDC();
	OnDrawFocusBackRect(dc);
	if (m_bHoldToClone)
		OnDrawFocusHodeText(dc);
}

//!< ����������ʵ����������ˮӡ�ģ���������Ч�������룬�����Ѿ�����
void MVC::Item::CItemGrid::OnDrawBackBitmap(CDC *dc)
{
	return;
	CRect rt;
	GetClientRect(&rt);
	OnDrawBackBitmap(dc, rt.left, rt.top, rt.right, rt.bottom);
}

//!< ��ӡʱ��ˮӡ��Ŀǰ����ȫû���ϣ���Ϊ��ӡ������Ч��������
//!< ��������Ϊƽʱ��ʾ�ʹ�ӡʱ�ĳߴ絥λ��һ����ɵģ�ƽʱ��ʾ�ĵ�λ�����أ���ӡʱ�����Ǻ��׻��߰���Ӣ��ȵ�λ
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

//!< �޸ı���
void MVC::Item::CItemGrid::ItemEdit()
{
	long row = GetCurrentRow();
	CUGCell cell;
	GetCell(0, row, &cell);
	std::shared_ptr<CItem> item = CItemMgr::GetMe().GetItem(cell.GetParam());//itemName);
	if(!item)					return;

	//SelectRowAt(row);									//!< �޸ĵ�ǰѡ�еı���������������ȡ��ѡ��
	//RedrawAll();
	std::shared_ptr<CItem> oldItem = std::shared_ptr<CItem>(new CItem(item->getName()));
	*oldItem = *item;
	EditItem(item);
	//SetFocus();
	//RedrawAll();
	if(*oldItem == *item)		return;					//!< ��ȱ�ʾû��
	CItemMgr::GetMe().m_mpItem[oldItem->getName()].reset();
	CItemMgr::GetMe().m_mpItem[item->getName()] = item;

	//!< ��ӳ�������
	std::shared_ptr<SItemUndo> undo = std::shared_ptr<SItemUndo>(new SItemUndo);
	undo->m_Item = oldItem;
	undo->m_uiEditType = CGbl::UNDO_TYPE_UPD;
	undo->m_bEnd = true;
	CItemDoc *pDoc = (CItemDoc *)((CItemView *)GetParent())->GetDocument();
	pDoc->AddUndoMember(undo);
	SelectRowAt(row);									//!< �޸ĵ�ǰѡ�еı���������������ȡ��ѡ��
	SetFocus();
	RedrawAll();
}

//!< ͳһ�޸ı���������
void MVC::Item::CItemGrid::ItemEditAll()
{
	CUGCell cell;
	std::shared_ptr<CItem> item;
	std::list<std::shared_ptr<CItem> > ltItem;		//!< ���б�ѡ�еı���
	std::vector<std::shared_ptr<CItem> > vtItem;		//!< ���б�ѡ�еı���
	std::vector<UINT> vtRow;							//!< ��ѡ�е���
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

	//!< ����һ��ԭ�еı���
	std::vector<std::shared_ptr<CItem> > vtOldItem;
	std::shared_ptr<CItem> oldItem;
	for (auto item : vtItem)
	{
		if(!item)			continue;
		oldItem = std::shared_ptr<CItem>(new CItem(item->getName()));
		*oldItem = *item;
		vtOldItem.push_back(oldItem);
	}

	//!< ��ʼ�༭����
	EditItems(ltItem);

	std::shared_ptr<SItemUndo> undo;
	CItemDoc* pDoc = (CItemDoc *)((CItemView *)GetParent())->GetDocument();
	UINT size = (UINT)vtItem.size();
	for(UINT i = 0; i < size; ++i)
	{
		if(*vtItem[i] == *vtOldItem[i])		continue;		//!< ����˱���û�иı�Ͳ���ˢ�¸���

		//!< ��ӳ�������
		undo = std::shared_ptr<SItemUndo>(new SItemUndo);
		undo->m_Item = vtOldItem[i];
		undo->m_uiEditType = CGbl::UNDO_TYPE_UPD;
		pDoc->AddUndoMember(undo);
	}
	pDoc->SetUndoEnd();
	RedrawAll();
}

//!< ���û�ҵ�������-1
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

//!< ˢ����Щ����
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
			if(mpFreshIDExist[rowID] == _T("e"))		//!< ����ҵ���Ҳ����˵��Ҫˢ��
			{
				ltItemID.remove(rowID);					//!< �Ȱ�����ɾ��
				if(itemMgr->GetItem(rowID))				InitItemOne(rowID, row);
				else									DeleteRow(row);
				selRow = row;
			}
		}
		for (auto id : ltItemID)							//!< ʣ�µĶ���Ҫ���ӵ�
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
			if(mpFreshIDExist[rowID] == _T("e"))						//!< ����ҵ���Ҳ����˵��Ҫˢ��
			{
				ltItemID.remove(rowID);									//!< �Ȱ�����ɾ��
				std::shared_ptr<CItem> item = itemMgr->GetItem(rowID);
				if(!item || item->getMyGroupID() !=	group->getID())		DeleteRow(row);
				else													InitItemOne(rowID, row);
				selRow = row;
			}
		}
		for (auto id : ltItemID)											//!< ʣ�µĶ���Ҫ���ӵ�
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

//!< ��ø����ַ���
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
