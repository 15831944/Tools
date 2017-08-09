#pragma once

namespace XmlInfo{
class CXmlParaInfo
{
public:
	UINT m_uiID;					//!< 参数的编号
	CString m_strName;				//!< 参数的名称
	CString m_strComment;			//!< 参数的注释信息，可放在propertygrid的下边显示
	CString m_strGroup;				//!< 参数所属组的名字，显示时参数将划分到同一组名下
	UINT m_uiValType;				//!< 参数的数值类型
	int m_nSpecialID;				//!< 特殊处理的编号，不特殊处理的-1
	UINT m_uiAccessRight;			//!< 参数的访问权限，0只读，1只写，2可读可写，3隐藏
	CComVariant m_cvrDefValue;		//!< 参数的默认值
	CComVariant m_cvrMaxValue;		//!< 参数的最大值
	CComVariant m_cvrMinValue;		//!< 参数的最小值
	CString m_strMaxValue;			//!< 参数的最大值，字符串型，如果是-1就表示无意义
	CString m_strMinValue;			//!< 参数的最小值，字符串型，如果是-1就表示无意义
	CString m_strUnit;				//!< 参数的单位
	UINT m_uiEnumID;				//!< 参数的值枚举编号
	UINT m_uiReadBID;				//!< 参数的读行为编号
	UINT m_uiWriteBID;				//!< 参数的写行为编号

public:
	CXmlParaInfo(void);
	~CXmlParaInfo(void);
	bool SerializeXml(TiXmlElement* pNode);		//!< 解析自己的节点

private:
	void ChangeType(UINT type);		//!< 根据参数类型的不同，我要改变variant类型属性的具体类型
};
}