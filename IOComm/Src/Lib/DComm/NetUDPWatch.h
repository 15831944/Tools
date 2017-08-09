#pragma once
#include "Comm.h"

namespace Comm{
class CNetUDPWatch : public IComm, public CSocket
{
private:
	WORD m_wPort;		//!< 监听端口号
	bool m_bStarted;	//!< 是否启动了监控
	std::list<ICommListener*> m_ltListener;

	virtual void OnReceive(int nErrorCode);				//!< 收到数据

public:
	CNetUDPWatch(void);
	~CNetUDPWatch(void){;}

	virtual bool Send(bool needRet, byte* data, int nLen, int nTime, ICommListener* pListener){return false;}		//!< 发送数据
	virtual bool StartCom(){return false;}				//!< 开始通信
	virtual bool StopCom();								//!< 停止通信
	virtual bool SetInfo(CString info);					//!< 设置信息，例如“192.168.1.172:9866”
	virtual bool WatchData(ICommListener* listener);	//!< 监听数据
	virtual void DisWatch(ICommListener* listener);		//!< 取消监听
};
}