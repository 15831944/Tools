#pragma once

namespace XmlInfo{
class CXmlProtocolOperateInfo
{
public:
	float m_uiID;				//!< �������ͣ�0λ������1��Ԫ������2˫��Ԫ������0.25��ʾ1/4����Ԫ����
	UINT m_uiStartAddr;			//!< ��Ԫ���׵�ַ
	UINT m_uiPerLen;			//!< ��Ԫ��ַ�ļ��
	UINT m_uiEndAddr;			//!< ��Ԫĩ��ַ
	UINT m_uiMaxRead;			//!< ������������ܶ�ȡ�ĵ�Ԫ/λ��
	UINT m_uiMinRead;			//!< ������������Ҫ��ȡ�ĵ�Ԫ/λ��
	UINT m_uiBlockRead;			//!< �Ƿ���������ÿ�ζ��̶������ݶΣ���8000��С��

public:
	CXmlProtocolOperateInfo(void){m_uiID = float(-1);}
	~CXmlProtocolOperateInfo(void){;}
	bool SerializeXml(TiXmlElement* pNode);					//!< �����Լ��Ľڵ�
};

class CXmlProtocolArea
{
public:
	UINT m_uiID;				//!< ���ı��
//	CString m_strName;			//!< ��������
	std::list<std::shared_ptr<CXmlProtocolOperateInfo> > m_ltOperate;		//!< ���Ĳ��������б�

public:
	CXmlProtocolArea(void){m_uiID = UINT(-1);}
	~CXmlProtocolArea(void){;}
	bool SerializeXml(TiXmlElement* pNode);					//!< �����Լ��Ľڵ�
};

class CXmlProtocol
{
public:
	UINT m_uiID;				//!< Э����
	CString m_strName;			//!< Э������
	CString m_strAuthor;		//!< Э�鴴������
	CString m_strEditTime;		//!< Э�鴴��ʱ��
	CString m_strExplain;		//!< Э��˵��
	std::list<std::shared_ptr<CXmlProtocolArea> > m_ltProtocolArea;	//!< Э����������б�
	std::list<UINT> m_ltBehaviorID;		//!< Э���������Ϊ�ı���б�

public:
	CXmlProtocol(void);
	~CXmlProtocol(void);
	bool SerializeXml(TiXmlElement* pNode);						//!< �����Լ��Ľڵ�
	bool FindBev(UINT id);										//!< ���������Ϊ֪�����

private:
	void SerializeArea(TiXmlElement* pNode);					//!< ����Э�����ڵ�
	void SerializeBehavior(TiXmlElement* pNode);				//!< ����Э����Ϊ�ڵ�
};
}