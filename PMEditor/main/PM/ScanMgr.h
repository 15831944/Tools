#pragma once
#include "DeviceInterface.h"

namespace MVC{namespace Device{
class CDeviceOne;
//!< 扫描的接口类，要过记录一些信息
class CScanInterface : public CDeviceInterface
{
public:
	UINT m_uiLevel;					//!< 自己所在级别
	CString  m_uiStrParentID;		//!< 父设备的编号
	CScanInterface(void);
	~CScanInterface(void){;}
};
class CScanMgr
{
	friend class CDevMgr;
private:
	std::list<boost::shared_ptr<CDeviceOne> > m_ltScanDev;
	char m_cOldData[64000];
	UINT m_uiOldNum;

private:
	CScanMgr(void);
	~CScanMgr(void);
	static CScanMgr& GetMe();

	template <class T>
	bool ReadData(T& t, int& index, const char* data, int size = 0, const int maxsize = -1);	//!< 从data的index位置上读size大小的信息放到t中

	bool OnCheckDev();			//!< 检查返回的数据的正确性
	void ReBuildData();			//!< 重建数据关系
	void ReBuildOneParent();	//!< 重建数据关系，保证每个设备都最多只有一个父设备
	void ReBuildMain();			//!< 重建数据关系，将原设备信息以树形的结构前序遍历的循序重新排布
	void ReBuildSlave(boost::shared_ptr<CDeviceOne> device, std::list<boost::shared_ptr<CDeviceOne> >& ltDevice);	//!< 重建从设备关系
	void OnConnectToProj();		//!< 连接到工程中
	void OnBuildConnect();		//!< 重建CDevMgr里的设备的链接关系
	bool SetOnLine(boost::shared_ptr<CDeviceOne> scanDev, boost::shared_ptr<CDeviceOne> projDev);					//!< 设置工程设备上线
	int IsSameDevice(boost::shared_ptr<CDeviceOne> scanDev, boost::shared_ptr<CDeviceOne> projDev);	//!< 是否是同一个设备
	bool IsSameParent(boost::shared_ptr<CDeviceOne> scanDev, boost::shared_ptr<CDeviceOne> projDev);//!< 是否有同一个父亲
	boost::shared_ptr<CDeviceOne> GetDevice(CString strDevID);				//!< 从扫描设备列表里，找到该设备
	boost::shared_ptr<CDeviceOne> GetDeviceFromProj(CString strDevID);		//!< 从工程设备列表里，找到该设备
	UINT GetDeviceCount(CString strDevID);									//!< 找到该strDevID的设备的数量
	void DelOffLineScanDev();	//!< 删除掉所有不在线的工程中的扫描设备

public:
	void ClearData(){memset(m_cOldData,0,64000);m_uiOldNum = 0;}			//!< 清空数据
	void OnScanRev(const char* data, const int num);
	void DelAllScanDev();		//!< 删除所有扫描设备
};
}
}
