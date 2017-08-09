#include "StdAfx.h"
#include "MyCrc.h"
#include "ProjectMgr.h"
#include "RongYuMgr.h"
#include "PropertyDlg.h"

const CString ROOTNAME = _T("RongYuSet");
const CString ACTIVED = _T("Active");
const CString NETTYPE = _T("Type");
const CString NETPORT = _T("Port");
const CString SN = _T("Sn");

using namespace Servers;
using namespace RongYu;
CRongYuMgr::CRongYuMgr(void)
:m_bActive(false)		// 是否使能
,m_nType(0)				// 通信类型,0UDP,1TCP
,m_nPort(13001)			// 端口号
,m_nSn(1)				// 验证号
{
}

CRongYuMgr::~CRongYuMgr(void)
{
}

void CRongYuMgr::OnCreate()
{
	m_bActive = false;
	GUID _guid;
	::CoCreateGuid(&_guid);
	m_nSn = Tool::CMyCrc::CRC16((byte *)&_guid, sizeof(GUID));
}

// 配置冗余属性
void CRongYuMgr::OnSetRongYu()
{
	if(CProjectMgr::GetMe().SayWatch())		return;
	Dialog::CPropertyDlg* propertyDlg = &Dialog::CPropertyDlg::GetMe();
	propertyDlg->DoModal(m_RongYuProperty);
}

//!< 保存xml格式的编辑信息
bool CRongYuMgr::SerializeXml(TiXmlElement* pNode, bool bRead)
{
	if(bRead)
	{
		//!< 子节点
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
			else if(NETTYPE == name)		{cvr.ChangeType(VT_I4);		m_nType = cvr.intVal;}
			else if(NETPORT == name)		{cvr.ChangeType(VT_I4);		m_nPort = cvr.intVal;}
			else if(SN == name)				{cvr.ChangeType(VT_I4);		m_nSn = cvr.intVal;}
			pAttr = pAttr->Next();
		}
	}
	else
	{
		pNode->SetAttribute(ACTIVED, m_bActive ? 1 : 0);
		pNode->SetAttribute(NETTYPE, m_nType);
		pNode->SetAttribute(NETPORT, m_nPort);
		pNode->SetAttribute(SN, m_nSn);
	}
	m_bModify = false;				//!< 加载或保存完工程后处于未修改状态
	return true;
}

void CRongYuMgr::SetActived(bool actived)	{if (actived != m_bActive)	SetModify();	m_bActive = actived;}
void CRongYuMgr::SetType(int type)			{if (type != m_nType)		SetModify();	m_nType = type;}
void CRongYuMgr::SetPort(int port)			{if (port != m_nPort)		SetModify();	m_nPort = port;}
void CRongYuMgr::SetSN(WORD sn)				{if (sn != m_nSn)			SetModify();	m_nSn = sn;}