#pragma once
#include "Comm.h"
#include "SerialPortBoost.h"

namespace Comm{
class CData;
class CNetSerialBoost : public IComm
{
private:
	bool m_bBusy;		//!< �Ƿ�������ͨ����
	int m_nComId;		//!< COM�ںţ�0��ʾCOM1����������
	int m_nBaud;		//!< �����ʣ�0��ʾ1200��9��ʾ256000
	int m_nCheck;		//!< У�鷽ʽ��0-��У��-N��1-��У��-O��2-żУ��-E��3-0У��-S��4-1У��-M
	int m_nDataBit;		//!< ����λ
	int m_nStopBit;		//!< ֹͣλ
	CSerialPortBoost m_SerialPortBoost;
	CData* m_SendData;
	std::deque<CData*> m_dqSendData;
	std::list<ICommListener*> m_ltListener;		//!< ����������

private:
	virtual void OnReceive(byte* data, int len);//!< �յ�����
	//bool IsBusy(){return !!m_SendData;}
	bool IsBusy(){if(m_SendData || !m_dqSendData.empty())	return true;	return false;}
	void EmptyData();
	void OnSend();
	void SetBusy(bool bBusy = true){m_bBusy = bBusy;}

public:
	CNetSerialBoost(void);
	~CNetSerialBoost(void);

	bool Send(bool needRet,	byte* data, int nLen, int nTime, ICommListener* pListener);						//!< ��������
	virtual bool StartCom();					//!< ��ʼͨ��
	virtual bool StopCom();						//!< ֹͣͨ��
	virtual bool SetInfo(CString info);			//!< ������Ϣ�����硰COM1:115200:��У��:8:1��
	virtual bool WatchData(ICommListener* listener);	//!< ��������
	virtual void DisWatch(ICommListener* listener);		//!< ȡ������
	virtual void OnDTimer(int nID);				//!< ʱ�䵽�ˣ�nMilSec��ʾ������һ�ε�ʱ�䱨�棬�����˶��ٸ�����
};
}
