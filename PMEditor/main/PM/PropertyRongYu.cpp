#include "StdAfx.h"
#include "RongYuMgr.h"
#include "PropertyRongYu.h"
#include "MyHelp.h"

using namespace Servers;
using namespace RongYu;

const CString ITEM_TITLE = _T("冗余配置");
const CString BASE_INFO = _T("配置信息");
const CString ACTIVE = _T("启用DXP冗余");
const CString ACTIVE_TOOLTIP = _T("是否启动DXP冗余服务");
const CString NETTYPE = _T("通信方式");
const CString NETTYPE_TOOLTIP = _T("确定活动DXP与冗余DXP间的数据同步方式");
const CString NETPORT = _T("端口号");
const CString NETPORT_TOOLTIP = _T("设置活动DXP与冗余DXP间的通信端口号");
const CString SN = _T("验证号");
const CString SN_TOOLTIP = _T("请输出0~65535之间的数字,作为活动DXP与备份DXP之间通信的验证号");
const UINT ACTIVE_ID = 1;
const UINT NETTYPE_ID = ACTIVE_ID + 1;
const UINT NETPORT_ID = NETTYPE_ID + 1;
const UINT SN_ID = NETPORT_ID + 1;

//!< 获得本窗体的名称
CString CPropertyRongYu::GetTitle()
{
	return ITEM_TITLE;
}

//!< 显示表格的函数
void CPropertyRongYu::ShowInfo(CXTPPropertyGrid& grid)
{
	CRongYuMgr* ryMgr = &CRongYuMgr::GetMe();
	CXTPPropertyGridItem* pGroup;					//!< 属性组
	std::list<CString> strList;

	//!< 变量基本信息
	pGroup = grid.AddCategory(BASE_INFO);
	pGroup->SetTooltip(BASE_INFO);
	pGroup->Expand();

	//!< 数据库密码
	strList.clear();
	strList.push_back(_T("No"));
	strList.push_back(_T("Yes"));
	AddItemList(*pGroup, ACTIVE, ACTIVE_TOOLTIP, strList, ryMgr->IsActive() ? 1 : 0, ACTIVE_ID);

	//!< 通信方式
	//strList.clear();
	//strList.push_back(_T("UDP"));
	//strList.push_back(_T("TCP"));
	//AddItemList(*pGroup, NETTYPE, NETTYPE_TOOLTIP, strList, ryMgr->GetType(), NETTYPE_ID);

	//!< 端口号
	AddItemNumber(*pGroup, NETPORT, NETPORT_TOOLTIP, ryMgr->GetPort(), NETPORT_ID);

	//!< 验证号
	//AddItemNumber(*pGroup, SN, SN_TOOLTIP, ryMgr->GetSN(), SN_ID);
}

//!< 要保存被改变的内容
bool CPropertyRongYu::OnSaveModify(CXTPPropertyGrid& grid)
{
	int num = grid.GetCount();
	CXTPPropertyGridItem* item;
	UINT itemID;
	CRongYuMgr* ryMgr = &CRongYuMgr::GetMe();

	for(int i = 0; i < num; ++i)
	{
		item = grid.GetItem(i);
		if(!item->IsValueChanged())		continue;			//!< 只有被修改的才会进行保存，优化了一下
		itemID = item->GetID();

		if(itemID == ACTIVE_ID){							// = _T("启用OPC服务器");
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

//!< 显示帮助信息
void CPropertyRongYu::OnShowHelp()
{
	SoftInfo::CMyHelp::GetMe().ShowHelp(_T("双机冗余"));
}