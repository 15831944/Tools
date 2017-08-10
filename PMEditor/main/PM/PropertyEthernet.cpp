#include "StdAfx.h"
#include "DevMgr.h"
#include "DEthernet.h"
#include "PropertyEthernet.h"

using namespace MVC;
using namespace Device;
using namespace InterfaceSet;

const CString ETH_LOCALPORT = _T("本地通信端口");
const CString SERIAL_RETRY = _T("重试次数");
const CString SERIAL_OVER = _T("通信超时时间(ms)");
const CString SERIAL_COOL = _T("通信冷却时间(ms)");
const CString SERIAL_REVIVE = _T("通信恢复时间(ms)");
const CString SERIAL_SCAN = _T("设备扫描周期(ms)");

const CString ETH_LOCALPORT_TOOLTIP = _T("端口号 0~65535，0表示随机数");
const CString SERIAL_RETRY_TOOLTIP = _T("重试次数 0～5");
const CString SERIAL_OVER_TOOLTIP = _T("通信超时时间 >= 100, <= 60000");
const CString SERIAL_COOL_TOOLTIP = _T("通信冷却时间 >= 通信超时时间*2");
const CString SERIAL_REVIVE_TOOLTIP = _T("通信恢复时间 >= 1000");
const CString SERIAL_SCAN_TOOLTIP = _T("设备扫描周期 >= 0");

const UINT ETH_LOCALPORT_ID = 1;
const UINT SERIAL_RETRY_ID = ETH_LOCALPORT_ID + 1;
const UINT SERIAL_OVER_ID = SERIAL_RETRY_ID + 1;
const UINT SERIAL_COOL_ID = SERIAL_OVER_ID + 1;
const UINT SERIAL_REVIVE_ID = SERIAL_COOL_ID + 1;
const UINT SERIAL_SCAN_ID = SERIAL_REVIVE_ID + 1;

CPropertyEthernet::CPropertyEthernet(void)
{
	m_uiLocalPort = 0;			//!< 本地通讯端口，0为随机端口
}

CPropertyEthernet::~CPropertyEthernet(void)
{
}

//!< 获得本窗体的名称
CString CPropertyEthernet::GetTitle()
{
	return _T("以太网通信配置");
}

void CPropertyEthernet::DataReady()
{
	//!< 获得以太网对象
	MVC::Device::CDevMgr* devMgr = &MVC::Device::CDevMgr::GetMe();
	std::shared_ptr<MVC::Device::InterfaceSet::CDEthernet> ether = devMgr->GetEthernet();
	ASSERT(ether);
	m_uiLocalPort = ether->getLocalPort();
	m_uiRetryNum = ether->getRetryNum();
	m_uiOverTime = ether->getOverTime();
	m_uiCoolTime = ether->getCoolTime();
	m_uiReviveTime = ether->getReviveTime();
}

//!< 显示表格的函数
void CPropertyEthernet::ShowInfo(CXTPPropertyGrid& grid)
{
	DataReady();
	//!< 重置表格
	grid.ResetContent();

	//!< 创建属性组
	CXTPPropertyGridItem* pGroup;
	pGroup = grid.AddCategory(_T("参数列表"));
	pGroup->SetTooltip("参数列表");
	pGroup->Expand();

	//!< LocalPort
	AddItemNumber(*pGroup, ETH_LOCALPORT, ETH_LOCALPORT_TOOLTIP, m_uiLocalPort, ETH_LOCALPORT_ID);

	//!< 重试次数
	AddItemNumber(*pGroup, SERIAL_RETRY, SERIAL_RETRY_TOOLTIP, m_uiRetryNum, SERIAL_RETRY_ID);

	//!< 通信超时时间
	AddItemNumber(*pGroup, SERIAL_OVER, SERIAL_OVER_TOOLTIP, m_uiOverTime, SERIAL_OVER_ID);

	//!< 冷却时间
	AddItemNumber(*pGroup, SERIAL_COOL, SERIAL_COOL_TOOLTIP, m_uiCoolTime, SERIAL_COOL_ID);

	//!< 恢复时间
	AddItemNumber(*pGroup, SERIAL_REVIVE, SERIAL_REVIVE_TOOLTIP, m_uiReviveTime, SERIAL_REVIVE_ID);
}

//!< 当某项的值发生改变时
void CPropertyEthernet::OnItemModify(CXTPPropertyGrid& grid, UINT ID)
{
	//!< 获得被修改的项
	CXTPPropertyGridItem* item = grid.FindItem(ID);
	CXTPPropertyGridItemNumber* pItem = (CXTPPropertyGridItemNumber *)item;
	ASSERT(item);
	CString strValue = item->GetValue();
	long lValue;

	//!< 判断修改了哪项，然后直接进行修改
	if (ETH_LOCALPORT_ID == ID)
	{
		lValue = pItem->GetNumber();
		if (lValue < 0)
		{
			pItem->SetNumber(0);
			m_uiLocalPort = 0;
			return;
		}
		if (lValue > 65535)
		{
			pItem->SetNumber(65535);
			m_uiLocalPort = 65535;
			return;
		}
		m_uiLocalPort = pItem->GetNumber();
	}
	else if(SERIAL_RETRY_ID == ID)
	{
		lValue = pItem->GetNumber();
		if(lValue < 0)
		{
			pItem->SetNumber(0);
			m_uiRetryNum = 0;
			return;
		}
		if(lValue > 5)
		{
			pItem->SetNumber(5);
			m_uiRetryNum = 5;
			return;
		}
		m_uiRetryNum = pItem->GetNumber();
	}
	else if(SERIAL_OVER_ID == ID)
	{
		lValue = pItem->GetNumber();
		if(lValue < 100)
		{
			pItem->SetNumber(100);
			m_uiOverTime = 100;
			return;
		}
		if(lValue > 60000)
		{
			pItem->SetNumber(60000);
			lValue = 60000;
		}
		m_uiOverTime = lValue;
		if(m_uiCoolTime < long(m_uiOverTime*2))
		{
			CXTPPropertyGridItemNumber* pItemCool = (CXTPPropertyGridItemNumber*)grid.FindItem(SERIAL_COOL_ID);
			pItemCool->SetNumber(m_uiOverTime * 2);
			m_uiCoolTime = m_uiOverTime * 2;
		}
	}
	else if(SERIAL_COOL_ID == ID)
	{
		lValue = pItem->GetNumber();
		if(lValue < long(m_uiOverTime*2))
		{
			pItem->SetNumber(m_uiOverTime * 2);
			m_uiCoolTime = m_uiOverTime * 2;
			return;
		}
		m_uiCoolTime = lValue;
	}
	else if(SERIAL_REVIVE_ID == ID)
	{
		lValue = pItem->GetNumber();
		if(lValue < 1000)
		{
			pItem->SetNumber(1000);
			m_uiReviveTime = 1000;
			return;
		}
		m_uiReviveTime = lValue;
	}
	else if(SERIAL_SCAN_ID == ID)
	{
		lValue = pItem->GetNumber();
		if(lValue < 0)
		{
			pItem->SetNumber(0);
			return;
		}
	}
}

bool CPropertyEthernet::OnSaveModify(CXTPPropertyGrid &grid)
{
	//!< 获得当前要显示的以太网
	MVC::Device::CDevMgr* devMgr = &MVC::Device::CDevMgr::GetMe();
 	std::shared_ptr<MVC::Device::InterfaceSet::CDEthernet> ether = devMgr->GetEthernet();
	ASSERT(ether);
	ether->setLocalPort(m_uiLocalPort);
	ether->setRetryNum(m_uiRetryNum);
	ether->setOverTime(m_uiOverTime);
	ether->setCoolTime(m_uiCoolTime);
	ether->setReviveTime(m_uiReviveTime);

	return true;
}

//!< 显示帮助信息
void CPropertyEthernet::OnShowHelp()
{
	SoftInfo::CMyHelp::GetMe().ShowHelp(_T("工程师站接口配置"));
}