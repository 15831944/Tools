#pragma once

namespace XmlInfo{
class CXmlDevice;
class CXmlScan;
class CXmlMgr
{
public:
	std::vector<boost::shared_ptr<CXmlDevice> > m_vtDevice;
	std::list<boost::shared_ptr<CXmlScan> > m_ltScan;

public:
	CXmlMgr(void);
	~CXmlMgr(void);

	bool OpenXml();
	bool SaveXml();
	bool SerializeXml(TiXmlElement* pNode, bool bRead);
	boost::shared_ptr<CXmlDevice> GetDevice(UINT type);
	boost::shared_ptr<CXmlDevice> GetDevice(CString name);					//!< 如果有重名的设备，返回第一个
	boost::shared_ptr<CXmlDevice> GetDevice(CString name, CString company);	//!< 如果有重名的设备，返回第一个

	bool AddDevice(boost::shared_ptr<CXmlDevice> device);					//!< 添加新设备
	bool DeleteDevice(UINT id);												//!< 移除设备
};
}