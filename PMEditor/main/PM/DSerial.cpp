#include "StdAfx.h"
#include "Gbl.h"
#include "DevMgr.h"
#include "DSerial.h"

using namespace MVC;
using namespace Device;
using namespace InterfaceSet;

const CString ROOT_NAME = _T("SerialInfo");
const CString SERIAL_NUMBER = _T("SerialNo");
const CString SERIAL_BAUD = _T("BaudRate");
const CString SERIAL_CHECK = _T("CheckStyle");
const CString SERIAL_DATA = _T("DataBitNum");
const CString SERIAL_STOP = _T("StopBitNum");
const CString SERIAL_RETRYNUM =_T("RetryNum");
const CString SERIAL_OVERTIME =_T("OverTime");
const CString SERIAL_COOLTIME =_T("CoolTime");
const CString SERIAL_REVIVETIME =_T("ReviveTime");
const CString SERIAL_SCANTIME =_T("ScanTime");

CDSerial::CDSerial(void)
:m_uiNumber(0)			//!< 串口代号，0代表COM1，1代表COM2，依此类推
,m_uiBaud(3)			//!< 波特率索引，单位bps
,m_uiCheckStyle(0)		//!< 检验类型索引，0-无校验，1-奇校验，2-偶校验，3-空格校验，4-符号校验
,m_uiDataBitNum(0)		//!< 数据位索引
,m_uiStopBitNum(0)		//!< 停止位索引，0-1，1-2
,m_uiRetryNum(1)		//!< 重试次数
,m_uiOverTime(1000)		//!< 通信超时时间
,m_uiCoolTime(15000)	//!< 冷却时间
,m_uiReviveTime(15000)	//!< 通信恢复时间
{
}

CDSerial::~CDSerial(void)
{
}

CString CDSerial::getName()
{
	CString text;
	if(getNumber() > INIT_1K)	return _T("无");

	text.Format("COM%d", getNumber() + 1);
	return text;
}

bool CDSerial::SerializeXml(TiXmlElement* pNode, bool bRead)
{
	CGbl* gbl = &CGbl::GetMe();
	if(bRead)
	{
		CString name, strValue;
		CComVariant cv;
		int iValue;
		name = pNode->Value();
		if(ROOT_NAME != name)		return false;
		TiXmlAttribute* pAttr = pNode->FirstAttribute();
		while(pAttr)
		{
			name = pAttr->Name();
			strValue = pAttr->Value();
			cv = strValue;
			if(SERIAL_NUMBER == name)			{cv.ChangeType(VT_I4);	iValue = cv.intVal;		m_uiNumber = UINT(iValue);}
			else if(SERIAL_BAUD == name)		{name = cv;				m_uiBaud = gbl->getBaudIndex(name);}
			else if(SERIAL_CHECK == name)		{cv.ChangeType(VT_I4);	iValue = cv.intVal;		m_uiCheckStyle = UINT(iValue);}
			else if(SERIAL_DATA == name)		{name = cv;				m_uiDataBitNum = gbl->getDataIndex(name);}
			else if(SERIAL_STOP == name)		{name = cv;				m_uiStopBitNum = gbl->getStopIndex(name);}
			else if(SERIAL_RETRYNUM == name)	{cv.ChangeType(VT_I4);	iValue = cv.intVal;		m_uiRetryNum = UINT(iValue);}
			else if(SERIAL_OVERTIME == name)	{cv.ChangeType(VT_I4);	iValue = cv.intVal;		m_uiOverTime = UINT(iValue);}
			else if(SERIAL_COOLTIME == name)	{cv.ChangeType(VT_I4);	iValue = cv.intVal;		m_uiCoolTime = UINT(iValue);}
			else if(SERIAL_REVIVETIME == name)	{cv.ChangeType(VT_I4);	iValue = cv.intVal;		m_uiReviveTime = UINT(iValue);}
			pAttr = pAttr->Next();
		}
	}
	else
	{
		pNode->SetAttribute(SERIAL_NUMBER, int(getNumber()));
		pNode->SetAttribute(SERIAL_BAUD, gbl->getBaudStr(m_uiBaud));
		pNode->SetAttribute(SERIAL_CHECK, m_uiCheckStyle);		//!< 就校验是存放索引信息
		pNode->SetAttribute(SERIAL_DATA, gbl->getDataBitNumStr(m_uiDataBitNum));
		pNode->SetAttribute(SERIAL_STOP, gbl->getStopBitNumStr(m_uiStopBitNum));
		pNode->SetAttribute(SERIAL_RETRYNUM, int(m_uiRetryNum));
		pNode->SetAttribute(SERIAL_OVERTIME, int(m_uiOverTime));
		pNode->SetAttribute(SERIAL_COOLTIME, int(m_uiCoolTime));
		pNode->SetAttribute(SERIAL_REVIVETIME, int(m_uiReviveTime));
	}
	return true;
}

CDSerial& CDSerial::operator = (CDSerial& serial)
{
	ASSERT(FALSE);
	setNumber(serial.getNumber());				//!< 串口代号，0代表COM1，1代表COM2，依此类推
	setBaud(serial.getBaud());					//!< 波特率索引，单位bps
	setCheckStyle(serial.getCheckStyle());		//!< 检验类型索引，0-无校验，1-奇校验，2-偶校验，3-空格校验，4-符号校验
	setDataBitNum(serial.getDataBitNum());		//!< 数据位索引
	setStopBitNum(serial.getStopBitNum());		//!< 停止位索引，0-1，1-2
	setRetryNum(serial.getRetryNum());			//!< 重试次数
	setOverTime(serial.getOverTime());			//!< 通信超时时间
	setCoolTime(serial.getCoolTime());			//!< 冷却时间
	setReviveTime(serial.getReviveTime());		//!< 通信恢复时间
	return *this;
}

void CDSerial::CopyFrom(CDSerial& serial)
{
	setNumber(serial.getNumber());				//!< 串口代号，0代表COM1，1代表COM2，依此类推
	setBaud(serial.getBaud());					//!< 波特率索引，单位bps
	setCheckStyle(serial.getCheckStyle());		//!< 检验类型索引，0-无校验，1-奇校验，2-偶校验，3-空格校验，4-符号校验
	setDataBitNum(serial.getDataBitNum());		//!< 数据位索引
	setStopBitNum(serial.getStopBitNum());		//!< 停止位索引，0-1，1-2
	setRetryNum(serial.getRetryNum());			//!< 重试次数
	setOverTime(serial.getOverTime());			//!< 通信超时时间
	setCoolTime(serial.getCoolTime());			//!< 冷却时间
	setReviveTime(serial.getReviveTime());		//!< 通信恢复时间
}