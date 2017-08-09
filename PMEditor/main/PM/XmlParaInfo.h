#pragma once

namespace XmlInfo{
class CXmlParaInfo
{
public:
	UINT m_uiID;					//!< �����ı��
	CString m_strName;				//!< ����������
	CString m_strComment;			//!< ������ע����Ϣ���ɷ���propertygrid���±���ʾ
	CString m_strGroup;				//!< ��������������֣���ʾʱ���������ֵ�ͬһ������
	UINT m_uiValType;				//!< ��������ֵ����
	int m_nSpecialID;				//!< ���⴦��ı�ţ������⴦���-1
	UINT m_uiAccessRight;			//!< �����ķ���Ȩ�ޣ�0ֻ����1ֻд��2�ɶ���д��3����
	CComVariant m_cvrDefValue;		//!< ������Ĭ��ֵ
	CComVariant m_cvrMaxValue;		//!< ���������ֵ
	CComVariant m_cvrMinValue;		//!< ��������Сֵ
	CString m_strMaxValue;			//!< ���������ֵ���ַ����ͣ������-1�ͱ�ʾ������
	CString m_strMinValue;			//!< ��������Сֵ���ַ����ͣ������-1�ͱ�ʾ������
	CString m_strUnit;				//!< �����ĵ�λ
	UINT m_uiEnumID;				//!< ������ֵö�ٱ��
	UINT m_uiReadBID;				//!< �����Ķ���Ϊ���
	UINT m_uiWriteBID;				//!< ������д��Ϊ���

public:
	CXmlParaInfo(void);
	~CXmlParaInfo(void);
	bool SerializeXml(TiXmlElement* pNode);		//!< �����Լ��Ľڵ�

private:
	void ChangeType(UINT type);		//!< ���ݲ������͵Ĳ�ͬ����Ҫ�ı�variant�������Եľ�������
};
}