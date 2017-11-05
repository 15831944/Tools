#include "stdafx.h"
#include "PMApp.h"
#include "ItemGroup.h"
#include "ItemMgr.h"
#include "Item.h"

using namespace MVC;
using namespace Item;

const CString GROUP_ID =_T("id");
const CString GROUP_NAME =_T("Name");
const CString GROUP_PARENTID =_T("ParentID");
const CString GROUP_ITEM =_T("GroupItem");
const CString GROUP_ITEMID =_T("id");

const CString DFT_NAME = _T("System");

CItemGroup::CItemGroup(CString name, UINT parentid)
:m_uiParentID(parentid)
,m_strName(name)
,m_ItemDoc(NULL)
{
}

CItemGroup::~CItemGroup(void)
{
}

// ��������б�
std::list<UINT> CItemGroup::getGroupIDList()
{
	std::list<UINT> gl;
	CItemMgr *itemMgr = &CItemMgr::GetMe();
	for(auto group: itemMgr->m_vtItemGroup){
		if(!group)			continue;
		if(group->getParentID() == m_uiID)
			gl.push_back(group->getID());		// �������б����飬�ҵ�����
	}
	return gl;
}

// ����ӱ����б�
std::list<UINT> CItemGroup::getMyItem()
{
	std::list<UINT> lt;
	CItemMgr *itemMgr = &CItemMgr::GetMe();
	for (std::shared_ptr<CItem> item : itemMgr->m_vtItem){
		if(!item)		continue;
		if(item->getMyGroupID() == m_uiID)
			lt.push_back(item->getID());
	}
	return lt;
}

// void CItemGroup::OnDelete(bool silenced/* =true */)
// {
// }

bool CItemGroup::SerializeXml(TiXmlElement* pNode, bool bRead)
{
	if(bRead)
	{
		CString name, strValue;
		TiXmlAttribute* pAttr = pNode->FirstAttribute();
		while(pAttr)
		{
			CComVariant cv;
			name = pAttr->NameTStr().c_str();
			strValue = pAttr->Value();
			cv = strValue;
			int iValue;

			if(name == GROUP_ID)			{	cv.ChangeType(VT_I4);	iValue = cv.intVal;		m_uiID = UINT(iValue);}
			else if(name == GROUP_NAME)		{	m_strName = cv;}
			else if(name == GROUP_PARENTID)	{	cv.ChangeType(VT_I4);	iValue = cv.intVal;		m_uiParentID = UINT(iValue);}

			pAttr = pAttr->Next();
		}
	}
	else
	{
		pNode->SetAttribute(GROUP_ID, getID());
		pNode->SetAttribute(GROUP_NAME, getName());
		pNode->SetAttribute(GROUP_PARENTID, getParentID());
	}
	return true;
}

void CItemGroup::InitDefault()
{
	setID(0);
	setName(DFT_NAME);
	setParentID(UINT(-1));
}

// �жϴ˱������Ƿ����Լ������飬���ٲ㶼�У�incMe�Ƿ�����Լ�
bool CItemGroup::IsChildGroup(UINT id, bool incMe /* = false */)
{
	if(id == m_uiID && incMe)			return true;
	std::shared_ptr<CItemGroup> group = CItemMgr::GetMe().GetGroup(id);
	if(group->getParentID() == m_uiID)	return true;		// ����������Լ�����ô����true
	if(group->getParentID() == -1)		return false;		// �����ͷ�ˣ�����false
	return IsChildGroup(group->getParentID(), incMe);		// ���򣬿�����һ���ǲ����Լ������飬�ݹ�
}
