#include "StdAfx.h"
#include "Gbl.h"
#include "XmlDevice.h"
#include "XmlParaInfo.h"
#include "XmlBehavior.h"
#include "XmlArea.h"
#include "XmlProtocol.h"
#include "XmlInterface.h"
#include "XmlEnum.h"

using namespace XmlInfo;

const CString DEVICEID = _T("ID");
const CString DEVICENAME1 = _T("Name");
const CString DEVICEICON = _T("DeviceIcon");
const CString COMPANY = _T("Company");
const CString INTERFACE2MAIN = _T("Interface2Main");
const CString INTERFACE2SLAVE = _T("Interface2Slave");
const CString MAXINTERFACE2SLAVE = _T("MaxInterface2Slave");
const CString PROGRAM = _T("Programmable");
const CString PATH = _T("Path");
const CString DEVICEVERSION = _T("Version");
const CString PARA2USER = _T("Para2User");

const CString DEVICEROOT = _T("DeviceDesc");
const CString DEVICETYPE = _T("DeviceType");
const CString DEVICENAME2 = _T("DeviceName");
const CString PARALIST = _T("ParaList");
const CString BEHAVIORLIST = _T("BehaviorList");
const CString AREALIST = _T("AreaList");
const CString PROTOCOLLIST = _T("ProtocolList");
const CString INTERFACELIST = _T("InterfaceList");
const CString ENUMLIST = _T("EnumList");
const CString PROGRAMLIST = _T("ProgramList");

const UINT MAXID = 1024;
const UINT MAXPARA = 10240;
const UINT MAXBEHAVIOR = 1024;
const UINT MAXAREA = 1024;
const UINT MAXPROTOCOL = 1024;
const UINT MAXINTERFACE = 1024;
const UINT MAXENUM = 10240;

CXmlDevice::CXmlDevice(void)
{
	m_bOpen = false;
	m_bPara2User = false;						//!< �Ƿ���Խ�������¶���û�
	m_uiDevIcon = 0;							//!< �豸ͼ��
	m_bProgrammable = false;					//!< �Ƿ���Զ�����
}

CXmlDevice::~CXmlDevice(void)
{
}

bool CXmlDevice::OpenXml()
{
	if(m_bOpen)		return true;
	TiXmlDocument pTiXml(m_strPathName);
	if(!pTiXml.LoadFile()){
		MessageBox(NULL, _T("�豸�����ļ���ʽ����"), _T("����"), MB_ICONEXCLAMATION);
		return false;
	}
	//!< ��ʼ����
	if(!SerializeXml(pTiXml.RootElement()))		return false;
	m_bOpen = true;

	//!< ��һ�μ���
#ifdef _DEBUG
	OnCheck();
#endif
	return true;
}

bool CXmlDevice::SerializeMgr(TiXmlElement* pNode, bool bRead)
{
	CString name, strValue;
	CComVariant cvr;

	if(bRead)
	{
		TiXmlAttribute* pAttr = pNode->FirstAttribute();
		int iValue;
		while(pAttr)
		{
			name = pAttr->Name();
			strValue = pAttr->Value();
			cvr = strValue;

			if(DEVICEID == name)				{cvr.ChangeType(VT_I4);	iValue = cvr.intVal;	m_uiID = UINT(iValue);}
			else if(DEVICENAME1 == name)		{m_strName = cvr.bstrVal;}
			else if(DEVICEICON == name)			{cvr.ChangeType(VT_I4);	iValue = cvr.intVal;	m_uiDevIcon = UINT(iValue);}
			else if(COMPANY == name)			{m_strCompany = cvr.bstrVal;}
			else if(INTERFACE2MAIN == name)		{cvr.ChangeType(VT_I4);	iValue = cvr.intVal;	m_uiInf2Main = UINT(iValue);}
			else if(INTERFACE2SLAVE == name)	{cvr.ChangeType(VT_I4);	iValue = cvr.intVal;	m_uiInf2Slave = UINT(iValue);}
			else if(MAXINTERFACE2SLAVE == name)	{cvr.ChangeType(VT_I4);	iValue = cvr.intVal;	m_uiMaxInf2Slave = UINT(iValue);}
			else if(PROGRAM == name)			{m_bProgrammable = strValue == _T("1") ? true : false;}
			else if(PATH == name)				{m_strPathName = cvr;	m_strPathName = CGbl::GetMe().getDataPath() + m_strPathName;}
			else if(DEVICEVERSION == name)		{m_strVersion = cvr;}
			else if(PARA2USER == name)			{m_bPara2User = strValue == _T("1") ? true : false;}
			pAttr = pAttr->Next();
		}
		//if(!IsSupportEthernet() && !IsSupportSerial())	return false;	//!< ʲô�ӿڶ���֧�־Ͳ��ü�������
		if(m_uiID > MAXID)								return false;	//!< �豸��Ų���̫��
	}
	else
	{
		pNode->SetAttribute(DEVICEID, m_uiID);
		pNode->SetAttribute(DEVICENAME1, m_strName);
		pNode->SetAttribute(DEVICEICON, m_uiDevIcon);
		pNode->SetAttribute(COMPANY, m_strCompany);
		pNode->SetAttribute(INTERFACE2MAIN, m_uiInf2Main);
		pNode->SetAttribute(INTERFACE2SLAVE, m_uiInf2Slave);
		pNode->SetAttribute(MAXINTERFACE2SLAVE, m_uiMaxInf2Slave);
		pNode->SetAttribute(PATH, CGbl::GetNameFromFilePathName(m_strPathName));
		pNode->SetAttribute(DEVICEVERSION, m_strVersion);
	}
	return true;
}

bool CXmlDevice::SerializeXml(TiXmlElement* pNode)
{
	CString name, strValue;
	CComVariant cvr;
		
	TiXmlElement* pElement = pNode->FirstChildElement();
	while(pElement)
	{
		name = pElement->Value();
		if(DEVICEROOT == name)
		{
			TiXmlElement* pChild = pElement->FirstChildElement();
			while(pChild)
			{
				name = pChild->Value();
				if(name == DEVICETYPE)				{cvr = pChild->GetText();	cvr.ChangeType(VT_I4);	m_uiDevType = UINT(cvr.intVal);}
				else if(DEVICENAME2 == name)		{cvr = pChild->GetText();	m_strName = cvr;}
				else if(DEVICEICON == name)			{cvr = pChild->GetText();	cvr.ChangeType(VT_I4);	m_uiDevIcon = UINT(cvr.intVal);}
				else if(COMPANY == name)			{cvr = pChild->GetText();	m_strCompany = cvr;}
				else if(INTERFACE2MAIN == name)		{cvr = pChild->GetText();	cvr.ChangeType(VT_I4);	m_uiInf2Main = UINT(cvr.intVal);}
				else if(INTERFACE2SLAVE == name)	{cvr = pChild->GetText();	cvr.ChangeType(VT_I4);	m_uiInf2Slave = UINT(cvr.intVal);}
				else if(MAXINTERFACE2SLAVE == name)	{cvr = pChild->GetText();	cvr.ChangeType(VT_I4);	m_uiMaxInf2Slave = UINT(cvr.intVal);}
				else if(DEVICEVERSION == name)		{cvr = pChild->GetText();	m_strVersion = cvr;}
				else if(PROGRAM == name)			{strValue = pChild->GetText();	m_bProgrammable = strValue == _T("1") ? true : false;}
				else if(PARA2USER == name)			{strValue = pChild->GetText();	m_bPara2User = strValue == _T("1") ? true : false;}
				else if(PARALIST == name)			SerializeT(pChild, m_vtPara, MAXPARA, _T("����"));
				else if(BEHAVIORLIST == name)		SerializeT(pChild, m_vtBehavior, MAXBEHAVIOR, _T("��Ϊ"));
				else if(AREALIST == name)			SerializeT(pChild, m_vtArea, MAXAREA, _T("������"));
				else if(PROTOCOLLIST == name)		SerializeT(pChild, m_vtProtocol, MAXPROTOCOL, _T("ͨ��Э��"));
				else if(INTERFACELIST == name)		SerializeT(pChild, m_vtInterface, MAXINTERFACE, _T("ͨ�Žӿ�"));
				else if(ENUMLIST == name)			SerializeT(pChild, m_vtEnum, MAXENUM, _T("ö��"));
				pChild = pChild->NextSiblingElement();
			}
			//if(!IsSupportEthernet() && !IsSupportSerial())		return false;	//!< ʲô�ӿڶ���֧�־Ͳ��ü�������
		}
		pElement = pElement->NextSiblingElement();
	}
	return true;
}

void CXmlDevice::SayError(CString text, UINT maxNum)
{
}

template <class T>
bool CXmlDevice::SerializeT(TiXmlElement* pNode, std::vector<std::shared_ptr<T> >& vtObject, UINT maxNum, CString errName)
{
	UINT num = 0;
	CString name;
	CComVariant cvr;
	TiXmlElement* pElement = pNode->FirstChildElement();
	std::list<std::shared_ptr<T> > ltObject;
	std::shared_ptr<T> pObject;
	while(pElement)
	{
		if(TiXmlNode::ELEMENT == pElement->Type())
		{
			pObject = std::shared_ptr<T>(new T);
			if(pObject->SerializeXml(pElement))
			{
				if(pObject->m_uiID > maxNum)	SayError(errName, pObject->m_uiID);
				else							ltObject.push_back(pObject);
			}
		}
		pElement = pElement->NextSiblingElement();
	}

	for (auto pObject : ltObject)		num = max(num, pObject->m_uiID);
	vtObject.clear();
	vtObject.resize(num + 1);
	for (auto pObject : ltObject)		vtObject[pObject->m_uiID] = pObject;

	return true;
}

std::shared_ptr<CXmlArea> CXmlDevice::getArea(UINT id)
{
	if(id < m_vtArea.size())
		return m_vtArea[id];
	std::shared_ptr<CXmlArea> empty;
	return empty;
}

std::shared_ptr<CXmlArea> CXmlDevice::getArea(CString name)
{
	std::shared_ptr<CXmlArea> area, empty;
	for (auto area : m_vtArea){
		if(!area)	continue;
		if(name == area->getName())
			return area;
	}
	return empty;
}

int CXmlDevice::GetAreaID(CString name)
{
	for (std::shared_ptr<CXmlArea> area : m_vtArea){
		if(!area)	continue;
		if(name == area->getName())
			return area->getID();
	}
	return -1;
}

//!< ��������б�defIndex������ʱ��ʾareaID������ȥʱ��ʾ�б��Ĭ������������Ĭ���ַ���
std::shared_ptr<CXmlArea> CXmlDevice::GetAreaNameList(std::list<CString>& strList, int& defIndex, bool bSort)
{
	std::shared_ptr<CXmlArea> defArea, area;
	int index = -1;
	for (auto area : m_vtArea)
	{
		if(!area)										continue;
		if(area->getName().Trim() == _T(""))			continue;
		if(area->getID() == defIndex)					//!< �ҵ���ĿǰĬ�ϵ���
		{
			index = (int)strList.size();
			defArea = area;
		}
		strList.push_back(area->getName());
	}
	defIndex = index;
	if(bSort)
	{
		strList.sort();									//!< Ҫ���������
		if(defArea)										//!< ����Ĭ��ֵ
		{
			index = 0;
			for (CString name : strList)
			{
				if(name == defArea->getName())
				{
					defIndex = index;
					break;
				}
				++index;
			}
		}
	}
	return defArea;
}

std::shared_ptr<CXmlBehavior> CXmlDevice::getBehavior(UINT id)
{
	if(id < m_vtBehavior.size())
		return m_vtBehavior[id];
	std::shared_ptr<CXmlBehavior> empty;
	return empty;
}

std::list<std::shared_ptr<CXmlBehavior> > CXmlDevice::getBehavior(CString name)
{
	std::list<std::shared_ptr<CXmlBehavior> > ltBehavior;
	std::shared_ptr<CXmlBehavior> behavior;
	for (auto behavior : m_vtBehavior){
		if(!behavior)	continue;
		if(name == behavior->m_strName)
			ltBehavior.push_back(behavior);
	}
	return ltBehavior;
}

std::shared_ptr<CXmlParaInfo> CXmlDevice::getPara(UINT id)
{
	if(id < m_vtPara.size())
		return m_vtPara[id];
	std::shared_ptr<CXmlParaInfo> empty;
	return empty;
}

std::shared_ptr<CXmlProtocol> CXmlDevice::getProtocol(CString name)
{
	std::shared_ptr<CXmlProtocol> protocol, empty;
	for (auto protocol : m_vtProtocol){
		if(!protocol)	continue;
		if(name == protocol->m_strName)
			return protocol;
	}
	return empty;
}

std::shared_ptr<CXmlProtocol> CXmlDevice::getProtocol(UINT id)
{
	if(id < m_vtProtocol.size())
		return m_vtProtocol[id];
	std::shared_ptr<CXmlProtocol> empty;
	return empty;
}

std::shared_ptr<CXmlInterface> CXmlDevice::getInterface(UINT id)
{
	if(id < m_vtInterface.size())
		return m_vtInterface[id];
	std::shared_ptr<CXmlInterface> empty;
	return empty;
}

std::shared_ptr<CXmlEnum> CXmlDevice::getEnum(UINT id)
{
	if(id < m_vtEnum.size())
		return m_vtEnum[id];
	std::shared_ptr<CXmlEnum> empty;
	return empty;
}

//!< ������и����Ƶ���Ϊ,��-�ָ�
CString CXmlDevice::GetAllBevID(CString strBev, bool bRead)
{
	CString strID, idOne;
	UINT uiType = bRead ? 0 : 1;
	std::shared_ptr<CXmlBehavior> bev;

	for (auto bev : m_vtBehavior)
	{
		if(!bev)						continue;
		if(bev->m_strName != strBev)	continue;
		if(bev->m_uiType != uiType && bev->m_uiType != 2)		continue;

		if(strID == _T(""))
		{
			strID.Format(_T("%d"), bev->m_uiID);
		}
		else
		{
			idOne.Format(_T("-%d"), bev->m_uiID);
			strID = strID + idOne;
		}
	}
	ASSERT(strID != _T(""));
	return strID;
}

//!< У��һ��
void XmlInfo::CXmlDevice::OnCheck()
{
	std::shared_ptr<CXmlParaInfo> xmlPara;
	std::shared_ptr<CXmlBehavior> xmlReadBev, xmlWriteBev, xmlBev;
	CString strError;

	//!< ��Ϊ�����һһ��Ӧ
	for (auto xmlPara : m_vtPara)
	{
		if(!xmlPara)		continue;
		if(xmlPara->m_uiReadBID != UINT(-1))
		{
			xmlReadBev = getBehavior(xmlPara->m_uiReadBID);
			if(!xmlReadBev)
			{
				strError.Format(xmlPara->m_strName + _T(" �����Ҳ�������Ϊ, ����ID:%d, ��ΪID:%d"), xmlPara->m_uiID, xmlPara->m_uiReadBID);
				CGbl::PrintOut(strError);
			}
			else if(!xmlReadBev->FindParam(xmlPara->m_uiID))
			{
				strError.Format(xmlReadBev->m_strName + _T(" ��Ϊ�Ҳ�������:") + xmlPara->m_strName + _T(", ����ID:%d, ��ΪID:%d"),
					xmlPara->m_uiID, xmlPara->m_uiReadBID);
				CGbl::PrintOut(strError);
			}
		}

		if(xmlPara->m_uiWriteBID != UINT(-1))
		{
			xmlWriteBev = getBehavior(xmlPara->m_uiWriteBID);
			if(!xmlWriteBev)
			{
				strError.Format(xmlPara->m_strName + _T(" �����Ҳ���д��Ϊ, ����ID:%d, ��ΪID:%d"), xmlPara->m_uiID, xmlPara->m_uiWriteBID);
				CGbl::PrintOut(strError);
			}
			else if(!xmlWriteBev->FindParam(xmlPara->m_uiID))
			{
				strError.Format(xmlWriteBev->m_strName + _T(" ��Ϊ�Ҳ�������:") + xmlPara->m_strName + _T(", ����ID:%d, ��ΪID:%d"),
					xmlPara->m_uiID, xmlPara->m_uiWriteBID);
				CGbl::PrintOut(strError);
			}
		}
	}

	//!< ��Ϊ��Ĳ������ô���
	for (auto xmlBev : m_vtBehavior)
	{
		if(!xmlBev)		continue;
		for (UINT para_id : xmlBev->m_ltParaID)
		{
			if(!getPara(para_id))
			{
				strError.Format(xmlBev->m_strName + _T(" ��Ϊ�Ҳ�������, ��ΪID:%d, ����ID:%d"), xmlBev->m_uiID, para_id);
				CGbl::PrintOut(strError);
			}
		}
	}
	
	//!< ��Ϊ��Э��һһ��Ӧ
	std::shared_ptr<CXmlProtocol> xmlProtocol;
	for (auto xmlBev : m_vtBehavior)
	{
		if(!xmlBev)		continue;
		for (UINT protocol_id : xmlBev->m_ltProtocolID)
		{
			xmlProtocol = getProtocol(protocol_id);
			if(!xmlProtocol)
			{
				strError.Format(xmlBev->m_strName + _T(" ��Ϊ�Ҳ���Э��, ��ΪID:%d, Э��ID:%d"), xmlBev->m_uiID, protocol_id);
				CGbl::PrintOut(strError);
			}
			else if(!xmlProtocol->FindBev(xmlBev->m_uiID))
			{
				strError.Format(xmlProtocol->m_strName + _T(" Э���Ҳ�����Ϊ, ��ΪID:%d, Э��ID:%d"), xmlBev->m_uiID, protocol_id);
				CGbl::PrintOut(strError);
			}
		}
	}
}