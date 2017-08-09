#include "StdAfx.h"
#include "NetUDPWatch.h"

using namespace Comm;
CNetUDPWatch::CNetUDPWatch(void)
:m_wPort(0)				//!< 发送端口号
,m_bStarted(false)		//!< 是否启动了监控
{
}

//!< 设置信息，例如“12000”
bool CNetUDPWatch::SetInfo(CString info)
{
	m_wPort = (WORD)atoi(info);
	IComm::SetInfo(info);
	return true;
}

//!< 停止通信
bool CNetUDPWatch::StopCom()
{
	if(!m_bStarted)		return true;
	m_bStarted = false;
	((CSocket *)this)->Close();
	m_ltListener.clear();
	return true;
}

//!< 监听数据，开始通信
bool CNetUDPWatch::WatchData(ICommListener* listener)
{
	if(!listener)	return false;
	m_ltListener.remove(listener);
	m_ltListener.push_back(listener);
	if(m_bStarted)	return true;

	//!< 启动监听
	m_bStarted = ((CSocket *)this)->Create(m_wPort, SOCK_DGRAM);
	return m_bStarted;
}

//!< 取消监听
void CNetUDPWatch::DisWatch(ICommListener* listener)
{
	if(!listener)	return;
	m_ltListener.remove(listener);
	if(!m_bStarted)	return;
	if(m_ltListener.empty())		//!< 没有人监听了，停止通信
	{
		((CSocket *)this)->Close();
	}
	m_bStarted = false;
}

//!< 收到数据
void CNetUDPWatch::OnReceive(int nErrorCode)
{
	CString strIP;
	UINT uiPort;
	char data[MAX_UDP_PACKAGE_LENGTH];
	int len = ReceiveFrom(data, MAX_UDP_PACKAGE_LENGTH, strIP, uiPort);
	if(len < 0)		return;

	strIP = _T("\t <— ") + strIP;
	ICommListener* listener;
	foreach(listener, m_ltListener)
	{
		if(listener)	listener->OnReceive(this, (byte*)data, len, strIP);
	}

	CSocket::OnReceive(nErrorCode);
}
