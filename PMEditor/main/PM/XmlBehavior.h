#pragma once

namespace XmlInfo{
class CXmlBehavior
{
public:
	UINT m_uiID;						//!< ��Ϊ��ID��
	CString m_strName;					//!< ��Ϊ������
	UINT m_uiType;						//!< ��Ϊ�����ͣ�0����1д��2ɨ��
	UINT m_uiCommType;					//!< �������ͨ�ŷ�ʽ��0�ͻ��˷�������1���ģ�2����ַ�
	BOOL m_bShowUser;					//!< �Ƿ���ʾ���û�
	UINT m_uiArrayNum;					//!< ��Ϊ��ĸ�����Ĭ��Ϊ1��������>1ʱ����Ӧ�Ĳ���Ҳ����Ӧ������
	CString m_strArrayName;				//!< ��Ϊ������ƣ�������豸���ã����ֵΪ�����豸Handle�š�
	std::list<UINT> m_ltProtocolID;		//!< ��Ϊ��֧�ֵ�Э���б�
	std::list<UINT> m_ltParaID;			//!< ��Ϊ�������Ĳ����б�

public:
	CXmlBehavior(void);
	~CXmlBehavior(void);
	bool SerializeXml(TiXmlElement* pNode);		//!< �����Լ��Ľڵ�
	bool FindParam(UINT id);					//!< �Ƿ�����������

private:
	void SerializeProtocol(TiXmlElement* pNode);
	void SerializePara(TiXmlElement* pNode);
};
}