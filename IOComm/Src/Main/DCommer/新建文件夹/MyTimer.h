#pragma once

namespace Timer{
class CTimerData;
class ITimerListener;
class CDTimer : public CWnd
{
	DECLARE_DYNAMIC(CDTimer)
private:
	std::list<boost::shared_ptr<CTimerData> > m_ltListener;
	int m_nMinSec;			//!< 记录时间

	CDTimer();
	virtual ~CDTimer();
	boost::shared_ptr<CTimerData> GetData(ITimerListener& listener, int nID);
	int CountTime();		//!< 统计时间

public:
	static CDTimer& GetMe();
	bool Init();
	void AddListener(ITimerListener& listener, int nMilSec, int nID = 0, bool bOnce = false);
	void KillListener(ITimerListener& listener, int nID);

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};

class ITimerListener
{
protected:
	void SetDTimer(int nMilSec, int nID = 0, bool bOnce = false)//!< 设置时间监听，最后一个参数表示是否只运行一次
	{CDTimer::GetMe().AddListener(*this, nMilSec, nID, bOnce);}
	void KillDTimer(int nID = 0)								//!< 取消时间监听
	{CDTimer::GetMe().KillListener(*this, nID);}

public:
	virtual void OnDTimer(int nID) = 0;		//!< 时间到了，nMilSec表示距离上一次的时间报告，经过了多少个毫秒
};

class CTimerData
{
public:
	ITimerListener* m_Listener;				//!< 时间监听者
	int m_nID;								//!< 监控标识
	int m_nMilSec;							//!< 监听时间，单位毫秒
	int m_nCount;							//!< 监控计数
	bool m_bOnce;							//!< 是否仅监听一次，false表示循环监听

public:
	CTimerData(ITimerListener& listener, int nMilSec, int nID, bool bOnce):m_Listener(&listener)
		,m_nMilSec(nMilSec),m_nID(nID),m_bOnce(bOnce),m_nCount(0){;}
	void OnTimer(){m_nCount -= m_nMilSec;			m_Listener->OnDTimer(m_nID);}	//!< 时间到
	bool OnTick(int nMilSec){m_nCount += nMilSec;	return m_nCount >= m_nMilSec;}	//!< 单位计时到
	void SetMilSec(int nMilSec){m_nMilSec = nMilSec; m_nCount = 0;}
};

}