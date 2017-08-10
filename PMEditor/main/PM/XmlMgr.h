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
	std::shared_ptr<CXmlDevice> GetDevice(CString name);					//!< ������������豸�����ص�һ��
	std::shared_ptr<CXmlDevice> GetDevice(CString name, CString company);	//!< ������������豸�����ص�һ��

	bool AddDevice(std::shared_ptr<CXmlDevice> device);					//!< ������豸
	bool DeleteDevice(UINT id);												//!< �Ƴ��豸
};
}