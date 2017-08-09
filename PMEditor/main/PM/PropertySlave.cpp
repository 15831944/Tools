#include "StdAfx.h"
#include "DXPEditor.h"
#include "MyHelp.h"
#include "Gbl.h"
#include "ProjectMgr.h"
#include "DevMgr.h"
#include "DeviceInterface.h"
#include "DeviceOne.h"
#include "DSerial.h"
#include "XmlMgr.h"
#include "XmlDevice.h"
#include "XmlProtocol.h"
#include "XmlInterface.h"
#include "PropertySlave.h"

const CString DEV_BASE_INFO = _T("�豸������Ϣ");
const CString DEV_TYPE = _T("�豸������");
const CString DEV_NAME = _T("�豸����");
const CString DEV_TAG = _T("�豸��ǩ");

const CString DEV_COMM_INFO = _T("ͨ�Ż�����Ϣ");
const CString DEV_COMM_TYPE = _T("�ӿ�����");
const CString DEV_ADDR = _T("���豸Handle��");

//////////////////////////////////////////////////////////////////////////
const CString DEV_BASE_INFO_TOOLTIP = _T("�豸������Ϣ");
const CString DEV_TYPE_TOOLTIP = _T("�豸������");
const CString DEV_NAME_TOOLTIP = _T("�豸����");
const CString DEV_TAG_TOOLTIP = _T("�豸��ǩ");

const CString DEV_COMM_INFO_TOOLTIP = _T("ͨ�Ż�����Ϣ");
const CString DEV_COMM_TYPE_TOOLTIP = _T("�ӿ�����");
const CString DEV_ADDR_TOOLTIP = _T("ͬһ�����豸�����д��豸��Handle�����ظ�����ֵ��ͨ�����豸��Ϊ�е�����\"���豸����\"�õ���");

//////////////////////////////////////////////////////////////////////////
const UINT DEV_BASE_INFO_ID = 1;
const UINT DEV_TYPE_ID = DEV_BASE_INFO_ID + 1;
const UINT DEV_NAME_ID = DEV_TYPE_ID + 1;
const UINT DEV_TAG_ID = DEV_NAME_ID + 1;

const UINT DEV_COMM_INFO_ID = 11;
const UINT DEV_COMM_TYPE_ID = DEV_COMM_INFO_ID + 1;
const UINT DEV_ADDR_ID = DEV_COMM_TYPE_ID + 1;

using namespace MVC;
using namespace Device;
CPropertySlave::CPropertySlave(void)
{
	m_uiType = 0;
	m_uiID = UINT(-1);
	m_ShowDev = boost::shared_ptr<CDeviceOne>(new CDeviceOne());
	m_ShowDev->m_vtInterface.resize(1);
	if(m_ShowDev->m_vtInterface[0])
		m_ShowDev->m_vtInterface[0]->SetType(16);
}

CPropertySlave::~CPropertySlave(void)
{
}

//!< ��ñ����������
CString CPropertySlave::GetTitle()
{
	return _T("�豸������Ϣ");
}

//!< �����������ͣ�type=0������id��ʾ�豸������ID�ţ�type=1��ʾ�޸ģ�id��ʾ�豸ID��
void CPropertySlave::SetType(UINT type, UINT id, UINT parentID)
{
	m_uiType = type;
	m_uiID = id;
	m_uiParentID = parentID;
	m_ShowDev->setDevType(id);

	CDevMgr* devMgr = &CDevMgr::GetMe();

	if(m_uiType == 0){				//!< ������½�
		SetOtherDevName();
		ASSERT(devMgr->GetDevice(parentID));
		m_ShowDev->setID(UINT(-1));
	}
	else if(m_uiType == 1)			//!< ������޸�
	{
		boost::shared_ptr<CDeviceOne> device = devMgr->GetDevice(id);
		ASSERT(device);
		m_ShowDev->CopyFrom(*device);
		m_ShowDev->setID(device->getID());
	}
	m_ShowDev->ResizeInterface(1);	//!< ���豸��1���ӿڣ��������Ǵ���
	m_ShowDev->m_vtInterface[0]->SetType(16);
}

//!< ��m_ShowDev����һ���µ�Ĭ������
void CPropertySlave::SetOtherDevName()
{
	CString name = m_ShowDev->getName();
	UINT id = 0;
	if(name == _T(""))		name =_T("�½����豸");
	name = CDevMgr::GetMe().GetSimilarName(name);
	m_ShowDev->setName(name);
}

//!< ��ʾ���ĺ���
void CPropertySlave::ShowInfo(CXTPPropertyGrid& grid)
{
	CXTPPropertyGridItem* pGroup;					//!< ������
	CXTPPropertyGridInplaceButton* pButton;
	CDevMgr* devMgr = &CDevMgr::GetMe();
	boost::shared_ptr<CDeviceOne> parent = devMgr->GetDevice(m_uiParentID);
	if(!parent)		return;
	boost::shared_ptr<XmlInfo::CXmlDevice> xmlDev = m_ShowDev->GetXmlInfo();
	boost::shared_ptr<XmlInfo::CXmlDevice> xmlParent = parent->GetXmlInfo();
	std::list<CString> strList;						//!< �����б������
	CString tooltip;								//!< ��Щ�����ʾ��Ϣ��Ҫ��������ʹ��
	int def = -1;

	ASSERT(!m_ShowDev->m_vtInterface.empty());
	//!< ����������Ϣ
	pGroup = grid.AddCategory(DEV_BASE_INFO);
	pGroup->SetTooltip(DEV_BASE_INFO_TOOLTIP);
	pGroup->Expand();

	//!< �豸������
	AddItemText(*pGroup, DEV_TYPE, DEV_TYPE_TOOLTIP, m_ShowDev->GetTypeName(), DEV_TYPE_ID)->SetReadOnly(TRUE);

	//!< �豸����
	AddItemText(*pGroup, DEV_NAME, DEV_NAME_TOOLTIP, m_ShowDev->getName(), DEV_NAME_ID);

	//!< �豸��ǩ
	AddItemText(*pGroup, DEV_TAG, DEV_TAG_TOOLTIP, m_ShowDev->getTag(), DEV_TAG_ID);

	//!< ͨ�Ż�����Ϣ
	pGroup = grid.AddCategory(DEV_COMM_INFO);
	pGroup->SetTooltip(DEV_COMM_INFO_TOOLTIP);
	pGroup->Expand();

	//!< ���豸��ַ
	strList.clear();
	strList.push_back(_T("����"));
//	strList.push_back(_T("ZigBee"));
	AddItemList(*pGroup, DEV_COMM_TYPE, DEV_COMM_TYPE_TOOLTIP, strList, 0, DEV_COMM_TYPE_ID);

	//!< ���豸��ַ
	strList.clear();
	CString text;
	int slaveCount = xmlParent?xmlParent->getMaxInf2Slave():16;		//!< Ĭ��16��
	for(int i = 0; i < slaveCount; ++i)
	{
		text.Format("%d", i);
		strList.push_back(text);
	}
	AddItemList(*pGroup, DEV_ADDR, DEV_ADDR_TOOLTIP, strList, m_ShowDev->m_vtInterface[0]->GetHandle(), DEV_ADDR_ID);
}

//!< ����ʼ����ɺ�
void CPropertySlave::OnGridFirstShow(CXTPPropertyGrid& grid)
{
	ShowAndHide(grid);
}

//!< ��ʾ�����������
void CPropertySlave::ShowAndHide(CXTPPropertyGrid& grid)
{
}

//!< ���رձ��ʱ
void CPropertySlave::OnCloseGrid()
{
}

//!< Ҫ�����޸ĵ�����
bool CPropertySlave::OnSaveModify(CXTPPropertyGrid& grid)
{
	if(CProjectMgr::GetMe().IsWatch())	return true;
	CXTPPropertyGridItem *pItem, *tmp;
	UINT count = grid.GetCount();
	CString strValue;
	CDevMgr* devMgr = &CDevMgr::GetMe();
	boost::shared_ptr<XmlInfo::CXmlDevice> xmlDev = m_ShowDev->GetXmlInfo();

	for(int i = 0; i < count; ++i)
	{
		pItem = grid.GetItem(i);
		if(!pItem->IsValueChanged())	continue;
		strValue = pItem->GetValue();
		UINT itemID = pItem->GetID();

		if(DEV_NAME_ID == itemID)							//!< �豸����
		{
			if(pItem->IsValueChanged() && devMgr->GetDevice(strValue))
			{
				AfxMessageBox(_T("����Ϊ \"") + strValue + _T("\" ���豸�Ѵ��ڣ�����������..."), MB_OK | MB_ICONEXCLAMATION);
				return false;
			}
			if(strValue.Trim() == _T(""))
			{
				AfxMessageBox(_T("���Ʋ���Ϊ��..."), MB_OK | MB_ICONEXCLAMATION);
				return false;
			}
			m_ShowDev->setName(strValue.Trim());
		}
		else if(DEV_TAG_ID == itemID)						//!< �豸��ǩ
		{
			m_ShowDev->setTag(strValue.Trim());
		}
		else if(DEV_ADDR_ID == itemID)						//!< ���豸��ַ
		{
			m_ShowDev->m_vtInterface[0]->SetHandle(pItem->GetConstraints()->GetCurrent());
		}
	}
	//!< �ӿڲ����ظ�
	boost::shared_ptr<CDeviceOne> device;
	boost::shared_ptr<CDeviceInterface> projInf, myInf;
	foreach(device, devMgr->m_vtDevice)
	{
		if(!device)											continue;
		if(device->getID() == m_ShowDev->getID())			continue;
		if(device->getParentID() != m_uiParentID)			continue;
		foreach(projInf, device->m_vtInterface)
		{
			foreach(myInf, m_ShowDev->m_vtInterface)
			{
				if(projInf->GetName() != myInf->GetName())	continue;
				CString str = _T("�豸 \"") + device->getName() + _T(" \"�Ѵ��ڽӿ� \"") + projInf->GetName() + _T(" \"�� �����¶���ýӿ�");
				MessageBox(NULL, str, _T("����"), MB_OK | MB_ICONEXCLAMATION);
				return false;
			}
		}
	}
	return true;
}

//!< ��ĳ���ֵ�����ı�ʱ
void CPropertySlave::OnItemModify(CXTPPropertyGrid& grid, UINT ID)
{
	if(CProjectMgr::GetMe().IsWatch())						return;
	ShowAndHide(grid);
}

//!< ��ʾ������Ϣ
void CPropertySlave::OnShowHelp()
{
	SoftInfo::CMyHelp::GetMe().ShowHelp(_T("���豸"));
}
