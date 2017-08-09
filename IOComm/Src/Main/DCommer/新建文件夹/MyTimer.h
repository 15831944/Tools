#pragma once

namespace Timer{
class CTimerData;
class ITimerListener;
class CDTimer : public CWnd
{
	DECLARE_DYNAMIC(CDTimer)
private:
	std::list<boost::shared_ptr<CTimerData> > m_ltListener;
	int m_nMinSec;			//!< ��¼ʱ��

	CDTimer();
	virtual ~CDTimer();
	boost::shared_ptr<CTimerData> GetData(ITimerListener& listener, int nID);
	int CountTime();		//!< ͳ��ʱ��

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
	void SetDTimer(int nMilSec, int nID = 0, bool bOnce = false)//!< ����ʱ����������һ��������ʾ�Ƿ�ֻ����һ��
	{CDTimer::GetMe().AddListener(*this, nMilSec, nID, bOnce);}
	void KillDTimer(int nID = 0)								//!< ȡ��ʱ�����
	{CDTimer::GetMe().KillListener(*this, nID);}

public:
	virtual void OnDTimer(int nID) = 0;		//!< ʱ�䵽�ˣ�nMilSec��ʾ������һ�ε�ʱ�䱨�棬�����˶��ٸ�����
};

class CTimerData
{
public:
	ITimerListener* m_Listener;				//!< ʱ�������
	int m_nID;								//!< ��ر�ʶ
	int m_nMilSec;							//!< ����ʱ�䣬��λ����
	int m_nCount;							//!< ��ؼ���
	bool m_bOnce;							//!< �Ƿ������һ�Σ�false��ʾѭ������

public:
	CTimerData(ITimerListener& listener, int nMilSec, int nID, bool bOnce):m_Listener(&listener)
		,m_nMilSec(nMilSec),m_nID(nID),m_bOnce(bOnce),m_nCount(0){;}
	void OnTimer(){m_nCount -= m_nMilSec;			m_Listener->OnDTimer(m_nID);}	//!< ʱ�䵽
	bool OnTick(int nMilSec){m_nCount += nMilSec;	return m_nCount >= m_nMilSec;}	//!< ��λ��ʱ��
	void SetMilSec(int nMilSec){m_nMilSec = nMilSec; m_nCount = 0;}
};

}