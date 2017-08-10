#include "StdAfx.h"
#include "XmlProtocol.h"

using namespace XmlInfo;

const CString ROOTNAME = _T("Protocol");
const CString PROTOCOLID = _T("PrtclID");
const CString PROTOCOLNAME = _T("PrtclName");
const CString PROTOCOLAUTHOR = _T("PrtclAuthor");
const CString PROTOCOLTIME = _T("EditTime");
const CString PROTOCOLEXPLAIN = _T("PrtclExplain");
const CString AREALIST = _T("ProtocolAreaList");
const CString AREA = _T("ProtocolArea");
const CString AREAID = _T("AreaID");
const CString OPERATELIST = _T("OperateInfoList");
const CString OPERATE = _T("OperateInfo");
const CString OPERATEID = _T("OperateID");
const CString STARTADDR = _T("StartAddr");
const CString PERLEN = _T("PerLength");
const CString ENDADDR = _T("EndAddr");
const CString MAXREAD = _T("MaxRead");
const CString MINREAD = _T("MinRead");
const CString BLOCKREAD = _T("BlockRead");
const CString BEHAVIORLIST = _T("ProtocolBehaviorList");
const CString BEHAVIOR = _T("ProtocolBehavior");
const CString BEHAVIORID = _T("BehaviorID");

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
bool CXmlProtocolOperateInfo::SerializeXml(TiXmlElement* pNode)
{
	CString name = pNode->Value();
	if(OPERATE != name)		return false;
	CComVariant cvr;
	TiXmlElement* pChild = pNode->FirstChildElement();
	int iValue;
	while(pChild)
	{
		name = pChild->Value();
		cvr = pChild->GetText();
		if(OPERATEID == name){		cvr.ChangeType(VT_R4);	m_uiID = cvr.fltVal;	}
		else if(STARTADDR == name){	cvr.ChangeType(VT_I4);	iValue = cvr.intVal;	m_uiStartAddr = UINT(iValue);	}
		else if(PERLEN == name){	cvr.ChangeType(VT_I4);	iValue = cvr.intVal;	m_uiPerLen = UINT(iValue);		}
		else if(ENDADDR == name){	cvr.ChangeType(VT_I4);	iValue = cvr.intVal;	m_uiEndAddr = UINT(iValue);		}
		else if(MAXREAD == name){	cvr.ChangeType(VT_I4);	iValue = cvr.intVal;	m_uiMaxRead = UINT(iValue);		}
		else if(MINREAD == name){	cvr.ChangeType(VT_I4);	iValue = cvr.intVal;	m_uiMinRead = UINT(iValue);		}
		else if(BLOCKREAD == name){	cvr.ChangeType(VT_I4);	iValue = cvr.intVal;	m_uiBlockRead = UINT(iValue);	}

		pChild = pChild->NextSiblingElement();
	}
	return m_uiID != UINT(-1);
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//!< 解析协议区节点
bool CXmlProtocolArea::SerializeXml(TiXmlElement* pNode)
{
	CString name = pNode->Value();
	CComVariant cvr;
	if(AREA != name)	return false;
	TiXmlElement* pRoot = pNode->FirstChildElement();
	std::shared_ptr<CXmlProtocolOperateInfo> pOperate;
	int iValue;
	while(pRoot){
		name = pRoot->Value();
		cvr = pRoot->GetText();
		if(AREAID == name){	cvr.ChangeType(VT_I4);	iValue = cvr.intVal;	m_uiID = UINT(iValue);}
		else if(OPERATELIST == name)
		{
			TiXmlElement* pChild = pRoot->FirstChildElement();
			while(pChild)
			{
				pOperate = std::shared_ptr<CXmlProtocolOperateInfo>(new CXmlProtocolOperateInfo);
				if(pOperate->SerializeXml(pChild))		m_ltOperate.push_back(pOperate);
				pChild = pChild->NextSiblingElement();
			}
		}
		pRoot = pRoot->NextSiblingElement();
	}
	return m_uiID != UINT(-1);
}


//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
CXmlProtocol::CXmlProtocol(void)
{
	m_uiID = UINT(-1);
}

CXmlProtocol::~CXmlProtocol(void)
{
}

//!< 解析自己的节点
bool CXmlProtocol::SerializeXml(TiXmlElement* pNode)
{
	CString name = pNode->Value();
	CComVariant cvr;
	if(ROOTNAME != name)	return false;
	TiXmlElement* pRoot = pNode->FirstChildElement();
	std::shared_ptr<CXmlProtocolArea> pArea;
	int iValue;
	while(pRoot){
		name = pRoot->Value();
		cvr = pRoot->GetText();
		if(PROTOCOLID == name){	cvr.ChangeType(VT_I4);	iValue = cvr.intVal;	m_uiID = UINT(iValue);	}
		else if(PROTOCOLNAME == name){					m_strName = cvr;		}
		else if(PROTOCOLAUTHOR == name){				m_strAuthor = cvr;		}
		else if(PROTOCOLTIME == name){					m_strEditTime = cvr;	}
		else if(PROTOCOLEXPLAIN == name){				m_strExplain = cvr;		}
		else if(AREALIST == name)
		{
			TiXmlElement* pChild = pRoot->FirstChildElement();
			while(pChild){
				pArea = std::shared_ptr<CXmlProtocolArea>(new CXmlProtocolArea);
				if(pArea->SerializeXml(pChild))			m_ltProtocolArea.push_back(pArea);
				pChild = pChild->NextSiblingElement();
			}
		}
		else if(BEHAVIORLIST == name){
			TiXmlElement* pChild = pRoot->FirstChildElement();
			while(pChild){SerializeBehavior(pChild);	pChild = pChild->NextSiblingElement();}
		}
		pRoot = pRoot->NextSiblingElement();
	}
	return true;
}

//!< 解析协议行为节点
void CXmlProtocol::SerializeBehavior(TiXmlElement* pNode)
{
	CString name = pNode->Value();
	if(BEHAVIOR != name)		return;
	CComVariant cvr;
	TiXmlElement* pChild = pNode->FirstChildElement();
	UINT id = UINT(-1);
	int iValue;
	while(pChild){
		name = pChild->Value();
		cvr = pChild->GetText();
		if(BEHAVIORID == name){
			cvr.ChangeType(VT_I4);
			iValue = cvr.intVal;
			id = UINT(iValue);
		}
		pChild = pChild->NextSiblingElement();
	}
	if(id != UINT(-1))		m_ltBehaviorID.push_back(id);
}

//!< 看看这个行为知否存在
bool CXmlProtocol::FindBev(UINT id)
{
	for (UINT bevID : m_ltBehaviorID){
		if(bevID == id)
			return true;
	}
	return false;
}
