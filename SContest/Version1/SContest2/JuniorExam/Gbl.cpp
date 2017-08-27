#include "stdafx.h"
#include "Gbl.h"


CGbl::CGbl()
{
	TCHAR exePath[MAX_PATH];
	::ZeroMemory(exePath, sizeof(TCHAR)*MAX_PATH);
	::GetModuleFileName(NULL, exePath, MAX_PATH);
	m_strExePath = exePath;
	m_strExePath = m_strExePath.Left(m_strExePath.ReverseFind('\\'));
	m_strExePath += _T('\\');
}


CGbl::~CGbl()
{
}
