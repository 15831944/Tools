#include "StdAfx.h"
#include "DXPEditor.h"
#include "Gbl.h"
#include "DeviceParam.h"
#include "XmlDevice.h"
#include "DeviceOne.h"
#include "XmlBehavior.h"
#include "XmlParaInfo.h"

const CString ROOT_NAME = _T("Para");
const CString PARAID = _T("ParaID");
const CString PARANAME = _T("ParaName");
const CString DATATYPE = _T("DataType");
const CString RBEVID = _T("RBevID");
const CString RBEVNAME = _T("RBevName");
const CString WBEVID = _T("WBevID");
const CString WBEVNAME = _T("WBevName");
const CString PARAVALUE = _T("ParaValue");

using namespace MVC;
using namespace Device;
MVC::Device::CDeviceParam::CDeviceParam(void)
{
	UINT m_uiGroupIndex = 0;		//!< 组索引，对应响应行为的索引，例如5号从设备AI通道数，这个值就是5
}

MVC::Device::CDeviceParam::~CDeviceParam(void)
{
}

bool MVC::Device::CDeviceParam::SerializeXml(TiXmlElement* pNode, bool bRead, CDeviceOne* dev)
{
	if(bRead)
	{
		CString name, strValue;
		CComVariant cv;
		int iValue;
		name = pNode->Value();
		if(ROOT_NAME != name)	return false;
		//!< 加载参数的基本信息
		TiXmlAttribute* pAttr = pNode->FirstAttribute();
		while(pAttr)
		{
			name = pAttr->Name();
			strValue = pAttr->Value();
			cv = strValue;
			if(PARAID == name)
			{
				cv.ChangeType(VT_I4);
				iValue = cv.intVal;
				setParaID((UINT)iValue, dev);
			}
			else if(PARANAME == name){m_strName = cv;}
			else if(DATATYPE == name){cv.ChangeType(VT_I4);	iValue = cv.intVal;	setType((UINT)iValue);}
			pAttr = pAttr->Next();
		}

		//!< 加载参数的数值信息
		m_vtValue.clear();
		m_vtValue.resize(pNode->GetChildSize((LPCTSTR)PARAVALUE));
		if(m_vtValue.empty()){m_vtValue.resize(1);	m_vtValue[0] = _T("0");}
		int index = 0;
		TiXmlElement* pChild = pNode->FirstChildElement();
		while(pChild)
		{
			name = pChild->Value();
			if(PARAVALUE == name)
			{
				strValue = pChild->GetText();
				cv = strValue;
				m_vtValue[index++] = cv;
			}
			pChild = pChild->NextSiblingElement();
		}

		//!< 修改VARIANT变量的类型
		if(m_uiType == 0)			ChangeType(VT_BOOL);
		else if(m_uiType == 1)		ChangeType(VT_I1);
		else if(m_uiType == 2)		ChangeType(VT_UI1);
		else if(m_uiType == 3)		ChangeType(VT_I2);
		else if(m_uiType == 4)		ChangeType(VT_UI2);
		else if(m_uiType == 5)		ChangeType(VT_I4);
		else if(m_uiType == 6)		ChangeType(VT_UI4);
		else if(m_uiType == 7)		ChangeType(VT_R4);
		else if(m_uiType == 8)		ChangeType(VT_R8);
		else if(m_uiType == 10)		ChangeType(VT_UI4);
		else if(m_uiType == 11)		ChangeType(VT_I4);
		else						ChangeType(VT_BSTR);
	}
	else
	{
		if(!m_spXmlParam)			{ASSERT(FALSE);	return true;}
		std::shared_ptr<XmlInfo::CXmlDevice> xmlDev = dev->GetXmlInfo();
		std::shared_ptr<XmlInfo::CXmlBehavior> rXmlBev = xmlDev->getBehavior(m_spXmlParam->m_uiReadBID);
		std::shared_ptr<XmlInfo::CXmlBehavior> wXmlBev = xmlDev->getBehavior(m_spXmlParam->m_uiWriteBID);
		pNode->SetAttribute(PARAID, (int)getParaID());
		pNode->SetAttribute(PARANAME, getName());
		pNode->SetAttribute(DATATYPE, (int)getType());
		if(rXmlBev){
			pNode->SetAttribute(RBEVID, xmlDev->GetAllBevID(rXmlBev->m_strName, true));
			pNode->SetAttribute(RBEVNAME, rXmlBev->m_strName);
		}
		if(wXmlBev){
			pNode->SetAttribute(WBEVID, xmlDev->GetAllBevID(wXmlBev->m_strName, false));
			pNode->SetAttribute(WBEVNAME, wXmlBev->m_strName);
		}

		CComVariant cvr;
		CString strValue;
		for(int i = 0; i < m_vtValue.size(); ++i)
		{
			cvr = m_vtValue[i];
			cvr.ChangeType(VT_BSTR);
			strValue = (CString)cvr.bstrVal;
			pNode->AddTiXmlChild((LPCTSTR)PARAVALUE)->AddTiXmlText((LPCTSTR)strValue);
		}
	}
	return true;
}

void MVC::Device::CDeviceParam::ChangeType(UINT vt)
{
	for(int i = 0; i < m_vtValue.size(); ++i)
		m_vtValue[i].ChangeType(vt);
}

//!< 记录对应的描述
void MVC::Device::CDeviceParam::setParaID(UINT paraID, CDeviceOne* projDev)
{
	std::shared_ptr<XmlInfo::CXmlDevice> xmlDev = projDev->GetXmlInfo();
	ASSERT(xmlDev);
	m_spXmlParam = xmlDev->getPara(paraID);
	m_uiParaID = paraID;
}

//!< 新建变量时要初始化参数
void MVC::Device::CDeviceParam::InitParaType(std::shared_ptr<XmlInfo::CXmlParaInfo> xmlPara, UINT groupNum/*=1*/)
{
	ASSERT(xmlPara);
	m_spXmlParam = xmlPara;
	m_uiParaID = m_spXmlParam->m_uiID;
	m_strName = m_spXmlParam->m_strName;
	m_uiType = m_spXmlParam->m_uiValType;
	SetMaxSize(groupNum);
}

//!< 设置参数的值数量，如果比原来小就不设了
void MVC::Device::CDeviceParam::SetMaxSize(UINT size)
{
	if(!m_spXmlParam)		return;
	if(size > 1)
		int j = 0;
	if(size > m_vtValue.size())
		m_vtValue.resize(size, m_spXmlParam->m_cvrDefValue);
}

CComVariant MVC::Device::CDeviceParam::getValue(UINT index)
{
	if(m_vtValue.empty())					return 0;
	if(index >= (UINT)m_vtValue.size())		index = 0;
	return m_vtValue[index];
}

void MVC::Device::CDeviceParam::setValue(CComVariant cv, UINT index)
{
	if(index >= (UINT)m_vtValue.size())		return;
	if(m_spXmlParam->m_strMaxValue.Trim() != _T("-1") && CGbl::VariantCompare(cv, m_spXmlParam->m_cvrMaxValue) > 0)
		cv = m_spXmlParam->m_cvrMaxValue;
	if(m_spXmlParam->m_strMinValue.Trim() != _T("-1") && CGbl::VariantCompare(cv, m_spXmlParam->m_cvrMinValue) < 0)
		cv = m_spXmlParam->m_cvrMinValue;
	if(!CGbl::ChangeVariantType(cv, m_vtValue[index].vt))
		return;	// 如果不成功,就不要赋值了
	try
	{
		m_vtValue[index] = cv;
	}
	catch (CMemoryException* e)
	{
		TCHAR   szCause[255];
		e->GetErrorMessage(szCause, 255);
#ifdef _DEBUG
		CGbl::PrintOut(szCause);
#endif
	}
	catch (CFileException* e)
	{
		TCHAR   szCause[255];
		e->GetErrorMessage(szCause, 255);
#ifdef _DEBUG
		CGbl::PrintOut(szCause);
#endif
	}
	catch (CException* e)
	{
		TCHAR   szCause[255];
		e->GetErrorMessage(szCause, 255);
#ifdef _DEBUG
		CGbl::PrintOut(szCause);
#endif
	}
}

MVC::Device::CDeviceParam& MVC::Device::CDeviceParam::operator = (CDeviceParam& para)
{
	m_uiParaID = para.getParaID();
	m_strName = para.getName();
	m_uiType = para.getType();
	m_spXmlParam = para.getXmlPara();
	m_vtValue.clear();
	for (CComVariant cvr : para.m_vtValue)
		m_vtValue.push_back(cvr);
	return *this;
}