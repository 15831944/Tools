#pragma once

namespace XmlInfo{
class CXmlProtocolOperateInfo
{
public:
	float m_uiID;				//!< 操作类型，0位操作，1单元操作，2双单元操作，0.25表示1/4个单元操作
	UINT m_uiStartAddr;			//!< 单元的首地址
	UINT m_uiPerLen;			//!< 单元地址的间隔
	UINT m_uiEndAddr;			//!< 单元末地址
	UINT m_uiMaxRead;			//!< 单个报文最多能读取的单元/位数
	UINT m_uiMinRead;			//!< 单个报文最少要读取的单元/位数
	UINT m_uiBlockRead;			//!< 是否整读，即每次读固定的数据段，如8000读小区

public:
	CXmlProtocolOperateInfo(void){m_uiID = float(-1);}
	~CXmlProtocolOperateInfo(void){;}
	bool SerializeXml(TiXmlElement* pNode);					//!< 解析自己的节点
};

class CXmlProtocolArea
{
public:
	UINT m_uiID;				//!< 区的编号
//	CString m_strName;			//!< 区的名称
	std::list<std::shared_ptr<CXmlProtocolOperateInfo> > m_ltOperate;		//!< 区的操作功能列表

public:
	CXmlProtocolArea(void){m_uiID = UINT(-1);}
	~CXmlProtocolArea(void){;}
	bool SerializeXml(TiXmlElement* pNode);					//!< 解析自己的节点
};

class CXmlProtocol
{
public:
	UINT m_uiID;				//!< 协议编号
	CString m_strName;			//!< 协议名称
	CString m_strAuthor;		//!< 协议创建作者
	CString m_strEditTime;		//!< 协议创建时间
	CString m_strExplain;		//!< 协议说明
	std::list<std::shared_ptr<CXmlProtocolArea> > m_ltProtocolArea;	//!< 协议包含的区列表
	std::list<UINT> m_ltBehaviorID;		//!< 协议包含的行为的编号列表

public:
	CXmlProtocol(void);
	~CXmlProtocol(void);
	bool SerializeXml(TiXmlElement* pNode);						//!< 解析自己的节点
	bool FindBev(UINT id);										//!< 看看这个行为知否存在

private:
	void SerializeArea(TiXmlElement* pNode);					//!< 解析协议区节点
	void SerializeBehavior(TiXmlElement* pNode);				//!< 解析协议行为节点
};
}