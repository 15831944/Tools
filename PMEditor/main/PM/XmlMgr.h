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
	boost::shared_ptr<CXmlDevice> GetDevice(CString name);					//!< ������������豸�����ص�һ��
	boost::shared_ptr<CXmlDevice> GetDevice(CString name, CString company);	//!< ������������豸�����ص�һ��

	bool AddDevice(boost::shared_ptr<CXmlDevice> device);					//!< ������豸
	bool DeleteDevice(UINT id);												//!< �Ƴ��豸
};
}