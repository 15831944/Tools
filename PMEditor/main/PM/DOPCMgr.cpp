#include "StdAfx.h"
#include "ProjectMgr.h"
#include "PropertyDlg.h"
#include "DOPCMgr.h"

const CString ROOTNAME = _T("DOPCSet");
const CString ACTIVED = _T("Active");

using namespace Servers;
using namespace DOPC;

CDOPCMgr::CDOPCMgr(void)
:m_bActive(false)
{
}

CDOPCMgr::~CDOPCMgr(void)
{
}

CDOPCMgr& CDOPCMgr::GetMe()
{
	static CDOPCMgr one;
	return one;
}

void CDOPCMgr::OnCreate()
{
	m_bActive = false;
}

void CDOPCMgr::OnSetDOPC()
{
	if(CProjectMgr::GetMe().SayWatch())		return;
	Dialog::CPropertyDlg* propertyDlg = &Dialog::CPropertyDlg::GetMe();
	propertyDlg->DoModal(m_DopcProperty);
}

//!< ����xml��ʽ�ı༭��Ϣ
bool CDOPCMgr::SerializeXml(TiXmlElement* pNode, bool bRead)
{
	if(bRead)
	{
		//!< �ӽڵ�
		CString name, strValue;
		CComVariant cvr;
		name = pNode->ValueStr().c_str();
		if(ROOTNAME != name)	return false;
		TiXmlAttribute* pAttr = pNode->FirstAttribute();
		while(pAttr)
		{
			name = pAttr->Name();
			strValue = pAttr->Value();
			cvr = strValue;
			if(ACTIVED == name)				{cvr.ChangeType(VT_I4);		m_bActive = cvr.intVal == 1 ? true : false;}
			pAttr = pAttr->Next();
		}
	}
	else
	{
		pNode->SetAttribute(ACTIVED, m_bActive ? 1 : 0);
	}
	m_bModify = false;				//!< ���ػ򱣴��깤�̺���δ�޸�״̬
	return true;
}

void CDOPCMgr::OnClose()
{
	m_bActive = false;				//!< �Ƿ�����
}

void CDOPCMgr::setActived(bool actived)		{if(actived != m_bActive)		SetModify();	m_bActive = actived;}
