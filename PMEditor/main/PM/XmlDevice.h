#pragma once

namespace XmlInfo{
class CXmlParaInfo;
class CXmlBehavior;
class CXmlArea;
class CXmlProtocol;
class CXmlInterface;
class CXmlEnum;
class CXmlProgramInfo;
class CXmlDevice
{
public:
	bool m_bOpen;														//!< 设备描述文件是否已经被打开并解析
	bool m_bPara2User;													//!< 参数是否暴露给用户
	bool m_bProgrammable;												//!< 是否可以对其编程
	UINT m_uiID;														//!< 设备的编号，描述管理文件中的
	CString m_strCompany;												//!< 所属公司名称
	CString m_strPathName;												//!< 描述文件路径名，绝对路径
	CString m_strVersion;												//!< 版本号
	UINT m_uiDevType;													//!< 类型编号，设备描述中的，应该与m_uiID相等
	UINT m_uiDevIcon;													//!< 设备图标
	CString m_strName;													//!< 类型名称
	UINT m_uiInf2Main;													//!< 支持链接主设备的通信接口，1以太网，2串口，4ZigBee，支持“按位或”操作
	UINT m_uiInf2Slave;													//!< 支持链接从设备的通信接口，1以太网，2串口，4ZigBee，支持“按位或”操作
	UINT m_uiMaxInf2Slave;												//!< 支持最多链接多少个从设备

	std::vector<boost::shared_ptr<CXmlParaInfo> > m_vtPara;				//!< 参数列表
	std::vector<boost::shared_ptr<CXmlBehavior> > m_vtBehavior;			//!< 行为列表
	std::vector<boost::shared_ptr<CXmlArea> > m_vtArea;					//!< 区列表
	std::vector<boost::shared_ptr<CXmlProtocol> > m_vtProtocol;			//!< 协议列表
	std::vector<boost::shared_ptr<CXmlInterface> > m_vtInterface;		//!< 接口列表
	std::vector<boost::shared_ptr<CXmlEnum> > m_vtEnum;					//!< 枚举列表
	std::vector<boost::shared_ptr<CXmlProgramInfo> > m_vtProgram;		//!< 功能块信息，略

public:
	UINT getID(){return m_uiID;}										//!< 获得设备编号
	UINT getIcon(){return m_uiDevIcon;}									//!< 设备的图标
	CString getCompany(){return m_strCompany;}							//!< 获得所属公司名称
	CString getPathName(){return m_strPathName;}						//!< 获得描述文件路径
	CString getVersion(){return m_strVersion;}							//!< 获得版本号
	UINT getDevType(){return m_uiDevType;}								//!< 获得设备类型
	CString getName(){return m_strName;}								//!< 获得设备名称
	UINT getInf2Main(){return m_uiInf2Main;}							//!< 获得链接主设备的通信接口，1以太网，2串口，4……，支持“按位或”操作
	UINT getInf2Slave(){return m_uiInf2Slave;}							//!< 获得链接从设备的通信接口，1以太网，2串口，4……，支持“按位或”操作
	UINT getMaxInf2Slave(){return m_uiMaxInf2Slave;}					//!< 获得最多链接多少个从设备

	bool IsPara2User(){return m_bPara2User;}							//!< 是否可以将参数暴露给用户
	bool IsSupportEthernet(){return bool((m_uiInf2Main & 1) || (m_uiInf2Main & 0x20));}	//!< 是否支持以太网
	bool IsSupportSerial(){return bool(m_uiInf2Main & 2);}				//!< 是否支持串口
	bool IsProgrammable(){return m_bProgrammable;}						//!< 是否可以对其编程

public:
	CXmlDevice(void);
	~CXmlDevice(void);

	boost::shared_ptr<CXmlArea> getArea(UINT id);
	boost::shared_ptr<CXmlArea> getArea(CString name);
	boost::shared_ptr<CXmlArea> GetAreaNameList(std::list<CString>& strList, int& defIndex, bool bSrot);//!< 获得区的列表，defIndex传进来时表示areaID，传出去时表示列表的默认索引，返回默认区
	int GetAreaID(CString name);

	boost::shared_ptr<CXmlBehavior> getBehavior(UINT id);
	std::list<boost::shared_ptr<CXmlBehavior> > getBehavior(CString name);
	CString GetAllBevID(CString strBev, bool bRead);					//!< 获得所有该名称的行为,以-分割

	boost::shared_ptr<CXmlParaInfo> getPara(UINT id);

	boost::shared_ptr<CXmlProtocol> getProtocol(CString name);
	boost::shared_ptr<CXmlProtocol> getProtocol(UINT id);

	boost::shared_ptr<CXmlInterface> getInterface(UINT id);

	boost::shared_ptr<CXmlEnum> getEnum(UINT id);

	bool OpenXml();
	bool SerializeMgr(TiXmlElement* pNode, bool bRead);					//!< 解析设备的管理信息
	bool SerializeXml(TiXmlElement* pNode);								//!< 解析设备信息，从设备单个文件中解析

private:
	template<class T>
	bool SerializeT(TiXmlElement* pNode, std::vector<boost::shared_ptr<T> >& vtObject, UINT maxNum, CString name);
	void SayError(CString text, UINT maxNum);							//!< 解析文件报错，text是解析的节点名，maxNUM是最大数量
	void OnCheck();														//!< 校验一下
};
}