#include "stdafx.h"
#include "PMApp.h"
#include "Gbl.h"
#include "Item.h"
#include "SourceProperty.h"

using namespace MVC;
using namespace Item;

const CString SRC_SCRIPTTEXT = _T("ScriptText");
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

	m_strScriptText = _T("");		// ��ֵ�ű��ı�
	m_uiIOType = 0;					// IO���ͣ��������ݿ������ļ�
	m_uiFreshTime = 500;			// ����ˢ��ʱ��,��λ ����
	m_uiConvertType = 0;			// ת�����ͣ�0��ת����1�Ƿ�ȡ��(λ����ʹ��)��1����ת��
	float fDef = 0.0;
	m_MaxProjectValue = m_MinProjectValue = m_MaxIOValue = m_MinIOValue = fDef;		// ����ת�����Ǹ����
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
			else if(name == SRC_IOTYPE)		{cv.ChangeType(VT_I4);	iValue = cv.intVal;		m_uiIOType = UINT(iValue);}
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
		pNode->SetAttribute(SRC_IOTYPE, getIOType());
		pNode->SetAttribute(SRC_FRESHTIME, getFreshTime());
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

// ��������Դ����
void CPropertySource::ReadFromPMExcel(std::vector<CString>& vtCell)
{
	m_strScriptText = vtCell[11].Trim();
	m_uiIOType = (UINT)atoi(vtCell[14].Trim());
	setFreshTime((UINT)atoi(vtCell[17].Trim()));
	m_uiConvertType = (UINT)atoi(vtCell[19].Trim());
	m_MinProjectValue = vtCell[20].Trim();
	m_MaxProjectValue = vtCell[21].Trim();
	m_MinIOValue = vtCell[22].Trim();
	m_MaxIOValue = vtCell[23].Trim();
}

CPropertySource& CPropertySource::operator = (CPropertySource& src)
{
	// m_pParent����Ҫ��
	m_strScriptText = src.getScriptText();
	setFreshTime(src.getFreshTime());
	m_uiIOType = src.getIOType();
	m_uiConvertType = src.getConvertType();
	m_MaxProjectValue = src.getProjMax();
	m_MinProjectValue = src.getProjMin();
	m_MaxIOValue = src.getIOMax();
	m_MinIOValue = src.getIOMin();
	return *this;
}

bool CPropertySource::operator == (CPropertySource& src)
{
	// m_pParent����Ҫ��
	if(m_strScriptText != src.getScriptText())		return false;
	if(m_uiFreshTime != src.getFreshTime())			return false;
	if(m_uiIOType != src.getIOType())				return false;
	if(m_uiConvertType != src.getConvertType())		return false;
	if(m_MaxProjectValue != src.getProjMax())		return false;
	if(m_MinProjectValue != src.getProjMin())		return false;
	if(m_MaxIOValue != src.getIOMax())				return false;
	if(m_MinIOValue != src.getIOMin())				return false;
	return true;
}

// �������ƻ������
int MVC::Item::CPropertySource::GetTypeFromStr(CString strType)
{
	if(strType == _T("�з���˫�ֽ�"))				return 3;		// �������ױ��õķ�������,���Ч��
	else if(strType == _T("�޷���˫�ֽ�"))			return 4;
	else if(strType == _T("�з������ֽ�"))			return 5;
	else if(strType == _T("�޷������ֽ�"))			return 6;
	else if(strType == _T("���ֽڸ�����"))			return 7;
	else if(strType == _T("λ����"))				return 0;
	else if(strType == _T("�з����ֽ�"))			return 1;
	else if(strType == _T("�޷����ֽ�"))			return 2;
	return -1;
}
