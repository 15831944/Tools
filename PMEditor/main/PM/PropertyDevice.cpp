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

const CString DEV_NET_SUPPORT1 = _T("以太网地址");
const CString DEV_NET_SUPPORT2 = _T("串口");
const CString DEV_NET_SUPPORT3 = _T("远程模块地址");

const CString DEV_BASE_INFO = _T("设备基本信息");
const CString DEV_TYPE = _T("设备类型名");
const CString DEV_NAME = _T("设备名称");
const CString DEV_TAG = _T("设备标签");

const CString DEV_COMM_INFO = _T("通信基本信息");
const CString DEV_INTERFACE_NUM = _T("通信接口数量");
const CString DEV_INTERFACE_SORT = _T("设置接口优化排序");

const CString DEV_INTERFACE_INFO = _T("通信接口设置");
const CString DEV_NET_TYPE = _T("接口类型");
const CString DEV_SERIAL_NO = _T("串口名");
const CString DEV_ADDR = _T("模块ID");
const CString DEV_IP = _T("以太网地址");
const CString DEV_PORT=_T("以太网通信端口"); 
//////////////////////////////////////////////////////////////////////////
const CString DEV_BASE_INFO_TOOLTIP = _T("设备基本信息");
const CString DEV_TYPE_TOOLTIP = _T("设备类型名");
const CString DEV_NAME_TOOLTIP = _T("设备名称");
const CString DEV_TAG_TOOLTIP = _T("设备标签");

const CString DEV_COMM_INFO_TOOLTIP = _T("通信基本信息");
const CString DEV_INTERFACE_NUM_TOOLTIP = _T("通信接口数量");
const CString DEV_INTERFACE_SORT_TOOLTIP = _T("启动接口优化排序，如果启动则通信时会首先选择最快或者最优的通信接口与设备通信");

const CString DEV_INTERFACE_INFO_TOOLTIP = _T("通信接口设置");
const CString DEV_NET_TYPE_TOOLTIP = _T("接口类型");
const CString DEV_SERIAL_NO_TOOLTIP = _T("串口名，选择本地计算机的串口号");
const CString DEV_ADDR_TOOLTIP = _T("模块ID");
const CString DEV_IP_TOOLTIP = _T("以太网地址");
const CString DEV_PORT_TOOLTIP=_T("以太网通信端口");
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

//!< 获得本窗体的名称
CString CPropertyDevice::GetTitle()
{
	return _T("设备基本信息");
}

//!< 设置属性类型，type=0新增，id表示设备描述的ID号，type=1表示修改，id表示设备ID号
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

//!< 重置新增设备的接口
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
	else if (bSerial && !bEthernet)		//!< 目前只考虑这种情况
	{
		boost::shared_ptr<CDeviceInterface> inf;
		foreach(inf, m_ShowDev->m_vtInterface){
			if(inf->GetType() == 0)		inf->SetType(1);
		}
	}
	else
		ASSERT(FALSE);
}

//!< 给m_ShowDev设置一个新的默认名称
void CPropertyDevice::SetOtherDevName()
{
	CString name = m_ShowDev->getName();
	UINT id = 0;
	if(name == _T(""))		name =_T("新建主设备");
	name = CDevMgr::GetMe().GetSimilarName(name);
	m_ShowDev->setName(name);
}

//!< 显示表格的函数
void CPropertyDevice::ShowInfo(CXTPPropertyGrid& grid)
{
	CXTPPropertyGridItem* pGroup;					//!< 属性组
	CXTPPropertyGridInplaceButton* pButton;
	CDevMgr* devMgr = &CDevMgr::GetMe();
	boost::shared_ptr<XmlInfo::CXmlDevice> xmlDev = m_ShowDev->GetXmlInfo();
	ASSERT(xmlDev);
	std::list<CString> strList, boolList;			//!< 下拉列表的内容
	boolList.push_back(_T("No"));
	boolList.push_back(_T("Yes"));
	CString tooltip;								//!< 有些项的提示信息需要整理后才能使用
	int def = -1;

	//!< 变量基本信息
	pGroup = grid.AddCategory(DEV_BASE_INFO);
	pGroup->SetTooltip(DEV_BASE_INFO_TOOLTIP);
	pGroup->Expand();

	//!< 设备类型名
	AddItemText(*pGroup, DEV_TYPE, DEV_TYPE_TOOLTIP, m_ShowDev->GetTypeName(), DEV_TYPE_ID)->SetReadOnly(TRUE);

	//!< 设备名称
	AddItemText(*pGroup, DEV_NAME, DEV_NAME_TOOLTIP, m_ShowDev->getName(), DEV_NAME_ID);

	//!< 设备别称
	AddItemText(*pGroup, DEV_TAG, DEV_NAME_TOOLTIP, m_ShowDev->getTag(), DEV_TAG_ID);

	//!< 通信基本信息
	pGroup = grid.AddCategory(DEV_COMM_INFO);
	pGroup->SetTooltip(DEV_COMM_INFO_TOOLTIP);
	pGroup->Expand();

	//!< 定义接口数量
	strList.clear();
	CString strInfCount;
	for(int i = 0; i < m_ShowDev->GetInterfaceSize(); ++i){
		strInfCount.Format("%d", i + 1);
		strList.push_back(strInfCount);
	}
	AddItemList(*pGroup, DEV_INTERFACE_NUM, DEV_INTERFACE_NUM_TOOLTIP, strList, (UINT)m_ShowDev->m_vtInterface.size() - 1, DEV_INTERFACE_NUM_ID);

	//!< 启动接口排序
	//AddItemList(*pGroup, DEV_INTERFACE_SORT, DEV_INTERFACE_SORT_TOOLTIP, boolList, m_ShowDev->getInterfaceSort()?1:0, DEV_INTERFACE_SORT_ID);

	//!< 通信接口设置
	boost::shared_ptr<CDeviceInterface> inf;
	CString name,netaddr;
	UINT index = 0;
	UINT infType = 0, serialNo = 0, addr = 1, ip = UINT(-1),devPort=11000;
	std::vector< boost::shared_ptr<CDeviceInterface> >::iterator iter = m_ShowDev->m_vtInterface.begin();
	for(int index = 0; index < m_ShowDev->GetInterfaceSize(); index++)
	{
		//!< 初始化默认值
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

		//!< 接口类型
		strList.clear();
		UINT inf2Main = xmlDev->getInf2Main();
		if(inf2Main & 0x01)			strList.push_back(DEV_NET_SUPPORT1);
		if(inf2Main & 0x02)			strList.push_back(DEV_NET_SUPPORT2);
		if(inf2Main & 0x20)			strList.push_back(DEV_NET_SUPPORT3);
		if(strList.size() == 0)		AddItemText(*pGroup, DEV_NET_TYPE, DEV_NET_TYPE_TOOLTIP, _T("不支持任何已知通信接口，无法与其通信"), DEV_NET_TYPE_ID * (index + 1));
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

		//!< 串口号
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

		//!< 串口地址
		AddItemNumber(*pGroup, DEV_ADDR, DEV_ADDR_TOOLTIP, addr, DEV_ADDR_ID * (index + 1));
       if (netaddr=="")						//!<初始化设备IP地址
			netaddr="192.168.0.1";
		//!< IP地址
		AddItemText(*pGroup,DEV_IP,DEV_IP_TOOLTIP,netaddr,DEV_IP_ID*(index+1));

		AddItemNumber(*pGroup,DEV_PORT,DEV_PORT_TOOLTIP,devPort,DEV_PORT_ID*(index+1));
		//AddItemIP(*pGroup, DEV_IP, DEV_IP_TOOLTIP, ip, DEV_IP_ID * (index + 1));
	}
}

//!< 表格初始化完成后
void CPropertyDevice::OnGridFirstShow(CXTPPropertyGrid& grid)
{
	ShowAndHide(grid);
}

//!< 显示或隐藏相关项
void CPropertyDevice::ShowAndHide(CXTPPropertyGrid& grid)
{
	CXTPPropertyGridItem *item, *tmp;
	CString strValue;

	item = grid.FindItem(DEV_NET_TYPE_ID);
	if(!item)	return;
	strValue = item->GetValue();
	if(DEV_NET_SUPPORT1 == strValue)				//!< 以太网IP
	{
		tmp = grid.FindItem(DEV_IP_ID);					if(tmp)		tmp->SetHidden(FALSE);
		tmp = grid.FindItem(DEV_SERIAL_NO_ID);			if(tmp)		tmp->SetHidden(TRUE);
		tmp = grid.FindItem(DEV_ADDR_ID);				if(tmp)		tmp->SetHidden(TRUE);
		tmp = grid.FindItem(DEV_PORT_ID);					if(tmp)		tmp->SetHidden(FALSE);
	}
	else if(DEV_NET_SUPPORT2 == strValue)			//!< 串口
	{
		tmp = grid.FindItem(DEV_IP_ID);					if(tmp)		tmp->SetHidden(TRUE);
		tmp = grid.FindItem(DEV_SERIAL_NO_ID);			if(tmp)		tmp->SetHidden(FALSE);
		tmp = grid.FindItem(DEV_ADDR_ID);				if(tmp)		tmp->SetHidden(FALSE);
		tmp = grid.FindItem(DEV_PORT_ID);					if(tmp)		tmp->SetHidden(TRUE);
	}
	if(DEV_NET_SUPPORT3 == strValue)				//!< 以太网,远程设备模块地址
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

//!< 当关闭表格时
void CPropertyDevice::OnCloseGrid()
{
}

//!< 要保存修改的内容
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

		if(DEV_NAME_ID == itemID)							//!< 设备名称
		{
			if(pItem->IsValueChanged() && devMgr->GetDevice(strValue))
			{
				AfxMessageBox(_T("名称为 \"") + strValue + _T("\" 的设备已存在，请重新命名..."), MB_OK | MB_ICONEXCLAMATION);
				return false;
			}
			if(strValue.Trim() == _T(""))
			{
				AfxMessageBox(_T("名称不能为空..."), MB_OK | MB_ICONEXCLAMATION);
				return false;
			}
			m_ShowDev->setName(strValue.Trim());
			continue;
		}
		else if(DEV_TAG_ID == itemID)						//!< 设备标签
		{
			m_ShowDev->setTag(strValue.Trim());
			continue;
		}
		else if(DEV_INTERFACE_SORT_ID == itemID)			//!< 接口是否排序
		{
			m_ShowDev->setInterfaceSort(pItem->GetConstraints()->GetCurrent());
			continue;
		}
		else if(DEV_INTERFACE_NUM_ID == itemID)				//!< 接口数量
		{
			int num = pItem->GetConstraints()->GetCurrent() + 1;
			if(num >= 1 && num <= m_ShowDev->GetInterfaceSize())	m_ShowDev->ResizeInterface(num);
			continue;
		}
		else
		{
			//!< 判断所有的接口
			for(int i = 0; i < m_ShowDev->GetInterfaceSize(); ++i)
			{
				if(DEV_NET_TYPE_ID * (i + 1) == itemID)		//!< 主口通信类型
				{
					if(DEV_NET_SUPPORT1 == strValue)		{m_ShowDev->m_vtInterface[i]->SetType(0x00);}// m_ShowDev->m_vtInterface[i]->SetIPorDns(true);}
					else if(DEV_NET_SUPPORT2 == strValue)	{m_ShowDev->m_vtInterface[i]->SetType(0x01);}
					else if(DEV_NET_SUPPORT3 == strValue)	{m_ShowDev->m_vtInterface[i]->SetType(0x20);}
					break;
				}
				else if(DEV_SERIAL_NO_ID * (i + 1) == itemID)//!< 主串口号
				{
					boost::shared_ptr<InterfaceSet::CDSerial> serial = devMgr->GetSerial(strValue);
					if(serial)									m_ShowDev->m_vtInterface[i]->SetSerialNo(serial->getNumber());
					break;
				}
				else if(DEV_ADDR_ID * (i + 1) == itemID)	//!< 主串口模块地址
				{
					m_ShowDev->m_vtInterface[i]->SetHandle(((CXTPPropertyGridItemNumber *)pItem)->GetNumber());
					break;
				}
				else if(DEV_IP_ID * (i + 1) == itemID)		//!< 主IP地址
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
								AfxMessageBox(_T("网络地址不合法,请检查并重新输入"), MB_OK | MB_ICONEXCLAMATION);
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
							AfxMessageBox(_T("网络地址不合法,请检查并重新输入"), MB_OK | MB_ICONEXCLAMATION);
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
	//!< 接口不能重复
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
				CString str = _T("设备 \"") +device->getName() + _T("\"已存在接口\"") + projInf->GetName() + _T("\"， 请重新定义该接口");
				MessageBox(NULL, str, _T("错误"), MB_OK | MB_ICONEXCLAMATION);
				return false;
			}
		}
	}
	return true;
}

//!< 当某项的值发生改变时
void CPropertyDevice::OnItemModify(CXTPPropertyGrid& grid, UINT ID)
{
	if(CProjectMgr::GetMe().IsWatch())				return;
	for(int index = 0; index < m_ShowDev->GetInterfaceSize(); ++index)
	{
		if(ID != DEV_ADDR_ID * (index + 1))		continue;		//!< 找到是不是模块地址被改了
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

//!< 显示帮助信息
void CPropertyDevice::OnShowHelp()
{
	SoftInfo::CMyHelp::GetMe().ShowHelp(_T("主设备"));
}
