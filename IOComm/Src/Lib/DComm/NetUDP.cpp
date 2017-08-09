#include "Stdafx.h"
#include "Gbl.h"
#include "NetUDP.h"
#include "Data.h"
#include <afxmt.h>

using namespace Comm;
CNetUDP::CNetUDP(void)
:m_nIP(-1)			//!< 发送到的IP地址
,m_wPort(0)			//!< 发送端口号
,m_bBusy(false)		//!< 是否正处于通信中
,m_SendData(NULL)
{
	((CSocket *)this)->Create(NULL, SOCK_DGRAM);
}

CNetUDP::~CNetUDP(void)
{
	KillDTimer();
	m_ltListener.clear();
	foreach(CData* cd, m_dqSendData)		delete cd;
	m_dqSendData.clear();
}

//!< 发送数据
bool CNetUDP::Send(bool needRet, byte* data, int nLen, int nTime, ICommListener* pListener)
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

void CNetUDP::OnSend()
{
	if(m_dqSendData.empty())	return;
	if(m_SendData)				delete m_SendData;
	m_SendData = m_dqSendData.front();
	m_dqSendData.pop_front();
	SetBusy();
	SendTo((char *)m_SendData->m_byData, m_SendData->m_nLen, m_wPort, m_strIP);
	SetDTimer(m_SendData->m_nTime, 0, true);
}

//!< 开始通信
bool CNetUDP::StartCom()
{
	return true;
//	return !!CSocket::Bind(m_wPort, m_strIP);
//	if(!m_SerialPort)	m_SerialPort = boost::shared_ptr<CSerialPort>(new CSerialPort);
//	if(!m_SerialPort->InitPort(this, m_nComId + 1, VT_BAUD_VAL[m_nBaud], VT_PARITY[m_nCheck],
//		VT_DATA_VAL[m_nDataBit], VT_STOP_VAL[m_nStopBit], EV_RXFLAG | EV_RXCHAR))
//	{	m_strInfo = _T("");	return false;}
//	return (bool)m_SerialPort->StartMonitoring();
}

//!< 停止通信
bool CNetUDP::StopCom()
{
	m_strInfo = _T("");
	return !!Bind(0);
}

//!< 设置信息
bool CNetUDP::SetInfo(CString info)
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

//!< 监听数据
bool CNetUDP::WatchData(ICommListener* listener)
{
	if(!listener)		return false;
	m_ltListener.remove(listener);
	m_ltListener.push_back(listener);
	return true;
}

//!< 取消监听
void CNetUDP::DisWatch(ICommListener* listener)
{
	m_ltListener.remove(listener);
}

//!< 收到数据
void CNetUDP::OnReceive(int nErrorCode)
{
	CString strIP, str;
	UINT uiPort;
	char data[MAX_UDP_PACKAGE_LENGTH];
	int len = ReceiveFrom(data, MAX_UDP_PACKAGE_LENGTH, strIP, uiPort);
	if(len < 0)		return;
	CString strInfo;
	strInfo.Format("ip:"+strIP+" port:%d", uiPort);

	if(m_SendData && m_SendData->m_bNeedRet)	m_SendData->m_nListener->OnReceive(this, (byte *)data, len, strInfo);
	ICommListener* listener;
	foreach(listener, m_ltListener)
	{
		if(m_SendData)
		{
			if(listener != m_SendData->m_nListener || !m_SendData->m_bNeedRet)	//!< 如果已经回过了就不用再回了
				listener->OnReceive(this, (byte *)data, len, strInfo);
		}
		else	listener->OnReceive(this, (byte *)data, len, strInfo);
	}
	KillDTimer();
	EmptyData();

	CSocket::OnReceive(nErrorCode);
}

//!< 时间到了，nMilSec表示距离上一次的时间报告，经过了多少个毫秒
void CNetUDP::OnDTimer(int nID)
{
	if(nID == 0){
		if(!m_SendData)		return;
		m_SendData->m_nListener->OnOverTime(this);
		EmptyData();
	}
}

void CNetUDP::EmptyData()
{
	if(m_SendData)			delete m_SendData;
	m_SendData = NULL;
	SetBusy(false);
	OnSend();
}
