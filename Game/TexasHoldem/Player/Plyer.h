#pragma once

namespace Analyze{
	class AnalyzeMgr;
	class Habit;
	class Player
	{
	private:
		//enum BET_IDEA{
		//	GIVE_UP,	// 放弃
		//	BET_50,		// 押到50
		//	CATCH_100,	// 跟到100
		//	BET_200,	// 押到200
		//	CATCH_300,	// 跟到300
		//	BET_500,	// 押到500
		//	CATCH_1000,	// 跟到1000
		//	MUST_WIN,	// 一定会赢
		//};

	private:
		bool m_bMe;		// If this player is myself
		int m_id;
		double m_curMoney;
		AnalyzeMgr *m_Mgr;
		std::shared_ptr<Habit> m_habit;
		std::list<double> m_winRateList;

		byte m_curPoker[5];
		double m_curWinRateOne;		// 我的1v1胜率
		double m_curWinRateAll;		// 我的1vn胜率
		double m_curPokerPowerAvg;	// 对方的平均牌力值
		//BET_IDEA m_BetIdea;			// 下注的策略

		int m_betDelta;				// 下注的差值
		int m_betMax;				// 下注的最大值
		double m_betMul;			// 乘法系数
		int m_catchMax;				// 跟注的最大值

	public:
		int GetID(){ return m_id; }
		void SetMeFlag(bool b){ m_bMe = b; }
		double CurWinRateOne(){ return m_curWinRateOne; }		// 我的1v1胜率
		double CurWinRateAll(){ return m_curWinRateAll; }		// 我的1vn胜率

	public:
		Player(int id, AnalyzeMgr *mgr);
		~Player();

		void InitPokers(byte *p);
		int GetBet(int nMax, int nPrevBet, int nMyBet, int nTotal, CString strAllBet);
		double GetWinState();	// 当前的状态0~0.2，默认0.1，0表示岌岌可危，必须冲，0.2表示胜券在握，需要稳
	};
}
