#include "StdAfx.h"
#include "PMApp.h"
#include "Gbl.h"
#include "ProjectMgr.h"
#include "Project.h"
#include "ItemProperty.h"
#include "Item.h"
#include "SourceProperty.h"
#include "AlarmProperty.h"
#include "ItemMgr.h"
#include "ItemGrid.h"
#include "ItemGroup.h"
#include "ScriptEditDlg.h"
#include "SoftInfo.h"
//#include "Power.h"

#include "DeviceOne.h"
#include "DevMgr.h"

#include "XmlMgr.h"
#include "XmlDevice.h"
#include "XmlArea.h"

const UINT ID_BUTTON_SCRIPT = 1001;

const CString ITEM_TITLE = _T(" 变量属性表");
const CString ITEM_NEW = _T("新建变量");

const CString ITEM_BASE_INFO = _T("变量基本信息");
const CString ITEM_BASE_INFO_TOOLTIP = _T("变量基本信息");
const CString ITEM_NAME = _T("变量名称");
const CString ITEM_NAME_TOOLTIP = _T("变量名称");
const CString ITEM_TAG = _T("变量标签");
const CString ITEM_TAG_TOOLTIP = _T("变量标签");
const CString ITEM_DESCRIPTION = _T("变量备注");
const CString ITEM_DESCRIPTION_TOOLTIP = _T("选择多行输入时，键盘Enter键表示换行，键盘Ctrl+Enter组合键表示确认输入");
const CString ITEM_GROUP = _T("所属变量组");
const CString ITEM_GROUP_TOOLTIP = _T("所属变量组");
const CString ITEM_VALTYPE = _T("变量值类型");
const CString ITEM_VALTYPE_TOOLTIP = _T("变量值类型");
const CString ITEM_SRCTYPE = _T("数据源类型");
const CString ITEM_SRCTYPE_TOOLTIP = _T("数据源类型");
const CString ITEM_ACCESSRIGHT = _T("访问权限");
const CString ITEM_ACCESSRIGHT_TOOLTIP = _T("访问权限");
const CString ITEM_DEFAULTVALUE = _T("默认值");
const CString ITEM_DEFAULTVALUE_TOOLTIP = _T("默认值");
const CString ITEM_RESERVE = _T("是否保留值");
const CString ITEM_RESERVE_TOOLTIP = _T("是否保留值");
const CString ITEM_RESFDB = _T("是否保存历史数据");
const CString ITEM_RESFDB_TOOLTIP = _T("是否保存历史数据");
const CString ITEM_MODBUS485 = _T("是否导出Modbus变量");
const CString ITEM_MODBUS485_TOOLTIP = _T("虚拟串口485服务会对导出的变量提供Modbus协议访问服务");

const CString SRC_INFO = _T("数据源信息");
const CString SRC_INFO_TOOLTIP = _T("数据源信息");
const CString SRC_SCRIPT = _T("内存变量赋值脚本");
const CString SRC_SCRIPT_TOOLTIP = _T("内存变量赋值脚本");
const CString SRC_FRESHTIME = _T("变量刷新时间,单位1ms");
const CString SRC_FRESHTIME_TOOLTIP = _T("变量刷新时间,单位1ms");
const CString SRC_DEVICEID = _T("所属设备");
const CString SRC_DEVICEID_TOOLTIP = _T("所属设备");
const CString SRC_AREAID = _T("区地址");
const CString SRC_AREAID_TOOLTIP = _T("区地址");
const CString SRC_UNITINDEX = _T("寻址偏移");
const CString SRC_UNITINDEX_TOOLTIP = _T("寻址偏移");
const CString SRC_BITINDEX = _T("位偏移");
const CString SRC_BITINDEX_TOOLTIP = _T("位偏移");
const CString SRC_IOTYPE = _T("IO类型");
const CString SRC_IOTYPE_TOOLTIP = _T("IO类型");
const CString SRC_DELAYFLAG = _T("是否允许滞后");
const CString SRC_DELAYFLAG_TOOLTIP = _T("滞后表示读取的是刷新值，不允许滞后表示读取的是存储值");
const CString SRC_CONVERT = _T("转换类型");
const CString SRC_CONVERT_TOOLTIP = _T("转换类型");
const CString SRC_MINPROJ = _T("最小工程值");
const CString SRC_MINPROJ_TOOLTIP = _T("最小工程值");
const CString SRC_MAXPROJ = _T("最大工程值");
const CString SRC_MAXPROJ_TOOLTIP = _T("最大工程值");
const CString SRC_MINIO = _T("最小原始值");
const CString SRC_MINIO_TOOLTIP = _T("最小原始值");
const CString SRC_MAXIO = _T("最大原始值");
const CString SRC_MAXIO_TOOLTIP = _T("最大原始值");

const CString ALARM_INFO = _T("报警属性");
const CString ALARM_INFO_TOOLTIP = _T("报警属性");
const CString ALARM_BITTYPE = _T("开关报警类型");
const CString ALARM_BITTYPE_TOOLTIP = _T("开关报警类型");
const CString ALARM_DEADAREA = _T("限值死区");
const CString ALARM_DEADAREA_TOOLTIP = _T("限值死区");
const CString ALARM_LOLOACTIVE = _T("下下限报警激活");
const CString ALARM_LOLOACTIVE_TOOLTIP = _T("下下限报警激活");
const CString ALARM_LOLOVALUE = _T("下下限报警值");
const CString ALARM_LOLOVALUE_TOOLTIP = _T("下限报警值");
const CString ALARM_LOWACTIVE = _T("下限报警激活");
const CString ALARM_LOWACTIVE_TOOLTIP = _T("下限报警激活");
const CString ALARM_LOWVALUE = _T("下限报警值");
const CString ALARM_LOWVALUE_TOOLTIP = _T("下限报警值");
const CString ALARM_HIGHACTIVE = _T("上限报警激活");
const CString ALARM_HIGHACTIVE_TOOLTIP = _T("上限报警激活");
const CString ALARM_HIGHVALUE = _T("上限报警值");
const CString ALARM_HIGHVALUE_TOOLTIP = _T("上限报警值");
const CString ALARM_HIHIACTIVE = _T("上上限报警激活");
const CString ALARM_HIHIACTIVE_TOOLTIP = _T("上上限报警激活");
const CString ALARM_HIHIVALUE = _T("上上限报警值");
const CString ALARM_HIHIVALUE_TOOLTIP = _T("上上限报警值");
const CString ALARM_AIMACTIVE = _T("目标报警激活");
const CString ALARM_AIMACTIVE_TOOLTIP = _T("目标报警激活");
const CString ALARM_AIMVALUE = _T("目标报警值");
const CString ALARM_AIMVALUE_TOOLTIP = _T("目标报警值");
const CString ALARM_AIMPERCENT = _T("偏差值");
const CString ALARM_AIMPERCENT_TOOLTIP = _T("目标报警偏差值，表示达到目标值上下浮动范围内时报警");
const CString ALARM_AIMDEAD = _T("目标报警死区值");
const CString ALARM_AIMDEAD_TOOLTIP = _T("目标报警死区值");
const CString ALARM_SHIFTACTIVE = _T("变化率报警激活");
const CString ALARM_SHIFTACTIVE_TOOLTIP = _T("最大变化量X，变化时间T秒，每次取值与上一次取值的差的绝对值为△x，\
两次取值的时间差为△t秒，则当且仅当 (△x/△t)>=(X/T) 时报警。");
const CString ALARM_SHIFTDELTA = _T("最大变化量");
const CString ALARM_SHIFTDELTA_TOOLTIP = _T("最大变化量X，变化时间T秒，每次取值与上一次取值的差的绝对值为△x，\
两次取值的时间差为△t秒，则当且仅当 (△x/△t)>=(X/T) 时报警。");
const CString ALARM_SHIFTTIME = _T("变化时间，单位秒");
const CString ALARM_SHIFTTIME_TOOLTIP = _T("最大变化量X，变化时间T秒，每次取值与上一次取值的差的绝对值为△x，\
两次取值的时间差为△t秒，则当且仅当 (△x/△t)>=(X/T) 时报警。");

const UINT ITEM_NAME_ID = 1;
const UINT ITEM_TAG_ID = ITEM_NAME_ID + 1;
const UINT ITEM_DESCRIPTION_ID = ITEM_TAG_ID + 1;
const UINT ITEM_GROUP_ID = ITEM_DESCRIPTION_ID + 1;
const UINT ITEM_VALTYPE_ID = ITEM_GROUP_ID + 1;
const UINT ITEM_SRCTYPE_ID = ITEM_VALTYPE_ID + 1;
const UINT ITEM_ACCESSRIGHT_ID = ITEM_SRCTYPE_ID + 1;
const UINT ITEM_DEFAULTVALUE_ID = ITEM_ACCESSRIGHT_ID + 1;
const UINT ITEM_RESERVE_ID = ITEM_DEFAULTVALUE_ID + 1;
const UINT ITEM_RESFDB_ID = ITEM_RESERVE_ID + 1;
const UINT ITEM_MODBUS485_ID = ITEM_RESFDB_ID + 1;

const UINT SRC_SCRIPT_ID = 101;
const UINT SRC_FRESHTIME_ID = SRC_SCRIPT_ID + 1;
const UINT SRC_DEVICE_ID = SRC_FRESHTIME_ID + 1;
const UINT SRC_AREA_ID = SRC_DEVICE_ID + 1;
const UINT SRC_IOTYPE_ID = SRC_AREA_ID  + 1;
const UINT SRC_UNITINDEX_ID = SRC_IOTYPE_ID + 1;
const UINT SRC_BITINDEX_ID = SRC_UNITINDEX_ID + 1;
const UINT SRC_DELAYFLAG_ID = SRC_BITINDEX_ID + 1;
const UINT SRC_CONVERT_ID = SRC_DELAYFLAG_ID + 1;
const UINT SRC_MINPROJ_ID = SRC_CONVERT_ID + 1;
const UINT SRC_MAXPROJ_ID = SRC_MINPROJ_ID + 1;
const UINT SRC_MINIO_ID = SRC_MAXPROJ_ID + 1;
const UINT SRC_MAXIO_ID = SRC_MINIO_ID + 1;

const UINT ALARM_BITTYPE_ID = 201;
const UINT ALARM_DEADAREA_ID = ALARM_BITTYPE_ID + 1;
const UINT ALARM_LOLOACTIVE_ID = ALARM_DEADAREA_ID + 1;
const UINT ALARM_LOLOVALUE_ID = ALARM_LOLOACTIVE_ID + 1;
const UINT ALARM_LOWACTIVE_ID = ALARM_LOLOVALUE_ID + 1;
const UINT ALARM_LOWVALUE_ID = ALARM_LOWACTIVE_ID + 1;
const UINT ALARM_HIGHACTIVE_ID = ALARM_LOWVALUE_ID + 1;
const UINT ALARM_HIGHVALUE_ID = ALARM_HIGHACTIVE_ID + 1;
const UINT ALARM_HIHIACTIVE_ID = ALARM_HIGHVALUE_ID + 1;
const UINT ALARM_HIHIVALUE_ID = ALARM_HIHIACTIVE_ID + 1;
const UINT ALARM_AIMACTIVE_ID = ALARM_HIHIVALUE_ID + 1;
const UINT ALARM_AIMVALUE_ID = ALARM_AIMACTIVE_ID + 1;
const UINT ALARM_AIMPERCENT_ID = ALARM_AIMVALUE_ID + 1;
const UINT ALARM_AIMDEAD_ID = ALARM_AIMPERCENT_ID + 1;
const UINT ALARM_SHIFTACTIVE_ID = ALARM_AIMDEAD_ID + 1;
const UINT ALARM_SHIFTDELTA_ID = ALARM_SHIFTACTIVE_ID + 1;
const UINT ALARM_SHIFTTIME_ID = ALARM_SHIFTDELTA_ID + 1;

using namespace MVC;
using namespace Item;
MVC::Item::CPropertyItem::CPropertyItem(void)
:m_GroupID(0)		//!< 无论是添加还是修改，都要记录一下变量所属组的ID号
,m_Item(NULL)		//!< 当前所指向的变量，如果是新建，这个为空
,m_DevIDOld(-1)		//!< 记录旧的所属设备节点对应设备的编号,与最新的比较,如果一样就不刷新变量区列表了
{
	m_NewItem = std::shared_ptr<CItem>(new CItem(_T("新建变量")));
	m_ShowItem = std::shared_ptr<CItem>(new CItem(_T("")));
}

MVC::Item::CPropertyItem::~CPropertyItem(void)
{
}

//!< 获得本窗体的名称
CString MVC::Item::CPropertyItem::GetTitle()
{
	if(m_bAdd)			return ITEM_NEW;
	else if(!m_Item)	return _T("");
	return m_Item->getName() + ITEM_TITLE;
}

//!< 显示表格的函数
void MVC::Item::CPropertyItem::ShowInfo(CXTPPropertyGrid& grid)
{
	if(m_bAdd){
		CreateNew();
		grid.SetWindowText(_T("新建变量"));
	}
	else{
		CreateEdit();
		grid.SetWindowText(_T("修改变量"));
	}

	CXTPPropertyGridItem *pGroup, *pItem;			//!< 属性组
	CXTPPropertyGridInplaceButton* pButton;
	std::shared_ptr<XmlInfo::CXmlMgr> xmlMgr = ((CPMApp *)AfxGetApp())->m_XmlMgr;		//!< 描述信息
	std::shared_ptr<CPropertySource> showSrc = m_ShowItem->getSrcInfo();
	std::shared_ptr<CPropertyAlarm> showAlarm = m_ShowItem->getAlarmInfo();
	CItemMgr* itemMgr = &CItemMgr::GetMe();
	ASSERT(showSrc);
	ASSERT(showAlarm);
	std::list<CString> strList, boolList;			//!< 下拉列表的内容
	boolList.push_back(_T("No"));
	boolList.push_back(_T("Yes"));

	//!< 变量基本信息
	pGroup = grid.AddCategory(ITEM_BASE_INFO);
	pGroup->SetTooltip(ITEM_BASE_INFO_TOOLTIP);
	pGroup->Expand();

	//!< 变量名称
	pItem = AddItemText(*pGroup, ITEM_NAME, ITEM_NAME_TOOLTIP, m_ShowItem->getName(), ITEM_NAME_ID);
	if(!m_bAdd && !SoftInfo::CSoftInfo::GetMe().isItemNabled())		pItem->SetReadOnly(TRUE);

	//!< 变量标签
	AddItemText(*pGroup, ITEM_TAG, ITEM_TAG_TOOLTIP, m_ShowItem->getTag(), ITEM_TAG_ID);

	//!< 变量备注
	AddItemMultiText(*pGroup, ITEM_DESCRIPTION, ITEM_DESCRIPTION_TOOLTIP, m_ShowItem->getDescription(), ITEM_DESCRIPTION_ID);

	//!< 所属变量组
	strList.clear();
	std::shared_ptr<CItemGroup> group;
	int dftGroup = 0;
	for (auto group : itemMgr->m_vtItemGroup)
	{
		if(!group)			continue;
		if(group->getID() == m_GroupID)		dftGroup = strList.size();
		strList.push_back(group->getName());
	}
	//CXTPPropertyGridItem* pItemText = AddItemText(*pGroup, ITEM_GROUP, ITEM_GROUP_TOOLTIP, m_ShowItem->GetGroupText(), ITEM_GROUP_ID);
	AddItemList(*pGroup, ITEM_GROUP, ITEM_GROUP_TOOLTIP, strList, dftGroup, ITEM_GROUP_ID);

	//!< 变量值类型
	strList.clear();
	strList.push_back(_T("位(bit)"));
	strList.push_back(_T("有符号字节(char)"));
	strList.push_back(_T("无符号字节(byte)"));
	strList.push_back(_T("有符号字(short)"));
	strList.push_back(_T("无符号字(unsigned short)"));
	strList.push_back(_T("有符号双字(int32)"));
	strList.push_back(_T("无符号双字(unsigned int32)"));
	strList.push_back(_T("单精度浮点(float)"));
	AddItemList(*pGroup, ITEM_VALTYPE, ITEM_VALTYPE_TOOLTIP, strList,m_ShowItem->getValType(), ITEM_VALTYPE_ID);

	//!< 变量数据源类型
	strList.clear();
	strList.push_back(_T("内存变量"));
	strList.push_back(_T("I/O变量"));
	AddItemList(*pGroup, ITEM_SRCTYPE, ITEM_SRCTYPE_TOOLTIP, strList, m_ShowItem->getSrcType(), ITEM_SRCTYPE_ID);

	//!< 变量的访问权限
	strList.clear();
	strList.push_back(_T("可读可写"));
	strList.push_back(_T("只读"));
	strList.push_back(_T("只写"));
	AddItemList(*pGroup, ITEM_ACCESSRIGHT, ITEM_ACCESSRIGHT_TOOLTIP, strList, m_ShowItem->getAccessRight(), ITEM_ACCESSRIGHT_ID);

	//!< 默认值, 根据类型插入不同的item
	AddItemVariant(*pGroup, ITEM_DEFAULTVALUE, ITEM_DEFAULTVALUE_TOOLTIP, m_ShowItem->getDefault(), ITEM_DEFAULTVALUE_ID);

	//!< 是否保留
	AddItemList(*pGroup, ITEM_RESERVE, ITEM_RESERVE_TOOLTIP, boolList, m_ShowItem->getReservFlag()?1:0, ITEM_RESERVE_ID);

	//!< 是否保存历史数据
	AddItemList(*pGroup, ITEM_RESFDB, ITEM_RESFDB_TOOLTIP, boolList, m_ShowItem->getReservDB()?1:0, ITEM_RESFDB_ID);

	//!< 是否导出Modbus变量
	if (SoftInfo::CSoftInfo::GetMe().IsModbus485())
	{
		AddItemList(*pGroup, ITEM_MODBUS485, ITEM_MODBUS485_TOOLTIP, boolList, m_ShowItem->getModbus485()?1:0, ITEM_MODBUS485_ID);
	}

	//!< 数据源信息
	pGroup = grid.AddCategory(SRC_INFO);
	pGroup->SetTooltip(SRC_INFO_TOOLTIP);
	pGroup->Expand();

	//!< 赋值脚本
	grid.SetVariableItemsHeight(TRUE);
	CXTPPropertyGridItem* pItemText = AddItemText(*pGroup, SRC_SCRIPT, SRC_SCRIPT_TOOLTIP, showSrc->getScriptText(), SRC_SCRIPT_ID);
	int lineCount = showSrc->getScriptText().GetLength() * 12 / pItemText->GetValueRect().Width();
	pItemText->SetMultiLinesCount(lineCount);
	pButton = pItemText->GetInplaceButtons()->AddButton(new CXTPPropertyGridInplaceButton(ID_BUTTON_SCRIPT));
	pButton->SetCaption(_T("编辑"));

	//!< 设备名称
	strList.clear();
	MVC::Device::CDevMgr* devMgr = &MVC::Device::CDevMgr::GetMe();
	std::shared_ptr<MVC::Device::CDeviceOne> myDev;		//!< 本变量所属的设备
	int defIndex = showSrc->getDeviceID();
	myDev = devMgr->GetDeviceNameList(strList, defIndex);
	if(myDev)		m_DevIDOld = myDev->getID();
	else			m_DevIDOld = -1;
	AddItemList(*pGroup, SRC_DEVICEID, SRC_DEVICEID_TOOLTIP, strList, defIndex, SRC_DEVICE_ID);

	//!< 区地址
	std::shared_ptr<XmlInfo::CXmlDevice> xmlDev;			//!< 变量所属设备的描述信息
	std::shared_ptr<XmlInfo::CXmlArea> myArea;			//!< 本变量所属的区
	if(myDev)		//!< 如果变量所属设备存在，并且描述也存在了
	{
		xmlDev = myDev->GetXmlInfo();
		defIndex = showSrc->getAreaID();
		if(xmlDev)
		{
			strList.clear();
			myArea = xmlDev->GetAreaNameList(strList, defIndex, true);
			if(!myArea || xmlDev->m_vtArea.empty())			strList.push_back(_T("无"));
			else if (!myArea)								myArea = xmlDev->m_vtArea.front();			//!< 必须保证存在一个

			CString strAreaName = SRC_AREAID;
			CString strAreaTooltip = SRC_AREAID_TOOLTIP;
			//!< Modbus设备,这句代码不是我想写的,领导非逼我写的,郁闷
			if(xmlDev->getDevType() == MVC::Device::CDevMgr::MODBUS_SERIAL ||
				xmlDev->getDevType() == MVC::Device::CDevMgr::MODBUS_UDP ||
				xmlDev->getDevType() == MVC::Device::CDevMgr::MODBUS_TCP){
				strAreaName = _T("操作方式");
				strAreaTooltip = strAreaName;
			}
			AddItemList(*pGroup, SRC_AREAID, SRC_AREAID_TOOLTIP, strList, defIndex, SRC_AREA_ID);
		}
	}
	else
	{
		strList.clear();
		strList.push_back(_T("无"));
		AddItemList(*pGroup, SRC_AREAID, SRC_AREAID_TOOLTIP, strList, 0, SRC_AREA_ID);
	}

	//!< IO类型
	if(myArea)
	{
		strList.clear();
		defIndex = showSrc->getIOType();
		strList = myArea->GetSupportUnitList(defIndex);
		if(strList.empty())		strList.push_back(_T("无"));
		AddItemList(*pGroup, SRC_IOTYPE, SRC_IOTYPE_TOOLTIP, strList, defIndex, SRC_IOTYPE_ID);
	}
	else
	{
		strList.clear();
		strList.push_back(_T("无"));
		AddItemList(*pGroup, SRC_IOTYPE, SRC_IOTYPE_TOOLTIP, strList, 0, SRC_IOTYPE_ID);
	}

	//!< 寻址偏移
	CString areaName = SRC_UNITINDEX;
	if(xmlDev)
	{
		if(xmlDev->getDevType() == MVC::Device::CDevMgr::MODBUS_SERIAL ||
			xmlDev->getDevType() == MVC::Device::CDevMgr::MODBUS_UDP ||
			xmlDev->getDevType() == MVC::Device::CDevMgr::MODBUS_TCP){	//!< Modbus设备,这句代码不是我想写的,领导非逼我写的,郁闷
			areaName = _T("Modbus地址");
		}
	}
	if(myArea)	areaName = areaName + _T("(单位：") + myArea->GetUnitName() + _T(")");
	CString areaTip;
	if(myArea)	areaTip = myArea->GetOperateRange(myArea->GetOperType(showSrc->getIOType()));
	AddItemNumber(*pGroup, areaName, areaTip, showSrc->getUnitIndex(), SRC_UNITINDEX_ID);

	//!< 位偏移
	if(myArea)	areaTip.Format("起始位偏移 %d，终止位偏移 %d",myArea->m_nUnitBitReadStart,myArea->m_nUnitBitReadEnd);
	AddItemNumber(*pGroup, SRC_BITINDEX, areaTip, showSrc->getBitIndex(), SRC_BITINDEX_ID);

	//!< 刷新时间
	AddItemNumber(*pGroup, SRC_FRESHTIME, SRC_FRESHTIME_TOOLTIP, showSrc->getFreshTime(), SRC_FRESHTIME_ID);

	//!< 是否允许滞后
	//AddItemList(*pGroup, SRC_DELAYFLAG, SRC_DELAYFLAG_TOOLTIP, boolList, showSrc->getDelayFlag()?1:0, SRC_DELAYFLAG_ID);

	//!< 转换类型
	strList.clear();
	strList.push_back(_T("不转换"));
	strList.push_back(_T("线性转换"));
//	strList.push_back(_T("平方根转换"));
	AddItemList(*pGroup, SRC_CONVERT, SRC_CONVERT_TOOLTIP, strList, showSrc->getConvertType(), SRC_CONVERT_ID);

	//!< 最小工程值
	AddItemVariant(*pGroup, SRC_MINPROJ, SRC_MINPROJ_TOOLTIP, showSrc->getProjMin(), SRC_MINPROJ_ID);

	//!< 最大工程值
	AddItemVariant(*pGroup, SRC_MAXPROJ, SRC_MAXPROJ_TOOLTIP, showSrc->getProjMax(), SRC_MAXPROJ_ID);

	//!< 最小IO值
	AddItemVariant(*pGroup, SRC_MINIO, SRC_MINIO_TOOLTIP, showSrc->getIOMin(), SRC_MINIO_ID);

	//!< 最大IO值
	AddItemVariant(*pGroup, SRC_MAXIO, SRC_MAXIO_TOOLTIP, showSrc->getIOMax(), SRC_MAXIO_ID);

	//!< 变量报警属性
	pGroup = grid.AddCategory(ALARM_INFO);
	pGroup->SetTooltip(ALARM_INFO_TOOLTIP);
	pGroup->Expand();

	//!< 位变量报警类型
	strList.clear();
	strList.push_back(_T("不报警"));
	strList.push_back(_T("开时报警"));
	strList.push_back(_T("关时报警"));
	strList.push_back(_T("开到关时报警"));
	strList.push_back(_T("关到开时报警"));
	strList.push_back(_T("变化就报警"));
	AddItemList(*pGroup, ALARM_BITTYPE, ALARM_BITTYPE_TOOLTIP, strList, showAlarm->getBitAlarmType(), ALARM_BITTYPE_ID);

	//!< 上下限报警死区值
	AddItemNumber(*pGroup, ALARM_DEADAREA, ALARM_DEADAREA_TOOLTIP, showAlarm->getDeadArea(), ALARM_DEADAREA_ID);

	//!< 下下限报警激活
	AddItemList(*pGroup, ALARM_LOLOACTIVE, ALARM_LOLOACTIVE_TOOLTIP, boolList, showAlarm->getLoloActive()?1:0, ALARM_LOLOACTIVE_ID);

	//!< 下下限报警值
	AddItemVariant(*pGroup, ALARM_LOLOVALUE, ALARM_LOLOVALUE_TOOLTIP, showAlarm->getLoloValue(), ALARM_LOLOVALUE_ID);

	//!< 下限报警激活
	AddItemList(*pGroup, ALARM_LOWACTIVE, ALARM_LOWACTIVE_TOOLTIP, boolList, showAlarm->getLowActive()?1:0, ALARM_LOWACTIVE_ID);

	//!< 下限报警值
	AddItemVariant(*pGroup, ALARM_LOWVALUE, ALARM_LOWVALUE_TOOLTIP, showAlarm->getLowValue(), ALARM_LOWVALUE_ID);

	//!< 上限报警激活
	AddItemList(*pGroup, ALARM_HIGHACTIVE, ALARM_HIGHACTIVE_TOOLTIP, boolList, showAlarm->getHighActive()?1:0, ALARM_HIGHACTIVE_ID);

	//!< 上限报警值
	AddItemVariant(*pGroup, ALARM_HIGHVALUE, ALARM_HIGHVALUE_TOOLTIP, showAlarm->getHighValue(), ALARM_HIGHVALUE_ID);

	//!< 上上限报警激活
	AddItemList(*pGroup, ALARM_HIHIACTIVE, ALARM_HIHIACTIVE_TOOLTIP, boolList, showAlarm->getHihiActive()?1:0, ALARM_HIHIACTIVE_ID);

	//!< 上上限报警值
	AddItemVariant(*pGroup, ALARM_HIHIVALUE, ALARM_HIHIVALUE_TOOLTIP, showAlarm->getHihiValue(), ALARM_HIHIVALUE_ID);

	//!< 目标报警激活
	AddItemList(*pGroup, ALARM_AIMACTIVE, ALARM_AIMACTIVE_TOOLTIP, boolList, showAlarm->getAimActive()?1:0, ALARM_AIMACTIVE_ID);

	//!< 目标值
	AddItemVariant(*pGroup, ALARM_AIMVALUE, ALARM_AIMVALUE_TOOLTIP, showAlarm->getAimValue(), ALARM_AIMVALUE_ID);

	//!< 目标报警偏移比率
	AddItemDouble(*pGroup, ALARM_AIMPERCENT, ALARM_AIMPERCENT_TOOLTIP, showAlarm->getAimPercent(), ALARM_AIMPERCENT_ID);

	//!< 目标报警死区
	AddItemDouble(*pGroup, ALARM_AIMDEAD, ALARM_AIMDEAD_TOOLTIP, showAlarm->getAimDeadPercent(), ALARM_AIMDEAD_ID);

	//!< 变化率报警激活
	AddItemList(*pGroup, ALARM_SHIFTACTIVE, ALARM_SHIFTACTIVE_TOOLTIP, boolList, showAlarm->getShiftActive()?1:0, ALARM_SHIFTACTIVE_ID);

	//!< 报警变化比率
	AddItemDouble(*pGroup, ALARM_SHIFTDELTA, ALARM_SHIFTDELTA_TOOLTIP, showAlarm->getShiftDelta(), ALARM_SHIFTDELTA_ID);

	//!< 取值时间数值
	AddItemNumber(*pGroup, ALARM_SHIFTTIME, ALARM_SHIFTTIME_TOOLTIP, showAlarm->getShiftTime(), ALARM_SHIFTTIME_ID);
}

void MVC::Item::CPropertyItem::OnGridFirstShow(CXTPPropertyGrid& grid)
{
	ShowAndHide(grid);
	EnableAndDisenable(grid);
}

//!< 创建新建的变量信息
void MVC::Item::CPropertyItem::CreateNew()
{
	if(!m_ShowItem)		m_ShowItem = std::shared_ptr<CItem>(new CItem(_T("")));

	//!< 给变量命名，需要等于上一次创建的变量名后边 + 1
	CString strHead;
	if(!m_NewItem)		strHead = _T("新建变量");
	else				strHead = m_NewItem->getName();

	int id = 1;
	//if(CGbl::GetNumberFromString(strHead, id, 1))	++id;
	CGbl::GetNumberFromString(strHead, id, 1);
	CString name;
	CItemMgr* mgr = &CItemMgr::GetMe();
	do{										//!< 这个函数是为了找到一个
		if(id == 0)		name = strHead;
		else{
			name.Format(strHead + "%d",id);
//			name = name;
		}
		if(!mgr->GetItem(name))				break;
	} while(++id);							//!< 这个基本上就是死循环，所以后边不用考虑没找到的问题，一定能找到

	m_NewItem->setItemName(name);
	m_NewItem->setMyGroupID(m_GroupID);
	*m_ShowItem = *m_NewItem;
}

//!< 创建编辑的变量信息
void MVC::Item::CPropertyItem::CreateEdit()
{
	if(!m_ShowItem)		m_ShowItem = std::shared_ptr<CItem>(new CItem(_T("")));
	if(!m_Item)			return;
	*m_ShowItem = *m_Item;
	m_GroupID = m_ShowItem->getMyGroupID();	//!< 记录组号,以便下次添加时能够自动变为该组
}

//!< 对话框前，将变量的信息保存起来
void MVC::Item::CPropertyItem::OnCloseGrid()
{
	if(m_bAdd){			//!< 如果是添加，则记录到m_NewItem中
		*m_NewItem = *m_ShowItem;
	}
	else{				//!< 
	}
}

//!< 在这里初始化那些相互关联的IO属性
void MVC::Item::CPropertyItem::ShowAndHide(CXTPPropertyGrid& grid)
{
	CXTPPropertyGridItem *itemProjType, *itemSrc, *itemDev, *itemArea, *itemIOType, *itemBitlen;
	CXTPPropertyGridItem *tmp;
	itemProjType = grid.FindItem(ITEM_VALTYPE_ID);
	itemSrc = grid.FindItem(ITEM_SRCTYPE_ID);
	itemDev = grid.FindItem(SRC_DEVICE_ID);
	itemArea = grid.FindItem(SRC_AREA_ID);
	itemIOType = grid.FindItem(SRC_IOTYPE_ID);
	itemBitlen = grid.FindItem(SRC_BITINDEX_ID);
	if(!itemProjType || !itemSrc || !itemDev || !itemArea || !itemIOType || !itemBitlen)	return;

	//!< 报警显示哪些
	if(itemProjType->GetConstraints()->GetCurrent() == 0){		//!< 位
		tmp = grid.FindItem(ALARM_BITTYPE_ID);			if(tmp)		tmp->SetHidden(FALSE);	//!< 显示报警位项
		for(int i = ALARM_DEADAREA_ID; i <= ALARM_SHIFTTIME_ID; ++i){
			tmp = grid.FindItem(i);						if(tmp)		tmp->SetHidden(TRUE);	//!< 隐藏报警其它项
		}
	}
	else{
		tmp = grid.FindItem(ALARM_BITTYPE_ID);			if(tmp)		tmp->SetHidden(TRUE);	//!< 隐藏报警位项
		for(int i = ALARM_DEADAREA_ID; i <= ALARM_SHIFTTIME_ID; ++i){
			tmp = grid.FindItem(i);						if(tmp)		tmp->SetHidden(FALSE);	//!< 显示报警其它项
		}
	}

	//!< 数据源影响的内容
	int index = itemSrc->GetConstraints()->GetCurrent();
	if(itemSrc->GetConstraints()->GetCurrent() == 0){			//!< 内存变量
		tmp = grid.FindItem(SRC_SCRIPT_ID);				if(tmp)		tmp->SetHidden(FALSE);	//!< 显示脚本编辑
		for( int i = SRC_DEVICE_ID; i <= SRC_MAXIO_ID; ++i){
			tmp = grid.FindItem(i);						if(tmp)		tmp->SetHidden(TRUE);	//!< 隐藏关于IO的项
		}
	}
	else{														//!< IO变量
		tmp = grid.FindItem(SRC_SCRIPT_ID);				if(tmp)		tmp->SetHidden(TRUE);	//!< 隐藏脚本编辑
		for( int i = SRC_DEVICE_ID; i <= SRC_MAXIO_ID; ++i){
			tmp = grid.FindItem(i);						if(tmp)		tmp->SetHidden(FALSE);	//!< 显示关于IO的项
		}
		//!< 但是转换的方式还收到工程类型的影响
		if(itemProjType->GetConstraints()->GetCurrent() == 0){
			for( int i = SRC_CONVERT_ID; i <= SRC_MAXIO_ID; ++i){
				tmp = grid.FindItem(i);					if(tmp)		tmp->SetHidden(TRUE);	//!< 隐藏关于IO转换的项
			}
		}
		//!< 位偏移是否显示
		MVC::Device::CDevMgr* devMgr = &MVC::Device::CDevMgr::GetMe();
		std::shared_ptr<MVC::Device::CDeviceOne> projDev = devMgr->GetDevice(itemDev->GetValue());
		if(projDev)
		{
			std::shared_ptr<XmlInfo::CXmlDevice> xmlDev = projDev->GetXmlInfo();
			std::shared_ptr<XmlInfo::CXmlArea> xmlArea = xmlDev->getArea(itemArea->GetValue());
			if(xmlArea)
			{
				itemBitlen->SetHidden(TRUE);												//!< 先隐藏位偏移
				int operType = xmlArea->GetOperType(xmlArea->GetSupportType(itemIOType->GetValue()));
				if(operType == 0)	itemBitlen->SetHidden(FALSE);
			}

			//!< Modbus设备不应该有位偏移,这句代码不是我想写的,领导非逼我写的,郁闷
			if(xmlDev->getDevType() == MVC::Device::CDevMgr::MODBUS_SERIAL ||
				xmlDev->getDevType() == MVC::Device::CDevMgr::MODBUS_UDP ||
				xmlDev->getDevType() == MVC::Device::CDevMgr::MODBUS_TCP)	//!< Modbus设备
			{
				itemBitlen->SetHidden(TRUE);
			}
		}
	}
}

//!< 在这里统一使能
void MVC::Item::CPropertyItem::EnableAndDisenable(CXTPPropertyGrid& grid)
{
	CXTPPropertyGridItem *item, *tmp;

	//!< 转换
	item = grid.FindItem(SRC_CONVERT_ID);
	if(item->GetConstraints()->GetCurrent() == 0){
		tmp = grid.FindItem(SRC_MINPROJ_ID);			if(tmp)		tmp->SetReadOnly(TRUE);
		tmp = grid.FindItem(SRC_MAXPROJ_ID);			if(tmp)		tmp->SetReadOnly(TRUE);
		tmp = grid.FindItem(SRC_MINIO_ID);				if(tmp)		tmp->SetReadOnly(TRUE);
		tmp = grid.FindItem(SRC_MAXIO_ID);				if(tmp)		tmp->SetReadOnly(TRUE);
	}
	else{
		tmp = grid.FindItem(SRC_MINPROJ_ID);			if(tmp)		tmp->SetReadOnly(FALSE);
		tmp = grid.FindItem(SRC_MAXPROJ_ID);			if(tmp)		tmp->SetReadOnly(FALSE);
		tmp = grid.FindItem(SRC_MINIO_ID);				if(tmp)		tmp->SetReadOnly(FALSE);
		tmp = grid.FindItem(SRC_MAXIO_ID);				if(tmp)		tmp->SetReadOnly(FALSE);
	}

	//!< 上上限报警
	item = grid.FindItem(ALARM_LOLOACTIVE_ID);
	tmp = grid.FindItem(ALARM_LOLOVALUE_ID);			if(tmp)		tmp->SetReadOnly(!item->GetConstraints()->GetCurrent());

	//!< 上限报警
	item = grid.FindItem(ALARM_LOWACTIVE_ID);
	tmp = grid.FindItem(ALARM_LOWVALUE_ID);				if(tmp)		tmp->SetReadOnly(!item->GetConstraints()->GetCurrent());

	//!< 下限报警
	item = grid.FindItem(ALARM_HIGHACTIVE_ID);
	tmp = grid.FindItem(ALARM_HIGHVALUE_ID);			if(tmp)		tmp->SetReadOnly(!item->GetConstraints()->GetCurrent());

	//!< 下下限报警
	item = grid.FindItem(ALARM_HIHIACTIVE_ID);
	tmp = grid.FindItem(ALARM_HIHIVALUE_ID);			if(tmp)		tmp->SetReadOnly(!item->GetConstraints()->GetCurrent());

	//!< 目标报警
	item = grid.FindItem(ALARM_AIMACTIVE_ID);
	tmp = grid.FindItem(ALARM_AIMVALUE_ID);				if(tmp)		tmp->SetReadOnly(!item->GetConstraints()->GetCurrent());
	tmp = grid.FindItem(ALARM_AIMPERCENT_ID);			if(tmp)		tmp->SetReadOnly(!item->GetConstraints()->GetCurrent());
	tmp = grid.FindItem(ALARM_AIMDEAD_ID);				if(tmp)		tmp->SetReadOnly(!item->GetConstraints()->GetCurrent());

	//!< 变化率报警
	item = grid.FindItem(ALARM_SHIFTACTIVE_ID);
	tmp = grid.FindItem(ALARM_SHIFTDELTA_ID);			if(tmp)		tmp->SetReadOnly(!item->GetConstraints()->GetCurrent());
	tmp = grid.FindItem(ALARM_SHIFTTIME_ID);			if(tmp)		tmp->SetReadOnly(!item->GetConstraints()->GetCurrent());
}

//!< 当改变所属设备时
void MVC::Item::CPropertyItem::OnChangeDev(CXTPPropertyGrid& grid)
{
	CXTPPropertyGridItem *itemDev, *itemArea;
	itemDev = grid.FindItem(SRC_DEVICE_ID);
	itemArea = grid.FindItem(SRC_AREA_ID);
	if(!itemDev || !itemArea)							return;
	if(IfSameArea(itemDev->GetValue(), m_DevIDOld))		return;	//!< 如果变量区完全一样,就不用刷新变量区了

	MVC::Device::CDevMgr* devMgr = &MVC::Device::CDevMgr::GetMe();
	std::shared_ptr<MVC::Device::CDeviceOne> projNewDev = devMgr->GetDevice(itemDev->GetValue());
	std::shared_ptr<XmlInfo::CXmlDevice> xmlDev;
	if(!projNewDev)										return;
	m_DevIDOld = projNewDev->getID();
	xmlDev = projNewDev->GetXmlInfo();							//!< 获得了设备描述

	itemArea->GetConstraints()->RemoveAll();
	if(!projNewDev || !xmlDev){									//!< 如果没有设备，或者这个设备没有找到描述信息
		itemArea->SetValue(_T(""));
	}
	else{														//!< 如果存在这个设备并且有描述信息
		std::shared_ptr<XmlInfo::CXmlArea> xmlArea;
		std::list<CString> strList;
		for (auto xmlArea : xmlDev->m_vtArea){
			if(!xmlArea)								continue;
			strList.push_back(xmlArea->getName());
		}
		strList.sort();		//!< 对区名称排个序
		if(strList.empty())
			strList.push_back(_T("无"));
		for (CString str : strList)
			itemArea->GetConstraints()->AddConstraint(str);
		itemArea->SetValue(_T(""));
		itemArea->SetDefaultValue(_T(""));

		if(xmlDev->getDevType() == 7 || xmlDev->getDevType() == 8 || xmlDev->getDevType() == 9)
		{
//			CString areaName = _T("Modbus地址(单位：") + xmlArea->GetUnitName() + _T(")");
			itemArea->SetCaption(_T("操作方式"));
		}
	}
	OnChangeArea(grid);
}

//!< 当改变所属区时
void MVC::Item::CPropertyItem::OnChangeArea(CXTPPropertyGrid& grid)
{
	CXTPPropertyGridItem *itemDev, *itemArea, *itemIOType, *itemUnit, *itemBit;
	itemDev = grid.FindItem(SRC_DEVICE_ID);
	itemArea = grid.FindItem(SRC_AREA_ID);
	itemIOType = grid.FindItem(SRC_IOTYPE_ID);
	itemUnit = grid.FindItem(SRC_UNITINDEX_ID);
	itemBit = grid.FindItem(SRC_BITINDEX_ID);
	if(!itemDev || !itemArea || !itemIOType)		return;

	MVC::Device::CDevMgr* devMgr = &MVC::Device::CDevMgr::GetMe();
	std::shared_ptr<MVC::Device::CDeviceOne> projDev = devMgr->GetDevice(itemDev->GetValue());
	std::shared_ptr<XmlInfo::CXmlDevice> xmlDev;
	if(projDev)		xmlDev = projDev->GetXmlInfo();
	std::shared_ptr<XmlInfo::CXmlArea> xmlArea;
	if(xmlDev)		xmlArea = xmlDev->getArea(itemArea->GetValue());
	itemIOType->GetConstraints()->RemoveAll();
	if(!projDev || !xmlDev || !xmlArea){				//!< 如果没有设备，或者这个设备没有找到描述信息
		itemIOType->SetValue(_T(""));
	}
	else{												//!< 如果存在这个设备并且有描述信息
		int def = 0;
		std::list<CString> strList = xmlArea->GetSupportUnitList(def);
		for (CString text : strList){
			itemIOType->GetConstraints()->AddConstraint(text);
		}
		itemIOType->SetValue(_T(""));
		itemIOType->SetDefaultValue(_T(""));

		CString name = SRC_UNITINDEX;
		if(xmlDev->getDevType() == 7 || xmlDev->getDevType() == 8 || xmlDev->getDevType() == 9)
		{
			name = _T("Modbus地址");
		}
		name = name + _T("(单位：") + xmlArea->GetUnitName() + _T(")");
		if(itemUnit)	itemUnit->SetCaption(name);
//		xmlArea->GetSupportUnitList()
		name = _T("请选择IO类型");
		itemUnit->SetDescription(name);
		itemUnit->SetTooltip(name);

		name.Format("起始位偏移 %d，终止位偏移 %d",xmlArea->m_nUnitBitReadStart,xmlArea->m_nUnitBitReadEnd);
		if(itemBit){
			itemBit->SetTooltip(name);
			itemBit->SetDescription(name);
		}
	}
	OnChangeIOType(grid);
}

//!< 当改变所属操作类型时
void MVC::Item::CPropertyItem::OnChangeIOType(CXTPPropertyGrid& grid)
{
	CXTPPropertyGridItem *itemDev, *itemArea, *itemOperate, *itemUnit, *itemBitlen;
	itemDev = grid.FindItem(SRC_DEVICE_ID);
	itemArea = grid.FindItem(SRC_AREA_ID);
	itemOperate = grid.FindItem(SRC_IOTYPE_ID);
	itemUnit = grid.FindItem(SRC_UNITINDEX_ID);
	itemBitlen = grid.FindItem(SRC_BITINDEX_ID);
	if(!itemDev || !itemArea || !itemOperate || !itemBitlen)		return;
	itemBitlen->SetHidden(TRUE);			//!< 先把它隐藏起来

	if(itemOperate->GetValue() == _T(""))							return;

	MVC::Device::CDevMgr* devMgr = &MVC::Device::CDevMgr::GetMe();
	std::shared_ptr<MVC::Device::CDeviceOne> projDev = devMgr->GetDevice(itemDev->GetValue());
	std::shared_ptr<XmlInfo::CXmlDevice> xmlDev;
	if(projDev)		xmlDev = projDev->GetXmlInfo();
	std::shared_ptr<XmlInfo::CXmlArea> xmlArea;
	if(xmlDev)		xmlArea = xmlDev->getArea(itemArea->GetValue());
	if(xmlArea){
		int supportUnitCount = xmlArea->GetSupportType(itemOperate->GetValue());
		if(0 == supportUnitCount)	itemBitlen->SetHidden(FALSE);		//!< 0是位变量，所以显示位偏移

		CString areaTip;
		areaTip = xmlArea->GetOperateRange(xmlArea->GetOperType(supportUnitCount));
		itemUnit->SetTooltip(areaTip);
		itemUnit->SetDescription(areaTip);
	}
}

//!< 这个函数专门负责响应用户对变量的修改
void MVC::Item::CPropertyItem::OnItemModify(CXTPPropertyGrid& grid, UINT id)
{
	CXTPPropertyGridItem* item = grid.FindItem(id);
	if(!item)			return;
	CString itemValue = item->GetValue();
	if(id == ITEM_NAME){						// = _T("变量名称");
		//!< 在这里判断不能重名，这个再议
	}
	else if(id == SRC_DEVICE_ID){				// = _T("所属设备");
		OnChangeDev(grid);
	}
	else if(id == SRC_AREA_ID){					// = _T("区");
		OnChangeArea(grid);
	}
	else if(id == SRC_IOTYPE_ID){
		OnChangeIOType(grid);
	}
	else if(id == SRC_FRESHTIME_ID){
		UINT uiVal = ((CXTPPropertyGridItemNumber *)item)->GetNumber();
		if(uiVal < 10)	 ((CXTPPropertyGridItemNumber *)item)->SetNumber(10);
	}

	//!< 死区必须为正数
	if(id == ALARM_DEADAREA_ID){
		int iVal = ((CXTPPropertyGridItemNumber *)item)->GetNumber();
		if(iVal < 0){
			iVal = abs(iVal);
			((CXTPPropertyGridItemNumber *)item)->SetNumber(iVal);
		}
	}
	else if(id == ALARM_AIMDEAD_ID || id == ALARM_AIMPERCENT_ID){
		double fVal = ((CXTPPropertyGridItemDouble*)item)->GetDouble();
		if(fVal < 0){
			fVal = abs(fVal);
			((CXTPPropertyGridItemDouble*)item)->SetDouble(fVal);
		}
	}
	ShowAndHide(grid);
	EnableAndDisenable(grid);
}

bool MVC::Item::CPropertyItem::OnSaveModify(CXTPPropertyGrid& grid)
{
	int num = grid.GetCount();
	CXTPPropertyGridItem* item;
	CString itemValue;
	UINT itemID;
	CComVariant cvr;
	std::shared_ptr<CPropertySource> showSrc = m_ShowItem->getSrcInfo();
	std::shared_ptr<CPropertyAlarm> showAlarm = m_ShowItem->getAlarmInfo();
	CItemMgr* itemMgr = &CItemMgr::GetMe();
	ASSERT(showSrc);
	ASSERT(showAlarm);
	for(int i = 0; i < num; ++i){
		item = grid.GetItem(i);
		if(!item->IsValueChanged())		continue;			//!< 只有被修改的才会进行保存，优化了一下
		itemID = item->GetID();
		itemValue = item->GetValue();

		if(itemID == ITEM_NAME_ID){							// = _T("变量名称");
			if(itemMgr->GetItem(item->GetValue())){
				//AfxMessageBox(_T("名称为 \"") + item->GetValue() + _T("\" 的变量已存在，请重新命名..."), MB_OK | MB_ICONEXCLAMATION);
				MessageBox(grid.GetParent()->GetSafeHwnd(), _T("名称为 \"") + item->GetValue() + _T("\" 的变量已存在，请重新命名..."),
					_T("提示"), MB_OK | MB_ICONEXCLAMATION);
				return false;
			}
			if(itemValue.Trim() == _T("")){
				MessageBox(grid.GetParent()->GetSafeHwnd(), _T("名称不能为空..."), _T("提示"), MB_OK | MB_ICONEXCLAMATION);
				return false;
			}
			if(!IfRightItemName(itemValue.Trim())){
				MessageBox(grid.GetParent()->GetSafeHwnd(), _T("变量名存在非法字符..."), _T("提示"), MB_OK | MB_ICONEXCLAMATION);
				return false;
			}
			m_ShowItem->setItemName(itemValue.Trim());
		}
		else if(itemID == ITEM_TAG_ID){
			m_ShowItem->setTag(itemValue.Trim());			// = _T("变量标签");
		}
		else if(itemID == ITEM_DESCRIPTION_ID){
			m_ShowItem->setDescription(itemValue.Trim());	// = _T("变量备注");
		}
		else if(itemID == ITEM_GROUP_ID){					// = _T("变量组");
			std::shared_ptr<CItemGroup> itemGroup = itemMgr->GetGroup(itemValue.Trim());
			if(!itemGroup)			return false;
			m_ShowItem->setMyGroupID(itemGroup->getID());
			m_GroupID = itemGroup->getID();
		}
		else if(itemID == ITEM_VALTYPE_ID){					// = _T("变量值类型");
			m_ShowItem->setValType(item->GetConstraints()->GetCurrent());
			m_ShowItem->ChangeVarType(m_ShowItem->getValType());
		}
		else if(itemID == ITEM_SRCTYPE_ID){					// = _T("数据源类型");
			m_ShowItem->setSrcType(item->GetConstraints()->GetCurrent());
		}
		else if(itemID == ITEM_ACCESSRIGHT_ID){				// = _T("访问权限");
			m_ShowItem->setAccessRight(item->GetConstraints()->GetCurrent());
		}
		else if(itemID == ITEM_DEFAULTVALUE_ID){			// = _T("默认值");
			CComVariant cvrProj = item->GetValue();
			cvrProj.ChangeType(m_ShowItem->getDefault().vt);
			m_ShowItem->setDefault(cvrProj);
		}
		else if(itemID == ITEM_RESERVE_ID){					// = _T("是否保留值");
			m_ShowItem->setReservFlag(item->GetConstraints()->GetCurrent());
			CProjectMgr::GetMe().GetProj()->SetModify(true);	// 这个必须要影响一下工程文件了
		}
		else if(itemID == ITEM_RESFDB_ID){					// = _T("是否保存历史数据");
			m_ShowItem->setReservDB(item->GetConstraints()->GetCurrent());
			CProjectMgr::GetMe().GetProj()->SetModify(true);	// 这个必须要影响一下工程文件了
		}
		else if(itemID == ITEM_MODBUS485_ID){				// = _T("是否导出Modbus变量");
			m_ShowItem->setModbus485(item->GetConstraints()->GetCurrent());
			CProjectMgr::GetMe().GetProj()->SetModify(true);
		}
		else if(itemID == SRC_SCRIPT_ID){					// = _T("内存变量赋值脚本");
			showSrc->setScriptText(itemValue);
		}
		else if(itemID == SRC_FRESHTIME_ID){				// = _T("变量刷新时间");
			showSrc->setFreshTime(((CXTPPropertyGridItemNumber *)item)->GetNumber());
		}
		else if(itemID == SRC_DEVICE_ID){					// = _T("所属设备");
			MVC::Device::CDevMgr* mgr = &MVC::Device::CDevMgr::GetMe();
			std::shared_ptr<MVC::Device::CDeviceOne> device;
			device = mgr->GetDevice(itemValue);
			if(device)
				showSrc->setDevID(device->getID());
		}
		else if(itemID == SRC_AREA_ID){					// = _T("区地址");
			MVC::Device::CDevMgr* devMgr = &MVC::Device::CDevMgr::GetMe();			//!< 获得工程管理
			std::shared_ptr<MVC::Device::CDeviceOne> projDev;
			projDev = devMgr->GetDevice(showSrc->getDeviceID());	//!< 获得对应的工程设备
			if(!projDev)		{AfxMessageBox(_T("未找到对应设备..."), MB_OK | MB_ICONEXCLAMATION);	return false;}
			std::shared_ptr<XmlInfo::CXmlDevice> xmlDev = projDev->GetXmlInfo();	//!< 通过工程设备类型，获得描述设备
			if(!xmlDev)			{AfxMessageBox(_T("未找到对应设备类型..."), MB_OK | MB_ICONEXCLAMATION);return false;}
			showSrc->setAreaID(xmlDev->GetAreaID(itemValue));
		}
		else if(itemID == SRC_IOTYPE_ID){					// = _T("区操作");
			MVC::Device::CDevMgr* devMgr = &MVC::Device::CDevMgr::GetMe();			//!< 获得工程管理
			std::shared_ptr<MVC::Device::CDeviceOne> projDev;
			projDev = devMgr->GetDevice(showSrc->getDeviceID());	//!< 获得对应的工程设备
			if(!projDev)		{AfxMessageBox(_T("未找到对应设备..."), MB_OK | MB_ICONEXCLAMATION);	return false;}
			std::shared_ptr<XmlInfo::CXmlDevice> xmlDev = projDev->GetXmlInfo();	//!< 通过工程设备类型，获得描述设备
			if(!xmlDev)			{AfxMessageBox(_T("未找到对应设备类型..."), MB_OK | MB_ICONEXCLAMATION);	return false;}
			CString areaName = grid.FindItem(SRC_AREA_ID)->GetValue();
			std::shared_ptr<XmlInfo::CXmlArea> xmlArea = xmlDev->getArea(areaName);
			if(!xmlArea)		{AfxMessageBox(_T("未找到对应变量区..."), MB_OK | MB_ICONEXCLAMATION);	return false;}
			UINT valResult = xmlArea->GetSupportType(itemValue);
			showSrc->setIOType(valResult);
		}
		else if(itemID == SRC_UNITINDEX_ID){				// = _T("寻址偏移");
			showSrc->setUnitIndex(((CXTPPropertyGridItemNumber *)item)->GetNumber());
		}
		else if(itemID == SRC_BITINDEX_ID){					// = _T("位偏移");
			showSrc->setBitIndex(((CXTPPropertyGridItemNumber *)item)->GetNumber());
		}
		else if(itemID == SRC_DELAYFLAG_ID){				// = _T("是否允许滞后");
			showSrc->setDealyFlag(item->GetConstraints()->GetCurrent());
		}
		else if(itemID == SRC_CONVERT_ID){					// = _T("转换类型");
			showSrc->setConvertType(item->GetConstraints()->GetCurrent());
		}
		else if(itemID == SRC_MINPROJ_ID){					// = _T("最小工程值");
			CComVariant cvrMinProj = item->GetValue();
			cvrMinProj.ChangeType(m_ShowItem->getDefault().vt);
			showSrc->setProjMin(cvrMinProj);
		}
		else if(itemID == SRC_MAXPROJ_ID){					// = _T("最大工程值");
			CComVariant cvrMaxProj = item->GetValue();
			cvrMaxProj.ChangeType(m_ShowItem->getDefault().vt);
			showSrc->setProjMax(cvrMaxProj);
		}
		else if(itemID == SRC_MINIO_ID){					// = _T("最小IO值");
			CComVariant cvrMinIO = item->GetValue();
			cvrMinIO.ChangeType(m_ShowItem->getDefault().vt);
			showSrc->setIOMin(cvrMinIO);
		}
		else if(itemID == SRC_MAXIO_ID){					// = _T("最大IO值");
			CComVariant cvrMaxIO = item->GetValue();
			cvrMaxIO.ChangeType(m_ShowItem->getDefault().vt);
			showSrc->setIOMax(cvrMaxIO);
		}
		else if(itemID == ALARM_BITTYPE_ID){				// = _T("开关报警类型");
			showAlarm->setBitAlarmType(item->GetConstraints()->GetCurrent());
		}
		else if(itemID == ALARM_DEADAREA_ID){				// = _T("限值死区");
			showAlarm->setDeadArea(((CXTPPropertyGridItemNumber *)item)->GetNumber());
		}
		else if(itemID == ALARM_LOLOACTIVE_ID){				// = _T("下下限报警激活");
			showAlarm->setLoloActive(item->GetConstraints()->GetCurrent());
		}
		else if(itemID == ALARM_LOLOVALUE_ID){				// = _T("下限报警值");
			showAlarm->setLoloValue((float)((CXTPPropertyGridItemDouble *)item)->GetDouble());
		}
		else if(itemID == ALARM_LOWACTIVE_ID){				// = _T("下限报警激活");
			showAlarm->setLowActive(item->GetConstraints()->GetCurrent());
		}
		else if(itemID == ALARM_LOWVALUE_ID){				// = _T("下限报警值");
			showAlarm->setLowValue((float)((CXTPPropertyGridItemDouble *)item)->GetDouble());
		}
		else if(itemID == ALARM_HIGHACTIVE_ID){				// = _T("上限报警激活");
			showAlarm->setHighActive(item->GetConstraints()->GetCurrent());
		}
		else if(itemID == ALARM_HIGHVALUE_ID){				// = _T("上限报警值");
			showAlarm->setHighValue((float)((CXTPPropertyGridItemDouble *)item)->GetDouble());
		}
		else if(itemID == ALARM_HIHIACTIVE_ID){				// = _T("上上限报警激活");
			showAlarm->setHihiActive(item->GetConstraints()->GetCurrent());
		}
		else if(itemID == ALARM_HIHIVALUE_ID){				// = _T("上上限报警值");
			showAlarm->setHihiValue((float)((CXTPPropertyGridItemDouble *)item)->GetDouble());
		}
		else if(itemID == ALARM_AIMACTIVE_ID){				// = _T("目标报警激活");
			showAlarm->setAimActive(item->GetConstraints()->GetCurrent());
		}
		else if(itemID == ALARM_AIMVALUE_ID){				// = _T("目标报警值");
			showAlarm->setAimValue((float)((CXTPPropertyGridItemDouble *)item)->GetDouble());
		}
		else if(itemID == ALARM_AIMPERCENT_ID){				// = _T("目标报警偏差值");
			showAlarm->setAimPercent((float)((CXTPPropertyGridItemDouble *)item)->GetDouble());
		}
		else if(itemID == ALARM_AIMDEAD_ID){				// = _T("目标报警死区值");
			showAlarm->setAimDeadPercent((float)((CXTPPropertyGridItemDouble *)item)->GetDouble());
		}
		else if(itemID == ALARM_SHIFTACTIVE_ID){			// = _T("变化率报警激活");
			showAlarm->setShiftActive(item->GetConstraints()->GetCurrent());
		}
		else if(itemID == ALARM_SHIFTDELTA_ID){			// = _T("变化率报警变化比率");
			showAlarm->setShiftDelta((float)((CXTPPropertyGridItemDouble *)item)->GetDouble());
		}
		else if(itemID == ALARM_SHIFTTIME_ID){				// = _T("变化率报警取值时间长度");
			showAlarm->setShiftTime(((CXTPPropertyGridItemNumber *)item)->GetNumber());
		}
		item->SetDefaultValue(item->GetValue());
	}
	if(m_bAdd){
		m_bAdd = false;
		grid.SetWindowText(_T("修改变量"));
		if(!m_NewItem)		m_NewItem = std::shared_ptr<CItem>(new CItem(_T("")));
		*m_NewItem = *m_ShowItem;							//!< m_NewItem每次不一样
		std::shared_ptr<CItem> newItem = std::shared_ptr<CItem>(new CItem(_T("")));
		*newItem = *m_ShowItem;
		CItemMgr::GetMe().AddItem(newItem,
			MAX_ITEM_COUNT,
			newItem->getMyGroupID());		//!< 
		m_NewItem->setID(newItem->getID());
	}
	else{
		if(m_Item)
			*m_Item = * m_ShowItem;
	}
	return true;
}

//!< 按钮被按下
void MVC::Item::CPropertyItem::OnButtonClick(CXTPPropertyGrid &grid, UINT btID)
{
	if(ID_BUTTON_SCRIPT == btID)
	{
		Dialog::CScriptEditDlg dlg(m_ShowItem->getSrcInfo()->getScriptText(), m_ShowItem->getName(), m_ShowItem->getDescription());
		int lr = dlg.DoModal();
		if(IDOK == lr)
		{
			m_ShowItem->getSrcInfo()->setScriptText(dlg.m_strScriptText);
			CXTPPropertyGridItem* item = grid.FindItem(SRC_SCRIPT_ID);
			if(item)		item->SetValue(dlg.m_strScriptText);
		}
	}
}

//!< 变量名是否合法
bool MVC::Item::CPropertyItem::IfRightItemName(CString strName)
{
	if(strName.Find('\\') != -1)	return false;
	if(strName.Find('\"') != -1)	return false;
	if(strName.Find('[') != -1)		return false;
	if(strName.Find(']') != -1)		return false;
	return true;
}

//!< 显示帮助
void MVC::Item::CPropertyItem::OnShowHelp()
{
	SoftInfo::CMyHelp::GetMe().ShowHelp(_T("变量属性说明"));
}

//!< 判断这两个设备是否具有完全相同的区
bool MVC::Item::CPropertyItem::IfSameArea(CString name1, UINT id2)
{
	MVC::Device::CDevMgr* devMgr = &MVC::Device::CDevMgr::GetMe();
	std::shared_ptr<MVC::Device::CDeviceOne> devLhs = devMgr->GetDevice(name1);
	std::shared_ptr<MVC::Device::CDeviceOne> devRhs = devMgr->GetDevice(id2);

	if(!devLhs && !devRhs)								return true;		// 都为空
	if(devLhs && !devRhs)								return false;		// 只有一个为空
	if(!devLhs && devRhs)								return false;		// 只有一个为空
	if(devLhs->getDevType() == devRhs->getDevType())	return true;		// 都不空,类型相同

	// 都不空,类型还不同,那就得一个区一个区比较了
	std::shared_ptr<XmlInfo::CXmlDevice> xmlDevLhs, xmlDevRhs;
	xmlDevLhs = devLhs->GetXmlInfo();
	xmlDevRhs = devRhs->GetXmlInfo();
	if(!xmlDevLhs || !xmlDevRhs)						return false;		// 找不到设备描述
	int sizeLth = (int)xmlDevLhs->m_vtArea.size();
	int sizeRth = (int)xmlDevRhs->m_vtArea.size();
	if(sizeLth != sizeRth)								return false;		// 变量区数量不同
	std::shared_ptr<XmlInfo::CXmlArea> xmlAreaLhs, xmlAreaRhs;
	std::vector<std::shared_ptr<XmlInfo::CXmlArea> >::iterator iterLhs = xmlDevLhs->m_vtArea.begin();
	std::vector<std::shared_ptr<XmlInfo::CXmlArea> >::iterator iterRhs = xmlDevRhs->m_vtArea.begin();
	for(; iterLhs != xmlDevLhs->m_vtArea.end(); iterLhs++, iterRhs++)
	{
		xmlAreaLhs = (*iterLhs);
		xmlAreaRhs = (*iterRhs);
		if(!xmlAreaLhs && xmlAreaRhs)					return false;		// 有一个变量区为空
		if(xmlAreaLhs && !xmlAreaRhs)					return false;		// 有一个变量区为空
		if(!xmlAreaLhs && !xmlAreaRhs)					continue;			// 都为空,继续判断下一组
		if(xmlAreaLhs->IsSame(xmlAreaRhs.get()))		continue;			// 相同,继续判断下一组
	}
	return true;															// 最后应该是相同了
}
