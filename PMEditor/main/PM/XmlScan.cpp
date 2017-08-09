#include "StdAfx.h"
#include "XmlScan.h"

using namespace XmlInfo;

const CString ID = _T("ID");
const CString NAME = _T("Name");
const CString COMPANY = _T("Company");
const CString INTERFACE2MAIN = _T("Interface2Main");
const CString PATH = _T("Path");
const CString CVERSION = _T("Version");
CXmlScan::CXmlScan(void)
{
}

CXmlScan::~CXmlScan(void)
{
}

//!< 解析设备的管理信息
bool CXmlScan::SerializeMgr(TiXmlElement* pNode, bool bRead)
{
	CString name, strValue;
	if(bRead)
	{
		TiXmlAttribute* pAttr = pNode->FirstAttribute();
		while(pAttr)
		{
			name = pAttr->Name();
			strValue = pAttr->Value();

			if(ID == name)					{m_strID = strValue;}
			else if(NAME == name)			{m_strName = strValue;}
			else if(COMPANY == name)		{m_strCompany = strValue;}
			else if(INTERFACE2MAIN == name)	{m_strInf2Main = strValue;}
			else if(PATH == name)			{m_strPath = strValue;}
			else if(CVERSION == name)		{m_strVersion = strValue;}
			pAttr = pAttr->Next();
		}
	}
	else
	{
		pNode->SetAttribute(ID, m_strID);
		pNode->SetAttribute(NAME, m_strName);
		pNode->SetAttribute(COMPANY, m_strCompany);
		pNode->SetAttribute(INTERFACE2MAIN, m_strInf2Main);
		pNode->SetAttribute(PATH, m_strPath);
		pNode->SetAttribute(CVERSION, m_strVersion);
	}
	return true;
}
