#pragma once

namespace MVC{namespace Device{namespace InterfaceSet{
class CDEthernet
{
private:
	UINT m_uiLocalPort;			//!< 本地通讯端口，0为随机端口
	UINT m_uiRetryNum;			//!< 重试次数
	UINT m_uiOverTime;			//!< 通信超时时间
	UINT m_uiCoolTime;			//!< 冷却时间
	UINT m_uiReviveTime;		//!< 通信恢复时间
	UINT m_uiScanTime;			//!< 设备扫描周期

public:
	UINT getLocalPort(){ return m_uiLocalPort; }			//!< 本地通讯端口，0为随机端口
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