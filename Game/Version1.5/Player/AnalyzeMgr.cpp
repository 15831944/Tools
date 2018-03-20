#include "stdafx.h"
#include "AnalyzeMgr.h"
#include "Plyer.h"
#include "PokerMgr.h"

using namespace Analyze;

AnalyzeMgr::AnalyzeMgr()
{
	Poker::PokerMgr::GetMe();
}


AnalyzeMgr::~AnalyzeMgr()
{
}

void AnalyzeMgr::OnInitPlayers(int id, int totalMoney, int playCount, int betUnit, int gameCount)
{
	m_curID = id;
	m_totalMoney = totalMoney;
	m_playCount = playCount;
	m_betUnit = betUnit;
	m_gameCount = gameCount;
	m_gameIndex = 0;

	m_ltPlayer.clear();

	for (int i = 0; i < playCount; ++i)
	{
		m_ltPlayer.push_back(std::shared_ptr<Player>(new Player(i, this)));
		if (i == id) {
			m_Me = m_ltPlayer.back();
			m_Me->SetMeFlag(true);
		}
	}
}

void AnalyzeMgr::OnGameBegin()
{
}

void AnalyzeMgr::OnReceivePoker(byte *pokers, int bankerID)
{
	m_curBankerID = bankerID;
	m_Me->InitPokers(pokers);
}
