#pragma once

namespace MVC{
namespace Item{
class CItem;
class CPropertySource
{
private:
	CItem* m_pParent;				// 所属的变量
	CString m_strScriptText;		// 赋值脚本文本

	UINT m_uiIOType;				// IO类型，具体内容看描述文件
	UINT m_uiFreshTime;				// 变量刷新时间,单位1毫秒
	UINT m_uiConvertType;			// 转换类型，0不转换，1是否取反(位变量使用)，1线性转换
	CComVariant m_MinProjectValue;	// 最小工程值
	CComVariant m_MaxProjectValue;	// 最大工程值
	CComVariant m_MinIOValue;		// 最小IO值
	CComVariant m_MaxIOValue;		// 最大IO值

public:
	void setScriptText(CString text){m_strScriptText = text;}
	void setFreshTime(UINT fresh_time){if(fresh_time < 10) fresh_time = 10;	m_uiFreshTime = fresh_time;}
	void setIOType(UINT type){m_uiIOType = type;}
	void setConvertType(UINT type){m_uiConvertType = type; if(m_pParent->getValType() == 0)	m_uiConvertType = 0;}
	void setProjMax(CComVariant cvr){m_MaxProjectValue = cvr;}
	void setProjMin(CComVariant cvr){m_MinProjectValue = cvr;}
	void setIOMax(CComVariant cvr){m_MaxIOValue = cvr;}
	void setIOMin(CComVariant cvr){m_MinIOValue = cvr;}

	CItem* getItem(){return m_pParent;}
	CString getScriptText(){return m_strScriptText;}
	UINT getFreshTime(){return m_uiFreshTime;}
	UINT getIOType(){return m_uiIOType;}
	UINT getConvertType(){return m_uiConvertType;}
	CComVariant getProjMax(){return m_MaxProjectValue;}
	CComVariant getProjMin(){return m_MinProjectValue;}
	CComVariant getIOMax(){return m_MaxIOValue;}
	CComVariant getIOMin(){return m_MinIOValue;}

private:
	int GetTypeFromStr(CString strType);				// 根据名称获得类型

public:
	CPropertySource(CItem* item);
	~CPropertySource(void);

	bool Serialize(CArchive& ar);						// 保存二进制信息
	bool SerializeXml(TiXmlElement* pNode, bool bRead, bool iExport = false);
	void ReadFromPMExcel(std::vector<CString>& vtCell);					// 解析数据源属性

	CPropertySource& operator = (CPropertySource& src);
	bool operator == (CPropertySource& src);
};
}
}