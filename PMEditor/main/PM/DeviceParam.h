#pragma once

namespace XmlInfo{class CXmlParaInfo;}
namespace MVC{namespace Device{
class CDeviceOne;
class CDeviceParam
{
public:
	vector<CComVariant> m_vtValue;		//!< 对应值的数组，下标就是对应行为索引的值，例如从设备类型就1个，但有16套类型值

private:
	UINT m_uiParaID;			//!< 编号
	CString m_strName;			//!< 参数名称，其实没什么用
	UINT m_uiType;				//!< 数据类型
	boost::shared_ptr<XmlInfo::CXmlParaInfo> m_spXmlParam;	//!< 

private:
	void setName(CString name){m_strName = name;}
	void setType(UINT type){m_uiType = type;}

public:
	UINT getParaID(){return m_uiParaID;}
	CString getName(){return m_strName;}
	UINT getType(){return m_uiType;}
	boost::shared_ptr<XmlInfo::CXmlParaInfo> getXmlPara(){return m_spXmlParam;}
	CDeviceParam& operator = (CDeviceParam& para);

public:
	CDeviceParam(void);
	~CDeviceParam(void);

	void InitParaType(boost::shared_ptr<XmlInfo::CXmlParaInfo> xmlPara, UINT groupNum=1);	//!< 新建变量时要初始化参数
	void SetMaxSize(UINT size);					//!< 设置参数的值数量，如果比原来小就不设了
	bool SerializeXml(TiXmlElement* pNode, bool bRead, CDeviceOne* dev);
	CComVariant getValue(UINT index);
	void setValue(CComVariant cv, UINT index);
	void ChangeType(UINT vt);
	

private:
	void setParaID(UINT paraID, CDeviceOne* projDev);						//!< 设置参数
};
}}