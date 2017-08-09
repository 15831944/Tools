#include "StdAfx.h"
#include <WinDNS.h>
#include "DXPEditor.h"
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
#include "PropertyDevice.h"
#include <boost/regex.hpp>

const CString DEV_NET_SUPPORT1 = _T("��̫����ַ");
const CString DEV_NET_SUPPORT2 = _T("����");
const CString DEV_NET_SUPPORT3 = _T("Զ��ģ���ַ");

const CString DEV_BASE_INFO = _T("�豸������Ϣ");
const CString DEV_TYPE = _T("�豸������");
const CString DEV_NAME = _T("�豸����");
const CString DEV_TAG = _T("�豸��ǩ");

const CString DEV_COMM_INFO = _T("ͨ�Ż�����Ϣ");
const CString DEV_INTERFACE_NUM = _T("ͨ�Žӿ�����");
const CString DEV_INTERFACE_SORT = _T("���ýӿ��Ż�����");

const CString DEV_INTERFACE_INFO = _T("ͨ�Žӿ�����");
const CString DEV_NET_TYPE = _T("�ӿ�����");
const CString DEV_SERIAL_NO = _T("������");
const CString DEV_ADDR = _T("ģ��ID");
const CString DEV_IP = _T("��̫����ַ");
const CString DEV_PORT=_T("��̫��ͨ�Ŷ˿�"); 
//////////////////////////////////////////////////////////////////////////
const CString DEV_BASE_INFO_TOOLTIP = _T("�豸������Ϣ");
const CString DEV_TYPE_TOOLTIP = _T("�豸������");
const CString DEV_NAME_TOOLTIP = _T("�豸����");
const CString DEV_TAG_TOOLTIP = _T("�豸��ǩ");

const CString DEV_COMM_INFO_TOOLTIP = _T("ͨ�Ż�����Ϣ");
const CString DEV_INTERFACE_NUM_TOOLTIP = _T("ͨ�Žӿ�����");
const CString DEV_INTERFACE_SORT_TOOLTIP = _T("�����ӿ��Ż��������������ͨ��ʱ������ѡ�����������ŵ�ͨ�Žӿ����豸ͨ��");

const CString DEV_INTERFACE_INFO_TOOLTIP = _T("ͨ�Žӿ�����");
const CString DEV_NET_TYPE_TOOLTIP = _T("�ӿ�����");
const CString DEV_SERIAL_NO_TOOLTIP = _T("��������ѡ�񱾵ؼ�����Ĵ��ں�");
const CString DEV_ADDR_TOOLTIP = _T("ģ��ID");
const CString DEV_IP_TOOLTIP = _T("��̫����ַ");
const CString DEV_PORT_TOOLTIP=_T("��̫��ͨ�Ŷ˿�");
//////////////////////////////////////////////////////////////////////////
const UINT DEV_BASE_INFO_ID = 1;
const UINT DEV_TYPE_ID = DEV_BASE_INFO_ID + 1;
const UINT DEV_NAME_ID = DEV_TYPE_ID + 1;
const UINT DEV_TAG_ID = DEV_NAME_ID + 1;

const UINT DEV_COMM_INFO_ID = 11;
const UINT DEV_INTERFACE_NUM_ID = DEV_COMM_INFO_ID + 1;
const UINT DEV_INTERFACE_SORT_ID = DEV_INTERFACE_NUM_ID + 1;

const UINT DEV_INTERFACE_INFO_ID = 101;
const UINT DEV_NET_TYPE_ID = DEV_INTERFACE_INFO_ID + 1;
const UINT DEV_SERIAL_NO_ID = DEV_NET_TYPE_ID + 1;
const UINT DEV_ADDR_ID = DEV_SERIAL_NO_ID + 1;
const UINT DEV_IP_ID = DEV_ADDR_ID + 1;
const UINT DEV_PORT_ID=DEV_IP_ID +1;
using namespace MVC;
using namespace Device;
CPropertyDevice::CPropertyDevice(void)
{
	m_uiType = 0;
	m_uiID = UINT(-1);
	m_ShowDev = boost::shared_ptr<CDeviceOne>(new CDeviceOne());
}

CPropertyDevice::~CPropertyDevice(void)
{
}

//!< ��ñ����������
CString CPropertyDevice::GetTitle()
{
	return _T("�豸������Ϣ");
}

//!< �����������ͣ�type=0������id��ʾ�豸������ID�ţ�type=1��ʾ�޸ģ�id��ʾ�豸ID��
bool CPropertyDevice::SetType(UINT type, UINT id)
{
	m_uiType = type;
	m_uiID = id;

	if(m_uiType == 0){
		if(!m_ShowDev->setDevType(id))
			return false;
		ReSetInteface();
		SetOtherDevName();
	}
	else if(m_uiType == 1)
	{
		CDevMgr* devMgr = &CDevMgr::GetMe();
		boost::shared_ptr<CDeviceOne> device = devMgr->GetDevice(id);
		ASSERT(device);
		m_ShowDev->CopyFrom(*device);
	}
	return true;
}

//!< ���������豸�Ľӿ�
void CPropertyDevice::ReSetInteface()
{
	boost::shared_ptr<XmlInfo::CXmlDevice> xmlDevice = m_ShowDev->GetXmlInfo();
	bool bEthernet = xmlDevice->IsSupportEthernet();
	bool bSerial = xmlDevice->IsSupportSerial();
	if (bEthernet)
	{
		int inf2Main = xmlDevice->getInf2Main();
		boost::shared_ptr<CDeviceInterface> inf;
		if (!(inf2Main & 0x01) && (inf2Main & 0x20)){
			foreach(inf, m_ShowDev->m_vtInterface)
				inf->SetType(0x20);
		}
		else if (inf2Main = 0x01){
			foreach(inf, m_ShowDev->m_vtInterface)
				inf->SetType(0);
		}
	}
	else if (bSerial && !bEthernet)		//!< Ŀǰֻ�����������
	{
		boost::shared_ptr<CDeviceInterface> inf;
		foreach(inf, m_ShowDev->m_vtInterface){
			if(inf->GetType() == 0)		inf->SetType(1);
		}
	}
	else
		ASSERT(FALSE);
}

//!< ��m_ShowDev����һ���µ�Ĭ������
void CPropertyDevice::SetOtherDevName()
{
	CString name = m_ShowDev->getName();
	UINT id = 0;
	if(name == _T(""))		name =_T("�½����豸");
	name = CDevMgr::GetMe().GetSimilarName(name);
	m_ShowDev->setName(name);
}

//!< ��ʾ���ĺ���
void CPropertyDevice::ShowInfo(CXTPPropertyGrid& grid)
{
	CXTPPropertyGridItem* pGroup;					//!< ������
	CXTPPropertyGridInplaceButton* pButton;
	CDevMgr* devMgr = &CDevMgr::GetMe();
	boost::shared_ptr<XmlInfo::CXmlDevice> xmlDev = m_ShowDev->GetXmlInfo();
	ASSERT(xmlDev);
	std::list<CString> strList, boolList;			//!< �����б������
	boolList.push_back(_T("No"));
	boolList.push_back(_T("Yes"));
	CString tooltip;								//!< ��Щ�����ʾ��Ϣ��Ҫ��������ʹ��
	int def = -1;

	//!< ����������Ϣ
	pGroup = grid.AddCategory(DEV_BASE_INFO);
	pGroup->SetTooltip(DEV_BASE_INFO_TOOLTIP);
	pGroup->Expand();

	//!< �豸������
	AddItemText(*pGroup, DEV_TYPE, DEV_TYPE_TOOLTIP, m_ShowDev->GetTypeName(), DEV_TYPE_ID)->SetReadOnly(TRUE);

	//!< �豸����
	AddItemText(*pGroup, DEV_NAME, DEV_NAME_TOOLTIP, m_ShowDev->getName(), DEV_NAME_ID);

	//!< �豸���
	AddItemText(*pGroup, DEV_TAG, DEV_NAME_TOOLTIP, m_ShowDev->getTag(), DEV_TAG_ID);

	//!< ͨ�Ż�����Ϣ
	pGroup = grid.AddCategory(DEV_COMM_INFO);
	pGroup->SetTooltip(DEV_COMM_INFO_TOOLTIP);
	pGroup->Expand();

	//!< ����ӿ�����
	strList.clear();
	CString strInfCount;
	for(int i = 0; i < m_ShowDev->GetInterfaceSize(); ++i){
		strInfCount.Format("%d", i + 1);
		strList.push_back(strInfCount);
	}
	AddItemList(*pGroup, DEV_INTERFACE_NUM, DEV_INTERFACE_NUM_TOOLTIP, strList, (UINT)m_ShowDev->m_vtInterface.size() - 1, DEV_INTERFACE_NUM_ID);

	//!< �����ӿ�����
	//AddItemList(*pGroup, DEV_INTERFACE_SORT, DEV_INTERFACE_SORT_TOOLTIP, boolList, m_ShowDev->getInterfaceSort()?1:0, DEV_INTERFACE_SORT_ID);

	//!< ͨ�Žӿ�����
	boost::shared_ptr<CDeviceInterface> inf;
	CString name,netaddr;
	UINT index = 0;
	UINT infType = 0, serialNo = 0, addr = 1, ip = UINT(-1),devPort=11000;
	std::vector< boost::shared_ptr<CDeviceInterface> >::iterator iter = m_ShowDev->m_vtInterface.begin();
	for(int index = 0; index < m_ShowDev->GetInterfaceSize(); index++)
	{
		//!< ��ʼ��Ĭ��ֵ
		if(iter == m_ShowDev->m_vtInterface.end())
		{
			infType = 0;serialNo = 0;addr = 1;ip = UINT(-1);
		}
		else
		{
			infType = (*iter)->GetType();
			serialNo = (*iter)->GetSerialNo();
			netaddr=(*iter)->GetDoMain();
			addr = (*iter)->GetHandle();
			devPort=(*iter)->GetDevPort();
			++iter;
		}

		name.Format("%d", index + 1);
		pGroup = grid.AddCategory(DEV_INTERFACE_INFO + name);
		pGroup->SetTooltip(DEV_INTERFACE_INFO_TOOLTIP);
		pGroup->SetID(DEV_INTERFACE_INFO_ID * (index + 1));
		pGroup->Expand();

		//!< �ӿ�����
		strList.clear();
		UINT inf2Main = xmlDev->getInf2Main();
		if(inf2Main & 0x01)			strList.push_back(DEV_NET_SUPPORT1);
		if(inf2Main & 0x02)			strList.push_back(DEV_NET_SUPPORT2);
		if(inf2Main & 0x20)			strList.push_back(DEV_NET_SUPPORT3);
		if(strList.size() == 0)		AddItemText(*pGroup, DEV_NET_TYPE, DEV_NET_TYPE_TOOLTIP, _T("��֧���κ���֪ͨ�Žӿڣ��޷�����ͨ��"), DEV_NET_TYPE_ID * (index + 1));
		else{
			def = -1;
			CString strDef;
			if(infType == 0)		strDef = DEV_NET_SUPPORT1;
			else if(infType == 1)	strDef = DEV_NET_SUPPORT2;
			else if(infType == 0x20)strDef = DEV_NET_SUPPORT3;
			std::list<CString>::iterator iter;
			int i;
			for(iter = strList.begin(), i = 0; iter != strList.end(); ++iter, ++i){
				if(*iter == strDef){def = i; break;}
			}
			AddItemList(*pGroup, DEV_NET_TYPE, DEV_NET_TYPE_TOOLTIP, strList, def, DEV_NET_TYPE_ID * (index + 1));
		}

		//!< ���ں�
		strList.clear();
		int def = 0;
		boost::shared_ptr<InterfaceSet::CDSerial> serial;
		foreach(serial, devMgr->m_ltSerial)
		{
			if(serialNo == serial->getNumber())
				def = (int)strList.size();
			strList.push_back(serial->getName());
		}
		AddItemList(*pGroup, DEV_SERIAL_NO, DEV_SERIAL_NO_TOOLTIP, strList, def, DEV_SERIAL_NO_ID * (index + 1));

		//!< ���ڵ�ַ
		AddItemNumber(*pGroup, DEV_ADDR, DEV_ADDR_TOOLTIP, addr, DEV_ADDR_ID * (index + 1));
       if (netaddr=="")						//!<��ʼ���豸IP��ַ
			netaddr="192.168.0.1";
		//!< IP��ַ
		AddItemText(*pGroup,DEV_IP,DEV_IP_TOOLTIP,netaddr,DEV_IP_ID*(index+1));

		AddItemNumber(*pGroup,DEV_PORT,DEV_PORT_TOOLTIP,devPort,DEV_PORT_ID*(index+1));
		//AddItemIP(*pGroup, DEV_IP, DEV_IP_TOOLTIP, ip, DEV_IP_ID * (index + 1));
	}
}

//!< ����ʼ����ɺ�
void CPropertyDevice::OnGridFirstShow(CXTPPropertyGrid& grid)
{
	ShowAndHide(grid);
}

//!< ��ʾ�����������
void CPropertyDevice::ShowAndHide(CXTPPropertyGrid& grid)
{
	CXTPPropertyGridItem *item, *tmp;
	CString strValue;

	item = grid.FindItem(DEV_NET_TYPE_ID);
	if(!item)	return;
	strValue = item->GetValue();
	if(DEV_NET_SUPPORT1 == strValue)				//!< ��̫��IP
	{
		tmp = grid.FindItem(DEV_IP_ID);					if(tmp)		tmp->SetHidden(FALSE);
		tmp = grid.FindItem(DEV_SERIAL_NO_ID);			if(tmp)		tmp->SetHidden(TRUE);
		tmp = grid.FindItem(DEV_ADDR_ID);				if(tmp)		tmp->SetHidden(TRUE);
		tmp = grid.FindItem(DEV_PORT_ID);					if(tmp)		tmp->SetHidden(FALSE);
	}
	else if(DEV_NET_SUPPORT2 == strValue)			//!< ����
	{
		tmp = grid.FindItem(DEV_IP_ID);					if(tmp)		tmp->SetHidden(TRUE);
		tmp = grid.FindItem(DEV_SERIAL_NO_ID);			if(tmp)		tmp->SetHidden(FALSE);
		tmp = grid.FindItem(DEV_ADDR_ID);				if(tmp)		tmp->SetHidden(FALSE);
		tmp = grid.FindItem(DEV_PORT_ID);					if(tmp)		tmp->SetHidden(TRUE);
	}
	if(DEV_NET_SUPPORT3 == strValue)				//!< ��̫��,Զ���豸ģ���ַ
	{
		tmp = grid.FindItem(DEV_IP_ID);					if(tmp)		tmp->SetHidden(TRUE);
		tmp = grid.FindItem(DEV_SERIAL_NO_ID);			if(tmp)		tmp->SetHidden(TRUE);
		tmp = grid.FindItem(DEV_ADDR_ID);				if(tmp)		tmp->SetHidden(FALSE);
		tmp = grid.FindItem(DEV_PORT_ID);					if(tmp)		tmp->SetHidden(TRUE);
	}
	else
	{
		tmp = grid.FindItem(DEV_IP_ID);					if(tmp)		tmp->SetHidden(TRUE);
		tmp = grid.FindItem(DEV_SERIAL_NO_ID);			if(tmp)		tmp->SetHidden(TRUE);
		tmp = grid.FindItem(DEV_ADDR_ID);				if(tmp)		tmp->SetHidden(TRUE);
		tmp = grid.FindItem(DEV_PORT_ID);					if(tmp)		tmp->SetHidden(TRUE);
	}

	item = grid.FindItem(DEV_INTERFACE_NUM_ID);
	if(!item)	return;
	UINT count = item->GetConstraints()->GetCurrent();
	CXTPPropertyGridItem *tmp2;
	for(int index = 0; index < m_ShowDev->GetInterfaceSize(); ++index)
	{
		tmp = grid.FindItem(DEV_INTERFACE_INFO_ID * (index + 1));	if(!tmp)	continue;
		if(index <= count)
		{
			tmp->SetHidden(FALSE);
			tmp2 = grid.FindItem(DEV_NET_TYPE_ID * (index + 1));
			if(!tmp2)			continue;
			strValue = tmp2->GetValue();
			if(tmp2->GetValue() == DEV_NET_SUPPORT1)
			{
				tmp2 = grid.FindItem(DEV_SERIAL_NO_ID * (index + 1));	if(tmp2)	tmp2->SetHidden(TRUE);
				tmp2 = grid.FindItem(DEV_ADDR_ID * (index + 1));		if(tmp2)	tmp2->SetHidden(TRUE);
				tmp2 = grid.FindItem(DEV_IP_ID * (index + 1));			if(tmp2)	tmp2->SetHidden(FALSE);
				tmp2 = grid.FindItem(DEV_PORT_ID *(index +1));			if(tmp2)	tmp2->SetHidden(FALSE);
			}
			else if(tmp2->GetValue() == DEV_NET_SUPPORT2)
			{
				tmp2 = grid.FindItem(DEV_SERIAL_NO_ID * (index + 1));	if(tmp2)	tmp2->SetHidden(FALSE);
				tmp2 = grid.FindItem(DEV_ADDR_ID * (index + 1));		if(tmp2)	tmp2->SetHidden(FALSE);
				tmp2 = grid.FindItem(DEV_IP_ID * (index + 1));			if(tmp2)	tmp2->SetHidden(TRUE);
				tmp2 = grid.FindItem(DEV_PORT_ID *(index +1));			if(tmp2)	tmp2->SetHidden(TRUE);
			}
			else if(tmp2->GetValue() == DEV_NET_SUPPORT3)
			{
				tmp2 = grid.FindItem(DEV_SERIAL_NO_ID * (index + 1));	if(tmp2)	tmp2->SetHidden(TRUE);
				tmp2 = grid.FindItem(DEV_ADDR_ID * (index + 1));		if(tmp2)	tmp2->SetHidden(FALSE);
				tmp2 = grid.FindItem(DEV_IP_ID * (index + 1));			if(tmp2)	tmp2->SetHidden(TRUE);
				tmp2 = grid.FindItem(DEV_PORT_ID *(index +1));			if(tmp2)	tmp2->SetHidden(TRUE);
			}
		}
		else					tmp->SetHidden(TRUE);
	}
}

//!< ���رձ��ʱ
void CPropertyDevice::OnCloseGrid()
{
}

//!< Ҫ�����޸ĵ�����
bool CPropertyDevice::OnSaveModify(CXTPPropertyGrid& grid)
{
	if(CProjectMgr::GetMe().IsWatch())				return true;
	CXTPPropertyGridItem *pItem, *tmp;
	UINT count = grid.GetCount();
	CString strValue;
	CDevMgr* devMgr = &CDevMgr::GetMe();
	boost::shared_ptr<XmlInfo::CXmlDevice> xmlDev = m_ShowDev->GetXmlInfo();
	//boost::regex reg_domain("[a-zA-Z0-9][-a-zA-Z0-9]{0,62}(\\.[a-zA-Z0-9][-a-zA-Z0-9]{0,62})+\\.?");
	//boost::regex reg_ip("((25[0-5])|(2[0-4]\\d)|(1\\d\\d)|([1-9]\\d)|\\d)(\\.((25[0-5])|(2[0-4]\\d)|(1\\d\\d)|([1-9]\\d)|\\d)){3}");
	boost::regex	 reg_temp("[a-zA-Z]+\\.?");
	for(int i = 0; i < count; ++i)
	{
		pItem = grid.GetItem(i);
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
			continue;
		}
		else if(DEV_TAG_ID == itemID)						//!< �豸��ǩ
		{
			m_ShowDev->setTag(strValue.Trim());
			continue;
		}
		else if(DEV_INTERFACE_SORT_ID == itemID)			//!< �ӿ��Ƿ�����
		{
			m_ShowDev->setInterfaceSort(pItem->GetConstraints()->GetCurrent());
			continue;
		}
		else if(DEV_INTERFACE_NUM_ID == itemID)				//!< �ӿ�����
		{
			int num = pItem->GetConstraints()->GetCurrent() + 1;
			if(num >= 1 && num <= m_ShowDev->GetInterfaceSize())	m_ShowDev->ResizeInterface(num);
			continue;
		}
		else
		{
			//!< �ж����еĽӿ�
			for(int i = 0; i < m_ShowDev->GetInterfaceSize(); ++i)
			{
				if(DEV_NET_TYPE_ID * (i + 1) == itemID)		//!< ����ͨ������
				{
					if(DEV_NET_SUPPORT1 == strValue)		{m_ShowDev->m_vtInterface[i]->SetType(0x00);}// m_ShowDev->m_vtInterface[i]->SetIPorDns(true);}
					else if(DEV_NET_SUPPORT2 == strValue)	{m_ShowDev->m_vtInterface[i]->SetType(0x01);}
					else if(DEV_NET_SUPPORT3 == strValue)	{m_ShowDev->m_vtInterface[i]->SetType(0x20);}
					break;
				}
				else if(DEV_SERIAL_NO_ID * (i + 1) == itemID)//!< �����ں�
				{
					boost::shared_ptr<InterfaceSet::CDSerial> serial = devMgr->GetSerial(strValue);
					if(serial)									m_ShowDev->m_vtInterface[i]->SetSerialNo(serial->getNumber());
					break;
				}
				else if(DEV_ADDR_ID * (i + 1) == itemID)	//!< ������ģ���ַ
				{
					m_ShowDev->m_vtInterface[i]->SetHandle(((CXTPPropertyGridItemNumber *)pItem)->GetNumber());
					break;
				}
				else if(DEV_IP_ID * (i + 1) == itemID)		//!< ��IP��ַ
				{
				//	char* t_value;
				//	t_value=strValue.GetBuffer();
				//	bool b_ip=regex_match(t_value,reg_ip);
				//	bool	b_domain=regex_match(t_value,reg_domain);
					if ( CGbl::RegexIP(strValue))
					{
						m_ShowDev->m_vtInterface[i]->SetDoMain(strValue.Trim());
						break;
					}
					else if(CGbl::RegexDomain(strValue)) 
						{
							int i_temp=strValue.ReverseFind('.');
							CString str_temp=strValue.Mid(i_temp+1);
							if (CGbl::Regexmatch("[a-zA-Z]+\\.?",str_temp))
							{
								m_ShowDev->m_vtInterface[i]->SetDoMain(strValue.Trim());
								break;
							}
							else
							{
								m_ShowDev->m_vtInterface[i]->SetDoMain("");
								AfxMessageBox(_T("�����ַ���Ϸ�,���鲢��������"), MB_OK | MB_ICONEXCLAMATION);
								return false;
							}
						}
				//	else  if (regex_match(t_value,reg_domain))
				//   {
               //       m_ShowDev->m_vtInterface[i]->SetDoMain(strValue.Trim());
				//	  break;
			//	   }
					else
						{
							m_ShowDev->m_vtInterface[i]->SetDoMain("");
							AfxMessageBox(_T("�����ַ���Ϸ�,���鲢��������"), MB_OK | MB_ICONEXCLAMATION);
							return false;
						}
					
				}
				else if(DEV_PORT_ID *(i+1) ==itemID)
				{
					m_ShowDev->m_vtInterface[i]->SetDevPort(((CXTPPropertyGridItemNumber *)pItem)->GetNumber());
				}
			}
		}
	}
	//!< �ӿڲ����ظ�
	boost::shared_ptr<CDeviceOne> device;
	boost::shared_ptr<CDeviceInterface> projInf, myInf;
	foreach(device, devMgr->m_vtDevice)
	{
		if(!device)									continue;
		if(device->getID() == m_ShowDev->getID())	continue;
		foreach(projInf, device->m_vtInterface)
		{
			foreach(myInf, m_ShowDev->m_vtInterface)
			{
				if (projInf->GetName() != myInf->GetName())		continue;
				if (projInf->GetDevPort()!=myInf->GetDevPort())	continue;
				CString str = _T("�豸 \"") +device->getName() + _T("\"�Ѵ��ڽӿ�\"") + projInf->GetName() + _T("\"�� �����¶���ýӿ�");
				MessageBox(NULL, str, _T("����"), MB_OK | MB_ICONEXCLAMATION);
				return false;
			}
		}
	}
	return true;
}

//!< ��ĳ���ֵ�����ı�ʱ
void CPropertyDevice::OnItemModify(CXTPPropertyGrid& grid, UINT ID)
{
	if(CProjectMgr::GetMe().IsWatch())				return;
	for(int index = 0; index < m_ShowDev->GetInterfaceSize(); ++index)
	{
		if(ID != DEV_ADDR_ID * (index + 1))		continue;		//!< �ҵ��ǲ���ģ���ַ������
		CXTPPropertyGridItemNumber* item = (CXTPPropertyGridItemNumber *)grid.FindItem(ID);
		ASSERT(item);
		UINT addr = item->GetNumber();
		for(index = 0; index < m_ShowDev->GetInterfaceSize(); ++index)
		{
			CXTPPropertyGridItemNumber* item = (CXTPPropertyGridItemNumber *)grid.FindItem(DEV_ADDR_ID * (index + 1));
			ASSERT(item);
			item->SetNumber(addr);
		}
		break;
	}
	ShowAndHide(grid);
}

//!< ��ʾ������Ϣ
void CPropertyDevice::OnShowHelp()
{
	SoftInfo::CMyHelp::GetMe().ShowHelp(_T("���豸"));
}
