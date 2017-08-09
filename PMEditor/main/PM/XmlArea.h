#pragma once

namespace XmlInfo{
class CXmlOperate
{
public:
	UINT m_uiType;
	CString m_strName;
	UINT m_uiReviseBitIdx;
	UINT m_uiOperateStart;					//!< 某个操作的起始地址
	UINT m_uiOperateEnd;					//!< 某个操作的终止地址

	bool operator == (CXmlOperate* oper);	//!< 判断两个操作是否相同
	bool SerializeXml(TiXmlElement* pNode);	//!< 解析描述文件
};

class CXmlArea
{
public:
	UINT m_uiID;							//!< 区编号
	CString m_strName;						//!< 区名称
	UINT m_uiUnitBitLen;					//!< 寻址单位长度，1位，8字节，16字，32双字，其它是N为寻址
	int m_nUnitBitReadStart;				//!< 位起始
	int m_nUnitBitReadEnd;					//!< 位终止
	std::list<UINT> m_ltProtocolID;			//!< 支持的协议编号列表——这个对我没什么用
	std::list<boost::shared_ptr<CXmlOperate> > m_ltOperate;		//!< 支持的操作

public:
	UINT getID(){return m_uiID;}
	CString getName(){return m_strName;}

public:
	CXmlArea(void);
	~CXmlArea(void);
	bool SerializeXml(TiXmlElement* pNode);				//!< 解析自己的节点
	bool IsSame(CXmlArea* area);						//!< 判断两个区描述是否一样
	bool operator == (CXmlArea* area);					//!< 判断两个区描述是否一样
	CString GetUnitName();								//!< 获得寻址单位的名字（如位、字节、字、双字等）
	UINT GetSupportType(CString text);					//!< 通过用户所选择的操作类型获得对应的操作ID
	UINT GetOperType(UINT valType);						//!< 根据数据类型，找到对应的操作类型
	UINT GetUnitEnd(UINT type);							//!< 获得该类型寻址的最大寻址值
	CString GetOperateRange(UINT type);					//!< 获得该操作的范围信息
	std::list<CString> GetSupportUnitList(int& defIndex);	//!< 获得区所支持的操作字符串列表

private:
	void SerializeProtocol(TiXmlElement* pNode);
	void SerializeOperate(TiXmlElement* pNode);
};
}