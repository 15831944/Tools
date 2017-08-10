#include "StdAfx.h"
#include "DevMgr.h"
#include "DEthernet.h"
#include "PropertyEthernet.h"

using namespace MVC;
using namespace Device;
using namespace InterfaceSet;

const CString ETH_LOCALPORT = _T("����ͨ�Ŷ˿�");
const CString SERIAL_RETRY = _T("���Դ���");
const CString SERIAL_OVER = _T("ͨ�ų�ʱʱ��(ms)");
const CString SERIAL_COOL = _T("ͨ����ȴʱ��(ms)");
const CString SERIAL_REVIVE = _T("ͨ�Żָ�ʱ��(ms)");
const CString SERIAL_SCAN = _T("�豸ɨ������(ms)");

const CString ETH_LOCALPORT_TOOLTIP = _T("�˿ں� 0~65535��0��ʾ�����");
const CString SERIAL_RETRY_TOOLTIP = _T("���Դ��� 0��5");
const CString SERIAL_OVER_TOOLTIP = _T("ͨ�ų�ʱʱ�� >= 100, <= 60000");
const CString SERIAL_COOL_TOOLTIP = _T("ͨ����ȴʱ�� >= ͨ�ų�ʱʱ��*2");
const CString SERIAL_REVIVE_TOOLTIP = _T("ͨ�Żָ�ʱ�� >= 1000");
const CString SERIAL_SCAN_TOOLTIP = _T("�豸ɨ������ >= 0");

const UINT ETH_LOCALPORT_ID = 1;
const UINT SERIAL_RETRY_ID = ETH_LOCALPORT_ID + 1;
const UINT SERIAL_OVER_ID = SERIAL_RETRY_ID + 1;
const UINT SERIAL_COOL_ID = SERIAL_OVER_ID + 1;
const UINT SERIAL_REVIVE_ID = SERIAL_COOL_ID + 1;
const UINT SERIAL_SCAN_ID = SERIAL_REVIVE_ID + 1;

CPropertyEthernet::CPropertyEthernet(void)
{
	m_uiLocalPort = 0;			//!< ����ͨѶ�˿ڣ�0Ϊ����˿�
}

CPropertyEthernet::~CPropertyEthernet(void)
{
}

//!< ��ñ����������
CString CPropertyEthernet::GetTitle()
{
	return _T("��̫��ͨ������");
}

void CPropertyEthernet::DataReady()
{
	//!< �����̫������
	MVC::Device::CDevMgr* devMgr = &MVC::Device::CDevMgr::GetMe();
	std::shared_ptr<MVC::Device::InterfaceSet::CDEthernet> ether = devMgr->GetEthernet();
	ASSERT(ether);
	m_uiLocalPort = ether->getLocalPort();
	m_uiRetryNum = ether->getRetryNum();
	m_uiOverTime = ether->getOverTime();
	m_uiCoolTime = ether->getCoolTime();
	m_uiReviveTime = ether->getReviveTime();
}

//!< ��ʾ���ĺ���
void CPropertyEthernet::ShowInfo(CXTPPropertyGrid& grid)
{
	DataReady();
	//!< ���ñ��
	grid.ResetContent();

	//!< ����������
	CXTPPropertyGridItem* pGroup;
	pGroup = grid.AddCategory(_T("�����б�"));
	pGroup->SetTooltip("�����б�");
	pGroup->Expand();

	//!< LocalPort
	AddItemNumber(*pGroup, ETH_LOCALPORT, ETH_LOCALPORT_TOOLTIP, m_uiLocalPort, ETH_LOCALPORT_ID);

	//!< ���Դ���
	AddItemNumber(*pGroup, SERIAL_RETRY, SERIAL_RETRY_TOOLTIP, m_uiRetryNum, SERIAL_RETRY_ID);

	//!< ͨ�ų�ʱʱ��
	AddItemNumber(*pGroup, SERIAL_OVER, SERIAL_OVER_TOOLTIP, m_uiOverTime, SERIAL_OVER_ID);

	//!< ��ȴʱ��
	AddItemNumber(*pGroup, SERIAL_COOL, SERIAL_COOL_TOOLTIP, m_uiCoolTime, SERIAL_COOL_ID);

	//!< �ָ�ʱ��
	AddItemNumber(*pGroup, SERIAL_REVIVE, SERIAL_REVIVE_TOOLTIP, m_uiReviveTime, SERIAL_REVIVE_ID);
}

//!< ��ĳ���ֵ�����ı�ʱ
void CPropertyEthernet::OnItemModify(CXTPPropertyGrid& grid, UINT ID)
{
	//!< ��ñ��޸ĵ���
	CXTPPropertyGridItem* item = grid.FindItem(ID);
	CXTPPropertyGridItemNumber* pItem = (CXTPPropertyGridItemNumber *)item;
	ASSERT(item);
	CString strValue = item->GetValue();
	long lValue;

	//!< �ж��޸������Ȼ��ֱ�ӽ����޸�
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
	//!< ��õ�ǰҪ��ʾ����̫��
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

//!< ��ʾ������Ϣ
void CPropertyEthernet::OnShowHelp()
{
	SoftInfo::CMyHelp::GetMe().ShowHelp(_T("����ʦվ�ӿ�����"));
}