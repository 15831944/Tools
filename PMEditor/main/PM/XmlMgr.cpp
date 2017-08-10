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
	CString pathName = CGbl::GetMe().getDataPath() + _T("��������.ddm");
	TiXmlDocument pTiXml(pathName);
	if(!pTiXml.LoadFile()){
		MessageBox(NULL, _T("�豸���������ļ���ʽ����"), _T("����"), MB_ICONEXCLAMATION);
		return false;
	}
	//!< ��ʼ����
	return SerializeXml(pTiXml.RootElement(), true);
}

bool CXmlMgr::SaveXml()
{
	CString pathAll = CGbl::GetMe().getDataPath() + _T("��������.ddm");
	TiXmlDocument pTiXml(pathAll);
	TiXmlDeclaration * decl = new TiXmlDeclaration( "1.0", "GB2312", "" );	//!< ��ʼ����
	pTiXml.LinkEndChild(decl);
	pTiXml.AddComment(_T("�����ֹ��༭���ĵ�"));							//!< ע����Ϣ
	TiXmlElement *pRoot = new TiXmlElement("DocumentRoot");					//!< д����ڵ�
	SerializeXml(pRoot, false);												//!< ��ӹ��̽ڵ�,д����
	pTiXml.LinkEndChild(pRoot);

	pTiXml.AddComment(_T("File::Type 0�����ļ� 1�������ļ� 2�豸�����ļ� 3�豸�����ļ� 4�����ļ� 5��������ļ� 6�������ļ� 7�豸�����ļ� 8���ݿ������ļ�"));
	pTiXml.AddComment(_T("Interface��ʾ֧�ֵ����ӷ�ʽ��1��̫����2���ڣ�4ZigBee��5��������������λ�����"));
	pTiXml.AddComment(_T("MaxInterface2Main �����豸�����ӷ�ʽ"));
	pTiXml.AddComment(_T("Interface2Slave ����豸�����ӷ�ʽ"));
	pTiXml.AddComment(_T("MaxInterface2Slave �����Ӵ��豸������"));
	pTiXml.AddComment(_T("ɨ����Ϣ���й�˾����һ�ף�ÿ����һ����˾��������scan.dd�ļ��е�scanlist������һ��scan���"));
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

//!< ������������豸�����ص�һ��
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

//!< ������������豸�����ص�һ��
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

//!< ������豸
bool CXmlMgr::AddDevice(std::shared_ptr<CXmlDevice> device)
{
	UINT id = device->getDevType();
	if(GetDevice(id))
	{
		MessageBox(NULL, _T("���豸�����Ѿ����ڣ����ʧ�ܣ�"), _T("����"), MB_OK | MB_ICONEXCLAMATION);
		return false;
	}
	if(id > MAX_DEVICE_TYPE)
	{
		MessageBox(NULL, _T("�޷��������豸���ͣ����ʧ�ܣ�"), _T("����"), MB_OK | MB_ICONEXCLAMATION);
		return false;
	}
	device->m_uiID = id;
	if(m_vtDevice.size() <= id)		m_vtDevice.resize(id + 1);
	m_vtDevice[id] = device;

	//!< ���豸�ļ�����������Ŀ¼��
	CString fileName = CGbl::GetNameFromFilePathName(device->getPathName());
	CString path = CGbl::GetMe().getDataPath();
	::CopyFile(device->getPathName(), path + fileName, FALSE);
	device->m_strPathName = path + fileName;

	SaveXml();
	return true;
}

//!< �Ƴ��豸
bool CXmlMgr::DeleteDevice(UINT id)
{
	ASSERT(id < (UINT)m_vtDevice.size());
	m_vtDevice[id].reset();
	SaveXml();
	return true;
}
