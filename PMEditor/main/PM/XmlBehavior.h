#pragma once

namespace XmlInfo{
class CXmlBehavior
{
public:
	UINT m_uiID;						//!< 行为的ID号
	CString m_strName;					//!< 行为的名称
	UINT m_uiType;						//!< 行为的类型，0读，1写，2扫描
	UINT m_uiCommType;					//!< 索引表的通信方式，0客户端服务器，1订阅，2报告分发
	BOOL m_bShowUser;					//!< 是否显示给用户
	UINT m_uiArrayNum;					//!< 行为组的个数，默认为1，当个数>1时，对应的参数也有相应的套数
	CString m_strArrayName;				//!< 行为组的名称，例如从设备配置，这个值为“从设备Handle号”
	std::list<UINT> m_ltProtocolID;		//!< 行为所支持的协力列表
	std::list<UINT> m_ltParaID;			//!< 行为所包含的参数列表

public:
	CXmlBehavior(void);
	~CXmlBehavior(void);
	bool SerializeXml(TiXmlElement* pNode);		//!< 解析自己的节点
	bool FindParam(UINT id);					//!< 是否存在这个参数

private:
	void SerializeProtocol(TiXmlElement* pNode);
	void SerializePara(TiXmlElement* pNode);
};
}