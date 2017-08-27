#pragma once
//#include "stdafx.h"

class PrintTime
{
private:
	__int64 __nStart = 0;
	__int64 __nEnd = 0;
	__int64 __nFrequency = 0;
	bool __bCreated;

public:
	PrintTime(void)
		: __nStart(0)
		, __nEnd(0)
		, __nFrequency(0)
		, __bCreated(false)
	{
		if (!QueryPerformanceFrequency((LARGE_INTEGER *)&__nFrequency))	return;
		if (!QueryPerformanceCounter((LARGE_INTEGER *)&__nStart))		return;
		__bCreated = true;
	}

	~PrintTime(void){
		if (!__bCreated)		return;
		CalcTime();
	}

	double CalcTime()
	{
		if (!QueryPerformanceCounter((LARGE_INTEGER *)&__nEnd))			return -1.0;
		__int64 n = __nEnd - __nStart;
		double t = (double)n / (double)__nFrequency;
		return t;
	}

	void MessageBoxTime()
	{
		double d = CalcTime();
		CString str;
		str.Format(_T("%f"), d);
		MessageBox(NULL, str, _T("Info"), MB_OK);
	}
};