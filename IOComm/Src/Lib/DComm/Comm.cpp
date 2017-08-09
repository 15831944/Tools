#include "stdafx.h"
#include "Gbl.h"
#include "Comm.h"
#include "MyTimer.h"
#include "Data.h"

#include "SerialPort.h"
#include "NetUDP.h"
#include "NetUDPWatch.h"
#include "NetTCP.h"
#include "NetSerial.h"
#include "NetSerialBoost.h"

using namespace Comm;

//////////////////////////////////////////////////////////////////////////
ICommListener::~ICommListener()
{
	foreach(IComm* comm, m_ltCommForNow)		delete comm;
	m_ltCommForNow.clear();
}

//!< 0-UDP,1UDPWatch,2Serial¡­¡­
//IComm* CreateComm(int type)
IComm* ICommListener::CreateComm(int type /* = NET_UDP */)
{
	IComm* commer = NULL;
	if(type == NET_UDP)					commer = /*boost::shared_ptr<IComm>*/(new CNetUDP);
	else if(type == NET_UDP_WATCH)		commer = /*boost::shared_ptr<IComm>*/(new CNetUDPWatch);
	else if(type == NET_TCPCLIENT)		commer = /*boost::shared_ptr<IComm>*/(new CNetTCP);
	else if(type == NET_SERIAL)			commer = /*boost::shared_ptr<IComm>*/(new CNetSerial);
	else if(type == NET_SERIALBOOST)	commer = /*boost::shared_ptr<IComm>*/(new CNetSerialBoost);
	m_ltCommForNow.push_back(commer);
	return commer;
}

void ICommListener::ReMoveComm(IComm* comm)
{
	m_ltCommForNow.remove(comm);
	if(comm)	delete comm;
}

//////////////////////////////////////////////////////////////////////////

IMPLEMENT_DYNAMIC(IComm, CWnd)

BEGIN_MESSAGE_MAP(IComm, CWnd)
	ON_MESSAGE(WM_COMM_RXCHAR, Receive)
END_MESSAGE_MAP()

//!< ÉèÖÃÐÅÏ¢
bool IComm::SetInfo(CString info)
{
	m_strInfo = info;
	return CreateWnd();
}

bool IComm::CreateWnd()
{
	if(GetSafeHwnd())			return true;
	CDialog* mf = (CDialog *)AfxGetApp()->GetMainWnd();
	if(!mf->GetSafeHwnd())		return false;
	CRect rect(0,0,0,0);
	if(!Create(NULL, _T("IComm"), WS_CHILD, rect, mf, 101))
		return false;
	return Timer::CDTimer::GetMe().Init();
}

LRESULT IComm::Receive(WPARAM pch, LPARAM number)
{
	OnReceive((byte*)pch, (int)number);
	return 0;
}
