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

	//m_ltPlayer.clear();
	if (m_vtPlayer.size() != playCount)
	{
		m_vtPlayer.resize(playCount);
		for (int i = 0; i < playCount; ++i)
		{
			m_vtPlayer[i] = std::shared_ptr<Player>(new Player(i, this));
			if (i == id) {
				m_Me = m_vtPlayer[i];
				m_Me->SetMeFlag(true);
			}
		}
	}
}

void AnalyzeMgr::OnGameBegin()
{
	m_gameBeginning = true;	// 是否是开局的首次下注
	m_curPlayLeft = m_playCount - 1;
}

void AnalyzeMgr::OnReceivePoker(byte *pokers, int bankerID)
{
	m_curBankerID = bankerID;
	m_Me->InitPokers(pokers);
}

int AnalyzeMgr::GetBet(int nMax, int nPrevBet, int nMyBet, int nTotal, CString strAllBet)
{
	if (m_gameBeginning)
	{
		m_gameBeginning = false;
		if (m_curBankerID != m_Me->GetID())
		{
			std::vector<CString> vtRows;
			CGbl::SpliteBy(strAllBet, _T("\r\n"), vtRows);
			int n = vtRows.size();
		}
	}
	return m_Me->GetBet(nMax, nPrevBet, nMyBet, nTotal, strAllBet);
}
