#include "StdAfx.h"
#include "RongYuMgr.h"
#include "PropertyRongYu.h"
#include "MyHelp.h"

using namespace Servers;
using namespace RongYu;

const CString ITEM_TITLE = _T("��������");
const CString BASE_INFO = _T("������Ϣ");
const CString ACTIVE = _T("����DXP����");
const CString ACTIVE_TOOLTIP = _T("�Ƿ�����DXP�������");
const CString NETTYPE = _T("ͨ�ŷ�ʽ");
const CString NETTYPE_TOOLTIP = _T("ȷ���DXP������DXP�������ͬ����ʽ");
const CString NETPORT = _T("�˿ں�");
const CString NETPORT_TOOLTIP = _T("���ûDXP������DXP���ͨ�Ŷ˿ں�");
const CString SN = _T("��֤��");
const CString SN_TOOLTIP = _T("�����0~65535֮�������,��Ϊ�DXP�뱸��DXP֮��ͨ�ŵ���֤��");
const UINT ACTIVE_ID = 1;
const UINT NETTYPE_ID = ACTIVE_ID + 1;
const UINT NETPORT_ID = NETTYPE_ID + 1;
const UINT SN_ID = NETPORT_ID + 1;

//!< ��ñ����������
CString CPropertyRongYu::GetTitle()
{
	return ITEM_TITLE;
}

//!< ��ʾ���ĺ���
void CPropertyRongYu::ShowInfo(CXTPPropertyGrid& grid)
{
	CRongYuMgr* ryMgr = &CRongYuMgr::GetMe();
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
	AddItemList(*pGroup, ACTIVE, ACTIVE_TOOLTIP, strList, ryMgr->IsActive() ? 1 : 0, ACTIVE_ID);

	//!< ͨ�ŷ�ʽ
	//strList.clear();
	//strList.push_back(_T("UDP"));
	//strList.push_back(_T("TCP"));
	//AddItemList(*pGroup, NETTYPE, NETTYPE_TOOLTIP, strList, ryMgr->GetType(), NETTYPE_ID);

	//!< �˿ں�
	AddItemNumber(*pGroup, NETPORT, NETPORT_TOOLTIP, ryMgr->GetPort(), NETPORT_ID);

	//!< ��֤��
	//AddItemNumber(*pGroup, SN, SN_TOOLTIP, ryMgr->GetSN(), SN_ID);
}

//!< Ҫ���汻�ı������
bool CPropertyRongYu::OnSaveModify(CXTPPropertyGrid& grid)
{
	int num = grid.GetCount();
	CXTPPropertyGridItem* item;
	UINT itemID;
	CRongYuMgr* ryMgr = &CRongYuMgr::GetMe();

	for(int i = 0; i < num; ++i)
	{
		item = grid.GetItem(i);
		if(!item->IsValueChanged())		continue;			//!< ֻ�б��޸ĵĲŻ���б��棬�Ż���һ��
		itemID = item->GetID();

		if(itemID == ACTIVE_ID){							// = _T("����OPC������");
			ryMgr->SetActived(item->GetConstraints()->GetCurrent() == 1 ? true : false);
		}
		//else if(itemID == NETTYPE_ID){
		//	ryMgr->SetType(item->GetConstraints()->GetCurrent());
		//}
		else if(itemID == NETPORT_ID){
			ryMgr->SetPort(((CXTPPropertyGridItemNumber *)item)->GetNumber());
		}
		//else if(itemID == SN_ID){
		//	ryMgr->SetSN(abs(((CXTPPropertyGridItemNumber *)item)->GetNumber()));
		//	((CXTPPropertyGridItemNumber *)item)->SetNumber(ryMgr->GetSN());
		//}
	}
	return true;
}

//!< ��ʾ������Ϣ
void CPropertyRongYu::OnShowHelp()
{
	SoftInfo::CMyHelp::GetMe().ShowHelp(_T("˫������"));
}