#include "stdafx.h"
#include "Gbl.h"


CGbl::CGbl()
{
}


CGbl::~CGbl()
{
}

CString CGbl::UnEncrypt(CString strIn)
{
	std::map<TCHAR, TCHAR> mpCharMap;
	mpCharMap[_T('a')] = _T('0');
	mpCharMap[_T('b')] = _T('1');
	mpCharMap[_T('c')] = _T('2');
	mpCharMap[_T('d')] = _T('3');
	mpCharMap[_T('e')] = _T('4');
	mpCharMap[_T('f')] = _T('5');
	mpCharMap[_T('g')] = _T('6');
	mpCharMap[_T('h')] = _T('7');
	mpCharMap[_T('i')] = _T('8');
	mpCharMap[_T('j')] = _T('9');
	mpCharMap[_T('k')] = _T('A');
	mpCharMap[_T('l')] = _T('J');
	mpCharMap[_T('m')] = _T('Q');
	mpCharMap[_T('n')] = _T('K');
	mpCharMap[_T('o')] = _T('T');
	mpCharMap[_T('p')] = _T('o');
	mpCharMap[_T('q')] = _T('s');
	mpCharMap[_T('r')] = _T(',');
	mpCharMap[_T('s')] = _T('\r');
	mpCharMap[_T('t')] = _T('\n');
	mpCharMap[_T('\0')] = _T('\0');

	CString strOut;
	TCHAR *pChar = strIn.GetBuffer();
	int len = strIn.GetLength();
	for (int i = 0; i < len; i++)
	{
		if (mpCharMap.find(pChar[i]) != mpCharMap.end())
		{
			strOut += mpCharMap[pChar[i]];
		}
		else
		{
			CString str = _T("Can't find this char : '");
			str = str + pChar[i] + _T("'.");
			MessageBox(NULL, str, _T("Error"), MB_OK | MB_ICONERROR);
		}
	}
	return strOut;
}

// Last c is '\'
CString CGbl::GetAppPath()
{
	TCHAR exePath[MAX_PATH];
	::ZeroMemory(exePath, sizeof(TCHAR)*MAX_PATH);
	::GetModuleFileName(NULL, exePath, MAX_PATH);
	CString strExePath = exePath;
	strExePath = strExePath.Left(strExePath.ReverseFind('\\') + 1);
	return strExePath;
}

void CGbl::SpliteBy(CString text, CString rule, std::vector<CString>& vtStr)
{
	int pos = 0;
	vtStr.clear();
	CString value = text.Tokenize(rule, pos);
	while (value != ""){
		vtStr.push_back(value);
		value = text.Tokenize(rule, pos);
	}
}