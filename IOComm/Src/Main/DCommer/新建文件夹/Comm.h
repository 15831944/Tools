#pragma once
#include "MyTimer.h"
#define WM_COMM_RXCHAR				WM_USER+7	// A character was received and placed in the input buffer. 

namespace Comm{
class IComm;
enum{
	NET_UDP = 0,				//!< ����UDPͨ�ŵı�ʶ
	NET_UDP_WATCH = 1,			//!< ����UDP��صı�ʶ
	NET_TCP = 2,				//!< ����TCPͨ�ŵı�ʶ
	NET_SERIAL = 3,				//!< ���ڴ���ͨ�ŵı�ʶ
	NET_DEF_OVERTIME = 1000,	//!< Ĭ�ϵĳ�ʱʱ����1000����
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
	bool m_bNeedRet;			//!< ��Ҫ����
	byte* m_byData;				//!< ����
	int m_nLen;					//!< ���ݳ���
	int m_nTime;				//!< ��ʱʱ�䣬��λ�ٺ���
	ICommListener* m_nListener;	//!< ������
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
	CString GetInfo(){return m_strInfo;}				//!< �����Ϣ
	virtual bool WatchData(ICommListener* listener){return true;}	//!< ��������
	virtual void DisWatch(ICommListener* listener){;}	//!< ȡ������
	virtual bool Send(CData& data, CString strInfo)		//!< ���÷��ͷ�ʽ����������
	{if(!SetInfo(strInfo))	return false;	return Send(data);}

public:
	virtual bool SetInfo(CString info) = 0;				//!< ������Ϣ
	virtual bool Send(CData& data) = 0;					//!< ��������
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

//boost::shared_ptr<IComm> CreateComm(int type);			//!< 0-UDP,1UDPWatch,2Serial����
void aabbbcccc(){;}
}