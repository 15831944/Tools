#pragma once

#include "XTooltip.h"

namespace XmlInfo{class CXmlDevice;}
namespace MVC{
namespace Device{
class CDeviceMapDoc;
class CDeviceParam;
class CDeviceInterface;
class CDeviceOne
{
protected:
	UINT m_uiID;					//!< 设备ID号
	UINT m_uiType;					//!< 设备的类型号，对应描述文件的类型编号
	CString m_strID;				//!< 设备的序列化编号，在扫描过程中读取上来的
	CString m_strType;				//!< 设备的类型名，冗余记录，希望在没有得到描述信息的时候，就已经知道了设备类型名，方便提示用户
	CString m_strName;				//!< 设备的名称，用户所起
	CString m_strTag;				//!< 设备的标签
	CString m_strPathName;			//!< 设备的保存文件地址
	UINT m_uiParentID;				//!< 父设备编号，默认-1
	UINT m_uiLevel;					//!< 设备所在级别，从0开始，游离设备级别为-1
	BOOL m_bInterfaceSort;			//!< 是否对设备接口的配置自动排序

	CPoint m_Point;					//!< 自己的位置
	bool m_bModify;					//!< 是否参数被修改了
	bool m_bSelect;					//!< 是否被选中了
	bool m_bExpand;					//!< 表示设备是否处于伸展状态
	bool m_bShow;					//!< 是否画，父设备缩起来的时候，子设备就不画
	boost::shared_ptr<XmlInfo::CXmlDevice> m_spXmlDevice;				//!< 设备对应的描述文件

	bool m_bLoadDcfg;				//!< 是否解析到设备属性配置文档*.dcfg
	bool m_bProj;					//!< 是否是工程设备，true是工程设备，false不是

public:
	std::list<UINT> m_ltChildID;	//!< 子设备链表
	std::vector< boost::shared_ptr<CDeviceInterface> > m_vtInterface;	//!< 设备所使用的接口列表
	std::vector< boost::shared_ptr<CDeviceParam> > m_vtParam;			//!< 参数列表

public:
	bool IsSelect(){return m_bSelect;}
	bool IsExpand(){return m_bExpand;}
	bool IsShow(){return m_bShow;}
	bool IsProj(){return m_bProj;}										//!< 判断自己是不是工程中的设备
	bool IsModify(){return m_bModify;}									//!< 是否参数被修改了
	UINT getID(){return m_uiID;}
	UINT getDevType(){return m_uiType;}
	CString getStrType(){return m_strType;}
	CString getStrID(){return m_strID;}
	CString getStrParentID();											//!< 获得父设备的ID
	CString getName(){return m_strName;}
	CString getTag(){return m_strTag;}
	CString getPath(){return m_strPathName;}
	UINT getParentID(){return m_uiParentID;}
	UINT getLevel(){return m_uiLevel;}
	BOOL getInterfaceSort(){return m_bInterfaceSort;}
	long getState();													//!< 返回设备当前状态，0离线，1在线，2在线未配置，3在线冲突

	void setID(UINT id){m_uiID = id;}
	bool setDevType(UINT type);											//!< 这个需要代码处理，计算一下存储对应的设备描述
	void setStrID(CString strID){m_strID = strID;}
	void setTag(CString tag){m_strTag = tag;}
	void setPath(CString path){m_strPathName = path;}
	void setParentID(UINT id){m_uiParentID = id;}
	void setLevel(UINT level){m_uiLevel = level;}
	void setInterfaceSort(BOOL sort){m_bInterfaceSort = sort;}
	void setProj(bool bProj){m_bProj = bProj;}							//!< 配置自己是不是工程中的设备
	void SetModify(){m_bModify = true;}									//!< 是否参数被修改了

	CString GetTypeName();												//!< 获得设备类型名称
	void SetSelect(bool select){m_bSelect = select;}
	void SetExpand(bool expand){m_bExpand = expand;}
	void SetShow(bool show){m_bShow = show;}
	void SetLTPt(LONG x, LONG y){m_Point.x = x; m_Point.y = y;}
	CPoint GetLTPt(){return m_Point;}

	boost::shared_ptr<XmlInfo::CXmlDevice> GetXmlInfo(){return m_spXmlDevice;}

protected:
	virtual void OnDrawFrame(CDC* pDC);		//!< 画设备的边框
	virtual void OnDrawWord(CDC* pDC);		//!< 画文字部分
	virtual void OnDrawPic(CDC* pDC);		//!< 画图片
	virtual void OnDrawToParent(CDC* pDC);	//!< 画设备
	virtual void OnDrawToHostLine(CDC* pDC);//!< 画到母线的连线

public:
	CDeviceOne();
	~CDeviceOne(void);
	virtual void OnAddNew();				//!< 作为新设备添加到拓扑中的事件
	virtual bool InitDevType(UINT devType);	//!< 新增的设备要初始化，这个很重要
	virtual void ResizeInterface(UINT num);	//!< 重新定义接口数组的元素数量
	virtual void setName(CString name);

	virtual int IsInMyRect(CPoint point);	//!< 判断坐标是否在区域范围内,0表示没选中，1表示选中了，2表示选中了加减号
	virtual void OnSetLevel(UINT level);	//!< 递归设置一下设备的级别
	virtual void OnCountLevel();			//!< 计算自己所在ZigBee结构中的层次
	virtual void OnCountPoint();			//!< 计算自己坐在的坐标
	virtual void OnDraw(CDC* pDC);			//!< 画设备
	virtual void ShowHideChild(bool show);	//!< 显示或隐藏在自己和孩子
	virtual void OnRButtonUp(UINT nFlags, CPoint point, CWnd* wnd);		//!< 对设备右键

	virtual bool SerializeXmlGeneral(TiXmlElement* pNode, bool bRead);	//!< 解析设备概要信息，从拓扑文件中解析
	virtual bool LoadXmlFile();
	virtual bool SaveXmlFile();				//!< 保存文件的地址是相对地址 + 工程目录的地址

	virtual bool DevOut(CString path, CString name);					//!< 导出设备配置
	virtual bool DevIn(CString path, CString name);						//!< 导入设备配置

	virtual void DeleteChild(UINT id);		//!< 删除这个孩子
	virtual void AddChild(UINT id);			//!< 添加孩子
	virtual void OnCopy();					//!< 被复制了
	virtual void OnCopyWithChild();			//!< 被复制了，自己的所有孩子也被复制了
	virtual void OnCut(CDeviceMapDoc* pDoc);//!< 被剪切了，孩子也得被剪切
	virtual void OnPaste();					//!< 被粘贴，将自己放到拓扑中

	virtual boost::shared_ptr<CDeviceInterface> GetInterface(UINT id);	//!< 获得第id个接口
	virtual boost::shared_ptr<CDeviceInterface> GetInterfaceFromType(UINT type);	//!< 根据接口的类型找到第一个属于这个类型的接口
	virtual boost::shared_ptr<CDeviceParam> GetParam(UINT id);						//!< 找参数

	virtual long GetOnLineInf();										//!< 设备是否在线，返回在线的接口ID，不在返回-1
	virtual void SetOnLineInf(UINT infID, bool bOnLine);				//!< 设置设备在线状态，参数是接口ID
	virtual void SetOffLine();											//!< 直接设置下线

	virtual void GetWatchInfo(Tool::TooltipText& toolTip);				//!< 获得监控时的信息
	virtual void LoadXml();												//!< 解析xml文档

	virtual bool IsMyParent(UINT id);									//!< id设备是否是自己的上代
	virtual bool CanAddSlave(bool bSayWhy = true);						//!< 判断是否能连从设备，如果不能连说明原因
	virtual bool CanAddSlaveType(UINT devType);							//!< 判断是否能连该类型的从设备

	virtual void UpLoadBehavior(UINT id, CComVariant cvrIndex);			//!< 上载一个行为的参数，和组索引
	virtual void DownLoadBehavior(UINT id, CComVariant cvrIndex);		//!< 下载一个行为的参数，和组索引

	virtual void CopyFrom(CDeviceOne& device);
	virtual bool operator == (CDeviceOne& device) const;				//!< 判断两个设备是否一样
	virtual bool DoSearch(CString str, bool bMatchWhole, bool bAllCase, bool bRegex = false);//!< 查找本变量，要匹配的文本，全字符匹配，大小写匹配

	void GetWholeAddr(std::list<UINT>& ltAddr, bool bOnLine = true);	//!< 获得根设备的接口类型，bOnLine表示是否需要第一个在线的，返回-1表示没有
	int GetInterfaceSize();												//!< 获得可容纳的接口数量

	void OnProgram();													//!< 启动对其编程

protected:
	virtual bool IsInExpandRect(CPoint point);							//!< 判断坐标是否在伸缩区域范围内
	virtual bool SerializeXml(TiXmlElement* pNode, bool bRead);			//!< 解析设备信息，从设备单个文件中解析
};
}
}
