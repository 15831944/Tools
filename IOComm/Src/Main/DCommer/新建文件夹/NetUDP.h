#pragma once
#include "Comm.h"

namespace Comm{
class CNetUDP : public CSocket, public IComm
{
private:
	int m_nIP;			//!< ���͵���IP��ַ
	CString m_strIP;	//!< ���͵���IP��ַ
	WORD m_wPort;		//!< ���Ͷ˿ں�
	bool m_bBusy;		//!< �Ƿ�������ͨ����
	boost::shared_ptr<CData> m_SendData;
	std::deque<boost::shared_ptr<CData> > m_dqSendData;
	std::list<ICommListener*> m_ltListener;		//!< ����������

private:
	bool IsBusy(){return !!m_SendData;}
	void EmptyData();
	void OnSend();
	void SetBusy(bool bBusy = true){m_bBusy = bBusy;}
	virtual void OnReceive(int nErrorCode);		//!< �յ�����

public:
	CNetUDP(void);
	~CNetUDP(void);

	bool Send(CData& data);						//!< ��������
	virtual bool StartCom();					//!< ��ʼͨ��
	virtual bool StopCom();						//!< ֹͣͨ��
	virtual bool SetInfo(CString info);			//!< ������Ϣ�����硰192.168.1.172:9866��
	virtual bool WatchData(ICommListener* listener);	//!< ��������
	virtual void DisWatch(ICommListener* listener);		//!< ȡ������
	virtual void OnDTimer(int nID);				//!< ʱ�䵽�ˣ�nMilSec��ʾ������һ�ε�ʱ�䱨�棬�����˶��ٸ�����
};
}
