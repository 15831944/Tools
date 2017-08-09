#include "StdAfx.h"
#include "XmlEnum.h"

using namespace XmlInfo;

const CString ROOTNAME = _T("Enum");
const CString ENUMID = _T("EnumID");
const CString ENUMNAME = _T("EnumName");
const CString UNITLIST = _T("UnitList");
const CString UNIT = _T("Unit");
const CString UNITID = _T("UnitID");
const CString UNITVALUE = _T("Value");
const CString UNITMEANING = _T("Meaning");

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
bool CXmlEnumItem::SerializeXml(TiXmlElement* pNode)
{
	CString name = pNode->Value();
	CComVariant cvr;
	if(UNIT != name)		return false;
	TiXmlElement* pRoot = pNode->FirstChildElement();
	boost::shared_ptr<CXmlEnumItem> pItem;
	int iValue;
	while(pRoot){
		name = pRoot->Value();
		cvr = pRoot->GetText();
		if(UNITID == name){			cvr.ChangeType(VT_I4);		iValue = cvr.intVal;	m_uiID = UINT(iValue);}
		else if(UNITVALUE == name){	cvr.ChangeType(VT_I4);		m_nValue = cvr.intVal;	}
		else if(UNITMEANING == name){							m_strShowText = cvr;	}
		pRoot = pRoot->NextSiblingElement();
	}
	return m_uiID != UINT(-1);
}


//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//!< 解析自己的节点
bool CXmlEnum::SerializeXml(TiXmlElement* pNode)
{
	CString name = pNode->Value();
	CComVariant cvr;
	if(ROOTNAME != name)		return false;
	TiXmlElement* pRoot = pNode->FirstChildElement();
	boost::shared_ptr<CXmlEnumItem> pItem;
	int iValue;
	m_vtEnumItem.clear();
	while(pRoot){
		name = pRoot->Value();
		cvr = pRoot->GetText();
		if(ENUMID == name){			cvr.ChangeType(VT_I4);		iValue = cvr.intVal;	m_uiID = UINT(iValue);	}
		else if(ENUMNAME == name){								m_strName = cvr;		}
		else if(UNITLIST == name)
		{
			TiXmlElement* pChild = pRoot->FirstChildElement();
			while(pChild)
			{
				pItem = boost::shared_ptr<CXmlEnumItem>(new CXmlEnumItem);
				if(pItem->SerializeXml(pChild))		m_vtEnumItem.push_back(pItem);
				pChild = pChild->NextSiblingElement();
			}
		}
		pRoot = pRoot->NextSiblingElement();
	}
	return true;
}

bool CXmlEnum::SerializeItem(TiXmlElement* pNode)
{
	return true;
}
int CXmlEnum::GetIndex(CComVariant cvr)
{
	cvr.ChangeType(VT_BSTR);
	CString text2 = (CString)cvr.bstrVal;
	int value = atoi(text2);
	boost::shared_ptr<CXmlEnumItem> item;
	for(int i = 0; i < (int)m_vtEnumItem.size(); ++i)
	{
		if(m_vtEnumItem[i]->m_nValue == value)
			return i;
	}
	return 0;
}

int CXmlEnum::GetValue(int index)
{
	if(m_vtEnumItem.empty())				return 0;
	if(index >= (int)m_vtEnumItem.size())	return 0;
	return m_vtEnumItem[index]->m_nValue;
}
