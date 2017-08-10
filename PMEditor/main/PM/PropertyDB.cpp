#include "StdAfx.h"
#include "DBMgr.h"
#include "ProjectMgr.h"
#include "Project.h"
#include "PropertyDB.h"


const UINT ID_BUTTON_PATH = 1000;

const CString ITEM_TITLE = _T("数据库配置");

const CString BASE_INFO = _T("配置信息");
const CString ACTIVE = _T("启用数据库");
const CString PASSWORD = _T("数据库密码");
const CString MYSQLPORT=_T("数据库端口");
const CString DISKMIN = _T("硬盘报警值（单位：M）");
const CString SAVETIME = _T("存储周期（单位：秒）");
const CString SAVETYPE = _T("存储方式");
//const CString SAVEAVERAGE = _T("平均值次数");
const CString SAVECHANGERATE = _T("最小变化率");
const CString TBROWMAX = _T("数据库分表条数");
const CString BACKUPACTIVE = _T("启用定时备份");
const CString BACKUPPATH = _T("数据库备份路径");
const CString BACKUPDAY = _T("备份天数间隔（单位：天）");
const CString BACKUPHOUR = _T("备份时间（单位：点）");

const CString ACTIVE_TOOLTIP = _T("是否启用数据库,默认启用");
const CString PASSWORD_TOOLTIP = _T("连接数据库的密码");
const CString MYSQLPORT_TOOLTIP=_T("连接数据库的端口号（默认为3306）");
const CString DISKMIN_TOOLTIP = _T("数据库磁盘的剩余空间小于指定大小时，蜂鸣器将报警");
const CString SAVETIME_TOOLTIP = _T("变量值存储数据库的时间间隔, 0.01~3600000");
const CString SAVETYPE_TOOLTIP = _T("直接存储：每次刷新数据直接保存历史数据\r\n改变再存储：变量发生改变时再保存历史数据");
//const CString SAVEAVERAGE_TOOLTIP = _T("平均次数, 1～999");
const CString SAVECHANGERATE_TOOLITP = _T("最小变化率，0~100，0表示只要变化就存储");
const CString TBROWMAX_TOOLITP = _T("当历史数据库中记录的条数大于设定值后，可重新建表，默认值为100000，最小值为100");
const CString BACKUPACTIVE_TOOLTIP = _T("启用数据库定时备份功能");
const CString BACKUPPATH_TOOLTIP = _T("备份数据库到指定地址");
const CString BACKUPDAY_TOOLTIP = _T("备份的天数间隔，每隔多少天进行一次备份");
const CString BACKUPHOUR_TOOLTIP = _T("备份的具体时间，表示备份当天的几点钟进行备份");

const UINT ACTIVE_ID = 1;
const UINT PASSWORD_ID = ACTIVE_ID + 1;
const UINT MYSQLPORT_ID = PASSWORD_ID + 1;
const UINT DISKMIN_ID = MYSQLPORT_ID+ 1;
const UINT SAVETIME_ID = DISKMIN_ID + 1;
const UINT SAVETYPE_ID = SAVETIME_ID + 1;
const UINT SAVECHANGERATE_ID = SAVETYPE_ID + 1;
const UINT TBROWMAX_ID = SAVECHANGERATE_ID + 1;
const UINT BACKUPACTIVE_ID = TBROWMAX_ID + 1;
const UINT BACKUPPATH_ID = BACKUPACTIVE_ID + 1;
const UINT BACKUPDAY_ID = BACKUPPATH_ID + 1;
const UINT BACKUPHOUR_ID = BACKUPDAY_ID + 1;

using namespace Servers;
using namespace DB;
CPropertyDB::CPropertyDB(void)
{
}

CPropertyDB::~CPropertyDB(void)
{
}

//!< 获得本窗体的名称
CString CPropertyDB::GetTitle()
{
	return ITEM_TITLE;
}

//!< 显示表格的函数
void CPropertyDB::ShowInfo(CXTPPropertyGrid& grid)
{
	Servers::DB::CDBMgr* dbMgr = &DB::CDBMgr::GetMe();
	CXTPPropertyGridItem* pGroup;					//!< 属性组
	CXTPPropertyGridInplaceButton* pButton;
	std::list<CString> strList;

	//!< 变量基本信息
	pGroup = grid.AddCategory(BASE_INFO);
	pGroup->SetTooltip(BASE_INFO);
	pGroup->Expand();

	//!< 数据库密码，这个取消，通过判断变量是否保存历史数据来设定这个值
// 	strList.clear();
// 	strList.push_back(_T("No"));
// 	strList.push_back(_T("Yes"));
// 	AddItemList(*pGroup, ACTIVE, ACTIVE_TOOLTIP, strList, dbMgr->IsActive() ? 1 : 0, ACTIVE_ID);

	//!< 数据库密码
	AddItemText(*pGroup, PASSWORD, PASSWORD_TOOLTIP, dbMgr->getPassWord(), PASSWORD_ID)->SetPasswordMask('*');

	//!<数据库连接端口

	AddItemNumber(*pGroup, MYSQLPORT, MYSQLPORT_TOOLTIP, dbMgr->getMySqlPort(), MYSQLPORT_ID);

	//!< 硬盘报警值
	AddItemNumber(*pGroup, DISKMIN, DISKMIN_TOOLTIP, dbMgr->getDiskMin(), DISKMIN_ID);

	//!< 存储周期
	AddItemDouble(*pGroup, SAVETIME, SAVETIME_TOOLTIP, dbMgr->getSaveTime(), SAVETIME_ID, FALSE, 2);

	//!< 分表条件
	AddItemNumber(*pGroup, TBROWMAX, TBROWMAX_TOOLITP, dbMgr->getTbRowMax(), TBROWMAX_ID);

	//!< 存储方式
	strList.clear();
	strList.push_back(_T("直接存储"));
	strList.push_back(_T("改变再存储"));
	AddItemList(*pGroup, SAVETYPE, SAVETYPE_TOOLTIP, strList, dbMgr->getSaveType(), SAVETYPE_ID);

	//!< 变化率
	AddItemNumber(*pGroup, SAVECHANGERATE, SAVECHANGERATE_TOOLITP, dbMgr->getChangeRate(), SAVECHANGERATE_ID);

	//!< 备份使能
	std::list<CString> boolList;			//!< 下拉列表的内容
	boolList.push_back(_T("No"));
	boolList.push_back(_T("Yes"));
	AddItemList(*pGroup, BACKUPACTIVE, BACKUPACTIVE_TOOLTIP, boolList, dbMgr->IsBackUp() ? 1 : 0, BACKUPACTIVE_ID);

	//!< 数据库备份路径
	CXTPPropertyGridItem* pItemText = AddItemText(*pGroup, BACKUPPATH, BACKUPPATH_TOOLTIP, dbMgr->getBackupPath(), BACKUPPATH_ID);
	pButton = pItemText->GetInplaceButtons()->AddButton(new CXTPPropertyGridInplaceButton(ID_BUTTON_PATH));
	pButton->SetCaption(_T("编辑"));

	//!< 备份天数间隔
	AddItemNumber(*pGroup, BACKUPDAY, BACKUPDAY_TOOLTIP, dbMgr->getBackupDay(), BACKUPDAY_ID);

	//!< 备份时间
	strList.clear();
	CString text;
	for(int i = 0; i < 24; ++i){
		text.Format("%d点",i);
		strList.push_back(text);
	}
	AddItemList(*pGroup, BACKUPHOUR, BACKUPHOUR_TOOLTIP, strList, dbMgr->getBackupHour(), BACKUPHOUR_ID);
}

//!< 表格初始化完成后
void CPropertyDB::OnGridFirstShow(CXTPPropertyGrid& grid)
{
	EnableAndDisenable(grid);
}

//!< 在这里统一使能
void CPropertyDB::EnableAndDisenable(CXTPPropertyGrid& grid)
{
	CXTPPropertyGridItem *item, *tmp;

	//!< 备份使能
	item = grid.FindItem(BACKUPACTIVE_ID);
	if(item->GetConstraints()->GetCurrent() == 0){
		tmp = grid.FindItem(BACKUPPATH_ID);		if(tmp)		tmp->SetReadOnly(TRUE);
		tmp = grid.FindItem(BACKUPDAY_ID);		if(tmp)		tmp->SetReadOnly(TRUE);
		tmp = grid.FindItem(BACKUPHOUR_ID);		if(tmp)		tmp->SetReadOnly(TRUE);
	}
	else{
		tmp = grid.FindItem(BACKUPPATH_ID);		if(tmp)		tmp->SetReadOnly(FALSE);
		tmp = grid.FindItem(BACKUPDAY_ID);		if(tmp)		tmp->SetReadOnly(FALSE);
		tmp = grid.FindItem(BACKUPHOUR_ID);		if(tmp)		tmp->SetReadOnly(FALSE);
	}

	//!< 存储方式
	item = grid.FindItem(SAVETYPE_ID);
	if(item->GetConstraints()->GetCurrent() != 1){			// 1是改变再存储，需要让用户配变化百分比
		tmp = grid.FindItem(SAVECHANGERATE_ID);	if(tmp)		tmp->SetHidden(TRUE);
	}
	else{
		tmp = grid.FindItem(SAVECHANGERATE_ID);	if (tmp)	tmp->SetHidden(FALSE);
	}
}

//!< 当关闭表格时
void CPropertyDB::OnCloseGrid()
{
}

//!< 当某一项被修改时
void CPropertyDB::OnItemModify(CXTPPropertyGrid& grid, UINT id)
{
	EnableAndDisenable(grid);
	CXTPPropertyGridItem *item;

	//!< 存储周期
	if(SAVETIME_ID == id)
	{
		item = grid.FindItem(SAVETIME_ID);
		float fValue = ((CXTPPropertyGridItemDouble *)item)->GetDouble();
		//int nValue = ((CXTPPropertyGridItemNumber *)item)->GetNumber();
		if(fValue <= 0.0099999)			fValue = 600.0;
		if(fValue >= 3600000.00001)		fValue = 3600000.0;
		((CXTPPropertyGridItemDouble *)item)->SetDouble(fValue);
	}

	//!< 变化率
	if(SAVECHANGERATE_ID == id)
	{
		item = grid.FindItem(SAVECHANGERATE_ID);
		int nValue = ((CXTPPropertyGridItemNumber *)item)->GetNumber();
		if(nValue < 0)			nValue = 0;
		if(nValue > 100)		nValue = 100;
		((CXTPPropertyGridItemNumber *)item)->SetNumber(nValue);
	}

	//!< 分表条件
	if (TBROWMAX_ID == id)
	{
		item = grid.FindItem(TBROWMAX_ID);
		UINT nValue = ((CXTPPropertyGridItemNumber *)item)->GetNumber();
		if (nValue < 100)		nValue = 100;
		((CXTPPropertyGridItemNumber *)item)->SetNumber(nValue);
	}
}

//!< 要保存被改变的内容
bool CPropertyDB::OnSaveModify(CXTPPropertyGrid& grid)
{
	int num = grid.GetCount();
	CXTPPropertyGridItem* item;
	CString itemValue;
	UINT itemID;
	CComVariant cvr;
	Servers::DB::CDBMgr* dbMgr = &DB::CDBMgr::GetMe();
	for(int i = 0; i < num; ++i)
	{
		item = grid.GetItem(i);
		if(!item->IsValueChanged())		continue;			//!< 只有被修改的才会进行保存，优化了一下
		itemID = item->GetID();
		itemValue = item->GetValue();
		cvr = itemValue.Trim();
		
// 		if(itemID == ACTIVE_ID){							// = _T("启用数据库");
// 			dbMgr->setActived(item->GetConstraints()->GetCurrent() == 1 ? true : false);
// 		}
		if(itemID == PASSWORD_ID){						// = _T("数据库密码");
			dbMgr->setPassWord(itemValue.Trim());
		}
		else if(itemID==MYSQLPORT_ID){					//=_T("数据库端口");
			cvr.ChangeType(VT_I4);
			dbMgr->setMySqlPort(cvr.intVal);
		}
		else if(itemID == DISKMIN_ID){						// = _T("硬盘最小下线");
			cvr.ChangeType(VT_I4);
			dbMgr->setDiskMin((UINT)cvr.intVal);
		}
		else if(itemID == SAVETIME_ID){						// = _T("存储周期，秒");
			cvr.ChangeType(VT_R4);
			dbMgr->setSaveTime(cvr.fltVal);			
		}
		else if(itemID == SAVETYPE_ID){						// = _T("存储方式");
			cvr.ChangeType(VT_I4);
			dbMgr->setSaveType(item->GetConstraints()->GetCurrent());
		}
		else if(itemID == SAVECHANGERATE_ID){				// = _T("变化率");
			cvr.ChangeType(VT_I4);
			dbMgr->setChangeRate((UINT)cvr.intVal);
		}
		else if (itemID == TBROWMAX_ID){					// = _T("分表条件");
			cvr.ChangeType(VT_UI4);
			dbMgr->setTbRowMax((UINT)cvr.intVal);
		}
		else if(itemID == BACKUPACTIVE_ID){					// = _T("备份使能");
			dbMgr->setBackUp(item->GetConstraints()->GetCurrent() == 1 ? true : false);
		}
		else if(itemID == BACKUPPATH_ID){					// = _T("备份地址");
			dbMgr->setBackupPath(itemValue.Trim());
		}
		else if(itemID == BACKUPDAY_ID){					// = _T("备份天数间隔");
			cvr.ChangeType(VT_I4);
			dbMgr->setBackupDay((UINT)cvr.intVal);
		}
		else if(itemID == BACKUPHOUR_ID){					// = _T("备份时间，点");
			dbMgr->setBackupHour((UINT)item->GetConstraints()->GetCurrent());
		}
	}
	return true;
}

//!< 按钮被按下
void CPropertyDB::OnButtonClick(CXTPPropertyGrid& grid, UINT btID)
{
	if(btID == ID_BUTTON_PATH)
	{
		CXTBrowseDialog dlg(&grid);
		dlg.SetTitle(_T("选择备份所在的目录"));
		CString defPath = CProjectMgr::GetMe().GetProj()->GetPath();
		if (!defPath.IsEmpty())
		{
			TCHAR path[_MAX_PATH];
			STRCPY_S(path, _MAX_PATH, defPath);
			dlg.SetSelPath(path);
		}
		if (dlg.DoModal() == IDOK)
		{
			defPath = dlg.GetSelPath();
			if(defPath.GetAt(defPath.GetLength() - 1) != '\\')
				defPath = defPath + _T("\\");
			CXTPPropertyGridItem* item = grid.FindItem(BACKUPPATH_ID);
			item->SetValue(defPath);
		}
	}
}

//!< 显示帮助信息
void CPropertyDB::OnShowHelp()
{
	SoftInfo::CMyHelp::GetMe().ShowHelp(_T("历史数据库"));
}
