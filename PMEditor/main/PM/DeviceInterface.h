#pragma once

namespace MVC{
namespace Device{
class CDeviceOne;
class CDeviceInterface
{
public:
	enum Inf_Color{
		INF_ETHERNET = RGB(0, 0, 255),
		INF_SERIAL = RGB(160, 40, 0),
		INF_SLAVE = RGB(150, 150, 40)
	};
private:
	bool m_bProj;					//!< 是否是工程的接口
	int m_nState;					//!< 在线状态，0离线，1在线，2在线未配置(只适用于从设备)
	CDeviceOne* m_pDevice;
	UINT m_uiID;					//!< 接口的编号
	UINT m_uiType;					//!< 类型，0以太网，1串口，2ZigBee，16从设备
	UINT m_uiSerialNo;				//!< 串口号，0代表COM1，以后依此类推
//	UINT m_uiIP;					//!< IP地址
	int	 m_uiDevPort;					//!<设备端口号
	UINT m_uiHandle;				//!< 串口地址，如果是主设备就是模块地址，如果是从设备就是Handle号

	CString m_strDoMain;  //!<设备的域名

public:
	CDeviceInterface(CDeviceOne* pDevice);
	~CDeviceInterface(void){;}

	bool IsOnline(){return m_nState == 1;}
	int GetState(){return m_nState;}
	CDeviceOne* GetDevice(){return m_pDevice;}
	UINT GetID(){return m_uiID;}
	UINT GetType(){return m_uiType;}
	UINT GetSerialNo(){return m_uiSerialNo;}
//	UINT GetIP(){return m_uiIP;}
	CString GetDoMain(){return m_strDoMain;}
	UINT GetHandle(){return m_uiHandle;}
	UINT GetDevPort(){return m_uiDevPort;}
	bool IsProj(){return m_bProj;}

	void SetState(int nState){m_nState = nState % 3;}
	void SetID(UINT id){m_uiID = id;}
	void SetType(UINT type){m_uiType = type;}
	void SetSerialNo(UINT no){m_uiSerialNo = no;}
//	void SetIP(UINT ip){m_uiIP = ip;}
	void SetDoMain(CString domain){m_strDoMain=domain;}
	void SetHandle(UINT handle){m_uiHandle = handle;}
	void SetProj(bool b = true){m_bProj = b;}
	void SetDevPort(UINT DevPort){m_uiDevPort=DevPort;}

public:
	CDeviceInterface& operator = (CDeviceInterface& inf);
	bool operator != (CDeviceInterface& inf) const;
	CString GetName(bool bShowScan = true);
	CString GetTypeName();
	bool SerializeXml(TiXmlElement* pNode, bool bRead);
	COLORREF GetInfColor();			//!< 获得线的颜色
};
}
}
