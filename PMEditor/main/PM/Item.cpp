#include "stdafx.h"
#include "DXPEditor.h"
#include "SoftInfo.h"
#include "Gbl.h"
#include "Item.h"
#include "ItemMgr.h"
#include "ItemDoc.h"
#include "AlarmProperty.h"
#include "SourceProperty.h"
#include "ItemGroup.h"

#include "DevMgr.h"
#include "DeviceOne.h"
#include "XmlDevice.h"
#include "XmlArea.h"

using namespace MVC;
using namespace Item;

const CString ITEM_ID = _T("id");
const CString ITEM_NAME = _T("Name");
const CString ITEM_GROUP = _T("GroupID");
const CString ITEM_TAG = _T("Tag");
const CString ITEM_DESCRIPTION = _T("Description");
const CString ITEM_VALTYPE = _T("ValType");
const CString ITEM_SRCTYPE = _T("SrcType");
const CString ITEM_ACCESSRIGHT = _T("OprAttribute");			//!< 可读可写，只读，只写
const CString ITEM_DEFVALUE = _T("DefaultValue");
const CString ITEM_RESFLAG = _T("ReservFlag");
const CString ITEM_RESFDB = _T("ReservDB");
const CString ITEM_MODBUS485 = _T("Modbus485");
const CString ITEM_ARRAYFLAG = _T("ArrayFlag");

const CString SRC = _T("SrcProp");
const CString ALARM = _T("AlarmProp");

const CString VAL_TYPE[] = {_T("位变量"), _T("有符号字节"), _T("无符号字节"), _T("有符号单字"), _T("无符号单字"),
_T("有符号双字"), _T("无符号双字"), _T("单精度浮点")/*, _T("双精度浮点")*/};

CItem::CItem(CString name)
{
	m_uiIRefCount = 0;
	m_strName = name;
	m_strTag = _T("");
	m_strDescription = _T("");
	m_uiValType = 5;					//!< 默认成int32
	m_cvrValueOld = m_cvrValue = 0;
	m_cvrIOValueOld = m_cvrIOValue = 0;
	m_uiSrcType = 1;					//!< 默认成IO变量
	m_uiGroupID = 0;					//!< 变量所属变量组的ID号
	m_uiAccessRight = 0;
	m_bReservFlag = FALSE;
	m_bReservDB = FALSE;
	m_bModbus485 = FALSE;				//!< 是否导出Modbus变量
	m_bAlarmBit = false;				//!< 位变量是否报警
	m_bAlarmLolo = false;				//!< 下下限是否报警
	m_bAlarmLow = false;				//!< 下限是否报警
	m_bAlarmHigh = false;				//!< 上限是否报警
	m_bAlarmHihi = false;				//!< 上上限是否报警
	m_bAlarmAim = false;				//!< 目标是否报警
	m_bAlarmShift = false;				//!< 变化率是否报警

	m_spSrcInfo = std::shared_ptr<CPropertySource>(new CPropertySource(this));
	m_spAlarmInfo = std::shared_ptr<CPropertyAlarm>(new CPropertyAlarm(this));

	ChangeVarType(VT_I4);
	::GetLocalTime(&m_stCreateTime);
	m_stUpdateTime = m_stCreateTime;
}

CItem::~CItem(void)
{
}

//!< 保存xml格式的编辑信息,iExport=0正常，1导出，2导入
bool CItem::SerializeXml(TiXmlElement* pNode, bool bRead, bool iExport/* = false*/)
{
	if(bRead)
	{
		CString name, strValue;
		CComVariant cv;
		TiXmlAttribute* pAttr = pNode->FirstAttribute();
		while(pAttr)
		{
			name = pAttr->Name();
			strValue = pAttr->Value();
			cv = strValue;

			if(name == ITEM_NAME)				{m_strName = cv;}
			else if(name == ITEM_TAG)			{m_strTag = cv;}
			else if(name == ITEM_ID)			{m_uiID = (UINT)pAttr->IntValue();}
			else if(name == ITEM_GROUP)			{m_uiGroupID = (UINT)pAttr->IntValue();}
			else if(name == ITEM_VALTYPE)		{m_uiValType = (UINT)pAttr->IntValue();		SetVarType();}
			else if(name == ITEM_SRCTYPE)		{m_uiSrcType = (UINT)pAttr->IntValue();}
			else if(name == ITEM_ACCESSRIGHT)	{m_uiAccessRight = (UINT)pAttr->IntValue();}
			else if(name == ITEM_DEFVALUE)		{m_cvrDefaultValue = cv;}
			else if(name == ITEM_RESFLAG)		{m_bReservFlag = (strValue == "1") ? TRUE : FALSE;}
			else if(name == ITEM_RESFDB)		{m_bReservDB = (strValue == "1") ? TRUE : FALSE;}
			else if(name == ITEM_MODBUS485)		{m_bModbus485 = (strValue == "1") ? TRUE : FALSE;}		//!< 是否导出Modbus变量}
			else if(name == ITEM_DESCRIPTION)	{m_strDescription = cv;}
			else if(name == ITEM_ARRAYFLAG)		{}
			pAttr = pAttr->Next();
		}

		TiXmlElement* pChild = pNode->FirstChildElement();
		while(pChild)
		{
			name = pChild->Value();
			if(name == SRC)				m_spSrcInfo->SerializeXml(pChild, bRead, iExport);
			else if(name == ALARM)		m_spAlarmInfo->SerializeXml(pChild, bRead);
			pChild = pChild->NextSiblingElement();
		}
	}
	else
	{
		int t = m_cvrDefaultValue.vt;
		m_cvrDefaultValue.ChangeType(VT_BSTR);
		pNode->SetAttribute(ITEM_ID, getID());
		pNode->SetAttribute(ITEM_NAME, getName());
		pNode->SetAttribute(ITEM_TAG, getTag());
		pNode->SetAttribute(ITEM_DESCRIPTION, getDescription());
		pNode->SetAttribute(ITEM_VALTYPE, getValType());
		pNode->SetAttribute(ITEM_SRCTYPE, getSrcType());
		pNode->SetAttribute(ITEM_ACCESSRIGHT, getAccessRight());
		pNode->SetAttribute(ITEM_DEFVALUE, (CString)m_cvrDefaultValue.bstrVal);
		pNode->SetAttribute(ITEM_RESFLAG, getReservFlag()?1:0);
		pNode->SetAttribute(ITEM_RESFDB, getReservDB()?1:0);
		pNode->SetAttribute(ITEM_MODBUS485, getModbus485()?1:0);
		pNode->SetAttribute(ITEM_GROUP, getMyGroupID());
		m_cvrDefaultValue.ChangeType(t);

		m_spSrcInfo->SerializeXml(pNode->AddTiXmlChild((LPCTSTR)SRC), bRead, iExport);
		m_spAlarmInfo->SerializeXml(pNode->AddTiXmlChild((LPCTSTR)ALARM), bRead);
	}
	return true;
}

//!< 从PLC_Config导出的变量表中读取变量
//bool MVC::Item::CItem::ReadFromPlcConfig(COperation& reader, int nRow, int devID)
bool MVC::Item::CItem::ReadFromPlcConfig(CString strLine, int devID)
{
	std::vector<CString> vtCell;
	CGbl::SpliteBy(strLine, "\t", vtCell);
	if (vtCell.size() > 6 || vtCell.size() < 4)			return false;
// 	CString str = reader.ReadRow(nRow, 1, 5);
	if (vtCell[0].Trim() == _T(""))						return false;
	if (vtCell[3].Trim() == _T(""))						return false;

	CString strAddr = vtCell[0].Trim();					//!< 变量地址
	if(CGbl::GetMe().IsNumber(strAddr))  return false;	//!< 第一列为数字证明是DView导出的Excle格式文件
	CString strType = vtCell[3].Trim();					//!< 变量类型
	CString strName = vtCell[2].Trim();					//!< 变量名
	if (strName != _T(""))		setItemName(strName);
	else						setItemName(strAddr);
	setTag(vtCell[2].Trim());							//!< 变量标签
	if (vtCell.size() > 4)
		setDescription(vtCell[4].Trim());				//!< 变量注释
	setSrcType(CItem::SRC_TYPE_IO);										//!< 变量数据源类型
	if (getSrcInfo()->SetPlcConfigAddr(devID, strAddr, strType))		//!< 数据源信息
		setValType(getSrcInfo()->getIOType());
	else
		setValType(strType);
	return true;
}

//!< 从DView导出的变量表中读取变量
//bool CItem::ReadFromDView(COperation& reader, int nRow, int devID)
bool MVC::Item::CItem::ReadFromDView(CString strLine, int devID)
{
	std::vector<CString> vtCell;
	CGbl::SpliteBy(strLine, "\t", vtCell);
	if (vtCell.size() < 40)								return false;
	CString str = vtCell[40];
	if (vtCell[1].Trim() == _T(""))						return false;
	if (vtCell[6].Trim() == _T(""))						return false;
	if (vtCell[7].Trim() == _T(""))						return false;

	CString strID = vtCell[0].Trim();					//!< 变量编号
	m_strName = vtCell[1].Trim();						//!< 变量名称
	m_strTag = vtCell[2].Trim();						//!< 变量标签
	m_strDescription = vtCell[3].Trim();				//!< 变量备注
	setDefault(vtCell[5].Trim().GetBuffer());			//!< 变量默认值
	setSrcType((UINT)atoi(vtCell[6].Trim()));			//!< 变量数据源类型
	setAccessRight((UINT)atoi(vtCell[7].Trim()));		//!< 变量反问权限
	setReservFlag(vtCell[8].Trim() == "1");				//!< 变量保留值
	setReservDB(vtCell[9].Trim() == "1");				//!< 变量保留历史数据

	m_spSrcInfo->ReadFromDViewExcel(vtCell);			//!< 解析数据源属性
	m_spSrcInfo->setDevID(devID);						//!< 设置所属设备
	m_spAlarmInfo->ReadFromDViewExcel(vtCell);			//!< 解析报警属性

	setValType((UINT)atoi(vtCell[4].Trim())); 			//!< 变量类型
	return true;
}

//!< 确认变量的类型
void CItem::SetVarType()
{
	//!< 修改VARIANT变量的类型
	if(m_uiValType == 0)			ChangeVarType(VT_BOOL);
	else if(m_uiValType == 1)		ChangeVarType(VT_I1);
	else if(m_uiValType == 2)		ChangeVarType(VT_UI1);
	else if(m_uiValType == 3)		ChangeVarType(VT_I2);
	else if(m_uiValType == 4)		ChangeVarType(VT_UI2);
	else if(m_uiValType == 5)		ChangeVarType(VT_I4);
	else if(m_uiValType == 6)		ChangeVarType(VT_UI4);
	else if(m_uiValType == 7)		ChangeVarType(VT_R4);
	else if(m_uiValType == 8)		ChangeVarType(VT_R8);
	else							ChangeVarType(VT_BSTR);
}

//!< 根据valType定义的类型，将变量各种variant类型的属性转成真正对应的属性
void CItem::ChangeVarType(UINT type)
{
	if(type == VT_BOOL)			m_cvrDefaultValue = m_cvrValue = m_cvrValueOld = false;
	else if(type == VT_I1)		m_cvrDefaultValue = m_cvrValue = m_cvrValueOld = char(0);
	else if(type == VT_UI1)		m_cvrDefaultValue = m_cvrValue = m_cvrValueOld = byte(0);
	else if(type == VT_I2)		m_cvrDefaultValue = m_cvrValue = m_cvrValueOld = short(0);
	else if(type == VT_UI2)		m_cvrDefaultValue = m_cvrValue = m_cvrValueOld = unsigned short(0);
	else if(type == VT_I4)		m_cvrDefaultValue = m_cvrValue = m_cvrValueOld = int(0);
	else if(type == VT_UI4)		m_cvrDefaultValue = m_cvrValue = m_cvrValueOld = UINT(0);
	else if(type == VT_R4)		m_cvrDefaultValue = m_cvrValue = m_cvrValueOld = float(0.0);
	else if(type==VT_R8)		m_cvrDefaultValue = m_cvrValue = m_cvrValueOld = double(0.0);
}

//!< 设置变量的值类型
void CItem::setValType(UINT type)
{
	if(m_uiValType != type){
		m_uiValType = type;
		if(m_uiValType == 0)			ChangeVarType(VT_BOOL);
		else if(m_uiValType == 1)		ChangeVarType(VT_I1);
		else if(m_uiValType == 2)		ChangeVarType(VT_UI1);
		else if(m_uiValType == 3)		ChangeVarType(VT_I2);
		else if(m_uiValType == 4)		ChangeVarType(VT_UI2);
		else if(m_uiValType == 5)		ChangeVarType(VT_I4);
		else if(m_uiValType == 6)		ChangeVarType(VT_UI4);
		else if(m_uiValType == 7)		ChangeVarType(VT_R4);
		else if(m_uiValType == 8)		ChangeVarType(VT_R8);
		else							ChangeVarType(VT_BSTR);
	}
	if(m_uiValType == 0){	//!< 位变量
		getSrcInfo()->setConvertType(0);
		getAlarmInfo()->setLoloActive(false);
		getAlarmInfo()->setLowActive(false);
		getAlarmInfo()->setHighActive(false);
		getAlarmInfo()->setHihiActive(false);
		getAlarmInfo()->setAimActive(false);
		getAlarmInfo()->setShiftActive(false);
	}
	else
	{
		getAlarmInfo()->setBitAlarmType(0);
	}
}

//!< 设置变量的值类型
void CItem::setValType(CString strType)
{
	if (strType == _T("位变量"))					setValType(0);
	else if (strType == _T("有符号双字节"))		setValType(3);
	else if (strType == _T("无符号双字节"))		setValType(4);
	else if (strType == _T("有符号四字节"))		setValType(5);
	else if (strType == _T("无符号四字节"))		setValType(6);
	else if (strType == _T("四字节浮点数"))		setValType(7);
}

CItem& CItem::operator = (CItem& item)
{
	setID(item.getID());
	setItemName(item.getName());
	setTag(item.getTag());
	setDescription(item.getDescription());
	setMyGroupID(item.getMyGroupID());
	setDefault(item.getDefault());
	setValType(item.getValType());
	setSrcType(item.getSrcType());
	setAccessRight(item.getAccessRight());
	setReservFlag(item.getReservFlag());
	setReservDB(item.getReservDB());
	setModbus485(item.getModbus485());

	(*m_spSrcInfo) = (*item.getSrcInfo());
	(*m_spAlarmInfo) = (*item.getAlarmInfo());
	return *this;
}

bool CItem::operator == (CItem &item) const
{
	if (m_strName != item.getName())					return false;	//!< 变量名称
	if (m_strTag != item.getTag())						return false;	//!< 变量标签
	if (m_strDescription != item.getDescription())		return false;	//!< 变量备注
	if (m_uiGroupID != item.getMyGroupID())				return false;	//!< 变量组
	if (m_cvrDefaultValue != item.getDefault())			return false;	//!< 变量默认值
	if (m_uiValType != item.getValType())				return false;	//!< 变量类型
	if (m_uiSrcType != item.getSrcType())				return false;	//!< 数据源类型，0内存变量，1I/O变量
	if (m_uiAccessRight != item.getAccessRight())		return false;	//!< 访问权限，0可读可写，1只读，只写
	if (m_bReservFlag != item.getReservFlag())			return false;	//!< 是否设置成保留值，true设置，false不设置
	if (m_bReservDB != item.getReservDB())				return false;	//!< 是否保存历史数据，true保留，false不保留
	if (m_bModbus485 != item.getModbus485())			return false;	//!< 是否导出Modbus变量

	if (!(*m_spSrcInfo == *item.getSrcInfo()))			return false;
	if (!(*m_spAlarmInfo == *item.getAlarmInfo()))		return false;
	return true;
}

//!< 要克隆一个自己，变量组里还有一个变量的ID链表，这里管不了，得在别的地方做
bool CItem::OnCloneMe(CItem& item, int index, int addrUnit, int nType /* = 0 */)
{
	item = *this;
	CItemMgr* itemMgr = &CItemMgr::GetMe();
	item.setMyGroupID(getMyGroupID());
	//!< 对IO变量特殊处理
	if(m_uiSrcType > 0)		OnCloneDealIOInfo(item, index * addrUnit);

	//!< 
	item.setID(UINT(-1));
	CString name = getName();
	CString text;
	if(nType == 0)
	{
		int number = 0;
		CGbl::GetNumberFromString(name, number, 1);
		CString strNumber;
		strNumber.Format("%d",number);
		int clonenumber = SoftInfo::CSoftInfo::GetMe().getCloneNumber();
		text.Format("%d", number + index * clonenumber);
		if(text.GetLength() > strNumber.GetLength() && !name.IsEmpty() && name.Right(1) == _T("0")){
			name = name.Left(name.GetLength() - 1);
		}
		name += text;
	}
	else
	{
		CString strInt, strDeci;
		CGbl::GetFloatFromString(name, strInt, strDeci, 1);
		if(strInt == _T(""))		strDeci = _T("0");
		if(strDeci == _T(""))		strDeci = _T("0");
		int uiInt, uiDeci;
		uiInt = atoi(strInt);
		uiDeci = atoi(strDeci);
		int uiFlt = SoftInfo::CSoftInfo::GetMe().getCloneFloat();
		if(uiDeci >= uiFlt)
		{
			uiDeci = 0;
			++uiInt;
		}
		uiDeci += index;
		while(uiDeci >= uiFlt)
		{
			uiDeci -= uiFlt;
			++uiInt;
		}
		while(uiDeci < 0)
		{
			uiDeci += uiFlt;
			--uiInt;
		}
		text.Format("%d.%d", uiInt, uiDeci);
		name += text;
	}
	if(itemMgr->GetItem(name))
	{
		CString strNewName = name;
		strNewName = strNewName + _T("多重复制0");
		strNewName = itemMgr->GetSimilarName(strNewName);
		text = _T("变量“")+ name +_T("”已存在，新增的多重复制变量名改为“") + strNewName + _T("”");
		name = strNewName;
		CGbl::PrintOut(text);
	}
	item.setItemName(name);
	return true;
}

void CItem::OnCloneDealIOInfo(CItem& item, int addrUnit)
{
	std::shared_ptr<MVC::Device::CDeviceOne> projDev = MVC::Device::CDevMgr::GetMe().GetDevice(item.getSrcInfo()->getDeviceID());
	if(!projDev)		return;
	std::shared_ptr<XmlInfo::CXmlDevice> xmlDev = projDev->GetXmlInfo();
	if(!xmlDev)			return;
	std::shared_ptr<XmlInfo::CXmlArea> xmlArea = xmlDev->getArea(item.getSrcInfo()->getAreaID());
	if(!xmlArea)		return;
	UINT bitLen = xmlArea->m_uiUnitBitLen;			//!< 获得寻址长度，PEC8000是字寻址，这里是16

	UINT ioType = item.getSrcInfo()->GetOperateType();
	if(ioType == 0)		//!< 如果操作类型是位操作，就在位偏移上 + addrUnit
	{
		item.getSrcInfo()->setBitIndex(item.getSrcInfo()->getBitIndex() + addrUnit);
		if(item.getSrcInfo()->getBitIndex() >= bitLen)
		{
			UINT unitNum = (UINT)item.getSrcInfo()->getBitIndex() / bitLen;
			item.getSrcInfo()->setBitIndex(item.getSrcInfo()->getBitIndex() % bitLen);
			item.getSrcInfo()->setUnitIndex(item.getSrcInfo()->getUnitIndex() + unitNum);
		}
	}
	else if(ioType == 1)//!< 如果操作类型不是位操作，就在寻址偏移上 + addrUnit
	{
		item.getSrcInfo()->setUnitIndex(item.getSrcInfo()->getUnitIndex() + addrUnit);
	}
	else if(ioType == 2)//!< 如果操作类型不是位操作，就在寻址偏移上 + index
	{
		item.getSrcInfo()->setUnitIndex(item.getSrcInfo()->getUnitIndex() + addrUnit);
	}
	else if(ioType == 4)//!< 如果操作类型不是位操作，就在寻址偏移上 + index
	{
		item.getSrcInfo()->setUnitIndex(item.getSrcInfo()->getUnitIndex() + addrUnit);
	}
}

//!< 获得一个检验值
CString CItem::GetCheckValue()
{
	CString value;
	value.Format("%d", getID() + 1);	//!< +1 是为了防止有些公式写0下去非法，比如除数不能是0
	if(m_uiValType == 0)			return _T("1.000001");
	else							return _T("0.000001");
// 	else if(m_uiValType == 1)		return value;
// 	else if(m_uiValType == 2)		return value;
// 	else if(m_uiValType == 3)		return value;
// 	else if(m_uiValType == 4)		return value;
// 	else if(m_uiValType == 5)		return value;
// 	else if(m_uiValType == 6)		return value;
// 	else if(m_uiValType == 7)		return value + _T(".1");
// 	else if(m_uiValType == 8)		return value + _T(".1");
	return value;
}

//!< 以字符串的形式返回工程值类型
CString CItem::GetValTypeStr()
{
	CString type[] = {_T("位变量"), _T("有符号字节"), _T("无符号字节"), _T("有符号字"), _T("无符号字"), \
		_T("有符号双字"), _T("无符号双字"), _T("浮点"), _T("双精度"), _T("字符串")};
	if(m_uiValType > 9)		return _T("其它类型");
	return type[m_uiValType];
}

//!< 返回变量所属组的名称
CString CItem::GetGroupName()
{
	std::shared_ptr<CItemGroup> group = CItemMgr::GetMe().GetGroup(getMyGroupID());
	ASSERT(group);
	if(!group)		return _T("");
	return group->getName();
}

//!< 设置报警
void CItem::SetAlarm(UINT type, bool alarm /* = false */)
{
	bool bAlarmOld = false, bAlarmNew = false;

	//!< 获得原始的报警颜色
	bAlarmOld = IsAlarm();

	if(type == 0)			m_bAlarmBit = alarm;
	else if(type == 1)		m_bAlarmLolo = alarm;
	else if(type == 2)		m_bAlarmLow = alarm;
	else if(type == 3)		m_bAlarmHigh = alarm;
	else if(type == 4)		m_bAlarmHihi = alarm;
	else if(type == 5)		m_bAlarmAim = alarm;
	else if(type == 6)		m_bAlarmShift = alarm;

	//!< 获得现在的报警颜色
	bAlarmNew = IsAlarm();

	//!< 颜色不同，证明需要重绘
	CItemMgr* itemMgr = &CItemMgr::GetMe();
	if(bAlarmOld != bAlarmNew){
		itemMgr->AddEditItem(getID());
	}
	itemMgr->SetEditEnd();
}

bool CItem::IsAlarm()
{
	if(getValType() == 0)		return IsAlarmBit();
	return IsAlarmLolo() | IsAlarmLow() | IsAlarmHigh() | IsAlarmHihi() | IsAlarmAim() | IsAlarmShift();
}

//!< 监控初始化，清楚状态
void CItem::WatchInit()
{
	SetAlarm(0, false);		//!< 位变量是否报警
	SetAlarm(1, false);		//!< 下下限是否报警
	SetAlarm(2, false);		//!< 下限是否报警
	SetAlarm(3, false);		//!< 上限是否报警
	SetAlarm(4, false);		//!< 上上限是否报警
	SetAlarm(5, false);		//!< 目标是否报警
	SetAlarm(6, false);		//!< 变化率是否报警
}

//!< 判断变量值是否改变
bool CItem::IsValueChange()
{
	int a[4], b[4];
	memcpy(a, &m_cvrValue, sizeof(CComVariant));
	memcpy(b, &m_cvrValueOld, sizeof(CComVariant));
	if(a[0] == b[0] && a[1] == b[1] && a[2] == b[2] && a[3] == b[3])
		return false;
	m_cvrValueOld = m_cvrValue;
	return true;
}

//!< 判断变量原始值是否改变
bool CItem::IsIOValueChange()
{
	int a[4], b[4];
	memcpy(a, &m_cvrIOValue, sizeof(CComVariant));
	memcpy(b, &m_cvrIOValueOld, sizeof(CComVariant));
	if(a[0] == b[0] && a[1] == b[1] && a[2] == b[2] && a[3] == b[3])
		return false;
	m_cvrIOValueOld = m_cvrIOValue;
	return true;
}

//!< 查找本变量，要匹配的文本，全字符匹配，大小写匹配
bool CItem::DoSearch(CString str, bool bMatchWhole, bool bAllCase, bool bRegex /* = false */)
{
	CString strInfo;
	bool bMatch = false;
	int nMatchCount = 0;
	CComVariant cvr;
	strInfo.Format("%d", m_uiID);
	strInfo = _T("Information:变量") + strInfo + _T(":变量 ") + m_strName + _T(" （编号") + strInfo + _T("）  ——  ");

	//!< 名称是否匹配
	cvr = m_strName;
	if(CGbl::SearchT(cvr, str, bMatchWhole, bAllCase, bRegex, _T("名称"), strInfo))		++nMatchCount;

	//!< 编号是否匹配
	cvr = m_uiID;
	if(CGbl::SearchT(cvr, str, bMatchWhole, bAllCase, bRegex, _T("编号"), strInfo))		++nMatchCount;

	//!< 标签是否匹配
	cvr = m_strTag;
	if(CGbl::SearchT(cvr, str, bMatchWhole, bAllCase, bRegex, _T("标签"), strInfo))		++nMatchCount;

	//!< 备注是否匹配
	cvr = m_strDescription;
	if(CGbl::SearchT(cvr, str, bMatchWhole, bAllCase, bRegex, _T("备注"), strInfo))		++nMatchCount;

	//!< 地址是否匹配
	if(getSrcType() == SRC_TYPE_IO)
	{
		MVC::Device::CDevMgr* devMgr = &MVC::Device::CDevMgr::GetMe();
		std::shared_ptr<MVC::Device::CDeviceOne> dev = devMgr->GetDevice(getSrcInfo()->getDeviceID());
		if(dev){
			cvr = dev->getName();
			if(CGbl::SearchT(cvr, str, bMatchWhole, bAllCase, bRegex, _T("所属设备"), strInfo))	++nMatchCount;
		}

		cvr = getSrcInfo()->GetAreaString();
		if(CGbl::SearchT(cvr, str, bMatchWhole, false, bRegex, _T("地址"), strInfo))		++nMatchCount;
	}
	//!< 脚本是否匹配
	else if(getSrcType() == SRC_TYPE_MEM)
	{
		cvr = getSrcInfo()->getScriptText();
		if(CGbl::SearchT(cvr, str, bMatchWhole, bAllCase, bRegex, _T("脚本"), strInfo))	++nMatchCount;
	}

	if(nMatchCount > 0){
		CGbl::PrintOut(strInfo);
		return true;
	}
	return false;
}
CString CItem::getStrValue(bool ishex)
{
	CString temp;
	CComVariant cvr = getValue();
	switch (cvr.vt)
	{
	case VT_BOOL:	temp = cvr.boolVal ? _T("1") : _T("0");	break;
	case VT_I1:		ishex ? temp.Format("0x%.2X", (byte)cvr.cVal) : temp.Format("%d", cvr.cVal);	break;
	case VT_I2:		ishex ? temp.Format("0x%.4X", (WORD)cvr.iVal) : temp.Format("%d", cvr.iVal);	break;
	case VT_I4:		ishex ? temp.Format("0x%.8X", (UINT)cvr.intVal) : temp.Format("%d", cvr.intVal);	break;
	case VT_UI1:	ishex ? temp.Format("0x%.2X", (byte)cvr.bVal) : temp.Format("%d", cvr.bVal);	break;
	case VT_UI2:	ishex ? temp.Format("0x%.4X", (byte)cvr.uiVal) : temp.Format("%d", cvr.intVal);	break;
	case VT_UI4:	ishex ? temp.Format("0x%.8X", (byte)cvr.uintVal) : temp.Format("%d", cvr.uintVal);	break;
	case VT_R4:		temp = GetFloatStr(cvr.fltVal);	break;
	default:
		if (cvr.vt != VT_BSTR) cvr.ChangeType(VT_BSTR);
		temp = cvr.bstrVal;
	}
	return temp;
}
CString CItem::getStrIOValue(bool ishex)
{	
	CString temp;
	CComVariant cvr = m_cvrIOValue;
	switch (cvr.vt)
	{
	case VT_BOOL:	temp = cvr.boolVal ? _T("1") : _T("0");	break;
	case VT_I1:		ishex ? temp.Format("0x%.2X", (byte)cvr.cVal) : temp.Format("%d", cvr.cVal);	break;
	case VT_I2:		ishex ? temp.Format("0x%.4X", (WORD)cvr.iVal) : temp.Format("%d", cvr.iVal);	break;
	case VT_I4:		ishex ? temp.Format("0x%.8X", (UINT)cvr.intVal) : temp.Format("%d", cvr.intVal);	break;
	case VT_UI1:	ishex ? temp.Format("0x%.2X", (byte)cvr.bVal) : temp.Format("%d", cvr.bVal);	break;
	case VT_UI2:	ishex ? temp.Format("0x%.4X", (byte)cvr.uiVal) : temp.Format("%d", cvr.intVal);	break;
	case VT_UI4:	ishex ? temp.Format("0x%.8X", (byte)cvr.uintVal) : temp.Format("%d", cvr.uintVal);	break;
	case VT_R4:		temp = GetFloatStr(cvr.fltVal);	break;
	default:
		if (cvr.vt != VT_BSTR) cvr.ChangeType(VT_BSTR);
		temp = cvr.bstrVal;
	}
	return temp;
}
CString CItem::GetFloatStr(float flt)
{
	UINT width = SoftInfo::CSoftInfo::GetMe().getItemFloatWidth();
	CString strFormat;
	strFormat.Format("%d", width);
	strFormat = _T("\%.") + strFormat + _T("f");
	CString strRet;
	strRet.Format(strFormat, flt);
	return strRet;
}
