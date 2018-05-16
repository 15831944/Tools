#include "stdafx.h"
#include "Plyer.h"
#include "AnalyzeMgr.h"
#include "Habit.h"
#include "HabitInfo.h"

#include "PokerMgr.h"

using namespace Analyze;
Player::Player(int id, AnalyzeMgr *mgr)
	: m_id(id)
	, m_Mgr(mgr)
	, m_bMe(false)
{
	m_habit = std::shared_ptr<Habit>(new Habit());
	m_curMoney = m_Mgr->GetTotalMoney();
}


Player::~Player()
{
}

void Player::InitPokers(byte *p)
{
	memcpy(m_curPoker, p, 5);
	Poker::PokerMgr::GetWinRate(p, m_curWinRateOne, m_curPokerPowerAvg);
	m_curWinRateAll = m_curWinRateOne;
	m_winRateList.push_back(m_curWinRateOne);
	int nCount = m_Mgr->GetPlayCount() - 1;
	if (nCount < 7)	nCount = 7;
	for (int i = 1; i < nCount; i++){
		m_curWinRateAll = m_curWinRateAll *  m_curWinRateOne;
	}
	m_curWinRateAll = m_curWinRateAll + 0.1 - GetWinState();
	if (m_curWinRateAll < 0.5)			{ m_betMax = -1; m_betMul = 0.0; m_betDelta = 0; m_catchMax = 0; }		// ����
	else if (m_curWinRateAll < 0.6)		{ m_betMax = 10; m_betMul = 1.0; m_betDelta = 10; m_catchMax = 0; }		// Ѻ��10 ����
	else if (m_curWinRateAll < 0.7)		{ m_betMax = 30; m_betMul = 1.0; m_betDelta = 20; m_catchMax = 40; }	// Ѻ��50 ����60
	else if (m_curWinRateAll < 0.78)	{ m_betMax = 50; m_betMul = 1.0; m_betDelta = 20; m_catchMax = 80; }
	else if (m_curWinRateAll < 0.85)	{ m_betMax = 80; m_betMul = m_curPokerPowerAvg; m_betDelta = 30 * m_curPokerPowerAvg; m_catchMax = 120; }
	else if (m_curWinRateAll < 0.89)	{ m_betMax = 120; m_betMul = m_curPokerPowerAvg; m_betDelta = 50 * m_curPokerPowerAvg; m_catchMax = 200; }
	else if (m_curWinRateAll < 0.92)	{ m_betMax = 200; m_betMul = m_curPokerPowerAvg; m_betDelta = 55 * m_curPokerPowerAvg; m_catchMax = 500; }
	else if (m_curWinRateAll < 0.95)	{ m_betMax = 400; m_betMul = m_curPokerPowerAvg; m_betDelta = 68 * m_curPokerPowerAvg; m_catchMax = 1000; }
	else if (m_curWinRateAll >= 0.95)	{ m_betMax = 90000000; m_betMul = 2.0; m_betDelta = 100; m_catchMax = 90000000; }
}

// ��ǰ��״̬0~0.2��Ĭ��0.1��0��ʾ�᧿�Σ������壬0.2��ʾʤȯ���գ���Ҫ��
double Player::GetWinState()
{
	double x = m_curMoney / m_Mgr->GetTotalMoney();
	if (x <= 0.5)	return 0.0;
	if (x >= 1.5)	return 2.0;

	//double state = (x - 0.5) * 2;
	// 10y = -2x*x/3 + 3x - 4/3
	return (((-2.0 * x * x) / 3.0) + (3.0 * x) - (4.0 / 3.0)) / 10.0;
}

/*
ʤ����Ӯ�Ķ�Ӧ��
		8��			  6��
0.85	0.320577088	  0.443705313
0.86	0.347927822	  0.470427018
0.87	0.377254795	  0.498420921
0.88	0.408675596	  0.527731917
0.89	0.442313349	  0.558405945
0.90	0.478296900	  0.59049
0.91	0.516761019	  0.624032145
0.92	0.557846601	  0.659081523
0.93	0.601700871	  0.695688369
0.94	0.648477594	  0.733904022
0.95	0.698337296	  0.773780938
0.96	0.751447478	  0.815372698
0.97	0.807982845	  0.858734026
0.98	0.868125533	  0.903920797
0.99	0.932065348	  0.95099005
1.00	1.000000000	  1
*/
int Player::GetBet(int nMax, int nPrevBet, int nMyBet, int nTotal, CString strAllBet)
{
	// return = -1 or nPrevBet <= return + nMyBet <= nMax
	if (m_betMax <= 0)	return -1;
	int nBet = (int)(m_betDelta / m_Mgr->GetBetUnit()) * m_Mgr->GetBetUnit();	// ������ҵ�ǰҪѺ��ע
	m_betDelta *= m_betMul;		// �´�ҪѺ��ֵ
	m_betMul -= 0.1;

	// ����Ѻע������Ѻע���
	if ((nBet + nMyBet) > m_betMax)
		nBet = m_betMax - nMyBet;		// �����Ŀǰ����ѹ�����Ѻע���

	// ����Ѻע�Ƿ񲻹�������ע��Ҫ��
	if ((nBet + nMyBet) < nPrevBet)
	{
		// �жϸ�ע����Ƿ��������Ҫ��
		if (m_catchMax >= nPrevBet)
			nBet = nPrevBet - nMyBet;		// ��ע��ֵ
		else
			return -1;			// �����˸�ע����������
	}

	if ((nBet + nMyBet) > nMax)
		nBet = nMax - nMyBet;	// ���Ѻעֵ�������˹��򣬽��͵�����ĵ�λ
	return nBet;
}