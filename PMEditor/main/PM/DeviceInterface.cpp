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
//,m_uiIP(UINT(16885952))				//!< 默认192.168.1.1
//,m_bIP(true)						//!< 当为以太网时,是否IP模式,还是DNS模式
,m_nState(0)
,m_uiHandle(0)
,m_bProj(true)							//!< 是否是工程的接口
,m_uiDevPort(11000)				//!<默认设备通讯接口
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
//	if(m_uiDevPort ==inf.m_uiDevPort)	return false;                    先注上，因为不知道哪里用到判断 
	return true;
}

CString CDeviceInterface::GetName(bool bShowScan /* = true */)
{
	CString addr, strScan;
	if(m_uiType == TYPE_ETHERNET)			addr.Format(m_strDoMain + ":%d", m_uiDevPort);
	else if(m_uiType == TYPE_SERIAL)		addr.Format("COM%d-%d", m_uiSerialNo + 1, m_uiHandle);
	else if(m_uiType == TYPE_SLAVE)			addr.Format("从-%d", m_uiHandle);
	else if(m_uiType == TYPE_REMOTETCP)		addr.Format("RemoteTcp-%d", m_uiHandle);
	if(!IsProj() || !m_pDevice->IsProj())	strScan = _T("(扫)");
	if(!bShowScan)							strScan = _T("");
	return addr + strScan;
}

CString CDeviceInterface::GetTypeName()
{
	CString name;
	if(m_uiType == TYPE_ETHERNET)			name = _T("以太网");
	else if(m_uiType == TYPE_SERIAL)		name = _T("串口");
	else if(m_uiType == TYPE_ZIGBEE)		name = _T("ZigBee");
	else if(m_uiType == TYPE_SLAVE)			name = _T("从设备");
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
		if (m_uiType == 0x20)				//!< 这种类型的接口其IP地址就是模块地址
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
				pNode->SetAttribute(IPADDR,CGbl::GetNumberFromIP(m_strDoMain));//!<值为IP直接转换
				pNode->SetAttribute(DOMAINADDR,m_strDoMain.GetBuffer(0));
				pNode->SetAttribute(DEVPORT,(UINT)m_uiDevPort);
			}
			else if (CGbl::RegexDomain(m_strDoMain))
			{
				int aa=0,bb=0;
				bb=CGbl::DomainToIp(m_strDoMain,aa);
				if(bb!=0)
					pNode->SetAttribute(IPADDR,aa);          //!<值为域名转换后存储
				else
					pNode->SetAttribute(IPADDR,0);
				pNode->SetAttribute(DOMAINADDR,m_strDoMain.GetBuffer(0));	
				pNode->SetAttribute(DEVPORT,(UINT)m_uiDevPort);
			}
		}			
	}
	return TRUE;
}

//!< 获得线的颜色
COLORREF CDeviceInterface::GetInfColor()
{
	if(GetType() == 0)			return INF_ETHERNET;
	else if(GetType() == 1)		return INF_SERIAL;
	else if(GetType() == 16)	return INF_SLAVE;
	else if(GetType() == 0x20)	return INF_ETHERNET;
	return RGB(0, 0, 0);
}
