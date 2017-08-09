#include "StdAfx.h"
#include "DEthernet.h"

using namespace MVC;
using namespace Device;
using namespace InterfaceSet;

const CString ROOT_NAME = _T("EthernetInfo");
const CString ETH_LOCALPORT = _T("LocalPort");
const CString SERIAL_RETRYNUM =_T("RetryNum");
const CString SERIAL_OVERTIME =_T("OverTime");
const CString SERIAL_COOLTIME =_T("CoolTime");
const CString SERIAL_REVIVETIME =_T("ReviveTime");
const CString SERIAL_SCANTIME =_T("ScanTime");

CDEthernet::CDEthernet(void)
:m_uiLocalPort(0)			//!< ����ͨѶ�˿ڣ�0Ϊ����˿�
,m_uiRetryNum(1)			//!< ���Դ���
,m_uiOverTime(1000)			//!< ͨ�ų�ʱʱ��
,m_uiCoolTime(15000)		//!< ��ȴʱ��
,m_uiReviveTime(15000)		//!< ͨ�Żָ�ʱ��
,m_uiScanTime(20000)		//!< �豸ɨ������
{
}

CDEthernet::~CDEthernet(void)
{
}

bool CDEthernet::SerializeXml(TiXmlElement* pNode, bool bRead)
{
	if(bRead)
	{
		CString name, strValue;
		CComVariant cv;
		int iValue;
		name = pNode->Value();
		if(ROOT_NAME != name)					return false;
		TiXmlAttribute* pAttr = pNode->FirstAttribute();
		while(pAttr)
		{
			name = pAttr->Name();
			strValue = pAttr->Value();
			cv = strValue;
			if (ETH_LOCALPORT == name)			{ cv.ChangeType(VT_I4);	iValue = cv.intVal;		setLocalPort(iValue); }
			else if (SERIAL_RETRYNUM == name)	{ cv.ChangeType(VT_I4);	iValue = cv.intVal;		m_uiRetryNum = UINT(iValue); }
			else if (SERIAL_OVERTIME == name)	{cv.ChangeType(VT_I4);	iValue = cv.intVal;		m_uiOverTime = UINT(iValue);}
			else if (SERIAL_COOLTIME == name)	{cv.ChangeType(VT_I4);	iValue = cv.intVal;		m_uiCoolTime = UINT(iValue);}
			else if (SERIAL_REVIVETIME == name)	{cv.ChangeType(VT_I4);	iValue = cv.intVal;		m_uiReviveTime = UINT(iValue);}
			else if (SERIAL_SCANTIME == name)	{cv.ChangeType(VT_I4);	iValue = cv.intVal;		m_uiScanTime = UINT(iValue);}
			pAttr = pAttr->Next();
		}
	}
	else
	{
		pNode->SetAttribute(ETH_LOCALPORT, int(m_uiLocalPort));
		pNode->SetAttribute(SERIAL_RETRYNUM, int(m_uiRetryNum));
		pNode->SetAttribute(SERIAL_OVERTIME, int(m_uiOverTime));
		pNode->SetAttribute(SERIAL_COOLTIME, int(m_uiCoolTime));
		pNode->SetAttribute(SERIAL_REVIVETIME, int(m_uiReviveTime));
		pNode->SetAttribute(SERIAL_SCANTIME, int(m_uiScanTime));
	}
	return true;
}