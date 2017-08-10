#pragma once

namespace XmlInfo{
class CXmlDevice;
class CXmlMgr
{
public:
	std::vector<std::shared_ptr<CXmlDevice> > m_vtDevice;

public:
	CXmlMgr(void);
	~CXmlMgr(void);

	bool OpenXml();
	bool SaveXml();
	bool SerializeXml(TiXmlElement* pNode, bool bRead);
	std::shared_ptr<CXmlDevice> GetDevice(UINT type);
	std::shared_ptr<CXmlDevice> GetDevice(CString name);					//!< 如果有重名的设备，返回第一个
	std::shared_ptr<CXmlDevice> GetDevice(CString name, CString company);	//!< 如果有重名的设备，返回第一个

	bool AddDevice(std::shared_ptr<CXmlDevice> device);					//!< 添加新设备
	bool DeleteDevice(UINT id);												//!< 移除设备
};
}