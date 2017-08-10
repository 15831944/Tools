#include "stdafx.h"
#include "PMApp.h"
#include "Gbl.h"
#include "Item.h"
#include "SourceProperty.h"

#include "DevMgr.h"
#include "DeviceOne.h"
#include "XmlDevice.h"
#include "XmlArea.h"

using namespace MVC;
using namespace Item;

const CString SRC_SCRIPTTEXT = _T("ScriptText");
const CString SRC_DEVICEID = _T("DeviceID");
const CString SRC_AREAID = _T("AreaID");
const CString SRC_UNITINDEX = _T("UnitIndex");
const CString SRC_BITINDEX = _T("BitIndex");
const CString SRC_IOTYPE = _T("IOType");
const CString SRC_OPERATE = _T("OperateType");
const CString SRC_FRESHTIME = _T("FreshTime");
const CString SRC_DELAYFLAG = _T("DelayFlag");
const CString SRC_CONVERTTYPE = _T("ConvertType");
const CString SRC_MINPROJ = _T("MinProjValue");
const CString SRC_MAXPROJ = _T("MaxProjValue");
const CString SRC_MINIO = _T("MinIOValue");
const CString SRC_MAXIO = _T("MaxIOValue");

CPropertySource::CPropertySource(CItem* item)
{
	m_pParent = item;

	m_strScriptText = _T("");		//!< 赋值脚本文本
	m_uiDeviceID = 0;				//!< 设备编号
	m_uiAreaID = 0;					//!< 区编号
	m_uiUnitIndex = 0;				//!< 寻址偏移
	m_uiBitIndex = 0;				//!< 位偏移
	m_uiIOType = 0;					//!< IO类型，具体内容看描述文件
	m_uiFreshTime = 500;			//!< 变量刷新时间,单位 毫秒
	m_bDelayFlag = true;			//!< 可否允许滞后
	m_uiConvertType = 0;			//!< 转换类型，0不转换，1是否取反(位变量使用)，1线性转换
	float fDef = 0.0;
	m_MaxProjectValue = m_MinProjectValue = m_MaxIOValue = m_MinIOValue = fDef;		//!< 量程转换都是浮点的
}

CPropertySource::~CPropertySource(void)
{
}

bool CPropertySource::SerializeXml(TiXmlElement* pNode, bool bRead, bool iExport/* = false*/)
{
	if(bRead)
	{
		CString name, strValue;
		CComVariant cv;
		TiXmlAttribute* pAttr = pNode->FirstAttribute();
		while(pAttr)
		{
			name = pAttr->Name();
			strValue = pAttr->Value();
			cv = strValue;
			int iValue;

			if(name == SRC_SCRIPTTEXT)		{m_strScriptText = strValue;}
			else if(name == SRC_FRESHTIME)	{cv.ChangeType(VT_I4);	iValue = cv.intVal;		setFreshTime(UINT(iValue));}
			else if(name == SRC_DEVICEID)	{cv.ChangeType(VT_I4);	iValue = cv.intVal;		m_uiDeviceID = UINT(iValue);}
			else if(name == SRC_AREAID)		{cv.ChangeType(VT_I4);	iValue = cv.intVal;		m_uiAreaID = UINT(iValue);}
			else if(name == SRC_UNITINDEX)	{cv.ChangeType(VT_I4);	iValue = cv.intVal;		m_uiUnitIndex = UINT(iValue);}
			else if(name == SRC_BITINDEX)	{cv.ChangeType(VT_I4);	iValue = cv.intVal;		m_uiBitIndex = UINT(iValue);}
			else if(name == SRC_IOTYPE)		{cv.ChangeType(VT_I4);	iValue = cv.intVal;		m_uiIOType = UINT(iValue);}
			else if(name == SRC_DELAYFLAG)	{cv.ChangeType(VT_BOOL);	m_bDelayFlag = cv.boolVal;}
			else if(name == SRC_CONVERTTYPE){cv.ChangeType(VT_I4);	iValue = cv.intVal;		m_uiConvertType = UINT(iValue);}
			else if(name == SRC_MINPROJ)	{cv.ChangeType(m_MinProjectValue.vt);			m_MinProjectValue = cv;}
			else if(name == SRC_MAXPROJ)	{cv.ChangeType(m_MaxProjectValue.vt);			m_MaxProjectValue = cv;}
			else if(name == SRC_MINIO)		{cv.ChangeType(m_MinIOValue.vt);				m_MinIOValue = cv;}
			else if(name == SRC_MAXIO)		{cv.ChangeType(m_MaxIOValue.vt);				m_MaxIOValue = cv;}
			pAttr = pAttr->Next();
		}
	}
	else
	{
		int vt = m_pParent->getDefault().vt;
		pNode->SetAttribute(SRC_SCRIPTTEXT, getScriptText());
		if(!iExport)	pNode->SetAttribute(SRC_DEVICEID, getDeviceID());
		pNode->SetAttribute(SRC_AREAID, getAreaID());
		pNode->SetAttribute(SRC_UNITINDEX, getUnitIndex());
		pNode->SetAttribute(SRC_BITINDEX, getBitIndex());
		pNode->SetAttribute(SRC_IOTYPE, getIOType());
		pNode->SetAttribute(SRC_OPERATE, GetOperateType());
		pNode->SetAttribute(SRC_FRESHTIME, getFreshTime());
		pNode->SetAttribute(SRC_DELAYFLAG, getDelayFlag()?1:0);
		pNode->SetAttribute(SRC_CONVERTTYPE, getConvertType());
		m_MaxProjectValue.ChangeType(VT_BSTR);
		m_MinProjectValue.ChangeType(VT_BSTR);
		m_MaxIOValue.ChangeType(VT_BSTR);
		m_MinIOValue.ChangeType(VT_BSTR);

		pNode->SetAttribute(SRC_MINPROJ, (CString)m_MinProjectValue.bstrVal);
		pNode->SetAttribute(SRC_MAXPROJ, (CString)m_MaxProjectValue.bstrVal);
		pNode->SetAttribute(SRC_MINIO, (CString)m_MinIOValue.bstrVal);
		pNode->SetAttribute(SRC_MAXIO, (CString)m_MaxIOValue.bstrVal);
		m_MaxProjectValue.ChangeType(vt);
		m_MinProjectValue.ChangeType(vt);
		m_MaxIOValue.ChangeType(vt);
		m_MinIOValue.ChangeType(vt);
	}
	return true;
}

//!< 解析数据源属性
void CPropertySource::ReadFromPMExcel(std::vector<CString>& vtCell)
{
	m_strScriptText = vtCell[11].Trim();
	m_uiDeviceID = (UINT)atoi(vtCell[12].Trim());
	m_uiAreaID = (UINT)atoi(vtCell[13].Trim());
	m_uiIOType = (UINT)atoi(vtCell[14].Trim());
	m_uiUnitIndex = (UINT)atoi(vtCell[15].Trim());
	m_uiBitIndex = (UINT)atoi(vtCell[16].Trim());
	setFreshTime((UINT)atoi(vtCell[17].Trim()));
	m_bDelayFlag = (vtCell[18].Trim() == "1");
	m_uiConvertType = (UINT)atoi(vtCell[19].Trim());
	m_MinProjectValue = vtCell[20].Trim();
	m_MaxProjectValue = vtCell[21].Trim();
	m_MinIOValue = vtCell[22].Trim();
	m_MaxIOValue = vtCell[23].Trim();
}

CPropertySource& CPropertySource::operator = (CPropertySource& src)
{
	//!< m_pParent不需要赋
	m_strScriptText = src.getScriptText();
	setFreshTime(src.getFreshTime());
	m_uiDeviceID = src.getDeviceID();
	m_uiAreaID = src.getAreaID();
	m_uiUnitIndex = src.getUnitIndex();
	m_uiBitIndex = src.getBitIndex();
	m_uiIOType = src.getIOType();
	m_bDelayFlag = src.getDelayFlag();
	m_uiConvertType = src.getConvertType();
	m_MaxProjectValue = src.getProjMax();
	m_MinProjectValue = src.getProjMin();
	m_MaxIOValue = src.getIOMax();
	m_MinIOValue = src.getIOMin();
	return *this;
}

bool CPropertySource::operator == (CPropertySource& src)
{
	//!< m_pParent不需要赋
	if(m_strScriptText != src.getScriptText())		return false;
	if(m_uiFreshTime != src.getFreshTime())			return false;
	if(m_uiDeviceID != src.getDeviceID())			return false;
	if(m_uiAreaID != src.getAreaID())				return false;
	if(m_uiUnitIndex != src.getUnitIndex())			return false;
	if(m_uiBitIndex != src.getBitIndex())			return false;
	if(m_uiIOType != src.getIOType())				return false;
	if(m_bDelayFlag != src.getDelayFlag())			return false;
	if(m_uiConvertType != src.getConvertType())		return false;
	if(m_MaxProjectValue != src.getProjMax())		return false;
	if(m_MinProjectValue != src.getProjMin())		return false;
	if(m_MaxIOValue != src.getIOMax())				return false;
	if(m_MinIOValue != src.getIOMin())				return false;
	return true;
}

CString CPropertySource::GetAreaString()
{
	CString strEmpty = _T("");
	if(!m_pParent)									return strEmpty;
	if(m_pParent->getSrcType() != CItem::SRC_TYPE_IO)				return strEmpty;
	std::shared_ptr<MVC::Device::CDeviceOne> device = MVC::Device::CDevMgr::GetMe().GetDevice(m_uiDeviceID);
	if(!device)										return strEmpty;
	std::shared_ptr<XmlInfo::CXmlDevice> xmlDev = device->GetXmlInfo();
	std::shared_ptr<XmlInfo::CXmlArea> xmlArea = xmlDev->getArea(m_uiAreaID);
	if(!xmlArea)									return strEmpty;
	CString strArea = xmlArea->getName();
	CString strUnit;
	strUnit.Format("%d",m_uiUnitIndex);
	CString strBit;
	if(m_uiIOType == 0)
	{
		strBit.Format("%d",m_uiBitIndex);
	}
	else if(m_uiIOType == 1 || m_uiIOType == 2)
	{
		strArea += _T("B");
	}
	else if(m_uiIOType == 3 || m_uiIOType == 4)
	{
		strArea += _T("W");
	}
	else if(m_uiIOType == 5 || m_uiIOType == 6 || m_uiIOType == 7)
	{
		strArea += _T("D");
	}
	if(xmlDev->getDevType() == MVC::Device::CDevMgr::MODBUS_SERIAL ||
		xmlDev->getDevType() == MVC::Device::CDevMgr::MODBUS_UDP ||
		xmlDev->getDevType() == MVC::Device::CDevMgr::MODBUS_TCP)
		strArea = _T("");

	strArea += strUnit;
	if(m_uiIOType == 0)
	{
		//!< 除Modbus设备外,所有的位都需要有个点,这个特殊处理太恶心,被领导逼迫的
		if(xmlDev->getDevType() != MVC::Device::CDevMgr::MODBUS_SERIAL &&
			xmlDev->getDevType() != MVC::Device::CDevMgr::MODBUS_UDP &&
			xmlDev->getDevType() != MVC::Device::CDevMgr::MODBUS_TCP)
			strArea = strArea + _T(".") + strBit;
	}
	return strArea;
}

//!< 获得当前IO类型值多少个单位寻址长度
UINT CPropertySource::GetOperateType()
{
	UINT ioType = getIOType();
	UINT operateType = 0;
	std::shared_ptr<MVC::Device::CDeviceOne> device = MVC::Device::CDevMgr::GetMe().GetDevice(m_uiDeviceID);
	if(!device)										return operateType;
	std::shared_ptr<XmlInfo::CXmlDevice> xmlDev = device->GetXmlInfo();
	std::shared_ptr<XmlInfo::CXmlArea> xmlArea = xmlDev->getArea(m_uiAreaID);
	if(!xmlArea)									return operateType;
	UINT bitLen = 0;
	if(ioType == 0)										bitLen = 1;
	else if(ioType == 1 || ioType == 2)					bitLen = 8;
	else if(ioType == 3 || ioType == 4)					bitLen = 16;
	else if(ioType == 5 || ioType == 6 || ioType == 7)	bitLen = 32;
	operateType = bitLen / xmlArea->m_uiUnitBitLen;		//!< 位长度除以单位寻址长度，等于操作类型

	return operateType;
}

//!< 设置Config的变量地址属性
bool MVC::Item::CPropertySource::SetConfigAddr(int devID, CString strAddr, CString strType)
{
	setDevID(devID);
	std::shared_ptr<MVC::Device::CDeviceOne> dev = MVC::Device::CDevMgr::GetMe().GetDevice(devID);
	if(!dev)													return false;
	std::shared_ptr<XmlInfo::CXmlDevice> xmlDev = dev->GetXmlInfo();
	if(!xmlDev)													return false;

	std::vector<std::string> vtAddr;
	std::string strRegex = _T("(^[a-zA-Z]*?)([0-9]*?)([.]?)([0-9]*?)$");
	std::string stringAddr = (LPCTSTR)strAddr;
	if(!CGbl::GetMe().RegexMatch(strRegex, stringAddr, vtAddr))	return false;
	CString str0 = vtAddr[0].c_str(), str1 = vtAddr[1].c_str(), str2 = vtAddr[2].c_str(), str3 = vtAddr[3].c_str(), str4 = vtAddr[4].c_str();
	if(str1 == _T("") || str4 == _T(""))						return false;

	if(strType == _T("位变量"))									//!< 位变量
	{	//vtAddr={"V20.12", "V", "20", ".", "12"}
		//vtAddr={"T10", "T", "", "", "10"}
		/************************************************************************/
		/* T、C的特殊处理部分	                                        */
		CString strArea = str1;
		if(strArea == _T("T") || strArea == _T("C")){
			strArea = strArea + _T("(位操作)");
			str2 = str4;
			str4 = _T("0");
		}
		/************************************************************************/
		setAreaID(xmlDev->GetAreaID(strArea));
		setIOType(0);
		setUnitIndex((UINT)atoi(str2));
		setBitIndex((UINT)atoi(str4));
	}
	else if(str2 == _T("") && str3 == _T(""))			//!< 字或双字
	{	//vtAddr={"VW2000", "VW", "2000", "", ""}
		/************************************************************************/
		/* V区扩展、T、C的特殊处理部分	                                        */
		CString strArea = str1;
		if(strArea.Right(1) == _T("D") || strArea.Right(1) == _T("W"))
			strArea = strArea.Left(strArea.GetLength() - 1);
		if(strArea == _T("V"))
		{
			UINT nUnitIndex = (UINT)atoi(str4);			//寻址偏移
			std::shared_ptr<XmlInfo::CXmlArea> xmlArea = xmlDev->getArea(strArea);
			ASSERT(xmlArea);
			if(nUnitIndex > xmlArea->GetUnitEnd(GetTypeFromStr(strType)))
				strArea = strArea + _T("x");
		}
		else if(strArea == _T("T") || strArea == _T("C"))
			strArea = strArea + _T("(字操作)");
		/************************************************************************/
		setAreaID(xmlDev->GetAreaID(strArea));
		setIOType(GetTypeFromStr(strType));
		setUnitIndex((UINT)atoi(str4));
	}
	else return false;
	return true;
}

//!< 根据名称获得类型
int MVC::Item::CPropertySource::GetTypeFromStr(CString strType)
{
	if(strType == _T("有符号双字节"))				return 3;		//!< 将最容易被用的放在上面,提高效率
	else if(strType == _T("无符号双字节"))			return 4;
	else if(strType == _T("有符号四字节"))			return 5;
	else if(strType == _T("无符号四字节"))			return 6;
	else if(strType == _T("四字节浮点数"))			return 7;
	else if(strType == _T("位变量"))				return 0;
	else if(strType == _T("有符号字节"))			return 1;
	else if(strType == _T("无符号字节"))			return 2;
	return -1;
}
