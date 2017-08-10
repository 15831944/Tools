#pragma once

namespace XmlInfo{
class CXmlInterface
{
public:
	UINT m_uiID;				//!< 接口编号
	CString m_strName;			//!< 接口名称
	UINT m_uiType;				//!< 接口类型，0以太网，1串口，2以太网从设备，3串口从设备
	UINT m_uiDataPort;			//!< 数据通信端口号
	UINT m_uiListenPort;		//!< 监听上限端口号
	UINT m_uiReviveTime;		//!< 重发时间间隔
	UINT m_uiRetryNum;			//!< 错误重试次数
	UINT m_uiCoolTime;			//!< 冷却时间
	UINT m_uiOverTime;			//!< 超时时间

	std::list<UINT> m_ltProtocol;	//!< 该接口支持的通信协议

public:
	CXmlInterface(void);
	~CXmlInterface(void);
	bool SerializeXml(TiXmlElement* pNode);		//!< 解析自己的节点

private:
	void SerializeProtocol(TiXmlElement* pNode);
};
}
