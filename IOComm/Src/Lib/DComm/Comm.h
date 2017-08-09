#pragma once
#include "MyTimer.h"
#define WM_COMM_RXCHAR				WM_USER+7	// A character was received and placed in the input buffer. 

// #ifdef _USRDLL
// #define RWD_API __declspec(dllexport)
// #else
// #define RWD_API __declspec(dllimport)
// #endif

namespace Comm{
class IComm;
enum{
	NET_UDP = 0,				//!< 用于UDP通信的标识
	NET_UDP_WATCH = 1,			//!< 用于UDP监控的标识
	NET_SERIAL = 2,				//!< 用于串口通信的标识
	NET_TCPCLIENT = 3,			//!< 用于TCP通信的标识
	NET_SERIALBOOST = 4,		//!< 用于串口通信的标识Boost通信代码
	NET_DEF_OVERTIME = 1000,	//!< 默认的超时时间是1000毫秒
	MAX_UDP_PACKAGE_LENGTH = 8192,
};

class /*RWD_API*/ ICommListener
{
	std::list<IComm*> m_ltCommForNow;

public:
	ICommListener(void){;}
	virtual ~ICommListener(void);
	virtual void OnReceive(IComm* commer, byte* data, int len, CString info = _T("")) = 0;
	virtual void OnOverTime(IComm* commer){;}
	IComm* CreateComm(int type = NET_UDP);			//!< 0-UDP,1UDPWatch,2Serial……
	void ReMoveComm(IComm* comm);
};

class /*RWD_API*/ IComm : public CWnd, public Timer::ITimerListener
{
protected:
	int m_nID;
	CString m_strSetting;
	CString m_strInfo;

public:
	IComm():m_nID(-1){;}
	virtual ~IComm(){;}
	CString GetInfo(){return m_strInfo;}				//!< 获得信息
	virtual bool WatchData(ICommListener* listener){return true;}	//!< 监听数据
	virtual void DisWatch(ICommListener* listener){;}	//!< 取消监听
	virtual bool Send(bool needRet,	byte* data, int nLen, int nTime, ICommListener* pListener, CString strInfo)		//!< 设置发送方式并发送数据
	{if(!SetInfo(strInfo))	return false;	return Send(needRet, data, nLen, nTime, pListener);}

public:
	virtual bool SetInfo(CString info) = 0;				//!< 设置信息
//	virtual bool Send(CData& data) = 0;					//!< 发送数据
	virtual bool Send(bool needRet,	byte* data, int nLen, int nTime, ICommListener* pListener) = 0;		//!< 发送数据
	virtual bool StartCom() = 0;						//!< 开始通信
	virtual bool StopCom() = 0;							//!< 停止通信
	virtual void OnDTimer(int nID){;}					//!< 时间到了，nMilSec表示距离上一次的时间报告，经过了多少个毫秒

	DECLARE_DYNAMIC(IComm)
	DECLARE_MESSAGE_MAP()
protected:
	bool CreateWnd();
	virtual void OnReceive(byte* data, int len){;}
	afx_msg LRESULT Receive(WPARAM pch, LPARAM number);
};
}

