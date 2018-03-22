#pragma once

namespace Analyze{
	class AnalyzeMgr;
	class Habit;
	class Player
	{
	private:
		//enum BET_IDEA{
		//	GIVE_UP,	// ����
		//	BET_50,		// Ѻ��50
		//	CATCH_100,	// ����100
		//	BET_200,	// Ѻ��200
		//	CATCH_300,	// ����300
		//	BET_500,	// Ѻ��500
		//	CATCH_1000,	// ����1000
		//	MUST_WIN,	// һ����Ӯ
		//};

	private:
		bool m_bMe;		// If this player is myself
		int m_id;
		double m_curMoney;
		AnalyzeMgr *m_Mgr;
		std::shared_ptr<Habit> m_habit;
		std::list<double> m_winRateList;

		byte m_curPoker[5];
		double m_curWinRateOne;		// �ҵ�1v1ʤ��
		double m_curWinRateAll;		// �ҵ�1vnʤ��
		double m_curPokerPowerAvg;	// �Է���ƽ������ֵ
		//BET_IDEA m_BetIdea;			// ��ע�Ĳ���

		int m_betDelta;				// ��ע�Ĳ�ֵ
		int m_betMax;				// ��ע�����ֵ
		double m_betMul;			// �˷�ϵ��
		int m_catchMax;				// ��ע�����ֵ

	public:
		int GetID(){ return m_id; }
		void SetMeFlag(bool b){ m_bMe = b; }
		double CurWinRateOne(){ return m_curWinRateOne; }		// �ҵ�1v1ʤ��
		double CurWinRateAll(){ return m_curWinRateAll; }		// �ҵ�1vnʤ��

	public:
		Player(int id, AnalyzeMgr *mgr);
		~Player();

		void InitPokers(byte *p);
		int GetBet(int nMax, int nPrevBet, int nMyBet, int nTotal, CString strAllBet);
		double GetWinState();	// ��ǰ��״̬0~0.2��Ĭ��0.1��0��ʾ�᧿�Σ������壬0.2��ʾʤȯ���գ���Ҫ��
	};
}
