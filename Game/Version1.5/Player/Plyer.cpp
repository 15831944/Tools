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
}