#include "stdafx.h"
#include "DXPEditor.h"
#include "Gbl.h"
//#include "Power.h"
#include "MainFrm.h"
#include "ProjectMgr.h"
#include "Project.h"
#include "ItemInConfigDlg.h"

#include "Item.h"
#include "SourceProperty.h"
#include "AlarmProperty.h"
#include "ItemGroup.h"
#include "ItemMgr.h"
#include "ItemDoc.h"
#include "ItemView.h"
#include "ItemFrame.h"
#include "FindDlg.h"

using namespace MVC;
using namespace Item;

const CString ITEM_EXPAND_NAME = _T("var");			//!< 扩展名

const CString INFO_HEAD = _T("Infomation");
const CString VERSION = _T("Version");
const CString GROUPS = _T("Group");
const CString ITEM = _T("Item");

const CString ITEM_OUTFILE = _T("ItemOutFile");		//!< 变量表导出的标志

CItemMgr::CItemMgr(void)
	: m_pItemDoc(NULL)
{
	m_vtItem.reserve(50000);
}

CItemMgr::~CItemMgr(void)
{}

CItemMgr& CItemMgr::GetMe()
{
	static CItemMgr me;
	return me;
}

CString CItemMgr::GetEditTime()
{
	if(IsModify())		::GetLocalTime(&m_EditTime);
	return CGbl::GetMe().GetSystemTime2Str(m_EditTime);
}

std::list<UINT> CItemMgr::GetItemIDAll()
{
	std::list<UINT> ltItemID;
	for (std::shared_ptr<CItem> item : m_vtItem)
		if(item)
			ltItemID.push_back(item->getID());

	return ltItemID;
}

std::list<UINT> CItemMgr::GetItemIDMem()
{
	std::list<UINT> ltItemID;
	for (std::shared_ptr<CItem> item : m_vtItem)
		if(item && item->getSrcType() == CItem::SRC_TYPE_MEM)
			ltItemID.push_back(item->getID());

	return ltItemID;
}

std::list<UINT> CItemMgr::GetItemIDIO()
{
	std::list<UINT> ltItemID;
	for (std::shared_ptr<CItem> item : m_vtItem)
		if(item && item->getSrcType() == CItem::SRC_TYPE_IO)
			ltItemID.push_back(item->getID());

	return ltItemID;
}

//!< 获得id号变量
std::shared_ptr<CItem> CItemMgr::GetItem(ULONG id)
{
	std::shared_ptr<CItem> _item;
	if(id >= m_vtItem.size())
		return _item;
	return m_vtItem[id];
}

//!< 得到名字为name的变量元素
std::shared_ptr<CItem> CItemMgr::GetItem(CString name)
{
	for (std::shared_ptr<CItem> item : m_vtItem){
		if(!item)						continue;
		if(item->getName() == name)		return item;
	}
	std::shared_ptr<CItem> _empty;
	return _empty;
}

//!< 获得变量,用快速的获得方式
std::shared_ptr<CItem> CItemMgr::GetItemFast(CString name)
{
	return m_mpItem[name];
}

//!< 获得第一个变量
std::shared_ptr<CItem> CItemMgr::GetFirstItem()
{
	std::vector<std::shared_ptr<CItem> >::iterator iter = m_vtItem.begin();
	for(;iter !=  m_vtItem.end(); ++iter){
		if(*iter)		return *iter;
	}
	std::shared_ptr<CItem> empty;
	return empty;
}

//!< 获得最后一个变量
std::shared_ptr<CItem> CItemMgr::GetLastItem()
{
	std::vector<std::shared_ptr<CItem> >::reverse_iterator iter = m_vtItem.rbegin();
	for(;iter !=  m_vtItem.rend(); ++iter){
		if(*iter)		return *iter;
	}
	std::shared_ptr<CItem> empty;
	return empty;
}

std::shared_ptr<CItemGroup> CItemMgr::GetGroup(UINT id)
{
	std::shared_ptr<CItemGroup> _group;
	if(id >= m_vtItemGroup.size())
		return _group;
	return m_vtItemGroup[id];
}

std::shared_ptr<CItemGroup> CItemMgr::GetGroup(CString name)
{
	for (std::shared_ptr<CItemGroup> _group : m_vtItemGroup){
		if(!_group)							continue;
		if(_group->getName() == name)		return _group;
	}
	std::shared_ptr<CItemGroup> _empty;
	return _empty;
}

//!< 在某个组中添加新变量，组内变量不能重名
//!< 用户添加的变量调用这个函数，别的不调
bool CItemMgr::AddItem(std::shared_ptr<CItem> item,int maxID,UINT groupid)
{
	if(!item)												return false;
	if(!FindGroup(groupid))									return false;
	maxID = maxID >> 1;
	int id = -1;
	for(UINT i = 0; i < m_vtItem.size(); ++i)
	{
		if(m_vtItem[i])										continue;
		if(i >= (maxID << 1))								goto SAY_ERROR;
		id = i;
		break;
		//item->setID(i);										//!< 找到了一个空的位置，把item的编号附上
		//item->setMyGroupID(groupid);
		//m_vtItem[i]=item;
		//m_mpItem[item->getName()] = item;
		//return true;
	}
	if (id == -1)	id = m_vtItem.size();
	//!< 如果前边没有空的位置，则从最后添加
	if (id < (maxID << 1))			//!< 数量不能超过权限值
	{
		item->setID((UINT)id);		//!< 设置变量ID号
		item->setMyGroupID(groupid);
		if (id == m_vtItem.size())	m_vtItem.push_back(item);
		else						m_vtItem[id] = item;
		m_mpItem[item->getName()] = item;
		return true;
	}
SAY_ERROR:
	CString strError;
	strError.Format("数量超过了限制 %d 个，无法再添加变量！", maxID << 1);
	MessageBox(NULL, strError, _T("警告"), MB_OK);
	return false;
}

//!< 在变量的最后边增加
bool CItemMgr::AddItemBack(std::shared_ptr<CItem> item, int maxID, UINT groupid)
{
	if (!item)												return false;
	if (!FindGroup(groupid))								return false; 
	maxID = maxID >> 1;
	if (m_vtItem.size() >= (maxID << 1))					goto SAY_ERROR;
	int id = m_vtItem.size();
	//!< 如果前边没有空的位置，则从最后添加
	if (id < (maxID << 1))			//!< 数量不能超过权限值
	{
		item->setID((UINT)id);		//!< 设置变量ID号
		item->setMyGroupID(groupid);
		m_vtItem.push_back(item);
		m_mpItem[item->getName()] = item;
		return true;
	}
SAY_ERROR:
	CString strError;
	strError.Format("数量超过了限制 %d 个，无法再添加变量！", maxID << 1);
	MessageBox(NULL, strError, _T("警告"), MB_OK);
	return false;
}

//!< 删除结尾的空变量
void CItemMgr::RemoveItemBackEmpty()
{
	int nIndex = -1;
	for (int i = m_vtItem.size() - 1; i >= 0; --i)
	{
		if (!m_vtItem[i])	continue;
		nIndex = i;
		break;
	}
	if (nIndex >= 0)
		m_vtItem.resize(nIndex + 1);
}

//!< 只适合用户添加组，不适合解析描述文件获得的组
bool CItemMgr::AddGroup(std::shared_ptr<CItemGroup> group,UINT parentid)
{
	if(!group)												return false;
	if(parentid!=-1 && !FindGroup(parentid))				return false;
	for(UINT i = 0; i < m_vtItemGroup.size(); ++i)
	{
		if(m_vtItemGroup[i])	continue;
		group->setID(i);		//!< 找到了一个空的位置，把group的编号附上
		m_vtItemGroup[i] = group;
		return true;
	}
	//!< 如果前边没有空的位置，则从最后添加
	if(m_vtItemGroup.size() < CGbl::GetMe().getMaxGroupNum())	//!< 数量不能超过权限值
	{
		group->setID((UINT)m_vtItemGroup.size());		//!< 设置变量ID号
		m_vtItemGroup.push_back(group);
		return true;
	}
	else
	{
		MessageBox(NULL, _T("数量超过了限制，无法再添加变量组！"), _T("警告"), MB_OK);
	}
	return false;
}

//!< 查找编号为no的元素，返回它的编号，没有返回-1
bool CItemMgr::FindItem(ULONG id)
{
	if(id >= m_vtItem.size())			return false;
	return !!m_vtItem[id];
}

bool CItemMgr::FindGroup(UINT id)
{
	if(id >= m_vtItemGroup.size())		return false;
	return !!m_vtItemGroup[id];
}

//!< 删除编号为no的元素，彻底删除
void CItemMgr::DeleteItem(ULONG id)
{
	std::shared_ptr<CItem> pItem = GetItem(id);
	if (!pItem)							return;
	m_vtItem[id].reset();
	m_mpItem[pItem->getName()].reset();
}

//!< 交换这两个变量
void CItemMgr::ExChangeItem(UINT id1, UINT id2)
{
	UINT size = (UINT)m_vtItem.size();
	ASSERT(id1 < size && id2 < size);
	std::shared_ptr<CItem> item1, item2;
	item1 = GetItem(id1);
	item2 = GetItem(id2);
	if(item1){	item1->setID(id2);	m_vtItem[id2] = item1;	}
	if(item2){	item2->setID(id1);	m_vtItem[id1] = item2;	}
}

void CItemMgr::RemoveGroup(UINT groupid)
{
	if(groupid == 0)	return;					//!< System不能移除
	std::shared_ptr<CItemGroup> _group = GetGroup(groupid);
	if(!_group)			return;

	if(_group->m_ItemDoc)	_group->m_ItemDoc->OnCloseDocument();		//!< 关闭显示此变量组变量表的文档视图
	std::list<UINT> ltItem = _group->getMyItem();
	for (UINT id : ltItem){
		DeleteItem(id);							//!< 删除包含的变量
	}
	std::list<UINT>& ltGroup = _group->getGroupIDList();
	for (UINT id : ltGroup)
		RemoveGroup(id);						//!< 递归删除所有子组
	if(m_vtItemGroup[groupid])		m_vtItemGroup[groupid].reset();
}

//!< 获得变量的数量
UINT CItemMgr::GetItemSize()
{
	UINT count = 0;
	for (std::shared_ptr<CItem> item : m_vtItem){
		if(item)		++count;
	}
	return count;
}

//!< 获得变量组的数量
UINT CItemMgr::GetGroupSize()
{
	UINT count = 0;
	for (std::shared_ptr<CItemGroup> group : m_vtItemGroup){
		if(group)		++count;
	}
	return count;
}

//!< 打开变量表文件，参数 ： 名称 + 路径 + 版本号，最近修改时间
bool CItemMgr::OpenItemFile(CString name, CString pathall, CString ver, CString stime)
{
	if(!CGbl::SetSystemTimeFromStr(m_EditTime, stime))		::GetLocalTime(&m_EditTime);
	m_strName = name;
	m_strFileName = m_strName + _T(".") + ITEM_EXPAND_NAME;
	m_strVersion = ver;
//	CGbl::StartHighTime();
	TiXmlDocument pTiXml((LPCTSTR)pathall);
	if(!pTiXml.LoadFile()){
		MessageBox(NULL, _T("变量表文件格式错误！"), _T("错误"), MB_ICONEXCLAMATION);
		return false;
	}
	//!< 开始解析
	SerializeXmlItem(pTiXml.RootElement(), true);
//	double t=CGbl::GetHighTime();
//	CString tm;
///	tm.Format("%.6fms",t*1000);
//	CGbl::PrintOut(tm);
	return true;
}

void CItemMgr::SaveItemFile()
{
	if(IsModify())
	{
		std::shared_ptr<CProject> proj = CProjectMgr::GetMe().GetProj();
		CString pathAll = proj->GetPath() + m_strName + _T(".") + ITEM_EXPAND_NAME;
		TiXmlDocument pTiXml(pathAll);

		TiXmlDeclaration * decl = new TiXmlDeclaration( "1.0", "GB2312", "" );	//!< 起始声明
		pTiXml.LinkEndChild(decl);
		TiXmlComment * comment = new TiXmlComment();							//!< 注释信息
		comment->SetValue(_T("请勿手工编辑此文档"));
		pTiXml.LinkEndChild(comment);
		TiXmlElement *pRoot=new TiXmlElement("DocumentRoot");					//!< 写入根节点
		
		SerializeXmlItem(pRoot, false);												//!< 添加工程节点,写操作
		pTiXml.LinkEndChild(pRoot);
		if(!pTiXml.SaveFile()){
			MessageBox(NULL, _T("保存失败！"), _T("错误"), MB_ICONEXCLAMATION);
			return ;
		}
	}
	CGbl::SetProgressStep(1);
	m_bModify = false;
}

bool CItemMgr::SerializeXmlItem(TiXmlElement* pNode, bool bRead)
{
	if(bRead)
	{
		//!< 软件版本，先不考虑
		//!< 子节点
		CString text, strValue;
		TiXmlElement* pChild = pNode->FirstChildElement();
		std::list<std::shared_ptr<CItemGroup> > ltGroup;
		std::list<std::shared_ptr<CItem> > ltItem;
		std::shared_ptr<CItemGroup> group;
		std::shared_ptr<CItem> item;
		int maxCount = MAX_ITEM_COUNT;
		int count = 0;
		while(pChild)
		{
			text = pChild->ValueStr().c_str();
			if(GROUPS == text)
			{
				group = std::shared_ptr<CItemGroup>(new CItemGroup(_T(""), 0));
				if(group->SerializeXml(pChild, bRead))		ltGroup.push_back(group);
			}
			else if(ITEM == text)
			{
				++count;
				if(count > maxCount)
				{
					CString strError;
					strError.Format(_T("变量超过数量限制%d个，解析工程变量信息终止。"), maxCount);
					MessageBox(NULL, strError, _T("警告"), MB_OK | MB_ICONWARNING);
					break;
				}
				item = std::shared_ptr<CItem>(new CItem(_T("")));
				if(item->SerializeXml(pChild, bRead))		ltItem.push_back(item);
			}
			pChild = pChild->NextSiblingElement();
		}
		UINT groupcount = 0, itemcount = 0;
		for (auto group : ltGroup)		groupcount = max(groupcount, group->getID());
		m_vtItemGroup.clear();
		m_vtItemGroup.resize(groupcount + 1);
		for (auto item : ltItem)		itemcount = max(itemcount, item->getID());
		m_mpItem.clear();
		m_vtItem.clear();
		m_vtItem.resize(itemcount + 1);
		for (auto group : ltGroup)		m_vtItemGroup[group->getID()] = group;
		for (auto item : ltItem)		{ m_vtItem[item->getID()] = item;	m_mpItem[item->getName()] = item; }
		//!< 每个变量都要记录自己所在的组
		for (auto group : m_vtItemGroup)
		{
			if(!group)		continue;
//			group->InitItem();
		}
	}
	else
	{
		pNode->SetAttribute(_T("FileType"), _T("EditItemFile"));
		/*! 原理是1.0，变量组内存有变量的列表，可以达到多对多的关系，但设置变量所属组变得非常麻烦
			现在是1.1，变量只能所属于一个变量组内，关系变清晰了，用户通过软件配置也简单了
			由于采用描述，解析的过程不需要修改，新旧都兼容
		*/
		pNode->SetAttribute(_T("Version"), _T("1.1"));

		TiXmlElement* pXmlInfo = new TiXmlElement(INFO_HEAD);
		pNode->LinkEndChild(pXmlInfo);

		TiXmlElement* pChild;
		for (std::shared_ptr<CItemGroup> group : m_vtItemGroup)
		{
			if(!group)		continue;
			pChild = pNode->AddTiXmlChild((LPCTSTR)GROUPS);
			group->SerializeXml(pChild, false);
		}
		for (std::shared_ptr<CItem> item : m_vtItem)
		{
			if(!item)		continue;
			pChild = pNode->AddTiXmlChild((LPCTSTR)ITEM);
			item->SerializeXml(pChild, false);
			CGbl::SetProgressStep(1);
		}
	}
	return true;
}

//!< 变量表导出
void CItemMgr::OutItem(std::list<UINT> ltItem)
{
	//CProjectMgr* projMgr = &CProjectMgr::GetMe();
	//std::shared_ptr<CProject> proj = projMgr->GetProj();
	//ASSERT(proj);
	//CString defFileName = _T("导出变量表");
	//defFileName = defFileName;
	//CString extendNames = _T("变量表导出文件 (*.vxpt)|*.vxpt|");
	//extendNames = extendNames + _T("Excel文件 (*.xls)|*.xls|");
	//extendNames = extendNames + _T("Txt文件 (*.txt)|*.txt|");
	//extendNames = extendNames + _T("|");
	//CFileDialog saveDlg(FALSE, (LPCTSTR)extendNames, (LPCTSTR)defFileName, OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT, (LPCTSTR)extendNames);
	//if(IDOK == saveDlg.DoModal())
	//{
	//	CString strExt = saveDlg.GetFileExt();
	//	if(strExt == _T("xls") || strExt == _T("txt"))
	//	{
	//		COperation excelop;
	//		std::shared_ptr<CItem> pItem;
	//		std::shared_ptr<CPropertySource> pSrc;
	//		std::shared_ptr<CPropertyAlarm> pAlarm;
	//		int nRow = 0;
	//		CString strValue;
	//		CComVariant cvrValue;

	//		//!< 启动导出器
	//		excelop.FileGuid(FALSE, saveDlg.GetFileName(), saveDlg.GetPathName());
	//		CGbl::SetProgressRange(ltItem.size() + 1);
	//		CGbl::SetProgressStep(1);

	//		//!< 加载标题
	//		strValue = _T("编号");			cvrValue = strValue;	excelop.WriteItem(nRow, 1, cvrValue);
	//		strValue = _T("名称");			cvrValue = strValue;	excelop.WriteItem(nRow, 2, cvrValue);
	//		strValue = _T("标签");			cvrValue = strValue;	excelop.WriteItem(nRow, 3, cvrValue);
	//		strValue = _T("备注");			cvrValue = strValue;	excelop.WriteItem(nRow, 4, cvrValue);
	//		strValue = _T("变量类型");		cvrValue = strValue;	excelop.WriteItem(nRow, 5, cvrValue);
	//		strValue = _T("默认值");		cvrValue = strValue;	excelop.WriteItem(nRow, 6, cvrValue);
	//		strValue = _T("数据源类型");	cvrValue = strValue;	excelop.WriteItem(nRow, 7, cvrValue);
	//		strValue = _T("访问权限");		cvrValue = strValue;	excelop.WriteItem(nRow, 8, cvrValue);
	//		strValue = _T("保留值");		cvrValue = strValue;	excelop.WriteItem(nRow, 9, cvrValue);
	//		strValue = _T("保留历史数据");	cvrValue = strValue;	excelop.WriteItem(nRow, 10, cvrValue);
	//		strValue = _T("创建时间");		cvrValue = strValue;	excelop.WriteItem(nRow, 11, cvrValue);
	//		strValue = _T("脚本");			cvrValue = strValue;	excelop.WriteItem(nRow, 12, cvrValue);
	//		strValue = _T("设备");			cvrValue = strValue;	excelop.WriteItem(nRow, 13, cvrValue);
	//		strValue = _T("区地址");		cvrValue = strValue;	excelop.WriteItem(nRow, 14, cvrValue);
	//		strValue = _T("IO类型");		cvrValue = strValue;	excelop.WriteItem(nRow, 15, cvrValue);
	//		strValue = _T("寻址偏移");		cvrValue = strValue;	excelop.WriteItem(nRow, 16, cvrValue);
	//		strValue = _T("位偏移");		cvrValue = strValue;	excelop.WriteItem(nRow, 17, cvrValue);
	//		strValue = _T("刷新时间");		cvrValue = strValue;	excelop.WriteItem(nRow, 18, cvrValue);
	//		strValue = _T("允许滞后");		cvrValue = strValue;	excelop.WriteItem(nRow, 19, cvrValue);
	//		strValue = _T("转换类型");		cvrValue = strValue;	excelop.WriteItem(nRow, 20, cvrValue);
	//		strValue = _T("最小工程值");	cvrValue = strValue;	excelop.WriteItem(nRow, 21, cvrValue);
	//		strValue = _T("最大工程值");	cvrValue = strValue;	excelop.WriteItem(nRow, 22, cvrValue);
	//		strValue = _T("最小IO值");		cvrValue = strValue;	excelop.WriteItem(nRow, 23, cvrValue);
	//		strValue = _T("最大IO值");		cvrValue = strValue;	excelop.WriteItem(nRow, 24, cvrValue);
	//		strValue = _T("开关报警类型");	cvrValue = strValue;	excelop.WriteItem(nRow, 25, cvrValue);
	//		strValue = _T("限值死区");		cvrValue = strValue;	excelop.WriteItem(nRow, 26, cvrValue);
	//		strValue = _T("下下限报警激活");cvrValue = strValue;	excelop.WriteItem(nRow, 27, cvrValue);
	//		strValue = _T("下下限报警值");	cvrValue = strValue;	excelop.WriteItem(nRow, 28, cvrValue);
	//		strValue = _T("下限报警激活");	cvrValue = strValue;	excelop.WriteItem(nRow, 29, cvrValue);
	//		strValue = _T("下限报警值");	cvrValue = strValue;	excelop.WriteItem(nRow, 30, cvrValue);
	//		strValue = _T("上限报警激活");	cvrValue = strValue;	excelop.WriteItem(nRow, 31, cvrValue);
	//		strValue = _T("上限报警值");	cvrValue = strValue;	excelop.WriteItem(nRow, 32, cvrValue);
	//		strValue = _T("上上限报警激活");cvrValue = strValue;	excelop.WriteItem(nRow, 33, cvrValue);
	//		strValue = _T("上上限报警值");	cvrValue = strValue;	excelop.WriteItem(nRow, 34, cvrValue);
	//		strValue = _T("目标报警激活");	cvrValue = strValue;	excelop.WriteItem(nRow, 35, cvrValue);
	//		strValue = _T("目标值");		cvrValue = strValue;	excelop.WriteItem(nRow, 36, cvrValue);
	//		strValue = _T("目标偏差值");	cvrValue = strValue;	excelop.WriteItem(nRow, 37, cvrValue);
	//		strValue = _T("目标死区");		cvrValue = strValue;	excelop.WriteItem(nRow, 38, cvrValue);
	//		strValue = _T("变化率报警激活");cvrValue = strValue;	excelop.WriteItem(nRow, 39, cvrValue);
	//		strValue = _T("最大变化量");	cvrValue = strValue;	excelop.WriteItem(nRow, 40, cvrValue);
	//		strValue = _T("变化时间");		cvrValue = strValue;	excelop.WriteItem(nRow, 41, cvrValue);

	//		++nRow;

	//		//!< 加载变量信息
	//		for (UINT id : ltItem)
	//		{
	//			CGbl::SetProgressStep(1);
	//			pItem = GetItem(id);
	//			if(!pItem)		continue;
	//			pSrc = pItem->getSrcInfo();
	//			pAlarm = pItem->getAlarmInfo();

	//			cvrValue = pItem->getID();				excelop.WriteItem(nRow, 1, cvrValue);	//!< 变量ID号
	//			cvrValue = pItem->getName();			excelop.WriteItem(nRow, 2, cvrValue);	//!< 变量名称
	//			cvrValue = pItem->getTag();				excelop.WriteItem(nRow, 3, cvrValue);	//!< 变量标签
	//			cvrValue = pItem->getDescription();		excelop.WriteItem(nRow, 4, cvrValue);	//!< 变量的备注信息
	//			cvrValue = pItem->getValType();			excelop.WriteItem(nRow, 5, cvrValue);	//!< 变量类型，0-bit，1-char，2-byte，3-short，4-word，5-int，6-dword，7-float，8-double
	//			cvrValue = pItem->getDefault();			excelop.WriteItem(nRow, 6, cvrValue);	//!< 变量默认值
	//			cvrValue = pItem->getSrcType();			excelop.WriteItem(nRow, 7, cvrValue);	//!< 数据源类型，0内存变量，1I/O变量
	//			cvrValue = pItem->getAccessRight();		excelop.WriteItem(nRow, 8, cvrValue);	//!< 访问权限，0可读可写，1只读，只写
	//			cvrValue = pItem->getReservFlag();		excelop.WriteItem(nRow, 9, cvrValue);	//!< 是否设置成保留值，true设置，false不设置
	//			cvrValue = pItem->getReservDB();		excelop.WriteItem(nRow, 10, cvrValue);	//!< 是否保存历史数据
	//			strValue = _T("2009-12-28");	cvrValue = strValue;	excelop.WriteItem(nRow, 11, cvrValue);	//!< 创建时间
	//			cvrValue = pSrc->getScriptText();		excelop.WriteItem(nRow, 12, cvrValue);	//!< 赋值脚本文本
	//			cvrValue = pSrc->getDeviceID();			excelop.WriteItem(nRow, 13, cvrValue);	//!< 设备编号
	//			cvrValue = pSrc->getAreaID();			excelop.WriteItem(nRow, 14, cvrValue);	//!< 区编号
	//			cvrValue = pSrc->getIOType();			excelop.WriteItem(nRow, 15, cvrValue);	//!< IO类型，具体内容看描述文件
	//			cvrValue = pSrc->getUnitIndex();		excelop.WriteItem(nRow, 16, cvrValue);	//!< 寻址偏移
	//			cvrValue = pSrc->getBitIndex();			excelop.WriteItem(nRow, 17, cvrValue);	//!< 位偏移
	//			cvrValue = pSrc->getFreshTime();		excelop.WriteItem(nRow, 18, cvrValue);	//!< 变量刷新时间,单位100毫秒
	//			cvrValue = pSrc->getDelayFlag();		excelop.WriteItem(nRow, 19, cvrValue);	//!< 可否允许滞后
	//			cvrValue = pSrc->getConvertType();		excelop.WriteItem(nRow, 20, cvrValue);	//!< 转换类型，0不转换，1是否取反(位变量使用)，1线性转换
	//			cvrValue = pSrc->getProjMin();			excelop.WriteItem(nRow, 21, cvrValue);	//!< 最小工程值
	//			cvrValue = pSrc->getProjMax();			excelop.WriteItem(nRow, 22, cvrValue);	//!< 最大工程值
	//			cvrValue = pSrc->getIOMin();			excelop.WriteItem(nRow, 23, cvrValue);	//!< 最小IO值
	//			cvrValue = pSrc->getIOMax();			excelop.WriteItem(nRow, 24, cvrValue);	//!< 最大IO值
	//			cvrValue = pAlarm->getBitAlarmType();	excelop.WriteItem(nRow, 25, cvrValue);	//!< 开关报警类型，0不报警，1开时报警，2关时报警，3开到关报警，4关到开报警，5变化就报警
	//			cvrValue = pAlarm->getDeadArea();		excelop.WriteItem(nRow, 26, cvrValue);	//!< 死区，开始报警和恢复报警之间的界限，为了放置报警波动。真正的报警值和恢复值 = 报警值 ± 死区值
	//			cvrValue = pAlarm->getLoloActive();		excelop.WriteItem(nRow, 27, cvrValue);	//!< 下下限报警是否启动
	//			cvrValue = pAlarm->getLoloValue();		excelop.WriteItem(nRow, 28, cvrValue);	//!< 下下限报警值
	//			cvrValue = pAlarm->getLowActive();		excelop.WriteItem(nRow, 29, cvrValue);	//!< 下限报警是否启动
	//			cvrValue = pAlarm->getLowValue();		excelop.WriteItem(nRow, 30, cvrValue);	//!< 下限报警值
	//			cvrValue = pAlarm->getHighActive();		excelop.WriteItem(nRow, 31, cvrValue);	//!< 上限报警是否启动
	//			cvrValue = pAlarm->getHighValue();		excelop.WriteItem(nRow, 32, cvrValue);	//!< 上限报警值
	//			cvrValue = pAlarm->getHihiActive();		excelop.WriteItem(nRow, 33, cvrValue);	//!< 上上限报警是否启动
	//			cvrValue = pAlarm->getHihiValue();		excelop.WriteItem(nRow, 34, cvrValue);	//!< 上上限报警值
	//			cvrValue = pAlarm->getAimActive();		excelop.WriteItem(nRow, 35, cvrValue);	//!< 目标报警是否启动
	//			cvrValue = pAlarm->getAimValue();		excelop.WriteItem(nRow, 36, cvrValue);	//!< 目标值
	//			cvrValue = pAlarm->getAimPercent();		excelop.WriteItem(nRow, 37, cvrValue);	//!< 目标偏差
	//			cvrValue = pAlarm->getAimDeadPercent();	excelop.WriteItem(nRow, 38, cvrValue);	//!< 目标偏差死区
	//			cvrValue = pAlarm->getShiftActive();	excelop.WriteItem(nRow, 39, cvrValue);	//!< 变化率报警是否启动
	//			cvrValue = pAlarm->getShiftDelta();		excelop.WriteItem(nRow, 40, cvrValue);	//!< 最大变化量
	//			cvrValue = pAlarm->getShiftTime();		excelop.WriteItem(nRow, 41, cvrValue);	//!< 变化时间，单位秒
	//			++nRow;
	//		}
	//		excelop.SaveFile();
	//		CGbl::SetProgressEnd();
	//	}
	//	else
	//	{
	//		TiXmlDocument pTiXml(saveDlg.GetPathName());
	//		TiXmlDeclaration * decl = new TiXmlDeclaration( "1.0", "GB2312", "" );	//!< 起始声明
	//		pTiXml.LinkEndChild(decl);
	//		TiXmlComment * comment = new TiXmlComment();							//!< 注释信息
	//		comment->SetValue(_T("请勿手工编辑此文档"));
	//		pTiXml.LinkEndChild(comment);
	//		TiXmlElement *pRoot=new TiXmlElement("DocumentRoot");					//!< 写入根节点
	//		SerializeOut(pRoot, ltItem);											//!< 添加工程节点,写操作
	//		pTiXml.LinkEndChild(pRoot);
	//		if(!pTiXml.SaveFile())
	//			MessageBox(NULL, _T("导出失败！"), _T("错误"), MB_ICONEXCLAMATION);
	//		else
	//			MessageBox(NULL, _T("导出成功！"), _T("提示"), MB_ICONINFORMATION);
	//	}
	//}
}

//!< 变量表导出
bool CItemMgr::SerializeOut(TiXmlElement* pNode, std::list<UINT> ltItem)
{
	pNode->SetAttribute(_T("FileType"), ITEM_OUTFILE);
	pNode->SetAttribute(_T("Version"), _T("1.0"));

	TiXmlElement* pChild;
	std::shared_ptr<CItem> item;
	for (UINT id : ltItem){
		item = GetItem(id);
		if(!item)		continue;
		pChild = pNode->AddTiXmlChild((LPCTSTR)ITEM);
		item->SerializeXml(pChild, false, 1);
	}
	return true;
}

void CItemMgr::OnCreate()
{
	m_vtItemGroup.clear();
	m_mpItem.clear();
	m_vtItem.clear();
	if(m_pItemDoc)		m_pItemDoc->OnCloseDocument();
	SetModify();		//!< 新建的工程默认为被修改状态

	//!< 创建基本变量和变量组，这里只有一个System变量组
	std::shared_ptr<CItemGroup> group;
	group = std::shared_ptr<CItemGroup>(new CItemGroup(_T(""),UINT(-1)));
	group->InitDefault();
	m_vtItemGroup.push_back(group);

	//!< 初始化路径信息
	m_strName = _T("变量表");													//!< 名字叫"变量表"，工程名叫"p"
	std::shared_ptr<CProject> proj = CProjectMgr::GetMe().GetProj();
	m_strFileName = m_strName + _T(".") + ITEM_EXPAND_NAME;						//!< 路径叫"p_变量表.var"
}

void CItemMgr::OnClose()
{
	m_vtItemGroup.clear();
	m_mpItem.clear();
	m_vtItem.clear();
	m_pItemDoc = NULL;
}

void CItemMgr::OpenDoc()
{
	if(!m_pItemDoc){
		m_pItemDoc=(MVC::Item::CItemDoc*)((CDXPEditorApp *)AfxGetApp())->m_pItemDocMgr->OpenDocumentFile(NULL);
		m_pItemDoc->ShowAllItem();
	}
	else{
		CView* view = (CView*)m_pItemDoc->GetView();
		CFrameWnd* frm = view->GetParentFrame();
		ASSERT(frm);
		if(frm->IsKindOf(RUNTIME_CLASS(CItemFrame)))
			frm->ActivateFrame();
	}
	m_pItemDoc->SetTitle(_T(""));
}

void CItemMgr::WriteItemValue(UINT id, bool proj, CString value)
{
	std::shared_ptr<CItem> pItem = GetItem(id);
	ASSERT(pItem);
	CMainFrame * mf = (CMainFrame *)g_App.GetMainWnd();
	CComVariant cvr = value;
	HRESULT hr;
	if(proj)	hr = cvr.ChangeType(pItem->getValue().vt);
	else		hr = cvr.ChangeType(pItem->getIOValue().vt);
	if(!SUCCEEDED(hr))
	{
		CGbl::PrintOut(_T("数值类型不匹配，请查看是否数值越界..."));
		return;
	}
	if(proj){
		mf->m_SevCommer.Write(id, cvr);
	}
	else{
		CComVariant _ccomvar;
		_ccomvar.ChangeType(VT_ARRAY|VT_VARIANT);
		_ccomvar.parray = ::SafeArrayCreateVector(VT_VARIANT,0,1);
		void* pDest;
		AfxCheckError(::SafeArrayAccessData(_ccomvar.parray, &pDest));
		memcpy(pDest, &cvr, sizeof(VARIANT));
		AfxCheckError(::SafeArrayUnaccessData(_ccomvar.parray));

		mf->m_SevCommer.Execute(5, _ccomvar, -1, id, 1);
	}
}

//!< 改变监控状态
void CItemMgr::SetItemWatch(const bool watch)
{
	if(!watch)		return;
	std::shared_ptr<CItem> item;
	for (auto item : m_vtItem){
		if(!item)		continue;
		item->WatchInit();
	}
}

void CItemMgr::ReadItemValue(bool bProj)
{
	std::shared_ptr<CItem> last_item = GetLastItem();
	if(!last_item)								return;		//!< 不存在就退出
	std::shared_ptr<CItem> item;
	UINT last_id = last_item->getID();
	UINT item_count = last_id + 1;

	CComVariant cvr;
	VARIANT safeArray;
	CMainFrame* mf = (CMainFrame*)g_App.GetMainWnd();

	//!< 读所有变量的工程值
	if(bProj)		safeArray = mf->m_SevCommer.ReadEx(0, last_id);
	else			safeArray = mf->m_SevCommer.Execute(4, cvr, -1, 0, item_count);

	VARIANT *pValue = NULL;
	if(safeArray.vt == (VT_ARRAY|VT_VARIANT))
	{
		SAFEARRAY *pArray = safeArray.parray;
		HRESULT hr = SafeArrayAccessData(pArray, (void**)&pValue);
		if(pArray->rgsabound->cElements != item_count)	return;		//!< 如果返回的数量不符，退出
		if(SUCCEEDED(hr))
		{
			for(UINT i = 0, j = 0; i <= last_id && j < item_count; ++i, ++j)
			{
				item = GetItem(i);
				if(!item || pValue[j].vt == VT_EMPTY)	continue;
				memcpy(&cvr, &pValue[j], sizeof(VARIANT));
				if(bProj)
				{
					if(cvr.vt != item->getValue().vt && cvr.vt == VT_BSTR)
						continue;
					item->setValue(cvr);
				}
				else
				{
					if(cvr.vt != item->getIOValue().vt && cvr.vt == VT_BSTR)
						continue;
					item->setIOValue(cvr);
				}
			}
		}
		SafeArrayUnaccessData(pArray);
		::VariantClear(&safeArray);
	}
}

//!< 显示出编号为id的变量，id为-1表示显示变量一览表
void CItemMgr::ShowItem(UINT id)
{
	if(!CProjectMgr::GetMe().GetProj())	return;

	// -1表示显示变量一览表
	if (id != -1)
	{
		std::shared_ptr<CItem> item = GetItem(id);
		if(!item)		return;

		//!< 如果变量表没打开，那么打开变量表一览
		BOOL isMax;
		CMainFrame* mf = (CMainFrame *)g_App.GetMainWnd();
		CFrameWnd* pFrame = mf->MDIGetActive(&isMax);		//!< 找到当前被激活的子窗口
		if(pFrame)
		{
			CDocument* pDoc = pFrame->GetActiveDocument();
			if(pDoc && pDoc->IsKindOf(RUNTIME_CLASS(CItemDoc)))
			{
				if(((CItemDoc *)pDoc)->GetView()->ShowItem(id))
					return;
			}
		}
	}
	OpenDoc();
	m_pItemDoc->GetView()->ShowItem(id);
}

//!< 查找符合该字符串的变量，返回匹配的数量
int CItemMgr::SearchItem(CString str, bool bMatchCase, bool bMatchWhole, bool bRegex /* = false */)
{
	int nMatchCount = 0;
	std::shared_ptr<CItem> item;
	if(!bMatchCase && !bRegex)		str = str.MakeUpper();

	for (auto item : m_vtItem)
	{
		if(item)
			if(item->DoSearch(str, bMatchWhole, bMatchCase, bRegex))
				++nMatchCount;
//		CGbl::SetProgressStep(1);
	}
	return nMatchCount;
}

//!< 获得一个相近的名字，等于也可以，但保证没有被仍和变量使用的
CString CItemMgr::GetSimilarName(CString name)
{
	int number = 0;
	CString noNumberName = name;
	bool haveNumber = CGbl::GetNumberFromString(noNumberName, number, 1);
	//!< 如果这个字符串中不包含数字
	if(!haveNumber)
	{
		if(!GetItem(name))			return name;
		name = name + _T("0");
		return GetSimilarName(name);
	}
	//!< 如果包含了数字
	if(!GetItem(name))				return name;
	CString newName;
	newName.Format("%d", number + 1);
	//!< 如果都是9，表示+1时会进位，呵呵，这时如果前边的字符串最后还跟个零，为了工整起见，需要将最后的零去掉
	CString strNumber;
	strNumber.Format("%d",number);
	bool bAll9 = true;
	for(int i = 0; i < strNumber.GetLength(); ++i)
	{
		char c = strNumber.GetAt(i);
		if(c != '9'){
			bAll9 = false;
			break;
		}
	}
	if(bAll9 && noNumberName.GetLength() > 0 && noNumberName.Right(1) == _T("0"))
	{
		newName = noNumberName.Left(noNumberName.GetLength() - 1) + newName;
	}
	else
	{
		newName = noNumberName + newName;
	}
	return GetSimilarName(newName);
}

//!< 添加刚刚被修改的变量
void CItemMgr::AddEditItem(UINT id)
{
	m_ltEditItemID.push_back(id);
}

//!< 设置这个操作结束了
void CItemMgr::SetEditEnd()
{
	if(m_ltEditItemID.empty())		return;
	POSITION pos = g_App.m_pItemDocMgr->GetFirstDocPosition();
	CItemDoc* pDoc;
	CItemView* pView;
	while(pos)
	{
		pDoc = (CItemDoc *)g_App.m_pItemDocMgr->GetNextDoc(pos);
		pView = pDoc->GetView();
		pView->m_ItemGrid.FreshItemRows(m_ltEditItemID);
	}
	m_ltEditItemID.clear();
}

//!< 刷新所有表格显示
void CItemMgr::FreshAllGrid()
{
	POSITION pos = g_App.m_pItemDocMgr->GetFirstDocPosition();
	CItemDoc* pDoc;
	CItemView* pView;
	while(pos)
	{
		pDoc = (CItemDoc *)g_App.m_pItemDocMgr->GetNextDoc(pos);
		pView = pDoc->GetView();
		pView->m_ItemGrid.DeleteRowAll();
		pView->m_ItemGrid.DeleteColAll();
		pView->m_ItemGrid.ResetAll();
		pView->m_ItemGrid.InitAll();
		pView->m_ItemGrid.RedrawGrid();
		pView->m_ItemGrid.RedrawAll();
		pDoc->UpdateAllViews(NULL);
	}
}

//!< 变量报警
void CItemMgr::OnVariableAlarm(int nAlarmType,int nID,VARIANT* varValue)
{
	std::shared_ptr<CItem> item = GetItem(nID);
	if(!item)					return;
	if(!varValue)				return;
	SYSTEMTIME t;
	GetLocalTime(&t);
	CComVariant cvr = *varValue;
	cvr.ChangeType(VT_BSTR);
	CString text;
	byte* alarmByte = (byte *)&nAlarmType;
	if(alarmByte[0] == 0){
		if(alarmByte[1] == 1)		text = _T("开");
		if(alarmByte[1] == 2)		text = _T("关");
		if(alarmByte[1] == 3)		text = _T("开->关");
		if(alarmByte[1] == 4)		text = _T("关->开");
		if(alarmByte[1] == 5)		text = _T("开关改变");
	}
	else{
		if(alarmByte[0] == 1)		text = _T("下下限报警");
		else if(alarmByte[0] == 2)	text = _T("下限报警");
		else if(alarmByte[0] == 3)	text = _T("上限报警");
		else if(alarmByte[0] == 4)	text = _T("上上限报警");
		else if(alarmByte[0] == 5)	text = _T("目标报警");
		else if(alarmByte[0] == 6)	text = _T("变化率报警");
	}
	CString title;
	if(alarmByte[3] == 0){
		item->SetAlarm(alarmByte[0], true);
		title = _T("系统提示-变量报警：变量报警 [");
	}
	else if(alarmByte[3] == 0xFF){
		item->SetAlarm(alarmByte[0], false);
		title = _T("系统提示-变量报警：报警解除 [");
	}
	else{
		return;
	}
	text = title + item->getName() + _T("] 类型 [") + text + _T("]");

	CString strTime;
	strTime.Format("%.4d/%.2d/%.2d %.2d:%.2d:%.2d(%.3d) —— ",t.wYear,t.wMonth,t.wDay,t.wHour,t.wMinute,t.wSecond,t.wMilliseconds);
	CGbl::PrintOut(strTime + text);
}