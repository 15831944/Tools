#pragma once
#include "Comm.h"

namespace Comm{
class CData;
class CNetTCP : public CSocket, public IComm
{
private:
	bool m_bConnect;	//!< 当前连接状态
	int m_nIP;			//!< 发送到的IP地址
	CString m_strIP;	//!< 发送到的IP地址
	WORD m_wPort;		//!< 发送端口号
	bool m_bBusy;		//!< 是否正处于通信中
	CData* m_SendData;
	std::deque<CData*> m_dqSendData;
	std::list<ICommListener*> m_ltListener;		//!< 监听者链表

private:
	bool IsBusy(){return !!m_SendData;}
	void EmptyData();
	void OnSend();
	void SetBusy(bool bBusy = true){m_bBusy = bBusy;}
	virtual void OnReceive(int nErrorCode);		//!< 收到数据
	virtual void OnClose(int nErrorCode);		//!< 连接断开

public:
	CNetTCP(void);
	~CNetTCP(void);

	bool Send(bool needRet,	byte* data, int nLen, int nTime, ICommListener* pListener);	//!< 发送数据
	virtual bool StartCom();					//!< 开始通信
	virtual bool StopCom();						//!< 停止通信
	virtual bool SetInfo(CString info);			//!< 设置信息，例如“192.168.1.172:9866”
	virtual bool WatchData(ICommListener* listener);	//!< 监听数据
	virtual void DisWatch(ICommListener* listener);		//!< 取消监听
	virtual void OnDTimer(int nID);				//!< 时间到了，nMilSec表示距离上一次的时间报告，经过了多少个毫秒
};
}