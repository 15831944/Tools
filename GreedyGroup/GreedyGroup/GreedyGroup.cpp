// GreedyGroup.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "GreedyGroup.h"
#include "GreedyMgr.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


__int64 __nStart = 0;
__int64 __nEnd = 0;
__int64 __nFrequency = 0;

//!< 启动高精度定时器,用于统计程序效率
bool StartHighTime()
{
	if (!QueryPerformanceFrequency((LARGE_INTEGER *)&__nFrequency))	return false;
	if (!QueryPerformanceCounter((LARGE_INTEGER *)&__nStart))		return false;
	return true;
}

//!< 获得定时器差值,并设置是否重新计时
double GetHighTime(bool bReStart)
{
	if (!QueryPerformanceCounter((LARGE_INTEGER *)&__nEnd))			return -1.0;
	__int64 n = __nEnd - __nStart;
	if (bReStart)	__nStart = __nEnd;
	double t = (double)n / (double)__nFrequency;
	return t;
}

// 唯一的应用程序对象

CWinApp theApp;

int _tmain(int argc, TCHAR* argv[], TCHAR* envp[])
{
	if (!AfxWinInit(::GetModuleHandle(NULL), NULL, ::GetCommandLine(), 0))	return 1;
	CGreedyMgr mgr;
	mgr.Init(2000, 1024, 300);
	int tT1 = 0, tT2 = 0, tT3 = 0;

	StartHighTime();
	int n1 = mgr.Start2GreedyGroup(tT1);
	double t1 = GetHighTime(false);

	StartHighTime();
	int n2 = mgr.Start2FinalSolution(tT2);
	double t2 = GetHighTime(false);

	StartHighTime();
	int n3 = mgr.Start2FinalSolutionFast(tT3);
	double t3 = GetHighTime(false);

	std::cout << "Time1=" << t1 << "  Count1=" << n1 << "  Throughout=" << tT1 << std::endl;
	std::cout << "Time2=" << t2 << "  Count2=" << n2 << "  Throughout=" << tT2 << std::endl;
	std::cout << "Time3=" << t3 << "  Count3=" << n3 << "  Throughout=" << tT3 << std::endl;

	std::cin>>n1;
	return 0;
}
