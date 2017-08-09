#pragma once

namespace XmlInfo{
class CXmlOperate
{
public:
	UINT m_uiType;
	CString m_strName;
	UINT m_uiReviseBitIdx;
	UINT m_uiOperateStart;					//!< ĳ����������ʼ��ַ
	UINT m_uiOperateEnd;					//!< ĳ����������ֹ��ַ

	bool operator == (CXmlOperate* oper);	//!< �ж����������Ƿ���ͬ
	bool SerializeXml(TiXmlElement* pNode);	//!< ���������ļ�
};

class CXmlArea
{
public:
	UINT m_uiID;							//!< �����
	CString m_strName;						//!< ������
	UINT m_uiUnitBitLen;					//!< Ѱַ��λ���ȣ�1λ��8�ֽڣ�16�֣�32˫�֣�������NΪѰַ
	int m_nUnitBitReadStart;				//!< λ��ʼ
	int m_nUnitBitReadEnd;					//!< λ��ֹ
	std::list<UINT> m_ltProtocolID;			//!< ֧�ֵ�Э�����б����������ûʲô��
	std::list<boost::shared_ptr<CXmlOperate> > m_ltOperate;		//!< ֧�ֵĲ���

public:
	UINT getID(){return m_uiID;}
	CString getName(){return m_strName;}

public:
	CXmlArea(void);
	~CXmlArea(void);
	bool SerializeXml(TiXmlElement* pNode);				//!< �����Լ��Ľڵ�
	bool IsSame(CXmlArea* area);						//!< �ж������������Ƿ�һ��
	bool operator == (CXmlArea* area);					//!< �ж������������Ƿ�һ��
	CString GetUnitName();								//!< ���Ѱַ��λ�����֣���λ���ֽڡ��֡�˫�ֵȣ�
	UINT GetSupportType(CString text);					//!< ͨ���û���ѡ��Ĳ������ͻ�ö�Ӧ�Ĳ���ID
	UINT GetOperType(UINT valType);						//!< �����������ͣ��ҵ���Ӧ�Ĳ�������
	UINT GetUnitEnd(UINT type);							//!< ��ø�����Ѱַ�����Ѱֵַ
	CString GetOperateRange(UINT type);					//!< ��øò����ķ�Χ��Ϣ
	std::list<CString> GetSupportUnitList(int& defIndex);	//!< �������֧�ֵĲ����ַ����б�

private:
	void SerializeProtocol(TiXmlElement* pNode);
	void SerializeOperate(TiXmlElement* pNode);
};
}