#include "StdAfx.h"
#include "PMApp.h"
#include "Gbl.h"
#include "SoftInfo.h"
#include "PropertyItems.h"
#include "ProjectMgr.h"
#include "Project.h"

#include "Item.h"
#include "SourceProperty.h"
#include "ItemMgr.h"
#include "ItemGroup.h"
#include "ScriptEditDlg.h"

const CString ITEM_TITLE = _T("变量属性统一修改");

const CString ITEM_BASE_INFO = _T("变量基本信息");
const CString ITEM_BASE_INFO_TOOLTIP = _T("变量基本信息");
const CString ITEM_VALTYPE = _T("变量值类型");
const CString ITEM_VALTYPE_TOOLTIP = _T("变量值类型");
const CString ITEM_SRCTYPE = _T("数据源类型");
const CString ITEM_SRCTYPE_TOOLTIP = _T("数据源类型");
const CString ITEM_GROUP = _T("所属变量组");
const CString ITEM_GROUP_TOOLTIP = _T("所属变量组");
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
const CString SRC_FRESHTIME = _T("变量刷新时间,单位1ms");
const CString SRC_FRESHTIME_TOOLTIP = _T("变量刷新时间,单位1ms");
const CString SRC_CONVERT = _T("转换类型");
const CString SRC_CONVERT_TOOLTIP = _T("转换类型，仅对IO变量有效");
const CString SRC_MINPROJ = _T("最小工程值");
const CString SRC_MINPROJ_TOOLTIP = _T("最小工程值，仅对IO变量有效");
const CString SRC_MAXPROJ = _T("最大工程值");
const CString SRC_MAXPROJ_TOOLTIP = _T("最大工程值，仅对IO变量有效");
const CString SRC_MINIO = _T("最小原始值");
const CString SRC_MINIO_TOOLTIP = _T("最小原始值，仅对IO变量有效");
const CString SRC_MAXIO = _T("最大原始值");
const CString SRC_MAXIO_TOOLTIP = _T("最大原始值，仅对IO变量有效");

const UINT ITEM_VALTYPE_ID = 1;
const UINT ITEM_SRCTYPE_ID = ITEM_VALTYPE_ID + 1;
const UINT ITEM_GROUP_ID = ITEM_SRCTYPE_ID + 1;
const UINT ITEM_ACCESSRIGHT_ID = ITEM_GROUP_ID + 1;
const UINT ITEM_DEFAULTVALUE_ID = ITEM_ACCESSRIGHT_ID + 1;
const UINT ITEM_RESERVE_ID = ITEM_DEFAULTVALUE_ID + 1;
const UINT ITEM_RESFDB_ID = ITEM_RESERVE_ID + 1;
const UINT ITEM_MODBUS485_ID = ITEM_RESFDB_ID + 1;

const UINT SRC_FRESHTIME_ID = 101;
const UINT SRC_CONVERT_ID = SRC_FRESHTIME_ID + 1;
const UINT SRC_MINPROJ_ID = SRC_CONVERT_ID + 1;
const UINT SRC_MAXPROJ_ID = SRC_MINPROJ_ID + 1;
const UINT SRC_MINIO_ID = SRC_MAXPROJ_ID + 1;
const UINT SRC_MAXIO_ID = SRC_MINIO_ID + 1;

using namespace MVC;
using namespace Item;
CPropertyItems::CPropertyItems(void)
{
	m_ShowItem = std::shared_ptr<CItem>(new CItem(_T("")));
}

CPropertyItems::~CPropertyItems(void)
{
}

// 获得本窗体的名称
CString CPropertyItems::GetTitle()
{
	return ITEM_TITLE;
}

// 显示表格的函数
void CPropertyItems::ShowInfo(CXTPPropertyGrid& grid)
{
	CreateEdit();

	CXTPPropertyGridItem* pGroup;					// 属性组
	std::shared_ptr<XmlInfo::CXmlMgr> xmlMgr = ((CPMApp *)AfxGetApp())->m_XmlMgr;		// 描述信息
	CItemMgr* itemMgr = &CItemMgr::GetMe();
	std::list<CString> strList, boolList;			// 下拉列表的内容
	boolList.push_back(_T("No"));
	boolList.push_back(_T("Yes"));
	CComVariant cvr = _T("");
	std::shared_ptr<CPropertySource> showSrc = m_ShowItem->getSrcInfo();

	// 变量基本信息
	pGroup = grid.AddCategory(ITEM_BASE_INFO);
	pGroup->SetTooltip(ITEM_BASE_INFO_TOOLTIP);
	pGroup->Expand();

	// 工程值类型
	strList.clear();
	strList.push_back(_T("位"));
	strList.push_back(_T("有符号字节"));
	strList.push_back(_T("无符号字节"));
	strList.push_back(_T("有符号字"));
	strList.push_back(_T("无符号字"));
	strList.push_back(_T("有符号双字"));
	strList.push_back(_T("无符号双字"));
	strList.push_back(_T("单精度浮点"));
//	strList.push_back(_T("双精度浮点"));
	if(m_bValType)
		AddItemList(*pGroup, ITEM_VALTYPE, ITEM_VALTYPE_TOOLTIP, strList, m_ShowItem->getValType(), ITEM_VALTYPE_ID);
	else
		AddItemList(*pGroup, ITEM_VALTYPE, ITEM_VALTYPE_TOOLTIP, strList, -1, ITEM_VALTYPE_ID);

	// 数据源类型
	strList.clear();
	strList.push_back(_T("内存变量"));
	strList.push_back(_T("I/O变量"));
	if(m_bSrcType)
		AddItemList(*pGroup, ITEM_SRCTYPE, ITEM_SRCTYPE_TOOLTIP, strList, m_ShowItem->getSrcType(), ITEM_SRCTYPE_ID);
	else
		AddItemList(*pGroup, ITEM_SRCTYPE, ITEM_SRCTYPE_TOOLTIP, strList, -1, ITEM_SRCTYPE_ID);

	// 所属变量组
	strList.clear();
	CString myGroupName = m_ShowItem->GetGroupName();
	std::shared_ptr<CItemGroup> group;
	int dftGroup = 0;
	for (auto group : itemMgr->m_vtItemGroup)
	{
		if(!group)			continue;
		if(group->getName() == myGroupName)		dftGroup = strList.size();
		strList.push_back(group->getName());
	}
	if(m_bGroup)
		AddItemList(*pGroup, ITEM_GROUP, ITEM_GROUP_TOOLTIP, strList, dftGroup, ITEM_GROUP_ID);
	else
		AddItemList(*pGroup, ITEM_GROUP, ITEM_GROUP_TOOLTIP, strList, -1, ITEM_GROUP_ID);

	// 访问权限
	strList.clear();
	strList.push_back(_T("可读可写"));
	strList.push_back(_T("只读"));
	strList.push_back(_T("只写"));
	if(m_bAccessRight)
		AddItemList(*pGroup, ITEM_ACCESSRIGHT, ITEM_ACCESSRIGHT_TOOLTIP, strList, m_ShowItem->getAccessRight(), ITEM_ACCESSRIGHT_ID);
	else
		AddItemList(*pGroup, ITEM_ACCESSRIGHT, ITEM_ACCESSRIGHT_TOOLTIP, strList, -1, ITEM_ACCESSRIGHT_ID);

	// 默认值类型
	AddItemVariant(*pGroup, ITEM_DEFAULTVALUE, ITEM_DEFAULTVALUE_TOOLTIP, m_ShowItem->getDefault(), ITEM_DEFAULTVALUE_ID, !m_bDefValue);

	// 是否是保留值
	if(m_bReservFlag)
		AddItemList(*pGroup, ITEM_RESERVE, ITEM_RESERVE_TOOLTIP, boolList, m_ShowItem->getReservFlag()?1:0, ITEM_RESERVE_ID);
	else
		AddItemList(*pGroup, ITEM_RESERVE, ITEM_RESERVE_TOOLTIP, boolList, -1, ITEM_RESERVE_ID);

	// 是否是保留数据库
	if(m_bReservDB)
		AddItemList(*pGroup, ITEM_RESFDB, ITEM_RESFDB_TOOLTIP, boolList, m_ShowItem->getReservDB()?1:0, ITEM_RESFDB_ID);
	else
		AddItemList(*pGroup, ITEM_RESFDB, ITEM_RESFDB_TOOLTIP, boolList, -1, ITEM_RESFDB_ID);

	// 数据源信息
	pGroup = grid.AddCategory(SRC_INFO);
	pGroup->SetTooltip(SRC_INFO_TOOLTIP);
	pGroup->Expand();

	// 刷新时间
	AddItemNumber(*pGroup, SRC_FRESHTIME, SRC_FRESHTIME_TOOLTIP, showSrc->getFreshTime(), SRC_FRESHTIME_ID, !m_bFreshTime);

	// 转换类型
	strList.clear();
	strList.push_back(_T("不转换"));
	strList.push_back(_T("线性转换"));
	if(m_bConvertFlag)
		AddItemList(*pGroup, SRC_CONVERT, SRC_CONVERT_TOOLTIP, strList, showSrc->getConvertType(), SRC_CONVERT_ID);
	else
		AddItemList(*pGroup, SRC_CONVERT, SRC_CONVERT_TOOLTIP, strList, -1, SRC_CONVERT_ID);

	// 最小工程值
	AddItemVariant(*pGroup, SRC_MINPROJ, SRC_MINPROJ_TOOLTIP, showSrc->getProjMin(), SRC_MINPROJ_ID, !m_bMinProj);

	// 最大工程值
	AddItemVariant(*pGroup, SRC_MAXPROJ, SRC_MAXPROJ_TOOLTIP, showSrc->getProjMax(), SRC_MAXPROJ_ID, !m_bMaxProj);

	// 最小原始值
	AddItemVariant(*pGroup, SRC_MINIO, SRC_MINIO_TOOLTIP, showSrc->getIOMin(), SRC_MINIO_ID, !m_bMinIO);

	// 最大原始值
	AddItemVariant(*pGroup, SRC_MAXIO, SRC_MAXIO_TOOLTIP, showSrc->getIOMax(), SRC_MAXIO_ID, !m_bMaxIO);
}

// 编辑前的数据准备方法
void CPropertyItems::CreateEdit()
{
	// 设置默认，都相同
	m_bValType = true;			// 工程值类型是否相同
	m_bSrcType = true;			// 数据源类型是否相同
	m_bGroup = true;			// 所属变量组是否相同
	m_bDefValue = true;			// 默认值类型是否相同
	m_bAccessRight = true;		// 访问权限是否相同
	m_bReservFlag = true;		// 是否是保留值是否相同
	m_bReservDB = true;			// 是否是保留数据库是否相同
	m_bFreshTime = true;		// 刷新时间是否相同
	m_bConvertFlag = true;		// 转换类型是否相同
	m_bMinProj = true;			// 最小工程值是否相同
	m_bMaxProj = true;			// 最大工程值是否相同
	m_bMinIO = true;			// 最小原始值是否相同
	m_bMaxIO = true;			// 最大原始值是否相同

	// 比较每一个，找到哪些属性一样，哪些属性不一样
	*m_ShowItem = *m_ltEditItem.front();
	std::shared_ptr<CItem> item, front;
	front = m_ltEditItem.front();
	std::shared_ptr<CPropertySource> showSrc, itemSrc;
	showSrc = m_ShowItem->getSrcInfo();
	ASSERT(showSrc);
	for (auto item : m_ltEditItem)
	{
		if(item == front)		continue;
		itemSrc = item->getSrcInfo();
		ASSERT(itemSrc);
		// 开始进行匹配
		if(m_bValType)			m_bValType = (m_ShowItem->getValType() == item->getValType());
		if(m_bSrcType)			m_bSrcType = (m_ShowItem->getSrcType() == item->getSrcType());
		if(m_bGroup)			m_bGroup = (m_ShowItem->getMyGroupID() == item->getMyGroupID());
		if(m_bDefValue)			m_bDefValue = (m_ShowItem->getDefault() == item->getDefault());
		if(m_bAccessRight)		m_bAccessRight = (m_ShowItem->getAccessRight() == item->getAccessRight());
		if(m_bReservFlag)		m_bReservFlag = (m_ShowItem->getReservFlag() == item->getReservFlag());
		if(m_bReservDB)			m_bReservDB = (m_ShowItem->getReservDB() == item->getReservDB());

		if(m_bFreshTime)		m_bFreshTime = (showSrc->getFreshTime() == itemSrc->getFreshTime());
		if(m_bConvertFlag)		m_bConvertFlag = (showSrc->getConvertType() == itemSrc->getConvertType());
		if(m_bMinProj)			m_bMinProj = (showSrc->getProjMin() == itemSrc->getProjMin());
		if(m_bMaxProj)			m_bMaxProj = (showSrc->getProjMax() == itemSrc->getProjMax());
		if(m_bMinIO)			m_bMinIO = (showSrc->getIOMin() == itemSrc->getIOMin());
		if(m_bMaxIO)			m_bMaxIO = (showSrc->getIOMax() == itemSrc->getIOMax());
	}
}

// 要保存被改变的内容
bool CPropertyItems::OnSaveModify(CXTPPropertyGrid& grid)
{
	int num = grid.GetCount();
	CXTPPropertyGridItem* item;
	CString itemValue;
	UINT itemID;
	CComVariant cvr;
	CItemMgr* itemMgr = &CItemMgr::GetMe();
	std::shared_ptr<CItem> pItem = m_ltEditItem.front();
	std::shared_ptr<CPropertySource> itemSrc = pItem->getSrcInfo();
	std::shared_ptr<CPropertySource> showSrc = m_ShowItem->getSrcInfo();
	ASSERT(pItem);
	ASSERT(itemSrc);
	ASSERT(showSrc);

	m_bValType = true;			// 工程值类型是否相同
	m_bSrcType = true;			// 数据源类型是否相同
	m_bGroup = true;			// 所属变量组是否相同
	m_bDefValue = true;			// 默认值类型是否相同
	m_bAccessRight = true;		// 访问权限是否相同
	m_bReservFlag = true;		// 是否是保留值是否相同
	m_bReservDB = true;			// 是否是保留数据库是否相同
	m_bFreshTime = true;		// 刷新时间是否相同
	m_bConvertFlag = true;		// 转换类型是否相同
	m_bMinProj = true;			// 最小工程值是否相同
	m_bMaxProj = true;			// 最大工程值是否相同
	m_bMinIO = true;			// 最小原始值是否相同
	m_bMaxIO = true;			// 最大原始值是否相同

	for(int i = 0; i < num; ++i){
		item = grid.GetItem(i);
		if(!item->IsValueChanged())		continue;			// 只有被修改的才会进行保存，优化了一下
		itemID = item->GetID();
		itemValue = item->GetValue();

		if(ITEM_VALTYPE_ID == itemID){						// 工程值类型
			m_ShowItem->setValType(item->GetConstraints()->GetCurrent());
//			m_bValType = (m_ShowItem->getValType() == pItem->getValType());
			m_bValType = false;
		}
		else if(ITEM_SRCTYPE_ID == itemID){					// 数据源类型
			m_ShowItem->setSrcType(item->GetConstraints()->GetCurrent());
//			m_bSrcType = (m_ShowItem->getSrcType() == pItem->getSrcType());
			m_bSrcType = false;
		}
		else if(ITEM_GROUP_ID == itemID){					// 所属变量组是否相同
			std::shared_ptr<CItemGroup> itemGroup = itemMgr->GetGroup(itemValue.Trim());
			m_ShowItem->setMyGroupID(itemGroup ? itemGroup->getID() : 0);
			m_bGroup = false;
		}
		else if(ITEM_ACCESSRIGHT_ID == itemID){				// 访问权限
			m_ShowItem->setAccessRight(item->GetConstraints()->GetCurrent());
			m_bAccessRight = false;
		}
		else if(ITEM_DEFAULTVALUE_ID == itemID){			// 默认值
			CComVariant cvrDef = item->GetValue();
			cvrDef.ChangeType(m_ShowItem->getDefault().vt);
			m_ShowItem->setDefault(cvrDef);
			m_bDefValue = false;
		}
		else if(ITEM_RESERVE_ID == itemID){					// 是否保留
			m_ShowItem->setReservFlag(item->GetConstraints()->GetCurrent());
			m_bReservFlag = false;
		}
		else if(ITEM_RESFDB_ID == itemID){					// 是否存数据库
			m_ShowItem->setReservDB(item->GetConstraints()->GetCurrent());
			m_bReservDB = false;
		}
		else if(SRC_FRESHTIME_ID == itemID){				// 刷新时间
			showSrc->setFreshTime(((CXTPPropertyGridItemNumber *)item)->GetNumber());
			m_bFreshTime = false;
		}
		else if(SRC_CONVERT_ID == itemID){					// 转换类型
			showSrc->setConvertType(item->GetConstraints()->GetCurrent());
			m_bConvertFlag = false;
		}
		else if(SRC_MINPROJ_ID == itemID){					// 最小工程值
			CComVariant cvrMinProj = item->GetValue();
			cvrMinProj.ChangeType(m_ShowItem->getDefault().vt);
			showSrc->setProjMin(cvrMinProj);
			m_bMinProj = false;
		}
		else if(SRC_MAXPROJ_ID == itemID){					// 最大工程值
			CComVariant cvrMaxProj = item->GetValue();
			cvrMaxProj.ChangeType(m_ShowItem->getDefault().vt);
			showSrc->setProjMax(cvrMaxProj);
			m_bMaxProj = false;
		}
		else if(SRC_MINIO_ID == itemID){					// 最小原始值
			CComVariant cvrMinIO = item->GetValue();
			cvrMinIO.ChangeType(m_ShowItem->getDefault().vt);
			showSrc->setIOMin(cvrMinIO);
			m_bMinIO = false;
		}
		else if(SRC_MAXIO_ID == itemID){					// 最大原始值
			CComVariant cvrMaxIO = item->GetValue();
			cvrMaxIO.ChangeType(m_ShowItem->getDefault().vt);
			showSrc->setIOMax(cvrMaxIO);
			m_bMaxIO = false;
		}
	}

	// 开始修改所有变量的属性
	for (auto pItem : m_ltEditItem)
	{
		ASSERT(pItem);
		itemSrc = pItem->getSrcInfo();
		ASSERT(itemSrc);

		if(!m_bValType)				pItem->setValType(m_ShowItem->getValType());
		if(!m_bSrcType)				pItem->setSrcType(m_ShowItem->getSrcType());
		if(!m_bGroup)				pItem->setMyGroupID(m_ShowItem->getMyGroupID());
		if(!m_bDefValue)			pItem->setDefault(m_ShowItem->getDefault());
		if(!m_bAccessRight)			pItem->setAccessRight(m_ShowItem->getAccessRight());
		if(!m_bReservFlag)			pItem->setReservFlag(m_ShowItem->getReservFlag());
		if(!m_bReservDB)			pItem->setReservDB(m_ShowItem->getReservDB());
		if(!m_bFreshTime)			itemSrc->setFreshTime(showSrc->getFreshTime());
		if(!m_bConvertFlag)			itemSrc->setConvertType(showSrc->getConvertType());
		if(!m_bMinProj)				itemSrc->setProjMin(showSrc->getProjMin());
		if(!m_bMaxProj)				itemSrc->setProjMax(showSrc->getProjMax());
		if(!m_bMinIO)				itemSrc->setIOMin(showSrc->getIOMin());
		if(!m_bMaxIO)				itemSrc->setIOMax(showSrc->getIOMax());

		if(!m_bReservFlag || !m_bReservDB)
			CProjectMgr::GetMe().GetProj()->SetModify(true);
	}
	return true;
}

// 当某一项被修改时
void CPropertyItems::OnItemModify(CXTPPropertyGrid& grid, UINT id)
{
	CXTPPropertyGridItem* item = grid.FindItem(id);
	if(!item)			return;
	CString itemValue = item->GetValue();
}

// 显示帮助信息
void CPropertyItems::OnShowHelp()
{
	SoftInfo::CMyHelp::GetMe().ShowHelp(_T("统一修改"));
}