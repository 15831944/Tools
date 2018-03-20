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
}


Player::~Player()
{
}

void Player::InitPokers(byte *p)
{
	memcpy(m_curPoker, p, 5);
	m_curWinRate = Poker::PokerMgr::GetWinRate(p);
	m_winRateList.push_back(m_curWinRate);
}

/*
胜率与赢的对应表
		8人			  6人
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
	return -1;
}