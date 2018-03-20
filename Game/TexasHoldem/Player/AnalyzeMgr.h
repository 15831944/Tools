#pragma once
namespace Analyze{
	class Player;
	class AnalyzeMgr
	{
	private:
		std::list<std::shared_ptr<Player>> m_ltPlayer;
		std::shared_ptr<Player> m_Me;
		int m_curID;
		int m_totalMoney;
		int m_playCount;
		int m_betUnit;
		int m_gameCount;
		int m_gameIndex;

		int m_curBankerID;
		int m_curPlayLeft;		// 本局还剩多少个对手
		bool m_gameBeginning;	// 是否是开局的首次下注

	private:
		AnalyzeMgr();
		~AnalyzeMgr();

	public:
		static AnalyzeMgr& GetMe(){ static AnalyzeMgr one; return one; }
		void OnInitPlayers(int id, int totalMoney, int playCount, int betUnit, int gameCount);
		void OnGameBegin();
		void OnReceivePoker(byte *pokers, int bankerID);
		int GetBet(int nMax, int nPrevBet, int nMyBet, int nTotal, CString strAllBet);
	};
}
