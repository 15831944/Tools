#pragma once

namespace XmlInfo{
class CXmlScan
{
private:
	CString m_strID;
	CString m_strName;
	CString m_strCompany;
	CString m_strInf2Main;
	CString m_strPath;
	CString m_strVersion;

public:
	CXmlScan(void);
	~CXmlScan(void);
	bool SerializeMgr(TiXmlElement* pNode, bool bRead);					//!< �����豸�Ĺ�����Ϣ
};
}