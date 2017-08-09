#pragma once

namespace MVC{namespace Device{namespace InterfaceSet{
class CDEthernet
{
private:
	UINT m_uiLocalPort;			//!< ����ͨѶ�˿ڣ�0Ϊ����˿�
	UINT m_uiRetryNum;			//!< ���Դ���
	UINT m_uiOverTime;			//!< ͨ�ų�ʱʱ��
	UINT m_uiCoolTime;			//!< ��ȴʱ��
	UINT m_uiReviveTime;		//!< ͨ�Żָ�ʱ��
	UINT m_uiScanTime;			//!< �豸ɨ������

public:
	UINT getLocalPort(){ return m_uiLocalPort; }			//!< ����ͨѶ�˿ڣ�0Ϊ����˿�
	UINT getRetryNum(){return m_uiRetryNum;}
	UINT getOverTime(){return m_uiOverTime;}
	UINT getCoolTime(){return m_uiCoolTime;}
	UINT getReviveTime(){return m_uiReviveTime;}
	UINT getScanTime(){return m_uiScanTime;}
	void setLocalPort(UINT port){ if (port > 65535) port = 0; m_uiLocalPort = port; }
	void setRetryNum(UINT num){m_uiRetryNum = num;}
	void setOverTime(UINT t){m_uiOverTime = t;}
	void setCoolTime(UINT t){m_uiCoolTime = t;}
	void setReviveTime(UINT t){m_uiReviveTime = t;}
	void setScanTime(UINT t){m_uiScanTime = t;}

public:
	explicit CDEthernet();
	~CDEthernet(void);
	bool SerializeXml(TiXmlElement* pNode, bool bRead);
};
}}}