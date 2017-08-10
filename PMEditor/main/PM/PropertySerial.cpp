#include "StdAfx.h"
#include "Gbl.h"
#include "DevMgr.h"
#include "DSerial.h"
#include "SerialSetDlg.h"
#include "PropertySerial.h"

using namespace MVC;
using namespace Device;
using namespace InterfaceSet;

const CString SERIAL_NAME = _T("串口名称");
const CString SERIAL_BAUD = _T("波特率");
const CString SERIAL_CHECK = _T("校验");
const CString SERIAL_DATA = _T("数据位");
const CString SERIAL_STOP = _T("停止位");
const CString SERIAL_RETRY = _T("重试次数");
const CString SERIAL_OVER = _T("通信超时时间(ms)");
const CString SERIAL_COOL = _T("通信冷却时间(ms)");
const CString SERIAL_REVIVE = _T("通信恢复时间(ms)");
const CString SERIAL_SCAN = _T("设备扫描周期(ms)");

const CString SERIAL_NAME_TOOLTIP = _T("串口名称");
const CString SERIAL_BAUD_TOOLTIP = _T("波特率");
const CString SERIAL_CHECK_TOOLTIP = _T("校验");
const CString SERIAL_DATA_TOOLTIP = _T("数据位");
const CString SERIAL_STOP_TOOLTIP = _T("停止位");
const CString SERIAL_RETRY_TOOLTIP = _T("重试次数 0~5");
const CString SERIAL_OVER_TOOLTIP = _T("通信超时时间 >= 500, <= 60000");
const CString SERIAL_COOL_TOOLTIP = _T("通信冷却时间 >= 通信超时时间*2");
const CString SERIAL_REVIVE_TOOLTIP = _T("通信恢复时间 >= 1000");
const CString SERIAL_SCAN_TOOLTIP = _T("设备扫描周期 > 0");

const UINT SERIAL_NAME_ID = 1;
const UINT SERIAL_BAUD_ID = SERIAL_NAME_ID + 1;
const UINT SERIAL_CHECK_ID = SERIAL_BAUD_ID + 1;
const UINT SERIAL_DATA_ID = SERIAL_CHECK_ID + 1;
const UINT SERIAL_STOP_ID = SERIAL_DATA_ID + 1;
const UINT SERIAL_RETRY_ID = SERIAL_STOP_ID + 1;
const UINT SERIAL_OVER_ID = SERIAL_RETRY_ID + 1;
const UINT SERIAL_COOL_ID = SERIAL_OVER_ID + 1;
const UINT SERIAL_REVIVE_ID = SERIAL_COOL_ID + 1;
const UINT SERIAL_SCAN_ID = SERIAL_REVIVE_ID + 1;

CPropertySerial::CPropertySerial()
{
	m_pParent = NULL;
}

CPropertySerial::~CPropertySerial(void)
{
}

//!< 设置串口
void CPropertySerial::SetInfo(CString serialName)
{
	m_strSerialName = serialName;
}

//!< 获得本窗体的名称
CString CPropertySerial::GetTitle()
{
	return _T("串口配置");
}

//!< 显示表格的函数
void CPropertySerial::ShowInfo(CXTPPropertyGrid& grid)
{
	ASSERT(m_pParent);
	std::list<CString> strList;
	UINT defIndex = 0;

	//!< 重置表格
	grid.ResetContent();

	//!< 获得当前要显示的串口
	CGbl* gbl = &CGbl::GetMe();
	MVC::Device::CDevMgr* devMgr = &MVC::Device::CDevMgr::GetMe();
	std::shared_ptr<MVC::Device::InterfaceSet::CDSerial> serial = m_pParent->GetSerial(m_strSerialName);
	ASSERT(serial);

	//!< 创建属性组
	CXTPPropertyGridItem* pGroup;
	pGroup = grid.AddCategory(_T("参数列表"));
	pGroup->SetTooltip("参数列表");
	pGroup->Expand();

	//!< 串口名称
	AddItemText(*pGroup, SERIAL_NAME, SERIAL_NAME_TOOLTIP, serial->getName(), SERIAL_NAME_ID)->SetReadOnly(TRUE);

	//!< 波特率
	strList.clear();
	strList = gbl->getBaudList();
	AddItemList(*pGroup, SERIAL_BAUD, SERIAL_BAUD_TOOLTIP, strList, serial->getBaud(), SERIAL_BAUD_ID);

	//!< 校验位
	strList.clear();
	strList = gbl->getCheckStyleList();
	AddItemList(*pGroup, SERIAL_CHECK, SERIAL_CHECK_TOOLTIP, strList, serial->getCheckStyle(), SERIAL_CHECK_ID);

	//!< 数据位
	strList.clear();
	strList = gbl->getDataBitNumList();
	AddItemList(*pGroup, SERIAL_DATA, SERIAL_DATA_TOOLTIP, strList, serial->getDataBitNum(), SERIAL_DATA_ID);

	//!< 停止位
	strList.clear();
	strList = gbl->getStopBitNumList();
	AddItemList(*pGroup, SERIAL_STOP, SERIAL_STOP_TOOLTIP, strList, serial->getStopBitNum(), SERIAL_STOP_ID);

	//!< 重试次数
	AddItemNumber(*pGroup, SERIAL_RETRY, SERIAL_RETRY_TOOLTIP, serial->getRetryNum(), SERIAL_RETRY_ID);

	//!< 通信超时时间
	AddItemNumber(*pGroup, SERIAL_OVER, SERIAL_OVER_TOOLTIP, serial->getOverTime(), SERIAL_OVER_ID);

	//!< 冷却时间
	AddItemNumber(*pGroup, SERIAL_COOL, SERIAL_COOL_TOOLTIP, serial->getCoolTime(), SERIAL_COOL_ID);

	//!< 恢复时间
	AddItemNumber(*pGroup, SERIAL_REVIVE, SERIAL_REVIVE_TOOLTIP, serial->getReviveTime(), SERIAL_REVIVE_ID);

	//!< 扫描周期
	AddItemNumber(*pGroup, SERIAL_SCAN, SERIAL_SCAN_TOOLTIP, serial->getScanTime(), SERIAL_SCAN_ID);
}

//!< 当某项的值发生改变时
void CPropertySerial::OnItemModify(CXTPPropertyGrid& grid, UINT ID)
{
	//!< 获得当前要显示的串口
	MVC::Device::CDevMgr* devMgr = &MVC::Device::CDevMgr::GetMe();
	std::shared_ptr<MVC::Device::InterfaceSet::CDSerial> serial = m_pParent->GetSerial(m_strSerialName);
	if(!serial)		return;
	ASSERT(serial);

	//!< 获得被修改的项
	CXTPPropertyGridItem* item = grid.FindItem(ID);
	CXTPPropertyGridItemNumber* pItem;
	ASSERT(item);
	CString strValue = item->GetValue();
	long lValue;

	//!< 判断修改了哪项，然后直接进行修改
	if(SERIAL_BAUD_ID == ID)			serial->setBaud((UINT)item->GetConstraints()->GetCurrent());
	else if(SERIAL_CHECK_ID == ID)		serial->setCheckStyle((UINT)item->GetConstraints()->GetCurrent());
	else if(SERIAL_DATA_ID == ID)		serial->setDataBitNum((UINT)item->GetConstraints()->GetCurrent());
	else if(SERIAL_STOP_ID == ID)		serial->setStopBitNum((UINT)item->GetConstraints()->GetCurrent());
	else if(SERIAL_RETRY_ID == ID)
	{
		pItem = (CXTPPropertyGridItemNumber*)item;
		lValue = pItem->GetNumber();
		if(lValue < 0)
		{
			pItem->SetNumber(0);
			serial->setRetryNum(0);
			return;
		}
		if(lValue > 5)
		{
			pItem->SetNumber(5);
			serial->setRetryNum(5);
			return;
		}
		serial->setRetryNum(lValue);
	}
	else if(SERIAL_OVER_ID == ID)
	{
		pItem = (CXTPPropertyGridItemNumber*)item;
		lValue = pItem->GetNumber();
		if(lValue < 500)
		{
			pItem->SetNumber(500);
			serial->setOverTime(500);
			return;
		}
		if(lValue > 60000)
		{
			pItem->SetNumber(60000);
			lValue = 60000;
		}
		serial->setOverTime(lValue);
		if(serial->getCoolTime() < long(lValue*2))
		{
			CXTPPropertyGridItemNumber* pItemCool = (CXTPPropertyGridItemNumber*)grid.FindItem(SERIAL_COOL_ID);
			pItemCool->SetNumber(serial->getOverTime() * 2);
			serial->setCoolTime(serial->getOverTime() * 2);
		}
	}
	else if(SERIAL_COOL_ID == ID)
	{
		pItem = (CXTPPropertyGridItemNumber*)item;
		lValue = pItem->GetNumber();
		if(lValue < (long)(serial->getOverTime()*2))
		{
			pItem->SetNumber(serial->getOverTime() * 2);
			serial->setCoolTime(serial->getOverTime() * 2);
			return;
		}
		serial->setCoolTime(lValue);
	}
	else if(SERIAL_REVIVE_ID == ID)
	{
		pItem = (CXTPPropertyGridItemNumber*)item;
		lValue = pItem->GetNumber();
		if(lValue < 1000)
		{
			pItem->SetNumber(1000);
			serial->setReviveTime(1000);
			return;
		}
		serial->setReviveTime(lValue);
	}
	else if(SERIAL_SCAN_ID == ID)
	{
		pItem = (CXTPPropertyGridItemNumber*)item;
		lValue = pItem->GetNumber();
		if(lValue < 0)
		{
			pItem->SetNumber(0);
			serial->setScanTime(0);
			return;
		}
		serial->setScanTime(lValue);
	}
}

//!< 显示帮助信息
void CPropertySerial::OnShowHelp()
{
	SoftInfo::CMyHelp::GetMe().ShowHelp(_T("工程师站接口配置"));
}