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

		byte m_curPoker[5];
		double m_curWinRate;

	public:
		void SetMeFlag(bool b){ m_bMe = b; }
		double CurWinRate(){ return m_curWinRate; }

	public:
		Player(int id, AnalyzeMgr *mgr);
		~Player();

		void InitPokers(byte *p);
	};
}
