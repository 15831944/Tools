#include "StdAfx.h"
#include "NetUDPWatch.h"

using namespace Comm;
CNetUDPWatch::CNetUDPWatch(void)
:m_wPort(0)				//!< ���Ͷ˿ں�
,m_bStarted(false)		//!< �Ƿ������˼��
{
}

//!< ������Ϣ�����硰12000��
bool CNetUDPWatch::SetInfo(CString info)
{
	m_wPort = (WORD)atoi(info);
	IComm::SetInfo(info);
	return true;
}

//!< ֹͣͨ��
bool CNetUDPWatch::StopCom()
{
	if(!m_bStarted)		return true;
	m_bStarted = false;
	((CSocket *)this)->Close();
	m_ltListener.clear();
	return true;
}

//!< �������ݣ���ʼͨ��
bool CNetUDPWatch::WatchData(ICommListener* listener)
{
	if(!listener)	return false;
	m_ltListener.remove(listener);
	m_ltListener.push_back(listener);
	if(m_bStarted)	return true;

	//!< ��������
	m_bStarted = ((CSocket *)this)->Create(m_wPort, SOCK_DGRAM);
	return m_bStarted;
}

//!< ȡ������
void CNetUDPWatch::DisWatch(ICommListener* listener)
{
	if(!listener)	return;
	m_ltListener.remove(listener);
	if(!m_bStarted)	return;
	if(m_ltListener.empty())		//!< û���˼����ˣ�ֹͣͨ��
	{
		((CSocket *)this)->Close();
	}
	m_bStarted = false;
}

//!< �յ�����
void CNetUDPWatch::OnReceive(int nErrorCode)
{
	CString strIP;
	UINT uiPort;
	char data[MAX_UDP_PACKAGE_LENGTH];
	int len = ReceiveFrom(data, MAX_UDP_PACKAGE_LENGTH, strIP, uiPort);
	if(len < 0)		return;

	strIP = _T("\t <�� ") + strIP;
	ICommListener* listener;
	foreach(listener, m_ltListener)
	{
		if(listener)	listener->OnReceive(this, (byte*)data, len, strIP);
	}

	CSocket::OnReceive(nErrorCode);
}
