#pragma once

namespace MVC{
namespace Item{
class CItemGroup;
class CPropertySource;
class CPropertyAlarm;
class CItem
{
public:
	enum{
		SRC_TYPE_MEM = 0,
		SRC_TYPE_IO = 1
	};

private:
	UINT m_uiID;									//!< 变量ID号
	CString m_strName;								//!< 变量名称
	CString m_strTag;								//!< 变量标签
	CString m_strValue;								//!< 字符串形式存储的变量值				
	CString m_strIOValue;							//!< 字符串形式存储的变量原始值		
	CString m_strDescription;						//!< 变量的备注信息
	CComVariant m_cvrDefaultValue;					//!< 变量默认值
	CComVariant m_cvrValue;							//!< 变量值
	CComVariant m_cvrValueOld;						//!< 变量值，变量刚才的值，这里要备份一个
	CComVariant m_cvrIOValue;						//!< 变量原始值
	CComVariant m_cvrIOValueOld;					//!< 变量原始值，变量刚才的原始值，这里要备份一个
	UINT m_uiGroupID;								//!< 变量所属变量组的ID号
	UINT m_uiValType;								//!< 变量类型，0-bit，1-char，2-byte，3-short，4-word，5-int，6-dword，7-float，8-double
	UINT m_uiSrcType;								//!< 数据源类型，0内存变量，1I/O变量
	UINT m_uiAccessRight;							//!< 访问权限，0可读可写，1只读，2只写，3无法访问
	BOOL m_bReservFlag;								//!< 是否设置成保留值，true设置，false不设置
	BOOL m_bReservDB;								//!< 是否保存历史数据
	BOOL m_bModbus485;								//!< 是否导出Modbus变量,PAC增加了Modbus服务器功能,需要解析这个属性
	boost::shared_ptr<CPropertySource> m_spSrcInfo;	//!< 源属性
	boost::shared_ptr<CPropertyAlarm> m_spAlarmInfo;//!< 报警属性
	SYSTEMTIME m_stUpdateTime;						//!< 最后一次修改时间
	SYSTEMTIME m_stCreateTime;						//!< 创建时间

	UINT m_uiIRefCount;								//!< 引用计数，就是被使用的次数
	bool m_bAlarmBit;								//!< 位变量是否报警
	bool m_bAlarmLolo;								//!< 下下限是否报警
	bool m_bAlarmLow;								//!< 下限是否报警
	bool m_bAlarmHigh;								//!< 上限是否报警
	bool m_bAlarmHihi;								//!< 上上限是否报警
	bool m_bAlarmAim;								//!< 目标是否报警
	bool m_bAlarmShift;								//!< 变化率是否报警

public:
	UINT getID(){return m_uiID;}										//!< 获得变量ID编号
	CString getName(){return m_strName;}								//!< 获得变量名称
	CString getTag(){return m_strTag;}									//!< 获得变量标签
	CString getDescription(){return m_strDescription;}					//!< 获得变量备注信息
	CComVariant getDefault(){return m_cvrDefaultValue;}					//!< 获得变量的默认值
	CComVariant getValue(){return m_cvrValue;}							//!< 获得变量的当前值
	CComVariant getIOValue(){return m_cvrIOValue;}						//!< 获得变量的当前原始值
	CString getStrValue(bool ishex);															//!<获得变量的字符形式的当前值
	CString getStrIOValue(bool ishex);														//!<获得变量的字符形式的当前原始值
	CString GetFloatStr(float flt);													//!< 获得浮点字符串
	UINT getMyGroupID(){return m_uiGroupID;}							//!< 变量所属变量组的ID号
	UINT getSrcType(){return m_uiSrcType;}								//!< 获得变量数据源类型，0内存，1I/O
	UINT getValType(){return m_uiValType;}								//!< 获得值的类型
	UINT getAccessRight(){return m_uiAccessRight;}						//!< 获得变量的访问类型，0可读可写，1只读，2只写
	BOOL getReservFlag(){return m_bReservFlag;}							//!< 获得变量是否设置为保留值
	BOOL getReservDB(){return m_bReservDB;}								//!< 获得变量是否保存历史数据
	BOOL getModbus485(){return m_bModbus485;}							//!< 是否导出Modbus变量
	boost::shared_ptr<CPropertySource> getSrcInfo(){return m_spSrcInfo;}	//!< 获得数据源属性对象的智能指针
	boost::shared_ptr<CPropertyAlarm> getAlarmInfo(){return m_spAlarmInfo;}	//!< 获得报警属性对象的智能指针
	SYSTEMTIME getCreateTime(){return m_stCreateTime;}					//!< 获得变量的创建时间
	SYSTEMTIME getUpdateTime(){return m_stUpdateTime;}					//!< 获得变量的修改时间

	bool IsAlarmBit(){return m_bAlarmBit;}								//!< 判断变量是否处于报警状态
	bool IsAlarmLolo(){return m_bAlarmLolo;}							//!< 判断变量是否处于报警状态
	bool IsAlarmLow(){return m_bAlarmLow;}								//!< 判断变量是否处于报警状态
	bool IsAlarmHigh(){return m_bAlarmHigh;}							//!< 判断变量是否处于报警状态
	bool IsAlarmHihi(){return m_bAlarmHihi;}							//!< 判断变量是否处于报警状态
	bool IsAlarmAim(){return m_bAlarmAim;}								//!< 判断变量是否处于报警状态
	bool IsAlarmShift(){return m_bAlarmShift;}							//!< 判断变量是否处于报警状态

	void setID(UINT id){m_uiID = id;}									//!< 设置变量的ID编号
	void setItemName(CString name){m_strName = name;}					//!< 设置变量的名称
	void setTag(CString tag){m_strTag = tag;}							//!< 设置变量的标签
	void setDescription(CString dsp){m_strDescription = dsp;}			//!< 设置变量备注信息
	void setDefault(CComVariant default_value){m_cvrDefaultValue=default_value;}//!< 设置变量的默认值
	void setMyGroupID(UINT id){m_uiGroupID = id;}						//!< 变量所属变量组的ID号
	void setSrcType(UINT type){m_uiSrcType = type;}						//!< 设置变量的数据源类型
	void setAccessRight(UINT type){m_uiAccessRight = type;}				//!< 设置变量的访问权限
	void setReservFlag(BOOL flag){m_bReservFlag = flag;}				//!< 设置变量是否保留值
	void setReservDB(BOOL flag){m_bReservDB = flag;}					//!< 设置变量是否保存历史数据
	void setModbus485(BOOL flag){m_bModbus485 = flag;}					//!< 是否导出Modbus变量
	void OnUpdateInfo(){GetLocalTime(&m_stUpdateTime);}					//!< 设置变量被修改后的修改时间
	void setUpdateTime(SYSTEMTIME st){m_stUpdateTime=st;}				//!< 设置更新时间
	void setCreateTime(SYSTEMTIME st){m_stCreateTime=st;}				//!< 设置创建时间
	void setValue(CComVariant value){m_cvrValue=value;}					//!< 设置变量的当前值
	void setIOValue(CComVariant value){m_cvrIOValue = value;}			//!< 设置变量的当前原始值

	void OnAdd(){m_uiIRefCount++;}							//!< 加载到组中，并不是真正的添加
	void OnDeleteFGroup(){/*略*/;}							//!< 从组中去掉，当引用计数归零时，彻底删除
	void OnDelete(){;}										//!< 彻底删除一个变量，不考虑引用计数，遍历所有变量组，去掉关于该变量的记录
	UINT getRefCount(){return m_uiIRefCount;}				//!< 获得引用计数

public:
	CItem(CString name);									//!< 
	~CItem(void);
	void setValType(UINT type);								//!< 设置变量的值类型
	void setValType(CString strType);						//!< 设置变量的值类型
	void SetVarType();										//!< 确认变量的类型
	void ChangeVarType(UINT type);							//!< 根据valType定义的类型，将变量各种variant类型的属性转成真正对应的属性

	bool Serialize(CArchive& ar);							//!< 保存二进制信息
	bool SerializeXml(TiXmlElement* pNode, bool bRead, bool iExport = false);	//!< 保存xml格式的编辑信息,iExport=false正常，true导入导出
	//bool ReadFromPlcConfig(COperation& reader, int nRow, int devID);			//!< 从PLC_Config导出的变量表中读取变量
	bool ReadFromPlcConfig(CString strLine, int devID);		//!< 从PLC_Config导出的变量表中读取变量
	//bool ReadFromDView(COperation& reader, int nRow, int devID);				//!< 从DView导出的变量表中读取变量
	bool ReadFromDView(CString strLine, int devID);			//!< 从DView导出的变量表中读取变量

	CItem& operator = (CItem& item);						//!< 赋值操作，包括ID和名称也全都赋过去了
	bool operator == (CItem& item) const;					//!< 判断本变量是否与item属性一样
	bool OnCloneMe(CItem& item, int index, int addrUnit, int nType = 0);	//!< 要克隆一个自己，nType=0整数命名，1小数命名
	void OnCloneDealIOInfo(CItem& item, int addrUnit);
	CString GetCheckValue();								//!< 获得一个检验值,用于内存脚本检验的
	CString GetValTypeStr();								//!< 以字符串的形式返回工程值类型
	CString GetGroupName();									//!< 返回变量所属组的名称
	void SetAlarm(UINT type, bool alarm = false);			//!< 设置报警
	bool IsAlarm();											//!< 判断是否在报警状态
	bool IsValueChange();									//!< 判断变量值是否改变
	bool IsIOValueChange();									//!< 判断变量原始值是否改变
	void WatchInit();										//!< 监控初始化，清楚状态
	bool DoSearch(CString str, bool bMatchWhole, bool bAllCase, bool bRegex = false);	//!< 查找本变量，要匹配的文本，全字符匹配，大小写匹配

private:
};
}
}