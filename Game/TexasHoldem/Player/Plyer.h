#pragma once

namespace Analyze{
	class AnalyzeMgr;
	class Habit;
	class Player
	{
	private:
		bool m_bMe;		// If this player is myself
		int m_id;
		AnalyzeMgr *m_Mgr;
		std::shared_ptr<Habit> m_habit;
		std::list<double> m_winRateList;

		byte m_curPoker[5];
		double m_curWinRate;	// 我的胜率
		double m_curWinRateH;	// 当前牌的最高胜率
		double m_curWinRateL;	// 当前牌的最低胜率

	public:
		int GetID(){ return m_id; }
		void SetMeFlag(bool b){ m_bMe = b; }
		double CurWinRate(){ return m_curWinRate; }

	public:
		Player(int id, AnalyzeMgr *mgr);
		~Player();

		void InitPokers(byte *p);
		int GetBet(int nMax, int nPrevBet, int nMyBet, int nTotal, CString strAllBet);
	};
}
