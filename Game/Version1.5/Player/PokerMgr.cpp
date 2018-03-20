#include "stdafx.h"
#include "PokerMgr.h"

using namespace Poker;

//std::map<__int64, int> g_mpResult;
std::hash_map<__int64, int> g_mpIntResult;

byte GetValueFromPoker(TCHAR c)
{
	if (c == _T('2'))	return 0;
	if (c == _T('3'))	return 1;
	if (c == _T('4'))	return 2;
	if (c == _T('5'))	return 3;
	if (c == _T('6'))	return 4;
	if (c == _T('7'))	return 5;
	if (c == _T('8'))	return 6;
	if (c == _T('9'))	return 7;
	if (c == _T('T') || c == _T('t'))	return 8;
	if (c == _T('J') || c == _T('j'))	return 9;
	if (c == _T('Q') || c == _T('q'))	return 10;
	if (c == _T('K') || c == _T('k'))	return 11;
	if (c == _T('A') || c == _T('a'))	return 12;
	if (c == _T('O') || c == _T('o'))	return 0;
	if (c == _T('S') || c == _T('s'))	return 1;
	ASSERT(FALSE);
	return 255;
}

void AddResult(CString key, CString val)
{
	key.Trim();
	val.Trim();
	if (key.GetLength() != POKER_COUNT + 1)	{
		ASSERT(FALSE);  return;
	}

	PokerValue pv;
	for (int i = 0; i < POKER_COUNT; i++){
		TCHAR c = key.GetAt(i); pv._ByVal.pk[i] = GetValueFromPoker(c);
	}
	TCHAR col = key.GetAt(POKER_COUNT); pv._ByVal.col = GetValueFromPoker(col);
	g_mpIntResult[pv._Int64Val] = ::_ttoi(val);
}

void InitResult()
{
	CString strPath = CGbl::GetAppPath() + _T("TexasTable.eny");
	CFile file;
	if (!file.Open(strPath, CFile::modeRead))
	{
		return;
	}

	UINT count = (UINT)file.GetLength() / sizeof(TCHAR);
	TCHAR *pChar = new TCHAR[count + 1];
	::ZeroMemory(pChar, (count + 1)* sizeof(TCHAR));
	file.Read(pChar, (UINT)file.GetLength());

	CString strIn = CString(pChar);
	delete[] pChar;
	file.Close();
	CString strOut = CGbl::UnEncrypt(strIn);

	g_mpIntResult.clear();
	std::vector<CString> vtLine;
	CGbl::SpliteBy(strOut, _T("\r\n"), vtLine);
	for (CString strLine : vtLine)
	{
		if (strLine.Find(_T(",")) == -1)	continue;

		std::vector<CString> vtKV;
		CGbl::SpliteBy(strLine, _T(","), vtKV);

		if (vtKV.size() != 2)				continue;
		AddResult(vtKV[0], vtKV[1]);
	}
}

bool SameColor(byte* p)
{
	byte nColor = (byte)(p[0] / 13);
	for (int i = 1; i < POKER_COUNT; i++)
	{
		if (nColor != (byte)(p[i] / 13))
			return false;
	}
	return true;
}

__int64 PokerMgr::GetPokerValue(byte* p)
{
	// Sort value
	static byte VTVal[POKER_COUNT] = { 0 };
	for (size_t i = 0; i < POKER_COUNT; i++)
	{
		VTVal[i] = p[i] % 13;
	}
	std::sort(std::begin(VTVal), std::end(VTVal));

	PokerValue pv;
	for (int i = 0; i < POKER_COUNT; i++)
	{
		pv._ByVal.pk[i] = VTVal[i];
	}
	if (SameColor(p))
		pv._ByVal.col = 1;
	return pv._Int64Val;
}

CString PokerMgr::GetPokerString(byte* p)
{
	CString strPoker;

	// Sort value
	static byte VTVal[POKER_COUNT] = { 0 };
	for (size_t i = 0; i < POKER_COUNT; i++)
	{
		VTVal[i] = p[i] % 13;
	}
	std::sort(std::begin(VTVal), std::end(VTVal));

	// Get values
	for (int i = 0; i < POKER_COUNT; i++)
	{
		switch (VTVal[i])
		{
		case 0:		strPoker += _T('2');	break;
		case 1:		strPoker += _T('3');	break;
		case 2:		strPoker += _T('4');	break;
		case 3:		strPoker += _T('5');	break;
		case 4:		strPoker += _T('6');	break;
		case 5:		strPoker += _T('7');	break;
		case 6:		strPoker += _T('8');	break;
		case 7:		strPoker += _T('9');	break;
		case 8:		strPoker += _T('T');	break;
		case 9:		strPoker += _T('J');	break;
		case 10:	strPoker += _T('Q');	break;
		case 11:	strPoker += _T('K');	break;
		case 12:	strPoker += _T('A');	break;
		}
	}

	// Get color
	strPoker += SameColor(p) ? _T("s") : _T("o");
	return strPoker;
}

double PokerMgr::GetWinRate(byte* pokers)
{
	double rate = 0.0;
	std::list<byte> ltpk;

	// get all pks without pokers
	for (byte i = 0; i < 52; i++) ltpk.push_back(i);
	for (byte i = 0; i < POKER_COUNT; i++) ltpk.remove(pokers[i]);

	__int64 pkVal = PokerMgr::GetPokerValue(pokers);
	if (g_mpIntResult.find(pkVal) == g_mpIntResult.end()) { ASSERT(FALSE); return 0.0; }
	int vL = g_mpIntResult[pkVal];

	// loop for all other pks
	byte pkCmp[5] = { 0 };
	memcpy(&pkCmp[2], &pokers[2], 3);
	int baseCount = (52 - POKER_COUNT) * (51 - POKER_COUNT) / 2; // C(52-5)2
	int winCount = 0, loseCount = 0;
	for (auto iter_i = ltpk.begin(); iter_i != ltpk.end(); iter_i++)
	{
		pkCmp[0] = *iter_i;
		auto iter_j = iter_i;
		iter_j++;
		while (iter_j != ltpk.end())
		{
			pkCmp[1] = *iter_j;
			__int64 pkValR = PokerMgr::GetPokerValue(pkCmp);
			if (g_mpIntResult.find(pkValR) == g_mpIntResult.end()) { ASSERT(FALSE); return 0.0; }
			int vR = g_mpIntResult[pkValR];

			if (vL <= vR)	winCount++;
			else			loseCount++;
			iter_j++;
		}
	}

	return (double)winCount / (double)baseCount;
}

PokerMgr::PokerMgr()
{
	InitResult();
}
