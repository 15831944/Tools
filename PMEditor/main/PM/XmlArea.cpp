#include "StdAfx.h"
#include "XmlArea.h"

using namespace XmlInfo;

const CString OPERATE_TYPE = _T("OperateType");
const CString OPERATE_NAME = _T("OperateName");
const CString OPERATE_BITIDX = _T("ReviseBitIdx");
const CString OPERATE_START = _T("UnitOperateStart");
const CString OPERATE_END = _T("UnitOperateEnd");

//!< 判断两个操作是否相同
bool CXmlOperate::operator == (CXmlOperate* oper)
{
	if(m_uiType != oper->m_uiType)					return false;
	if(m_strName != oper->m_strName)				return false;
	if(m_uiReviseBitIdx != oper->m_uiReviseBitIdx)	return false;
	if(m_uiOperateStart != oper->m_uiOperateStart)	return false;
	if(m_uiOperateEnd != oper->m_uiOperateEnd)		return false;
	return true;
}

//!< 解析描述文件
bool CXmlOperate::SerializeXml(TiXmlElement* pNode)
{
	CComVariant cvr;
	TiXmlElement* pElement = pNode->FirstChildElement();
	int iValue;
	while(pElement){
		CString name = pElement->Value();
		cvr = pElement->GetText();
		if(OPERATE_TYPE == name)		{cvr.ChangeType(VT_I4);		iValue = cvr.intVal;	m_uiType = UINT(iValue);}
		else if(OPERATE_NAME == name)	{m_strName = cvr;}
		else if(OPERATE_BITIDX == name)	{cvr.ChangeType(VT_I4);		iValue = cvr.intVal;	m_uiReviseBitIdx = UINT(iValue);}
		else if(OPERATE_START == name)	{cvr.ChangeType(VT_I4);		iValue = cvr.intVal;	m_uiOperateStart = UINT(iValue);}
		else if(OPERATE_END == name)	{cvr.ChangeType(VT_I4);		iValue = cvr.intVal;	m_uiOperateEnd = UINT(iValue);}
		pElement = pElement->NextSiblingElement();
	}
	return true;
}
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
const CString ROOTNAME = _T("Area");
const CString AREAID = _T("AreaID");
const CString AREANAME = _T("AreaName");
const CString UNITBITNUM = _T("ItemUnitBitLen");
const CString UNITBITSTART = _T("UnitBitReadStart");
const CString UNITBITEND = _T("UnitBitReadEnd");
const CString UNITNUM = _T("UnitNum");
const CString FIRSTUNIT = _T("FirstUnitIndex");
const CString PROTOCOLLIST = _T("ProtocolUsedList");
const CString PROTOCOLUSED = _T("ProtocolUsed");
const CString OPERATELIST = _T("AreaOperateList");

const CString OPERATE = _T("AreaOperate");
const CString SUPPORTUNITCOUNT = _T("OperateType");

const CString IOTYPE_BIT = _T("位(bit)");
const CString IOTYPE_CHAR = _T("有符号字节(char)");
const CString IOTYPE_BYTE = _T("无符号字节(byte)");
const CString IOTYPE_SHORT = _T("有符号字(short)");
const CString IOTYPE_USHORT = _T("无符号字(unsigned short)");
const CString IOTYPE_INT32 = _T("有符号双字(int32)");
const CString IOTYPE_UINT32 = _T("无符号双字(unsigned int32)");
const CString IOTYPE_FLOAT = _T("浮点(float)");


CXmlArea::CXmlArea(void)
{
	m_nUnitBitReadStart = 0;
	m_nUnitBitReadEnd = 15;
}

CXmlArea::~CXmlArea(void)
{
}

//!< 解析自己的节点
bool CXmlArea::SerializeXml(TiXmlElement* pNode)
{
	CString name = pNode->Value();
	CComVariant cvr;
	if(name != ROOTNAME)	return false;
	TiXmlElement* pElement = pNode->FirstChildElement();
	int iValue;
	m_ltOperate.clear();
	while(pElement){
		name = pElement->Value();
		cvr = pElement->GetText();
		if(AREAID == name)			{cvr.ChangeType(VT_I4);		iValue = cvr.intVal;	m_uiID = UINT(iValue);}
		else if(AREANAME == name)	{m_strName = cvr;}
		else if(UNITBITNUM == name)	{cvr.ChangeType(VT_I4);		iValue = cvr.intVal;	m_uiUnitBitLen = UINT(iValue);}
		else if(UNITBITSTART == name){cvr.ChangeType(VT_I4);	iValue = cvr.intVal;	m_nUnitBitReadStart = iValue;}
		else if(UNITBITEND == name)	{cvr.ChangeType(VT_I4);		iValue = cvr.intVal;	m_nUnitBitReadEnd = iValue;}
		else if(PROTOCOLLIST == name){
			TiXmlElement* pChild = pElement->FirstChildElement();
			while(pChild){SerializeProtocol(pChild);	pChild = pChild->NextSiblingElement();}
		}
		else if(name == OPERATELIST){
			TiXmlElement* pChild = pElement->FirstChildElement();
			while(pChild){SerializeOperate(pChild);		pChild = pChild->NextSiblingElement();}
		}
		pElement = pElement->NextSiblingElement();
	}
	return true;
}

void CXmlArea::SerializeProtocol(TiXmlElement* pNode)
{
	CString name = pNode->Value();
	if(PROTOCOLUSED != name)		return;
	CComVariant cvr = pNode->GetText();
	cvr.ChangeType(VT_I4);
	int iValue = cvr.intVal;
	m_ltProtocolID.push_back(UINT(iValue));
}

void CXmlArea::SerializeOperate(TiXmlElement* pNode)
{
	CString name = pNode->Value();
	if(OPERATE != name)				return;
	boost::shared_ptr<CXmlOperate> xmlOper = boost::shared_ptr<CXmlOperate>(new CXmlOperate);
	xmlOper->SerializeXml(pNode);
	m_ltOperate.push_back(xmlOper);
// 	CComVariant cvr;
// 	TiXmlElement* pChild = pNode->FirstChildElement();
// 	while(pChild){
// 		name = pChild->Value();
// 		cvr = pChild->GetText();
// 		if(SUPPORTUNITCOUNT == name){cvr.ChangeType(VT_R4);		m_ltSupportUnitCount.push_back(cvr.fltVal);}
// 		pChild = pChild->NextSiblingElement();
// 	}
}

bool CXmlArea::operator ==(CXmlArea* area)
{
	if(area == NULL)	return false;
	if(this == area)	return true;
	if(m_strName != area->m_strName)					return false;
	if(m_uiUnitBitLen != area->m_uiUnitBitLen)			return false;
	if(m_ltOperate.size() != area->m_ltOperate.size())	return false;
	std::list<boost::shared_ptr<CXmlOperate> >::iterator thisItor, areaItor;
	for(thisItor = m_ltOperate.begin(), areaItor = area->m_ltOperate.begin(); thisItor != m_ltOperate.end(); ++thisItor, ++areaItor)
	{
		if(!(*thisItor == *areaItor))					return false;
	}

	return true;
}

//!< 判断两个区描述是否一样
bool CXmlArea::IsSame(CXmlArea* area)
{
	if(area == NULL)	return false;
	if(this == area)	return true;
	if(m_strName != area->m_strName)					return false;
	if(m_uiUnitBitLen != area->m_uiUnitBitLen)			return false;
	if(m_ltOperate.size() != area->m_ltOperate.size())	return false;
	std::list<boost::shared_ptr<CXmlOperate> >::iterator thisItor, areaItor;
	for(thisItor = m_ltOperate.begin(), areaItor = area->m_ltOperate.begin(); thisItor != m_ltOperate.end(); ++thisItor, ++areaItor)
	{
		if(!(*thisItor == *areaItor))					return false;
	}

	return true;
}

//!< 获得区所支持的操作字符串列表
//!< defIndex在传入时，外部环境需要付给他一个值，这个值要等于操作类型链表中某一项
//!< defIndex在传出时，表示的是该类型在list中的索引
std::list<CString> CXmlArea::GetSupportUnitList(int& defIndex)
{
	std::list<CString> ltStr;
	CString text;
	UINT bitLen, byteLen, index = 0;
	UINT listIndex = UINT(-1);
	boost::shared_ptr<CXmlOperate> xmlOper;
	foreach(xmlOper, m_ltOperate)
	{
		bitLen = xmlOper->m_uiType * m_uiUnitBitLen;	//!< 获得当前操作的操作位数
		byteLen = bitLen / 8;							//!< 计算当前操作的操作字节数
		if(byteLen == 0)
		{
			ltStr.push_back(IOTYPE_BIT);
			if(defIndex == 0)	listIndex = index;
			++index;
		}
		else if(byteLen == 1)
		{
			ltStr.push_back(IOTYPE_CHAR);
			if(defIndex == 1)	listIndex = index;
			++index;
			ltStr.push_back(IOTYPE_BYTE);
			if(defIndex == 2)	listIndex = index;
			++index;
		}
		else if(byteLen == 2)
		{
			ltStr.push_back(IOTYPE_SHORT);
			if(defIndex == 3)	listIndex = index;
			++index;
			ltStr.push_back(IOTYPE_USHORT);
			if(defIndex == 4)	listIndex = index;
			++index;
		}
		else if(byteLen == 4)
		{
			ltStr.push_back(IOTYPE_INT32);
			if(defIndex == 5)	listIndex = index;
			++index;
			ltStr.push_back(IOTYPE_UINT32);
			if(defIndex == 6)	listIndex = index;
			++index;
			ltStr.push_back(IOTYPE_FLOAT);
			if(defIndex == 7)	listIndex = index;
			++index;
		}
	}
	defIndex = listIndex;
	return ltStr;
}

//!< 通过用户所选择的操作类型获得对应的操作ID
UINT CXmlArea::GetSupportType(CString text)
{
	if(IOTYPE_BIT == text)				return 0;
	else if(IOTYPE_CHAR == text)		return 1;
	else if(IOTYPE_BYTE == text)		return 2;
	else if(IOTYPE_SHORT == text)		return 3;
	else if(IOTYPE_USHORT == text)		return 4;
	else if(IOTYPE_INT32 == text)		return 5;
	else if(IOTYPE_UINT32 == text)		return 6;
	else if(IOTYPE_FLOAT == text)		return 7;
	return UINT(-1);
}

//!< 根据数据类型，找到对应的操作类型
UINT CXmlArea::GetOperType(UINT valType)
{
	UINT bitLen = 1;
	if(valType == 1 || valType == 2)						bitLen = 1 * 8;
	else if(valType == 3 || valType == 4)					bitLen = 2 * 8;
	else if(valType == 5 || valType == 6 || valType == 7)	bitLen = 4 * 8;
	return (UINT)(bitLen / m_uiUnitBitLen);					//!< 操作类型 = 总位数/单元位数
}

//!< 获得寻址单位的名字（如位、字节、字、双字等）
CString CXmlArea::GetUnitName()
{
	UINT byteCount = m_uiUnitBitLen / 8;
	if(byteCount == 0)			return _T("位");
	else if(byteCount == 1)		return _T("字节");
	else if(byteCount == 2)		return _T("单字");
	else if(byteCount == 4)		return _T("双字");
	CString text;
	text.Format("%d位",m_uiUnitBitLen);
	return text;
}

//!< 获得该操作的范围信息
CString CXmlArea::GetOperateRange(UINT type)
{
	foreach(boost::shared_ptr<CXmlOperate> oper, m_ltOperate)
	{
		if(oper->m_uiType == type)
		{
			CString text;
			text.Format("起始地址 %d， 终止地址 %d", oper->m_uiOperateStart, oper->m_uiOperateEnd);
			return text;
		}
	}
	return _T("");
}

//!< 获得该类型寻址的最大寻址值
UINT CXmlArea::GetUnitEnd(UINT type)
{
	UINT operType = GetOperType(type);
	foreach(boost::shared_ptr<CXmlOperate> oper, m_ltOperate)
		if(oper->m_uiType == operType)
			return oper->m_uiOperateEnd;
	return 0;
}
