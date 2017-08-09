#include "Stdafx.h"
#include "Gbl.h"
#include "NetTCP.h"
#include "Data.h"
#include <afxmt.h>

using namespace Comm;
CNetTCP::CNetTCP(void)
:m_nIP(-1)			//!< ���͵���IP��ַ
,m_wPort(0)			//!< ���Ͷ˿ں�
,m_bBusy(false)		//!< �Ƿ�������ͨ����
,m_SendData(NULL)
,m_bConnect(false)	//!< ��ǰ����״̬
{
	((CSocket *)this)->Create(NULL, SOCK_STREAM);
}

CNetTCP::~CNetTCP(void)
{
	KillDTimer();
	m_ltListener.clear();
	foreach(CData* cd, m_dqSendData)		delete cd;
	m_dqSendData.clear();
}

//!< ��������
bool CNetTCP::Send(bool needRet, byte* data, int nLen, int nTime, ICommListener* pListener)
{
	CData* send_data = new CData(nLen);
	send_data->m_bNeedRet = needRet;
	send_data->m_nLen = nLen;
	send_data->m_nTime = nTime;
	send_data->m_nListener = pListener;
	memcpy(send_data->m_byData, data, nLen);
	m_dqSendData.push_back(send_data);
	if(!IsBusy())				OnSend();
	return true;
}

void CNetTCP::OnSend()
{
	if(m_dqSendData.empty())	return;
	if(!m_bConnect)				{ASSERT(FALSE);	return;}
	if(m_SendData)				delete m_SendData;
	m_SendData = m_dqSendData.front();
	m_dqSendData.pop_front();
	SetBusy();
//	SendTo((char *)m_SendData->m_byData, m_SendData->m_nLen, m_wPort, m_strIP);
	CSocket::Send((void *)m_SendData->m_byData, m_SendData->m_nLen);
	SetDTimer(m_SendData->m_nTime, 0, true);
}

//!< ��ʼͨ��
bool CNetTCP::StartCom()
{
	return true;
	//	return !!CSocket::Bind(m_wPort, m_strIP);
	//	if(!m_SerialPort)	m_SerialPort = boost::shared_ptr<CSerialPort>(new CSerialPort);
	//	if(!m_SerialPort->InitPort(this, m_nComId + 1, VT_BAUD_VAL[m_nBaud], VT_PARITY[m_nCheck],
	//		VT_DATA_VAL[m_nDataBit], VT_STOP_VAL[m_nStopBit], EV_RXFLAG | EV_RXCHAR))
	//	{	m_strInfo = _T("");	return false;}
	//	return (bool)m_SerialPort->StartMonitoring();
}

//!< ֹͣͨ��
bool CNetTCP::StopCom()
{
	m_strInfo = _T("");
	return !!Bind(0);
}

//!< ������Ϣ
bool CNetTCP::SetInfo(CString info)
{
	if(info == GetInfo())						return true;
	UINT ip = UINT(-1);
	std::vector<CString> vtStr;
	CGbl::SpliteBy(info, ":", vtStr);
	if(vtStr.size() != 2)						return false;
	if(!CGbl::GetNumberFromIP(vtStr[0], ip))	return false;
	m_nIP = (int)ip;
	m_wPort = (WORD)atoi(vtStr[1]);
	m_strIP = vtStr[0];
	IComm::SetInfo(info);
	return true;
}

//!< ��������
bool CNetTCP::WatchData(ICommListener* listener)
{
	if(!listener)		return false;
	m_ltListener.remove(listener);
	m_ltListener.push_back(listener);
	if(m_ltListener.size() == 1){
		if(m_bConnect)	{Close();		return m_bConnect;}
		m_bConnect = Connect(m_strIP, m_wPort);
	}
	return m_bConnect;
}

//!< ȡ������
void CNetTCP::DisWatch(ICommListener* listener)
{
	m_ltListener.remove(listener);
	if(!m_ltListener.empty())		return;
	Close();
}

//!< �յ�����
void CNetTCP::OnReceive(int nErrorCode)
{
	CString strIP, str;
	UINT uiPort;
	char data[MAX_UDP_PACKAGE_LENGTH];
	int len = ReceiveFrom(data, MAX_UDP_PACKAGE_LENGTH, strIP, uiPort);
	if(len < 0)		return;

	if(m_SendData && m_SendData->m_bNeedRet)	m_SendData->m_nListener->OnReceive(this, (byte *)data, len);
	ICommListener* listener;
	foreach(listener, m_ltListener)
	{
		if(m_SendData)
		{
			if(listener != m_SendData->m_nListener || !m_SendData->m_bNeedRet)	//!< ����Ѿ��ع��˾Ͳ����ٻ���
				listener->OnReceive(this, (byte *)data, len);
		}
		else	listener->OnReceive(this, (byte *)data, len);
	}
	KillDTimer();
	EmptyData();

	CSocket::OnReceive(nErrorCode);
}

//!< ���ӶϿ�
void CNetTCP::OnClose(int nErrorCode)
{
	if(!m_ltListener.empty())
	{
		m_bConnect = Connect(m_strIP, m_wPort);
		return;
	}
	m_bConnect = false;
}

//!< ʱ�䵽�ˣ�nMilSec��ʾ������һ�ε�ʱ�䱨�棬�����˶��ٸ�����
void CNetTCP::OnDTimer(int nID)
{
	if(nID == 0){
		if(!m_SendData)		return;
		m_SendData->m_nListener->OnOverTime(this);
		EmptyData();
	}
}

void CNetTCP::EmptyData()
{
	if(m_SendData)			delete m_SendData;
	m_SendData = NULL;
	SetBusy(false);
	OnSend();
}
