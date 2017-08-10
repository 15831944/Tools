#include "StdAfx.h"
#include "PMApp.h"
#include "Gbl.h"
#include "XmlMgr.h"
#include "XmlDevice.h"
#include "XmlScan.h"

using namespace XmlInfo;

const UINT MAX_DEVICE_TYPE = 1024;
const CString DEVICE = _T("Device");
const CString SCAN = _T("Scan");
CXmlMgr::CXmlMgr(void)
{
}

CXmlMgr::~CXmlMgr(void)
{
}

bool CXmlMgr::OpenXml()
{
	CString pathName = CGbl::GetMe().getDataPath() + _T("描述管理.ddm");
	TiXmlDocument pTiXml(pathName);
	if(!pTiXml.LoadFile()){
		MessageBox(NULL, _T("设备描述管理文件格式错误！"), _T("错误"), MB_ICONEXCLAMATION);
		return false;
	}
	//!< 开始解析
	return SerializeXml(pTiXml.RootElement(), true);
}

bool CXmlMgr::SaveXml()
{
	CString pathAll = CGbl::GetMe().getDataPath() + _T("描述管理.ddm");
	TiXmlDocument pTiXml(pathAll);
	TiXmlDeclaration * decl = new TiXmlDeclaration( "1.0", "GB2312", "" );	//!< 起始声明
	pTiXml.LinkEndChild(decl);
	pTiXml.AddComment(_T("请勿手工编辑此文档"));							//!< 注释信息
	TiXmlElement *pRoot = new TiXmlElement("DocumentRoot");					//!< 写入根节点
	SerializeXml(pRoot, false);												//!< 添加工程节点,写操作
	pTiXml.LinkEndChild(pRoot);

	pTiXml.AddComment(_T("File::Type 0工程文件 1变量表文件 2设备拓扑文件 3设备配置文件 4编译文件 5软件配置文件 6主描述文件 7设备描述文件 8数据库配置文件"));
	pTiXml.AddComment(_T("Interface表示支持的连接方式：1以太网，2串口，4ZigBee，5……。可以做按位与操作"));
	pTiXml.AddComment(_T("MaxInterface2Main 与主设备的链接方式"));
	pTiXml.AddComment(_T("Interface2Slave 与从设备的链接方式"));
	pTiXml.AddComment(_T("MaxInterface2Slave 可链接从设备的数量"));
	pTiXml.AddComment(_T("扫描信息所有公司共用一套，每增加一个公司，可以在scan.dd文件中的scanlist下增加一个scan结点"));
	pTiXml.SaveFile();
	return true;
}

bool CXmlMgr::SerializeXml(TiXmlElement* pNode, bool bRead)
{
	std::shared_ptr<CXmlDevice> device;
	std::shared_ptr<CXmlScan> scan;
	if(bRead)
	{
		std::list<std::shared_ptr<CXmlDevice> > ltDevice;
		TiXmlElement* pElement = pNode->FirstChildElement();
		CString name;
		while(pElement)
		{
			name = pElement->Value();
			if(DEVICE == name){
				device = std::shared_ptr<CXmlDevice>(new CXmlDevice);
				if(device->SerializeMgr(pElement, true))	ltDevice.push_back(device);
			}
			else if(SCAN == name){
				scan = std::shared_ptr<CXmlScan>(new CXmlScan);
				if(scan->SerializeMgr(pElement, true))		m_ltScan.push_back(scan);
			}
			pElement = pElement->NextSiblingElement();
		}

		UINT maxNum = 0;
		for (auto device : ltDevice)		maxNum = max(maxNum, device->m_uiID);
		m_vtDevice.resize(maxNum + 1);
		for (auto device : ltDevice)		m_vtDevice[device->m_uiID] = device;
	}
	else
	{
		pNode->AddTiXmlChild(_T("Information"));
		for (auto device : m_vtDevice)
		{
			if(!device)		continue;
			device->SerializeMgr(pNode->AddTiXmlChild((LPCTSTR)DEVICE), false);
		}
		for (auto scan : m_ltScan)
			scan->SerializeMgr(pNode->AddTiXmlChild((LPCTSTR)SCAN), false);
	}
	return true;
}

std::shared_ptr<CXmlDevice> CXmlMgr::GetDevice(UINT type)
{
	std::shared_ptr<CXmlDevice> empty;
	if(type >= m_vtDevice.size())
		return empty;
	return m_vtDevice[type];
}

//!< 如果有重名的设备，返回第一个
std::shared_ptr<CXmlDevice> CXmlMgr::GetDevice(CString name)
{
	std::shared_ptr<CXmlDevice> device, empty;
	for (auto device : m_vtDevice){
		if(!device)		continue;
		if(device->getName() == name)
			return device;
	}
	return empty;
}

//!< 如果有重名的设备，返回第一个
std::shared_ptr<CXmlDevice> CXmlMgr::GetDevice(CString name, CString companyName)
{
	std::shared_ptr<CXmlDevice> device, empty;
	for (auto device : m_vtDevice){
		if(!device)		continue;
		if(device->getName() == name && device->getCompany() == companyName)
			return device;
	}
	return empty;
}

//!< 添加新设备
bool CXmlMgr::AddDevice(std::shared_ptr<CXmlDevice> device)
{
	UINT id = device->getDevType();
	if(GetDevice(id))
	{
		MessageBox(NULL, _T("该设备类型已经存在，添加失败！"), _T("错误"), MB_OK | MB_ICONEXCLAMATION);
		return false;
	}
	if(id > MAX_DEVICE_TYPE)
	{
		MessageBox(NULL, _T("无法解析的设备类型，添加失败！"), _T("错误"), MB_OK | MB_ICONEXCLAMATION);
		return false;
	}
	device->m_uiID = id;
	if(m_vtDevice.size() <= id)		m_vtDevice.resize(id + 1);
	m_vtDevice[id] = device;

	//!< 把设备文件拷贝到本地目录下
	CString fileName = CGbl::GetNameFromFilePathName(device->getPathName());
	CString path = CGbl::GetMe().getDataPath();
	::CopyFile(device->getPathName(), path + fileName, FALSE);
	device->m_strPathName = path + fileName;

	SaveXml();
	return true;
}

//!< 移除设备
bool CXmlMgr::DeleteDevice(UINT id)
{
	ASSERT(id < (UINT)m_vtDevice.size());
	m_vtDevice[id].reset();
	SaveXml();
	return true;
}
