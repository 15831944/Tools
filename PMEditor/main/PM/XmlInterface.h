#pragma once

namespace XmlInfo{
class CXmlInterface
{
public:
	UINT m_uiID;				//!< �ӿڱ��
	CString m_strName;			//!< �ӿ�����
	UINT m_uiType;				//!< �ӿ����ͣ�0��̫����1���ڣ�2��̫�����豸��3���ڴ��豸
	UINT m_uiDataPort;			//!< ����ͨ�Ŷ˿ں�
	UINT m_uiListenPort;		//!< �������޶˿ں�
	UINT m_uiReviveTime;		//!< �ط�ʱ����
	UINT m_uiRetryNum;			//!< �������Դ���
	UINT m_uiCoolTime;			//!< ��ȴʱ��
	UINT m_uiOverTime;			//!< ��ʱʱ��

	std::list<UINT> m_ltProtocol;	//!< �ýӿ�֧�ֵ�ͨ��Э��

public:
	CXmlInterface(void);
	~CXmlInterface(void);
	bool SerializeXml(TiXmlElement* pNode);		//!< �����Լ��Ľڵ�

private:
	void SerializeProtocol(TiXmlElement* pNode);
};
}
