#include "StdAfx.h"
#include "Gbl.h"
#include "DeviceOne.h"
#include "DeviceInterface.h"

const CString INTERFACE_ID = _T("InterfaceID");
const CString SERIAL_NO = _T("SerialNo");
const CString HANDLE_ID = _T("Handle");
const CString IPADDR = _T("IP");
const CString DOMAINADDR=_T("Domain");
const CString DEVPORT=_T("DevPort");

const UINT TYPE_ETHERNET = 0;
const UINT TYPE_SERIAL = 1;
const UINT TYPE_ZIGBEE = 2;
const UINT TYPE_SLAVE = 16;
const UINT TYPE_REMOTETCP = 32;

using namespace MVC;
using namespace Device;

CDeviceInterface::CDeviceInterface(CDeviceOne* pDevice)
:m_pDevice(pDevice)
,m_uiID(0)
,m_uiType(0)
,m_uiSerialNo(1)
//,m_uiIP(UINT(16885952))				//!< Ĭ��192.168.1.1
//,m_bIP(true)						//!< ��Ϊ��̫��ʱ,�Ƿ�IPģʽ,����DNSģʽ
,m_nState(0)
,m_uiHandle(0)
,m_bProj(true)							//!< �Ƿ��ǹ��̵Ľӿ�
,m_uiDevPort(11000)				//!<Ĭ���豸ͨѶ�ӿ�
{
	ASSERT(pDevice);
	m_strDoMain="";			
	}

CDeviceInterface& CDeviceInterface::operator = (CDeviceInterface& inf)
{
	m_uiID = inf.m_uiID;
	m_uiType = inf.m_uiType;
	m_uiSerialNo = inf.m_uiSerialNo;
	m_strDoMain = inf.m_strDoMain;
	m_uiHandle = inf.m_uiHandle;

	m_uiDevPort =inf.m_uiDevPort;
	return *this;
}

bool CDeviceInterface::operator != (CDeviceInterface& inf) const
{
	if(m_uiID == inf.m_uiID)				return false;
	if(m_uiType == inf.m_uiType)			return false;
	if(m_uiSerialNo == inf.m_uiSerialNo)	return false;
	if(m_strDoMain == inf.m_strDoMain)				return false;
	if(m_uiHandle == inf.m_uiHandle)		return false;
//	if(m_uiDevPort ==inf.m_uiDevPort)	return false;                    ��ע�ϣ���Ϊ��֪�������õ��ж� 
	return true;
}

CString CDeviceInterface::GetName(bool bShowScan /* = true */)
{
	CString addr, strScan;
	if(m_uiType == TYPE_ETHERNET)			addr.Format(m_strDoMain + ":%d", m_uiDevPort);
	else if(m_uiType == TYPE_SERIAL)		addr.Format("COM%d-%d", m_uiSerialNo + 1, m_uiHandle);
	else if(m_uiType == TYPE_SLAVE)			addr.Format("��-%d", m_uiHandle);
	else if(m_uiType == TYPE_REMOTETCP)		addr.Format("RemoteTcp-%d", m_uiHandle);
	if(!IsProj() || !m_pDevice->IsProj())	strScan = _T("(ɨ)");
	if(!bShowScan)							strScan = _T("");
	return addr + strScan;
}

CString CDeviceInterface::GetTypeName()
{
	CString name;
	if(m_uiType == TYPE_ETHERNET)			name = _T("��̫��");
	else if(m_uiType == TYPE_SERIAL)		name = _T("����");
	else if(m_uiType == TYPE_ZIGBEE)		name = _T("ZigBee");
	else if(m_uiType == TYPE_SLAVE)			name = _T("���豸");
	else if(m_uiType == TYPE_REMOTETCP)		name = _T("RemoteTcp");
	return name;
}

bool CDeviceInterface::SerializeXml(TiXmlElement* pNode, bool bRead)
{
	if(bRead)
	{
		CString name, strValue;
		CComVariant cv;
		TiXmlAttribute* pAttr = pNode->FirstAttribute();
		int iValue;
		while(pAttr)
		{
			name = pAttr->Name();
			strValue = pAttr->Value();
			cv = strValue;

			if(INTERFACE_ID == name)	{cv.ChangeType(VT_I4);	iValue = cv.intVal;	m_uiType = UINT(iValue);}
			else if(SERIAL_NO == name)	{cv.ChangeType(VT_I4);	iValue = cv.intVal;	m_uiSerialNo = UINT(iValue);}
			else if(HANDLE_ID == name)	{cv.ChangeType(VT_I4);	iValue = cv.intVal;	m_uiHandle = UINT(iValue);}
			else if(IPADDR == name)	/*	{cv.ChangeType(VT_I4);	iValue = cv.intVal;	m_uiIP = UINT(iValue);} */{cv.ChangeType(VT_I4);	iValue = cv.intVal;	m_strDoMain=CGbl::GetIPFromNumber(iValue);} 
			else if (DOMAINADDR ==name)	{m_strDoMain=strValue;}
			else if(DEVPORT ==name) {cv.ChangeType(VT_UI4);iValue =cv.intVal;m_uiDevPort =UINT(iValue);}
			pAttr = pAttr->Next();
		}
		if(m_pDevice && m_pDevice->getParentID() != UINT(-1))	m_uiType = TYPE_SLAVE;
	}
	else
	{
		pNode->SetAttribute(INTERFACE_ID, (int)m_uiType);
		pNode->SetAttribute(SERIAL_NO, (int)m_uiSerialNo);
		pNode->SetAttribute(HANDLE_ID, (int)m_uiHandle);
		if (m_uiType == 0x20)				//!< �������͵Ľӿ���IP��ַ����ģ���ַ
		{
			m_strDoMain =CGbl::UintToCString((UINT)m_uiHandle);	
			pNode->SetAttribute(IPADDR,(int)m_uiHandle);
			pNode->SetAttribute(DOMAINADDR,(int)m_uiHandle);
			pNode->SetAttribute(DEVPORT,-1);
		}
		else
		{
			if(CGbl::RegexIP(m_strDoMain))					
			{
				pNode->SetAttribute(IPADDR,CGbl::GetNumberFromIP(m_strDoMain));//!<ֵΪIPֱ��ת��
				pNode->SetAttribute(DOMAINADDR,m_strDoMain.GetBuffer(0));
				pNode->SetAttribute(DEVPORT,(UINT)m_uiDevPort);
			}
			else if (CGbl::RegexDomain(m_strDoMain))
			{
				int aa=0,bb=0;
				bb=CGbl::DomainToIp(m_strDoMain,aa);
				if(bb!=0)
					pNode->SetAttribute(IPADDR,aa);          //!<ֵΪ����ת����洢
				else
					pNode->SetAttribute(IPADDR,0);
				pNode->SetAttribute(DOMAINADDR,m_strDoMain.GetBuffer(0));	
				pNode->SetAttribute(DEVPORT,(UINT)m_uiDevPort);
			}
		}			
	}
	return TRUE;
}

//!< ����ߵ���ɫ
COLORREF CDeviceInterface::GetInfColor()
{
	if(GetType() == 0)			return INF_ETHERNET;
	else if(GetType() == 1)		return INF_SERIAL;
	else if(GetType() == 16)	return INF_SLAVE;
	else if(GetType() == 0x20)	return INF_ETHERNET;
	return RGB(0, 0, 0);
}
