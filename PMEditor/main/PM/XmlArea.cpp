#include "StdAfx.h"
#include "XmlArea.h"

using namespace XmlInfo;

const CString OPERATE_TYPE = _T("OperateType");
const CString OPERATE_NAME = _T("OperateName");
const CString OPERATE_BITIDX = _T("ReviseBitIdx");
const CString OPERATE_START = _T("UnitOperateStart");
const CString OPERATE_END = _T("UnitOperateEnd");

//!< �ж����������Ƿ���ͬ
bool CXmlOperate::operator == (CXmlOperate* oper)
{
	if(m_uiType != oper->m_uiType)					return false;
	if(m_strName != oper->m_strName)				return false;
	if(m_uiReviseBitIdx != oper->m_uiReviseBitIdx)	return false;
	if(m_uiOperateStart != oper->m_uiOperateStart)	return false;
	if(m_uiOperateEnd != oper->m_uiOperateEnd)		return false;
	return true;
}

//!< ���������ļ�
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

const CString IOTYPE_BIT = _T("λ(bit)");
const CString IOTYPE_CHAR = _T("�з����ֽ�(char)");
const CString IOTYPE_BYTE = _T("�޷����ֽ�(byte)");
const CString IOTYPE_SHORT = _T("�з�����(short)");
const CString IOTYPE_USHORT = _T("�޷�����(unsigned short)");
const CString IOTYPE_INT32 = _T("�з���˫��(int32)");
const CString IOTYPE_UINT32 = _T("�޷���˫��(unsigned int32)");
const CString IOTYPE_FLOAT = _T("����(float)");


CXmlArea::CXmlArea(void)
{
	m_nUnitBitReadStart = 0;
	m_nUnitBitReadEnd = 15;
}

CXmlArea::~CXmlArea(void)
{
}

//!< �����Լ��Ľڵ�
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

//!< �ж������������Ƿ�һ��
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

//!< �������֧�ֵĲ����ַ����б�
//!< defIndex�ڴ���ʱ���ⲿ������Ҫ������һ��ֵ�����ֵҪ���ڲ�������������ĳһ��
//!< defIndex�ڴ���ʱ����ʾ���Ǹ�������list�е�����
std::list<CString> CXmlArea::GetSupportUnitList(int& defIndex)
{
	std::list<CString> ltStr;
	CString text;
	UINT bitLen, byteLen, index = 0;
	UINT listIndex = UINT(-1);
	boost::shared_ptr<CXmlOperate> xmlOper;
	foreach(xmlOper, m_ltOperate)
	{
		bitLen = xmlOper->m_uiType * m_uiUnitBitLen;	//!< ��õ�ǰ�����Ĳ���λ��
		byteLen = bitLen / 8;							//!< ���㵱ǰ�����Ĳ����ֽ���
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

//!< ͨ���û���ѡ��Ĳ������ͻ�ö�Ӧ�Ĳ���ID
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

//!< �����������ͣ��ҵ���Ӧ�Ĳ�������
UINT CXmlArea::GetOperType(UINT valType)
{
	UINT bitLen = 1;
	if(valType == 1 || valType == 2)						bitLen = 1 * 8;
	else if(valType == 3 || valType == 4)					bitLen = 2 * 8;
	else if(valType == 5 || valType == 6 || valType == 7)	bitLen = 4 * 8;
	return (UINT)(bitLen / m_uiUnitBitLen);					//!< �������� = ��λ��/��Ԫλ��
}

//!< ���Ѱַ��λ�����֣���λ���ֽڡ��֡�˫�ֵȣ�
CString CXmlArea::GetUnitName()
{
	UINT byteCount = m_uiUnitBitLen / 8;
	if(byteCount == 0)			return _T("λ");
	else if(byteCount == 1)		return _T("�ֽ�");
	else if(byteCount == 2)		return _T("����");
	else if(byteCount == 4)		return _T("˫��");
	CString text;
	text.Format("%dλ",m_uiUnitBitLen);
	return text;
}

//!< ��øò����ķ�Χ��Ϣ
CString CXmlArea::GetOperateRange(UINT type)
{
	foreach(boost::shared_ptr<CXmlOperate> oper, m_ltOperate)
	{
		if(oper->m_uiType == type)
		{
			CString text;
			text.Format("��ʼ��ַ %d�� ��ֹ��ַ %d", oper->m_uiOperateStart, oper->m_uiOperateEnd);
			return text;
		}
	}
	return _T("");
}

//!< ��ø�����Ѱַ�����Ѱֵַ
UINT CXmlArea::GetUnitEnd(UINT type)
{
	UINT operType = GetOperType(type);
	foreach(boost::shared_ptr<CXmlOperate> oper, m_ltOperate)
		if(oper->m_uiType == operType)
			return oper->m_uiOperateEnd;
	return 0;
}
