// MyTimer.cpp : 实现文件
//

#include "stdafx.h"
#include "Gbl.h"
#include "DCommer.h"
#include "MyTimer.h"

using namespace Timer;

// CMyTimer
IMPLEMENT_DYNAMIC(CDTimer, CWnd)
CDTimer::CDTimer()
{
}

CDTimer::~CDTimer()
{
}

BEGIN_MESSAGE_MAP(CDTimer, CWnd)
	ON_WM_TIMER()
END_MESSAGE_MAP()

// CMyTimer 消息处理程序
CDTimer& CDTimer::GetMe()
{
	static CDTimer one;
	return one;
}

bool CDTimer::Init()
{
	if(GetSafeHwnd())			return true;
	CDialog* mf = (CDialog *)AfxGetApp()->GetMainWnd();
	if(!mf->GetSafeHwnd())		return false;
	return Create(NULL, "CDTimer", WS_CHILD, CRect(0,0,0,0), mf, 102);
}

//!< 
boost::shared_ptr<CTimerData> CDTimer::GetData(ITimerListener& listener, int nID)
{
	boost::shared_ptr<CTimerData> data;
	foreach(data, m_ltListener)
	{
		if(data->m_Listener == &listener && data->m_nID == nID)
			return data;
	}
	boost::shared_ptr<CTimerData> empty;
	return empty;
}

//!< 
void CDTimer::AddListener(ITimerListener& listener, int nMilSec, int nID /* = 0 */, bool bOnce /* = false */)
{
//	CGbl::PrintOut("AddListener");
	boost::shared_ptr<CTimerData> data = GetData(listener, nID);
	if(data){		data->SetMilSec(nMilSec);		return;}
	data = boost::shared_ptr<CTimerData>(new CTimerData(listener, nMilSec, nID, bOnce));
	m_ltListener.push_back(data);
	if(m_ltListener.size() != 1)	return;
	SYSTEMTIME st;
	GetSystemTime(&st);
	m_nMinSec = st.wMinute * 60000 + st.wSecond * 1000 + st.wMilliseconds;
	SetTimer(1, 50, NULL);
}

//!< 
void CDTimer::KillListener(ITimerListener& listener, int nID)
{
	boost::shared_ptr<CTimerData> data = GetData(listener, nID);
	if(data)	m_ltListener.remove(data);
	if(m_ltListener.empty() && GetSafeHwnd())
		KillTimer(1);
}

void CDTimer::OnTimer(UINT_PTR nIDEvent)
{
	if(nIDEvent == 1)
	{
		boost::shared_ptr<CTimerData> data;
		std::list<boost::shared_ptr<CTimerData> > ltForTime;
		int nMinSec = CountTime();
		foreach(data, m_ltListener)
		{
			if(data->OnTick(nMinSec))	ltForTime.push_back(data);
		}
		foreach(data, ltForTime)
		{
			if(data->m_bOnce)			m_ltListener.remove(data);
			data->OnTimer();
		}
	}
	CWnd::OnTimer(nIDEvent);
}

//!< 统计时间
int CDTimer::CountTime()
{
	SYSTEMTIME st;
	GetSystemTime(&st);
	int nMilSec = st.wMinute * 60000 + st.wSecond * 1000 + st.wMilliseconds;
	while(nMilSec < m_nMinSec)	nMilSec += 60000;		//!< 每次加1分钟
	int nRet = nMilSec - m_nMinSec;
	m_nMinSec = nMilSec;
	ASSERT(nRet >= 0);
	return nRet;
}
