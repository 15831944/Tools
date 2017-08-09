#include "StdAfx.h"
#include "XmlParaInfo.h"

using namespace XmlInfo;

const CString ROOTNAME = _T("Para");
const CString PARAID = _T("ParaID");
const CString PARANAME = _T("ParaName");
const CString PARACOMMENT = _T("ParaExplain");
const CString PARAGROUP = _T("GroupName");
const CString ACCESSRIGHT = _T("Attribute");
const CString DATATYPE = _T("DataType");
const CString SPECIAL = _T("SpecialID");
const CString DEFAULT = _T("DefaultValue");
const CString MAXVALUE = _T("MaxValue");
const CString MINVALUE = _T("MinValue");
const CString UINTTEXT = _T("Unit");
const CString ENUMID = _T("EnumID");
const CString RBEHAVIOR = _T("RdBehaviorID");
const CString WBEHAVIOR = _T("WtBehaviorID");

CXmlParaInfo::CXmlParaInfo(void)
{
	m_strGroup = _T("");
	m_nSpecialID = -1;
}

CXmlParaInfo::~CXmlParaInfo(void)
{
}

//!< 解析自己的节点
bool CXmlParaInfo::SerializeXml(TiXmlElement* pNode)
{
	CString name;
	CComVariant cvr;
	name = pNode->Value();
	if(ROOTNAME != name)	return false;
	TiXmlElement* pRoot = pNode->FirstChildElement();
	int iValue;
	while(pRoot){
		name = pRoot->Value();
		cvr = pRoot->GetText();
		if(PARAID == name)			{cvr.ChangeType(VT_I4);		iValue = cvr.intVal;	m_uiID = UINT(iValue);			}
		else if(PARANAME == name)	{							m_strName = cvr;		}
		else if(PARACOMMENT == name){							m_strComment = cvr;		}
		else if(PARAGROUP == name)	{							m_strGroup = cvr;		}
		else if(ACCESSRIGHT == name){cvr.ChangeType(VT_I4);		iValue = cvr.intVal;	m_uiAccessRight = UINT(iValue);	}
		else if(DATATYPE == name)	{cvr.ChangeType(VT_I4);		iValue = cvr.intVal;	m_uiValType = UINT(iValue);		}
		else if(SPECIAL == name)	{cvr.ChangeType(VT_I4);		iValue = cvr.intVal;	m_nSpecialID = iValue;			}
		else if(DEFAULT == name)	{							m_cvrDefValue = cvr;	}
		else if(MAXVALUE == name)	{							m_cvrMaxValue = cvr;	m_strMaxValue = cvr;			}
		else if(MINVALUE == name)	{							m_cvrMinValue = cvr;	m_strMinValue = cvr;			}
		else if(UINTTEXT == name)	{							m_strUnit = cvr;		}
		else if(ENUMID == name)		{cvr.ChangeType(VT_I4);		iValue = cvr.intVal;	m_uiEnumID = UINT(iValue);		}
		else if(RBEHAVIOR == name)	{cvr.ChangeType(VT_I4);		iValue = cvr.intVal;	m_uiReadBID = UINT(iValue);		}
		else if(WBEHAVIOR == name)	{cvr.ChangeType(VT_I4);		iValue = cvr.intVal;	m_uiWriteBID = UINT(iValue);	}
		pRoot = pRoot->NextSiblingElement();
	}
	//!< 修改VARIANT变量的类型
	if(m_uiValType == 0)			ChangeType(VT_BOOL);
	else if(m_uiValType == 1)		ChangeType(VT_I1);
	else if(m_uiValType == 2)		ChangeType(VT_UI1);
	else if(m_uiValType == 3)		ChangeType(VT_I2);
	else if(m_uiValType == 4)		ChangeType(VT_UI2);
	else if(m_uiValType == 5)		ChangeType(VT_I4);
	else if(m_uiValType == 6)		ChangeType(VT_UI4);
	else if(m_uiValType == 7)		ChangeType(VT_R4);
	else if(m_uiValType == 8)		ChangeType(VT_R8);
	else if(m_uiValType == 10)		ChangeType(VT_UI4);
	else if(m_uiValType == 11)		ChangeType(VT_I4);
	else							ChangeType(VT_BSTR);
	return true;
}

//!< 根据参数类型的不同，我要改变variant类型属性的具体类型
void CXmlParaInfo::ChangeType(UINT type)
{
	m_cvrDefValue.ChangeType(type);		//!< 参数的默认值
	m_cvrMaxValue.ChangeType(type);		//!< 参数的最大值
	m_cvrMinValue.ChangeType(type);		//!< 参数的最小值
}