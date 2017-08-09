#include "StdAfx.h"
#include "DCommer.h"
#include "Gbl.h"
#include "Comm.h"
#include "MyTimer.h"

#include "SerialPort.h"
#include "NetUDP.h"
#include "NetUDPWatch.h"
#include "NetSerial.h"

using namespace Comm;

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

//!< 0-UDP,1UDPWatch,2Serial¡­¡­
// boost::shared_ptr<IComm> CreateComm(int type)
// {
// 	boost::shared_ptr<IComm> commer;
// 	if(type == 0)		commer = boost::shared_ptr<IComm>(new CNetUDP);
// 	else if(type == 1)	commer = boost::shared_ptr<IComm>(new CNetUDPWatch);
// 	else if(type == 2)	commer = boost::shared_ptr<IComm>(new CNetSerial);
// 	return commer;
// }

void aabbbcccc()
{
}