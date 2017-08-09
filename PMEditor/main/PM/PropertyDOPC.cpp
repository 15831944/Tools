#include "StdAfx.h"
#include "DOPCMgr.h"
#include "PropertyDOPC.h"

const CString ITEM_TITLE = _T("OPC配置");
const CString BASE_INFO = _T("配置信息");
const CString ACTIVE = _T("启用OPC服务器");
const CString ACTIVE_TOOLTIP = _T("是否启用数据库,默认不启用");
const UINT ACTIVE_ID = 1;

using namespace Servers;
using namespace DOPC;

CPropertyDOPC::CPropertyDOPC(void)
{
}

CPropertyDOPC::~CPropertyDOPC(void)
{
}

//!< 获得本窗体的名称
CString CPropertyDOPC::GetTitle()
{
	return ITEM_TITLE;
}

//!< 显示表格的函数
void CPropertyDOPC::ShowInfo(CXTPPropertyGrid& grid)
{
	CDOPCMgr* opcMgr = &CDOPCMgr::GetMe();
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
	AddItemList(*pGroup, ACTIVE, ACTIVE_TOOLTIP, strList, opcMgr->IsActive() ? 1 : 0, ACTIVE_ID);
}

//!< 要保存被改变的内容
bool CPropertyDOPC::OnSaveModify(CXTPPropertyGrid& grid)
{
	int num = grid.GetCount();
	CXTPPropertyGridItem* item;
	UINT itemID;
	CDOPCMgr* opcMgr = &CDOPCMgr::GetMe();

	for(int i = 0; i < num; ++i)
	{
		item = grid.GetItem(i);
		if(!item->IsValueChanged())		continue;			//!< 只有被修改的才会进行保存，优化了一下
		itemID = item->GetID();

		if(itemID == ACTIVE_ID){							// = _T("启用OPC服务器");
			opcMgr->setActived(item->GetConstraints()->GetCurrent() == 1 ? true : false);
		}
	}
	return true;
}

//!< 显示帮助信息
void CPropertyDOPC::OnShowHelp()
{
	SoftInfo::CMyHelp::GetMe().ShowHelp(_T("OPC服务器"));
}