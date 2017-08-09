#include "StdAfx.h"
#include "Gbl.h"
#include "ProjectMgr.h"
#include "DBMgr.h"
#include "PropertyDlg.h"
#include "ItemMgr.h"
#include "Item.h"

const CString ROOTNAME = _T("DBSet");
const CString ACTIVED = _T("Active");
const CString PASSWORD = _T("PassWord");
const CString DISKMIN = _T("DiskMin");
const CString SAVETIME = _T("SaveTime");
const CString SAVETYPE = _T("SaveType");
//const CString SAVEAVERAGE = _T("SaveAverage");
const CString SAVECHANGERATE = _T("ChangeRate");
const CString TBROWMAX = _T("TbRowMax");
const CString BACKUPACTIVE = _T("BackupActive");
const CString BACKUPPATH = _T("BackupPath");
const CString BACKUPDAY = _T("BackupDay");
const CString BACKUPHOUR = _T("BackupHour");
const CString EDIT_TIME = _T("EditTime");
const CString MYSQLPORT=_T("Port");
using namespace Servers;
using namespace DB;

CDBMgr::CDBMgr(void)
//:m_bActive(false)
:m_strPassWord(_T(""))
,m_uiDiskMin(300)
,m_fSaveTime(600.00)
,m_uiSaveType(0)			//!< 存储方式，0直接存储，1平均值存储
//,m_uiSaveAverage(5)			//!< 平均值次数
, m_uiChangeRate(5)
, m_uiTbRowMax(100000)			//!< 分表上限，默认100000，最小值100
,m_strBackupPath(_T(""))
,m_uiBackupDay(7)
,m_uiBackupHour(23)
,m_nMySqlPort(3306)
{
}

CDBMgr::~CDBMgr(void)
{
}

CDBMgr& CDBMgr::GetMe()
{
	static CDBMgr one;
	return one;
}

CString CDBMgr::GetEditTime()
{
	if(IsModify())		return CGbl::GetMe().GetCulTime(true);
	return CGbl::GetMe().GetSystemTime2Str(m_EditTime);
}

void CDBMgr::SetEditTime(CString stime)
{
	if(stime == _T(""))		::GetLocalTime(&m_EditTime);
	CGbl::GetMe().SetSystemTimeFromStr(m_EditTime, stime);
}

void CDBMgr::OnCreate()
{
	m_bModify = true;
}

void CDBMgr::OnSetDB()
{
	if(CProjectMgr::GetMe().SayWatch())		return;
	Dialog::CPropertyDlg* propertyDlg = &Dialog::CPropertyDlg::GetMe();
	if(IDOK == propertyDlg->DoModal(m_DbProperty)){
	}
}

//!< 保存xml格式的编辑信息
bool CDBMgr::SerializeXml(TiXmlElement* pNode, bool bRead)
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
//			if(ACTIVED == name)				{cvr.ChangeType(VT_I4);		m_bActive = cvr.intVal == 1 ? true : false;}
			if(PASSWORD == name)			{m_strPassWord = strValue;}
			else if(DISKMIN == name)		{cvr.ChangeType(VT_UI4);	m_uiDiskMin = cvr.uintVal;}
			else if(SAVETIME == name)		{cvr.ChangeType(VT_R4);		m_fSaveTime = cvr.fltVal;}
			else if(SAVETYPE == name)		{cvr.ChangeType(VT_UI4);	m_uiSaveType = cvr.uintVal;}
			//else if(SAVEAVERAGE == name)	{cvr.ChangeType(VT_UI4);	m_uiSaveAverage = cvr.uintVal;}
			else if (SAVECHANGERATE == name){ cvr.ChangeType(VT_UI4);	m_uiChangeRate = cvr.uintVal; }
			else if (TBROWMAX == name)		{ cvr.ChangeType(VT_UI4);	m_uiTbRowMax = cvr.uintVal; }
			else if(BACKUPACTIVE == name)	{cvr.ChangeType(VT_I4);		m_bBackUp = cvr.intVal == 1 ? true : false;}
			else if(BACKUPPATH == name)		{m_strBackupPath = strValue;}
			else if(BACKUPDAY == name)		{cvr.ChangeType(VT_UI4);	m_uiBackupDay = cvr.uintVal;}
			else if(BACKUPHOUR == name)		{cvr.ChangeType(VT_UI4);	m_uiBackupHour = cvr.uintVal;}
			else if(EDIT_TIME == name)		{SetEditTime(strValue);}
			else if(MYSQLPORT == name )	{ cvr.ChangeType(VT_UI4); m_nMySqlPort=cvr.uintVal;}
			pAttr = pAttr->Next();
		}
	}
	else
	{
		CString str;
		pNode->SetAttribute(ACTIVED, IsActive() ? 1 : 0);
		pNode->SetAttribute(PASSWORD, m_strPassWord);
		pNode->SetAttribute(DISKMIN, m_uiDiskMin);
		str.Format("%.2f", m_fSaveTime);
		pNode->SetAttribute(SAVETIME, str);
		pNode->SetAttribute(SAVETYPE, m_uiSaveType);
		//pNode->SetAttribute(SAVEAVERAGE, m_uiSaveAverage);
		pNode->SetAttribute(SAVECHANGERATE, m_uiChangeRate);
		pNode->SetAttribute(TBROWMAX, m_uiTbRowMax);
		pNode->SetAttribute(BACKUPACTIVE, m_bBackUp ? 1 : 0);
		pNode->SetAttribute(BACKUPPATH, m_strBackupPath);
		pNode->SetAttribute(BACKUPDAY, m_uiBackupDay);
		pNode->SetAttribute(BACKUPHOUR, m_uiBackupHour);
		pNode->SetAttribute(EDIT_TIME, GetEditTime());
		pNode->SetAttribute(MYSQLPORT,m_nMySqlPort);
	}
	m_bModify = false;				//!< 加载或保存完工程后处于未修改状态
	return true;
}

void CDBMgr::OnClose()
{
//	m_bActive = true;				//!< 是否启用
	m_uiDiskMin = 300;				//!< 磁盘最小报警空间
	m_fSaveTime = 600.0;			//!< 存储时间间隔
	m_uiSaveType = 0;				//!< 存储方式，0直接存储，1改变再存储，2DCCE_SD同步
	//m_uiSaveAverage = 5;			//!< 平均值次数
	m_uiChangeRate = 5;				//!< 变化率，乘100了
	m_uiTbRowMax = 100000;			//!< 分表上限，默认100000，最小值100
	m_bBackUp = false;				//!< 备份是否启用
	m_strBackupPath = _T("");		//!< 备份路径
	m_uiBackupDay = 7;				//!< 备份时间间隔，天
	m_uiBackupHour = 23;			//!< 备份时间，点
}

// 是否需要激活数据库
bool CDBMgr::IsActive()
{
	MVC::Item::CItemMgr* itemMgr = &MVC::Item::CItemMgr::GetMe();
	boost::shared_ptr<MVC::Item::CItem> item;
	foreach(item, itemMgr->m_vtItem)
		if(item && item->getReservDB())
			return true;			// 只要有1个变量保留历史数据,那么就启动数据库
	return false;					// 所有变量都不保留历史数据,那么就不启动数据库
}
bool CDBMgr::IsNeed(UINT dwData)
{
	if (dwData == DBM_CONFIG)  return true;
	else return false;
}
void CDBMgr::OnReceive(COPYDATASTRUCT* pCopyDataStruct)
{
	InitStateXml(*pCopyDataStruct);
}
void  CDBMgr::InitStateXml(COPYDATASTRUCT& pCopyDataStruct)
{
		CString strAll=(char *)pCopyDataStruct.lpData;
		if (strAll.IsEmpty()) {ASSERT(FALSE); return;}
		std::vector<CString> vtStr,vtLR;
		CGbl::SpliteBy(strAll,_T("|"),vtStr);
		CString str,strL,strR;
		foreach(str,vtStr)
		{
			vtLR.clear();
			CGbl::SpliteBy(str,_T("?"),vtLR);
			if (vtLR.size()!=2) continue ;
			strL=vtLR[0].Trim();
			strR=vtLR[1].Trim();
			if(strL==_T("PassWord")){setPassWord(strR);}
			else if(strL==_T("DiskMin")){setDiskMin(atoi(strR));}
			else if(strL==_T("SaveTime")){setSaveTime(atof(strR));}
			else if(strL==_T("BackupActive"))
			{ 
				if (strR==_T("0"))  
					setBackUp(false); 
				else if (strR==_T("1")) 
					setBackUp(true); 
			}
			else if(strL==_T("ShowTag")){continue;}
			else if(strL==_T("BackupPath")){setBackupPath(strR);}
			else if(strL==_T("BackupDay")){setBackupDay(atoi(strR));}
			else if(strL==_T("BackupHour")){setBackupHour(atoi(strR));}
			else if(strL==_T("SaveType")){setSaveType(atoi(strR));}
			else if(strL==_T("ChangeRate")){setChangeRate(atoi(strR));}
			else if (strL == _T("TbRowMax")){ setTbRowMax(atoi(strR)); }
			else if(strL==_T("Port")){setMySqlPort(atoi(strR));}
		}
		
}

//void CDBMgr::setActived(bool actived)		{if(actived != m_bActive)		SetModify();	m_bActive = actived;}
void CDBMgr::setPassWord(CString passWord)	{if(passWord != m_strPassWord)	SetModify();	m_strPassWord = passWord;}
void CDBMgr::setDiskMin(UINT deskMin)		{if(deskMin != m_uiDiskMin)		SetModify();	m_uiDiskMin = deskMin;}
void CDBMgr::setSaveTime(float time)		{if(time != m_fSaveTime)		SetModify();	m_fSaveTime = time;}
void CDBMgr::setSaveType(UINT type)			{if(type != m_uiSaveType)		SetModify();	m_uiSaveType = type;}
//void CDBMgr::setSaveAverage(UINT ave)		{if(ave != m_uiSaveAverage)		SetModify();	m_uiSaveAverage = ave;}
void CDBMgr::setChangeRate(UINT rate)		{if(rate != m_uiChangeRate)		SetModify();	m_uiChangeRate = rate; }
void CDBMgr::setTbRowMax(UINT size)			{ if (size != m_uiTbRowMax)		SetModify();	m_uiTbRowMax = size; }
void CDBMgr::setBackupPath(CString path)	{if(path != m_strBackupPath)	SetModify();	m_strBackupPath = path;}
void CDBMgr::setBackupDay(UINT day)			{if(day != m_uiBackupDay)		SetModify();	m_uiBackupDay = day;}
void CDBMgr::setBackupHour(UINT hour)		{if(hour != m_uiBackupHour)		SetModify();	m_uiBackupHour = hour % 24;}
void CDBMgr::setBackUp(bool backUp/*=true*/){if(backUp != m_bBackUp)		SetModify();	m_bBackUp = backUp;}
void CDBMgr::setMySqlPort(UINT port)		{if(port!=m_nMySqlPort)			SetModify();	m_nMySqlPort = port;} 
