#include "StdAfx.h"
#include "XmlInterface.h"

using namespace XmlInfo;

const CString INTERFACES = _T("Interface");
const CString INTERFACEID = _T("InterfaceID");
const CString INTERFACENAME = _T("InterfaceName");
const CString INTERFACETYPE = _T("InterfaceType");
const CString DATAPORT = _T("DataPort");
const CString LISTENPORT = _T("ListenPort");
const CString SCANTIME = _T("ScanTime");
const CString REVIVETIME = _T("ReviveTime");
const CString RETRYNUM = _T("RetryNum");
const CString COOLTIME = _T("CoolTime");
const CString OVERTIME = _T("OverTime");
const CString PROTOCOLLIST = _T("ProtocolUsedList");
const CString PROTOCOLUSED = _T("ProtocolUsed");
CXmlInterface::CXmlInterface(void)
{
	m_uiID = UINT(-1);
}

CXmlInterface::~CXmlInterface(void)
{
}

//!< 解析自己的节点
bool CXmlInterface::SerializeXml(TiXmlElement* pNode)
{
	CString name = pNode->Value();
	CComVariant cvr;
	if(INTERFACES != name)				return false;
	TiXmlElement* pRoot = pNode->FirstChildElement();
	int iValue;
	while(pRoot){
		name = pRoot->Value();
		cvr = pRoot->GetText();
		if(INTERFACEID == name){		cvr.ChangeType(VT_I4);		iValue = cvr.intVal;	m_uiID = UINT(iValue);			}
		else if(INTERFACENAME == name){								m_strName = cvr;				}
		else if(INTERFACETYPE == name){	cvr.ChangeType(VT_I4);		iValue = cvr.intVal;	m_uiType = UINT(iValue);		}
		else if(DATAPORT == name){		cvr.ChangeType(VT_I4);		iValue = cvr.intVal;	m_uiDataPort = UINT(iValue);	}
		else if(LISTENPORT == name){	cvr.ChangeType(VT_I4);		iValue = cvr.intVal;	m_uiListenPort = UINT(iValue);	}
		else if(SCANTIME == name){		cvr.ChangeType(VT_I4);		iValue = cvr.intVal;	m_uiScanTime = UINT(iValue);	}
		else if(REVIVETIME == name){	cvr.ChangeType(VT_I4);		iValue = cvr.intVal;	m_uiReviveTime = UINT(iValue);	}
		else if(RETRYNUM == name){		cvr.ChangeType(VT_I4);		iValue = cvr.intVal;	m_uiRetryNum = UINT(iValue);	}
		else if(COOLTIME == name){		cvr.ChangeType(VT_I4);		iValue = cvr.intVal;	m_uiCoolTime = UINT(iValue);	}
		else if(OVERTIME == name){		cvr.ChangeType(VT_I4);		iValue = cvr.intVal;	m_uiOverTime = UINT(iValue);	}
		else if(PROTOCOLLIST == name)
		{
			TiXmlElement* pChild = pRoot->FirstChildElement();
			while(pChild){SerializeProtocol(pChild);	pChild = pChild->NextSiblingElement();}
		}
		pRoot = pRoot->NextSiblingElement();
	}
	return true;
}

void CXmlInterface::SerializeProtocol(TiXmlElement* pNode)
{
	CString name = pNode->Value();
	if(PROTOCOLUSED != name)		return;
	CComVariant cvr = pNode->GetText();
	cvr.ChangeType(VT_I4);
	int iValue = cvr.intVal;
	m_ltProtocol.push_back(UINT(iValue));
}