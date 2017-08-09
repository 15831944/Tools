#pragma once
#include "MyTimer.h"
#define WM_COMM_RXCHAR				WM_USER+7	// A character was received and placed in the input buffer. 

namespace Comm{
class IComm;
enum{
	NET_UDP = 0,				//!< 用于UDP通信的标识
	NET_UDP_WATCH = 1,			//!< 用于UDP监控的标识
	NET_TCP = 2,				//!< 用于TCP通信的标识
	NET_SERIAL = 3,				//!< 用于串口通信的标识
	NET_DEF_OVERTIME = 1000,	//!< 默认的超时时间是1000毫秒
	MAX_UDP_PACKAGE_LENGTH = 8192,
};
class ICommListener
{
public:
	ICommListener(void){;}
	virtual ~ICommListener(void){;}
	virtual void OnReceive(IComm* commer, byte* data, int len, CString info = _T("")) = 0;
	virtual void OnOverTime(IComm* commer){;}
};

class CData
{
public:
	bool m_bNeedRet;			//!< 需要返回
	byte* m_byData;				//!< 数据
	int m_nLen;					//!< 数据长度
	int m_nTime;				//!< 超时时间，单位百毫秒
	ICommListener* m_nListener;	//!< 接收者
	CData(int nLen):m_bNeedRet(false),m_nLen(nLen),m_nTime(-1),m_nListener(NULL){if(m_nLen > 0)	m_byData = new byte[m_nLen];}
	~CData(){if(m_nLen > 0 && m_byData != NULL)	{delete[] m_byData; m_byData = NULL;}}
	void CopyWith(CData& src)
	{
		m_bNeedRet = src.m_bNeedRet;
		m_nLen = src.m_nLen;
		m_nTime = src.m_nTime;
		m_nListener = src.m_nListener;
		memcpy(m_byData, src.m_byData, m_nLen);
	}
	void Init(bool bNeedRet, int nTime, ICommListener& listener, byte* data)
	{
		m_bNeedRet = bNeedRet;
		m_nTime = nTime;
		m_nListener = &listener;
		if(m_nLen > 0)	memcpy(m_byData, data, m_nLen);
	}
};

class IComm : public CWnd, public Timer::ITimerListener
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
	virtual bool Send(CData& data, CString strInfo)		//!< 设置发送方式并发送数据
	{if(!SetInfo(strInfo))	return false;	return Send(data);}

public:
	virtual bool SetInfo(CString info) = 0;				//!< 设置信息
	virtual bool Send(CData& data) = 0;					//!< 发送数据
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

//boost::shared_ptr<IComm> CreateComm(int type);			//!< 0-UDP,1UDPWatch,2Serial……
void aabbbcccc(){;}
}