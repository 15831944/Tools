#pragma once
#include "Comm.h"

namespace Comm{
class CNetUDPWatch : public IComm, public CSocket
{
private:
	WORD m_wPort;		//!< �����˿ں�
	bool m_bStarted;	//!< �Ƿ������˼��
	std::list<ICommListener*> m_ltListener;

	virtual void OnReceive(int nErrorCode);				//!< �յ�����

public:
	CNetUDPWatch(void);
	~CNetUDPWatch(void){;}

	virtual bool Send(bool needRet, byte* data, int nLen, int nTime, ICommListener* pListener){return false;}		//!< ��������
	virtual bool StartCom(){return false;}				//!< ��ʼͨ��
	virtual bool StopCom();								//!< ֹͣͨ��
	virtual bool SetInfo(CString info);					//!< ������Ϣ�����硰192.168.1.172:9866��
	virtual bool WatchData(ICommListener* listener);	//!< ��������
	virtual void DisWatch(ICommListener* listener);		//!< ȡ������
};
}