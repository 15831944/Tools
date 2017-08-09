#pragma once

namespace XmlInfo{
class CXmlEnumItem
{
public:
	UINT m_uiID;
	int m_nValue;
	CString m_strShowText;

public:
	CXmlEnumItem(void){m_uiID = UINT(-1);}
	~CXmlEnumItem(void){;}
	bool SerializeXml(TiXmlElement* pNode);
};

class CXmlEnum
{
public:
	UINT m_uiID;
	CString m_strName;
	std::vector<boost::shared_ptr<CXmlEnumItem> > m_vtEnumItem;

public:
	CXmlEnum(void){m_uiID = UINT(-1);}
	~CXmlEnum(void){;}
	bool SerializeXml(TiXmlElement* pNode);		//!< 解析自己的节点
	int GetIndex(CComVariant cvr);
	int GetValue(int index); 

private:
	bool SerializeItem(TiXmlElement* pNode);
};
}