#pragma once
#include "Comm.h"
#include "SerialPortBoost.h"

namespace Comm{
class CData;
class CNetSerialBoost : public IComm
{
private:
	bool m_bBusy;		//!< 是否正处于通信中
	int m_nComId;		//!< COM口号，0表示COM1，依此类推
	int m_nBaud;		//!< 波特率，0表示1200，9表示256000
	int m_nCheck;		//!< 校验方式，0-无校验-N；1-奇校验-O；2-偶校验-E；3-0校验-S；4-1校验-M
	int m_nDataBit;		//!< 数据位
	int m_nStopBit;		//!< 停止位
	CSerialPortBoost m_SerialPortBoost;
	CData* m_SendData;
	std::deque<CData*> m_dqSendData;
	std::list<ICommListener*> m_ltListener;		//!< 监听者链表

private:
	virtual void OnReceive(byte* data, int len);//!< 收到数据
	//bool IsBusy(){return !!m_SendData;}
	bool IsBusy(){if(m_SendData || !m_dqSendData.empty())	return true;	return false;}
	void EmptyData();
	void OnSend();
	void SetBusy(bool bBusy = true){m_bBusy = bBusy;}

public:
	CNetSerialBoost(void);
	~CNetSerialBoost(void);

	bool Send(bool needRet,	byte* data, int nLen, int nTime, ICommListener* pListener);						//!< 发送数据
	virtual bool StartCom();					//!< 开始通信
	virtual bool StopCom();						//!< 停止通信
	virtual bool SetInfo(CString info);			//!< 设置信息，例如“COM1:115200:无校验:8:1”
	virtual bool WatchData(ICommListener* listener);	//!< 监听数据
	virtual void DisWatch(ICommListener* listener);		//!< 取消监听
	virtual void OnDTimer(int nID);				//!< 时间到了，nMilSec表示距离上一次的时间报告，经过了多少个毫秒
};
}
