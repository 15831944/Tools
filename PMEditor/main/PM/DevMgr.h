#pragma once
#include "PropertyDevice.h"
#include "DEthernet.h"

namespace MVC{
namespace Device{
class CDeviceOne;
class CDeviceMapDoc;
namespace InterfaceSet{class CDSerial;}

typedef struct tagBehaviorRequest{
	virtual void OnDataReady() = 0;							//!< 数据已经刷新好了
	virtual void OnDeviceChange() = 0;						//!< 当所有有所改变时
}SBehaviorRequest;

class CDevMgr
{
public:
	enum{							//!< Modbus设备需要特殊处理,唉
		SUPPORT_DNS = 1,			// 是否显示设备域名
		MODBUS_SERIAL = 7,
		MODBUS_UDP = 8,
		MODBUS_TCP = 9
	};
	CDeviceMapDoc* m_DevMapDoc;
	std::vector<std::shared_ptr<CDeviceOne> > m_vtDevice;
	std::list<std::shared_ptr<CDeviceOne> > m_ltDevClipBoard;			//!< 设备的剪贴板
	std::list<std::shared_ptr<InterfaceSet::CDSerial> > m_ltSerial;
	CPropertyDevice m_DeviceProperty;

private:
	SYSTEMTIME m_EditTime;			//!< 最近修改的事件
	bool m_bModify;					//!< 是否被修改
	CString m_strName;
	CString m_strFileName;
	CString m_strVersion;
	UINT m_uiSerialNum;
	std::shared_ptr<InterfaceSet::CDEthernet> m_Ethernet;		//!< 以太网通信配置对象，目前只支持一个
	std::list<SBehaviorRequest *> m_ltBRequest;					//!< 发送请求的监听者

public:
	bool IsModify(){return m_bModify;}
	CString getName(){return m_strName;}
	CString getFileName(){return m_strFileName;}
	CString getVersion(){return m_strVersion;}

	void SetModify(){m_bModify = true;}

private:
	CDevMgr(void);
	~CDevMgr(void);

	bool SerializeXml(TiXmlElement* pNode, bool bRead);			//!< 保存xml格式的编辑信息
	void AwayFromSameInf(std::shared_ptr<CDeviceOne> device);	//!< 保证设备拥有与别人不同的接口
	void MakeSureDfrtInf(std::shared_ptr<CDeviceOne> device);	//!< 保证设备拥有与别人不同的接口，如果相同则重新设置
	void OnDeleteDeviceChild(UINT id);							//!< 删除该设备的所有孩子，包括孩子的孩子，等等
	void InitComputerSerial();									//!< 初始化串口信息，把计算机扫描出来的串口强行加载进链表中

public:
	static CDevMgr& GetMe();
	CString GetEditTime();
	void OnCreate();
	void OnClose();
	void SaveFile();
	bool CheckAndConnect();										//!< 为设备之间建立连接
	void OpenDoc();
	void SetDevWatch(const bool bWatch);						//!< 设置监控状态
	std::shared_ptr<CDeviceOne> AddNewDevice(std::shared_ptr<CDeviceOne> device);
	std::shared_ptr<CDeviceOne> GetDevice(UINT id);
	std::shared_ptr<CDeviceOne> GetDevice(CString name);
	std::shared_ptr<CDeviceOne> GetDevice(UINT devType, UINT infType, UINT level, long* plAddr);
	std::shared_ptr<CDeviceOne> GetDeviceNameList(std::list<CString>& strList, int& defIndex);	//!< 获得所有设备的名称列表，返回defIndex号的设备
	std::shared_ptr<InterfaceSet::CDSerial> GetSerial(CString name);
	std::shared_ptr<InterfaceSet::CDEthernet> GetEthernet(){return m_Ethernet;}
	UINT GetDeviceSize();										//!< 获得设备的真实数量
	void DeleteDevice(UINT id);									//!< 删除设备，包括其子设备，不做任何处理
	void UndoDelDevice(UINT id);								//!< 删除设备，但要记录撤销信息
	void UndoUpdDevice(UINT id);								//!< 修改设备，但要记录撤销信息
	bool OpenDevMgrFile(CString name, CString pathall, CString ver, CString stime);					//!< 打开拓扑文件，参数 ： 名称 + 路径 + 版本号，最近修改时间
	void OnDevCopy(UINT id);									//!< 复制这个编号的设备到剪贴板
	void OnDevCopyWithChild(UINT id);							//!< 复制这个编号的设备以及他的所有从设备到剪贴板
	void OnDevCut(CDeviceMapDoc* pDoc, UINT id);				//!< 剪切这个编号的设备以及他的所有从设备到剪贴板
	void OnDevPaste(CDeviceMapDoc* pDoc, UINT id = UINT(-1));	//!< 将剪贴板里的设备粘贴到这个设备下面，如果不存在就直连工程师站
	int SearchDev(CString str, bool bMatchCase, bool bMatchWhole, bool bRegex = false);	//!< 查找符合该字符串的变量，返回匹配的数量
	void OnBehavior(long lBehaviorID, long lDeviceID, VARIANT& varValue, long lResult);

	void FreshView();											//!< 如果存在拓扑也，刷一下页面
	CString GetSimilarName(CString);							//!< 获得一个和它相近的名字

	void OnBehaviorOver();										//!< 当行为结束后要通知所有监听者
	void RegisteBRequest(SBehaviorRequest* one);				//!< 注册监听者
	void UnRegisteBRequest(SBehaviorRequest* one);				//!< 注销监听者

	int GetMaxOverTime(std::shared_ptr<CDeviceOne> device);	//!< 获得该设备所使用的最大的超时时间
};
}
}