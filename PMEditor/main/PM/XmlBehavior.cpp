#include "StdAfx.h"
#include "XmlBehavior.h"

using namespace XmlInfo;
const CString ROOTNAME = _T("Behavior");
const CString ID = _T("BehaviorID");
const CString NAME = _T("BehaviorName");
const CString TYPE = _T("BehaviorType");
const CString SHOW = _T("ShowToUser");
const CString ARRAYNUM = _T("ArrayNum");
const CString ARRAYNAME = _T("ArrayName");
const CString COMMTYPE = _T("CommType");
const CString PROTOCOLLIST = _T("ProtocolUsedList");
const CString PROTOCOLUSED = _T("ProtocolUsed");
const CString PARALIST = _T("ParaInfoList");
const CString PARA = _T("ParaInfo");
const CString PARAID = _T("ParaID");

CXmlBehavior::CXmlBehavior(void)
{
	m_uiType = 0;
	m_bShowUser = TRUE;
	m_uiArrayNum = 1;
}

CXmlBehavior::~CXmlBehavior(void)
{
}

//!< 解析自己的节点
bool CXmlBehavior::SerializeXml(TiXmlElement* pNode)
{
	CString name;
	CComVariant cvr;
	name = pNode->Value();
	if(ROOTNAME != name)				return false;
	TiXmlElement* pRoot = pNode->FirstChildElement();
	int iValue;
	while(pRoot){
		name = pRoot->Value();
		cvr = pRoot->GetText();
		if(ID == name)				{cvr.ChangeType(VT_I4);		iValue = cvr.intVal;	m_uiID = UINT(iValue);		}
		else if(NAME== name)		{							m_strName = cvr;		}
		else if(TYPE == name)		{cvr.ChangeType(VT_I4);		iValue = cvr.intVal;	m_uiType = UINT(iValue);	}
		else if(COMMTYPE == name)	{cvr.ChangeType(VT_I4);		iValue = cvr.intVal;	m_uiCommType = UINT(iValue);}
		else if(SHOW == name)		{cvr.ChangeType(VT_I4);		iValue = cvr.intVal;	m_bShowUser = iValue==0?FALSE:TRUE;}
		else if(ARRAYNUM == name)	{cvr.ChangeType(VT_I4);		iValue = cvr.intVal;	m_uiArrayNum = UINT(iValue);}
		else if(ARRAYNAME == name)	{													m_strArrayName = cvr;}
		else if(PROTOCOLLIST == name)
		{
			TiXmlElement* pChild = pRoot->FirstChildElement();
			while(pChild){	SerializeProtocol(pChild);			pChild = pChild->NextSiblingElement();	}
		}
		else if(name == PARALIST)
		{
			TiXmlElement* pChild = pRoot->FirstChildElement();
			while(pChild){	SerializePara(pChild);				pChild = pChild->NextSiblingElement();	}
		}
		pRoot = pRoot->NextSiblingElement();
	}

	return true;
}

void CXmlBehavior::SerializeProtocol(TiXmlElement* pNode)
{
	CString name;
	CComVariant cvr;
	name = pNode->Value();
	if(PROTOCOLUSED != name)	return;
	cvr = pNode->GetText();		//!< 由于描述在此地方没有描述到子节点，只使用了text作为id号的表示，所以这里解析起来方便一些
	cvr.ChangeType(VT_I4);
	int iValue = cvr.intVal;
	m_ltProtocolID.push_back(UINT(iValue));
}

void CXmlBehavior::SerializePara(TiXmlElement* pNode)
{
	CString name;
	CComVariant cvr;
	name = pNode->Value();
	if(PARA != name)			return;
	TiXmlElement* pRoot = pNode->FirstChildElement();
	int iValue;
	while(pRoot){
		name = pRoot->Value();
		cvr = pRoot->GetText();
		if(name == PARAID)	{cvr.ChangeType(VT_I4);		iValue = cvr.intVal;	m_ltParaID.push_back((UINT)iValue);}
		pRoot = pRoot->NextSiblingElement();
	}
}

//!< 是否存在这个参数
bool CXmlBehavior::FindParam(UINT id)
{
	foreach(UINT index, m_ltParaID){
		if(index == id)
			return true;
	}
	return false;
}
