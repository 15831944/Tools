#pragma once
#define POKER_COUNT 5

namespace Poker{
	union PokerValue
	{
		struct {
			byte pk[POKER_COUNT];
			byte col;
			byte reserved[2];
		}_ByVal;
		__int64 _Int64Val;

		PokerValue() { _Int64Val = 0; }
	};

	class PokerMgr
	{
	private:
		PokerMgr();

	public:
		static PokerMgr& GetMe(){ static PokerMgr one; return one; }
		static CString GetPokerString(byte* p);
		static __int64 GetPokerValue(byte* p);
		static double GetWinRate(byte* pokers);
	};
}
