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
	NET_UDP = 0,				//!< ����UDPͨ�ŵı�ʶ
	NET_UDP_WATCH = 1,			//!< ����UDP��صı�ʶ
	NET_SERIAL = 2,				//!< ���ڴ���ͨ�ŵı�ʶ
	NET_TCPCLIENT = 3,			//!< ����TCPͨ�ŵı�ʶ
	NET_SERIALBOOST = 4,		//!< ���ڴ���ͨ�ŵı�ʶBoostͨ�Ŵ���
	NET_DEF_OVERTIME = 1000,	//!< Ĭ�ϵĳ�ʱʱ����1000����
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
	IComm* CreateComm(int type = NET_UDP);			//!< 0-UDP,1UDPWatch,2Serial����
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
	CString GetInfo(){return m_strInfo;}				//!< �����Ϣ
	virtual bool WatchData(ICommListener* listener){return true;}	//!< ��������
	virtual void DisWatch(ICommListener* listener){;}	//!< ȡ������
	virtual bool Send(bool needRet,	byte* data, int nLen, int nTime, ICommListener* pListener, CString strInfo)		//!< ���÷��ͷ�ʽ����������
	{if(!SetInfo(strInfo))	return false;	return Send(needRet, data, nLen, nTime, pListener);}

public:
	virtual bool SetInfo(CString info) = 0;				//!< ������Ϣ
//	virtual bool Send(CData& data) = 0;					//!< ��������
	virtual bool Send(bool needRet,	byte* data, int nLen, int nTime, ICommListener* pListener) = 0;		//!< ��������
	virtual bool StartCom() = 0;						//!< ��ʼͨ��
	virtual bool StopCom() = 0;							//!< ֹͣͨ��
	virtual void OnDTimer(int nID){;}					//!< ʱ�䵽�ˣ�nMilSec��ʾ������һ�ε�ʱ�䱨�棬�����˶��ٸ�����

	DECLARE_DYNAMIC(IComm)
	DECLARE_MESSAGE_MAP()
protected:
	bool CreateWnd();
	virtual void OnReceive(byte* data, int len){;}
	afx_msg LRESULT Receive(WPARAM pch, LPARAM number);
};
}

