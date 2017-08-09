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

const CString DEV_BASE_INFO = _T("设备基本信息");
const CString DEV_TYPE = _T("设备类型名");
const CString DEV_NAME = _T("设备名称");
const CString DEV_TAG = _T("设备标签");

const CString DEV_COMM_INFO = _T("通信基本信息");
const CString DEV_COMM_TYPE = _T("接口类型");
const CString DEV_ADDR = _T("从设备Handle号");

//////////////////////////////////////////////////////////////////////////
const CString DEV_BASE_INFO_TOOLTIP = _T("设备基本信息");
const CString DEV_TYPE_TOOLTIP = _T("设备类型名");
const CString DEV_NAME_TOOLTIP = _T("设备名称");
const CString DEV_TAG_TOOLTIP = _T("设备标签");

const CString DEV_COMM_INFO_TOOLTIP = _T("通信基本信息");
const CString DEV_COMM_TYPE_TOOLTIP = _T("接口类型");
const CString DEV_ADDR_TOOLTIP = _T("同一个主设备的所有从设备中Handle不可重复。该值可通过主设备行为中的上载\"从设备参数\"得到。");

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

//!< 获得本窗体的名称
CString CPropertySlave::GetTitle()
{
	return _T("设备基本信息");
}

//!< 设置属性类型，type=0新增，id表示设备描述的ID号，type=1表示修改，id表示设备ID号
void CPropertySlave::SetType(UINT type, UINT id, UINT parentID)
{
	m_uiType = type;
	m_uiID = id;
	m_uiParentID = parentID;
	m_ShowDev->setDevType(id);

	CDevMgr* devMgr = &CDevMgr::GetMe();

	if(m_uiType == 0){				//!< 如果是新建
		SetOtherDevName();
		ASSERT(devMgr->GetDevice(parentID));
		m_ShowDev->setID(UINT(-1));
	}
	else if(m_uiType == 1)			//!< 如果是修改
	{
		boost::shared_ptr<CDeviceOne> device = devMgr->GetDevice(id);
		ASSERT(device);
		m_ShowDev->CopyFrom(*device);
		m_ShowDev->setID(device->getID());
	}
	m_ShowDev->ResizeInterface(1);	//!< 从设备就1个接口，还必须是串口
	m_ShowDev->m_vtInterface[0]->SetType(16);
}

//!< 给m_ShowDev设置一个新的默认名称
void CPropertySlave::SetOtherDevName()
{
	CString name = m_ShowDev->getName();
	UINT id = 0;
	if(name == _T(""))		name =_T("新建从设备");
	name = CDevMgr::GetMe().GetSimilarName(name);
	m_ShowDev->setName(name);
}

//!< 显示表格的函数
void CPropertySlave::ShowInfo(CXTPPropertyGrid& grid)
{
	CXTPPropertyGridItem* pGroup;					//!< 属性组
	CXTPPropertyGridInplaceButton* pButton;
	CDevMgr* devMgr = &CDevMgr::GetMe();
	boost::shared_ptr<CDeviceOne> parent = devMgr->GetDevice(m_uiParentID);
	if(!parent)		return;
	boost::shared_ptr<XmlInfo::CXmlDevice> xmlDev = m_ShowDev->GetXmlInfo();
	boost::shared_ptr<XmlInfo::CXmlDevice> xmlParent = parent->GetXmlInfo();
	std::list<CString> strList;						//!< 下拉列表的内容
	CString tooltip;								//!< 有些项的提示信息需要整理后才能使用
	int def = -1;

	ASSERT(!m_ShowDev->m_vtInterface.empty());
	//!< 变量基本信息
	pGroup = grid.AddCategory(DEV_BASE_INFO);
	pGroup->SetTooltip(DEV_BASE_INFO_TOOLTIP);
	pGroup->Expand();

	//!< 设备类型名
	AddItemText(*pGroup, DEV_TYPE, DEV_TYPE_TOOLTIP, m_ShowDev->GetTypeName(), DEV_TYPE_ID)->SetReadOnly(TRUE);

	//!< 设备名称
	AddItemText(*pGroup, DEV_NAME, DEV_NAME_TOOLTIP, m_ShowDev->getName(), DEV_NAME_ID);

	//!< 设备标签
	AddItemText(*pGroup, DEV_TAG, DEV_TAG_TOOLTIP, m_ShowDev->getTag(), DEV_TAG_ID);

	//!< 通信基本信息
	pGroup = grid.AddCategory(DEV_COMM_INFO);
	pGroup->SetTooltip(DEV_COMM_INFO_TOOLTIP);
	pGroup->Expand();

	//!< 从设备地址
	strList.clear();
	strList.push_back(_T("串口"));
//	strList.push_back(_T("ZigBee"));
	AddItemList(*pGroup, DEV_COMM_TYPE, DEV_COMM_TYPE_TOOLTIP, strList, 0, DEV_COMM_TYPE_ID);

	//!< 从设备地址
	strList.clear();
	CString text;
	int slaveCount = xmlParent?xmlParent->getMaxInf2Slave():16;		//!< 默认16个
	for(int i = 0; i < slaveCount; ++i)
	{
		text.Format("%d", i);
		strList.push_back(text);
	}
	AddItemList(*pGroup, DEV_ADDR, DEV_ADDR_TOOLTIP, strList, m_ShowDev->m_vtInterface[0]->GetHandle(), DEV_ADDR_ID);
}

//!< 表格初始化完成后
void CPropertySlave::OnGridFirstShow(CXTPPropertyGrid& grid)
{
	ShowAndHide(grid);
}

//!< 显示或隐藏相关项
void CPropertySlave::ShowAndHide(CXTPPropertyGrid& grid)
{
}

//!< 当关闭表格时
void CPropertySlave::OnCloseGrid()
{
}

//!< 要保存修改的内容
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
		}
		else if(DEV_TAG_ID == itemID)						//!< 设备标签
		{
			m_ShowDev->setTag(strValue.Trim());
		}
		else if(DEV_ADDR_ID == itemID)						//!< 从设备地址
		{
			m_ShowDev->m_vtInterface[0]->SetHandle(pItem->GetConstraints()->GetCurrent());
		}
	}
	//!< 接口不能重复
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
				CString str = _T("设备 \"") + device->getName() + _T(" \"已存在接口 \"") + projInf->GetName() + _T(" \"， 请重新定义该接口");
				MessageBox(NULL, str, _T("错误"), MB_OK | MB_ICONEXCLAMATION);
				return false;
			}
		}
	}
	return true;
}

//!< 当某项的值发生改变时
void CPropertySlave::OnItemModify(CXTPPropertyGrid& grid, UINT ID)
{
	if(CProjectMgr::GetMe().IsWatch())						return;
	ShowAndHide(grid);
}

//!< 显示帮助信息
void CPropertySlave::OnShowHelp()
{
	SoftInfo::CMyHelp::GetMe().ShowHelp(_T("从设备"));
}
