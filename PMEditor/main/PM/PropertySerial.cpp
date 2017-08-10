#include "StdAfx.h"
#include "Gbl.h"
#include "DevMgr.h"
#include "DSerial.h"
#include "SerialSetDlg.h"
#include "PropertySerial.h"

using namespace MVC;
using namespace Device;
using namespace InterfaceSet;

const CString SERIAL_NAME = _T("��������");
const CString SERIAL_BAUD = _T("������");
const CString SERIAL_CHECK = _T("У��");
const CString SERIAL_DATA = _T("����λ");
const CString SERIAL_STOP = _T("ֹͣλ");
const CString SERIAL_RETRY = _T("���Դ���");
const CString SERIAL_OVER = _T("ͨ�ų�ʱʱ��(ms)");
const CString SERIAL_COOL = _T("ͨ����ȴʱ��(ms)");
const CString SERIAL_REVIVE = _T("ͨ�Żָ�ʱ��(ms)");
const CString SERIAL_SCAN = _T("�豸ɨ������(ms)");

const CString SERIAL_NAME_TOOLTIP = _T("��������");
const CString SERIAL_BAUD_TOOLTIP = _T("������");
const CString SERIAL_CHECK_TOOLTIP = _T("У��");
const CString SERIAL_DATA_TOOLTIP = _T("����λ");
const CString SERIAL_STOP_TOOLTIP = _T("ֹͣλ");
const CString SERIAL_RETRY_TOOLTIP = _T("���Դ��� 0~5");
const CString SERIAL_OVER_TOOLTIP = _T("ͨ�ų�ʱʱ�� >= 500, <= 60000");
const CString SERIAL_COOL_TOOLTIP = _T("ͨ����ȴʱ�� >= ͨ�ų�ʱʱ��*2");
const CString SERIAL_REVIVE_TOOLTIP = _T("ͨ�Żָ�ʱ�� >= 1000");
const CString SERIAL_SCAN_TOOLTIP = _T("�豸ɨ������ > 0");

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

//!< ���ô���
void CPropertySerial::SetInfo(CString serialName)
{
	m_strSerialName = serialName;
}

//!< ��ñ����������
CString CPropertySerial::GetTitle()
{
	return _T("��������");
}

//!< ��ʾ���ĺ���
void CPropertySerial::ShowInfo(CXTPPropertyGrid& grid)
{
	ASSERT(m_pParent);
	std::list<CString> strList;
	UINT defIndex = 0;

	//!< ���ñ��
	grid.ResetContent();

	//!< ��õ�ǰҪ��ʾ�Ĵ���
	CGbl* gbl = &CGbl::GetMe();
	MVC::Device::CDevMgr* devMgr = &MVC::Device::CDevMgr::GetMe();
	std::shared_ptr<MVC::Device::InterfaceSet::CDSerial> serial = m_pParent->GetSerial(m_strSerialName);
	ASSERT(serial);

	//!< ����������
	CXTPPropertyGridItem* pGroup;
	pGroup = grid.AddCategory(_T("�����б�"));
	pGroup->SetTooltip("�����б�");
	pGroup->Expand();

	//!< ��������
	AddItemText(*pGroup, SERIAL_NAME, SERIAL_NAME_TOOLTIP, serial->getName(), SERIAL_NAME_ID)->SetReadOnly(TRUE);

	//!< ������
	strList.clear();
	strList = gbl->getBaudList();
	AddItemList(*pGroup, SERIAL_BAUD, SERIAL_BAUD_TOOLTIP, strList, serial->getBaud(), SERIAL_BAUD_ID);

	//!< У��λ
	strList.clear();
	strList = gbl->getCheckStyleList();
	AddItemList(*pGroup, SERIAL_CHECK, SERIAL_CHECK_TOOLTIP, strList, serial->getCheckStyle(), SERIAL_CHECK_ID);

	//!< ����λ
	strList.clear();
	strList = gbl->getDataBitNumList();
	AddItemList(*pGroup, SERIAL_DATA, SERIAL_DATA_TOOLTIP, strList, serial->getDataBitNum(), SERIAL_DATA_ID);

	//!< ֹͣλ
	strList.clear();
	strList = gbl->getStopBitNumList();
	AddItemList(*pGroup, SERIAL_STOP, SERIAL_STOP_TOOLTIP, strList, serial->getStopBitNum(), SERIAL_STOP_ID);

	//!< ���Դ���
	AddItemNumber(*pGroup, SERIAL_RETRY, SERIAL_RETRY_TOOLTIP, serial->getRetryNum(), SERIAL_RETRY_ID);

	//!< ͨ�ų�ʱʱ��
	AddItemNumber(*pGroup, SERIAL_OVER, SERIAL_OVER_TOOLTIP, serial->getOverTime(), SERIAL_OVER_ID);

	//!< ��ȴʱ��
	AddItemNumber(*pGroup, SERIAL_COOL, SERIAL_COOL_TOOLTIP, serial->getCoolTime(), SERIAL_COOL_ID);

	//!< �ָ�ʱ��
	AddItemNumber(*pGroup, SERIAL_REVIVE, SERIAL_REVIVE_TOOLTIP, serial->getReviveTime(), SERIAL_REVIVE_ID);

	//!< ɨ������
	AddItemNumber(*pGroup, SERIAL_SCAN, SERIAL_SCAN_TOOLTIP, serial->getScanTime(), SERIAL_SCAN_ID);
}

//!< ��ĳ���ֵ�����ı�ʱ
void CPropertySerial::OnItemModify(CXTPPropertyGrid& grid, UINT ID)
{
	//!< ��õ�ǰҪ��ʾ�Ĵ���
	MVC::Device::CDevMgr* devMgr = &MVC::Device::CDevMgr::GetMe();
	std::shared_ptr<MVC::Device::InterfaceSet::CDSerial> serial = m_pParent->GetSerial(m_strSerialName);
	if(!serial)		return;
	ASSERT(serial);

	//!< ��ñ��޸ĵ���
	CXTPPropertyGridItem* item = grid.FindItem(ID);
	CXTPPropertyGridItemNumber* pItem;
	ASSERT(item);
	CString strValue = item->GetValue();
	long lValue;

	//!< �ж��޸������Ȼ��ֱ�ӽ����޸�
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

//!< ��ʾ������Ϣ
void CPropertySerial::OnShowHelp()
{
	SoftInfo::CMyHelp::GetMe().ShowHelp(_T("����ʦվ�ӿ�����"));
}