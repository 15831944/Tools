#include "StdAfx.h"
#include "DOPCMgr.h"
#include "PropertyDOPC.h"

const CString ITEM_TITLE = _T("OPC����");
const CString BASE_INFO = _T("������Ϣ");
const CString ACTIVE = _T("����OPC������");
const CString ACTIVE_TOOLTIP = _T("�Ƿ��������ݿ�,Ĭ�ϲ�����");
const UINT ACTIVE_ID = 1;

using namespace Servers;
using namespace DOPC;

CPropertyDOPC::CPropertyDOPC(void)
{
}

CPropertyDOPC::~CPropertyDOPC(void)
{
}

//!< ��ñ����������
CString CPropertyDOPC::GetTitle()
{
	return ITEM_TITLE;
}

//!< ��ʾ���ĺ���
void CPropertyDOPC::ShowInfo(CXTPPropertyGrid& grid)
{
	CDOPCMgr* opcMgr = &CDOPCMgr::GetMe();
	CXTPPropertyGridItem* pGroup;					//!< ������
	std::list<CString> strList;

	//!< ����������Ϣ
	pGroup = grid.AddCategory(BASE_INFO);
	pGroup->SetTooltip(BASE_INFO);
	pGroup->Expand();

	//!< ���ݿ�����
	strList.clear();
	strList.push_back(_T("No"));
	strList.push_back(_T("Yes"));
	AddItemList(*pGroup, ACTIVE, ACTIVE_TOOLTIP, strList, opcMgr->IsActive() ? 1 : 0, ACTIVE_ID);
}

//!< Ҫ���汻�ı������
bool CPropertyDOPC::OnSaveModify(CXTPPropertyGrid& grid)
{
	int num = grid.GetCount();
	CXTPPropertyGridItem* item;
	UINT itemID;
	CDOPCMgr* opcMgr = &CDOPCMgr::GetMe();

	for(int i = 0; i < num; ++i)
	{
		item = grid.GetItem(i);
		if(!item->IsValueChanged())		continue;			//!< ֻ�б��޸ĵĲŻ���б��棬�Ż���һ��
		itemID = item->GetID();

		if(itemID == ACTIVE_ID){							// = _T("����OPC������");
			opcMgr->setActived(item->GetConstraints()->GetCurrent() == 1 ? true : false);
		}
	}
	return true;
}

//!< ��ʾ������Ϣ
void CPropertyDOPC::OnShowHelp()
{
	SoftInfo::CMyHelp::GetMe().ShowHelp(_T("OPC������"));
}