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

using namespace std;
int NCOUNT = 0;

int _tmain(int argc, TCHAR* argv[], TCHAR* envp[])
{
	if (!AfxWinInit(::GetModuleHandle(NULL), NULL, ::GetCommandLine(), 0))	return 1;
	CGreedyMgr mgr;
	mgr.Init(10, 255);
	StartHighTime();

	int n = mgr.Start2GreedyGroup();

	double t = GetHighTime(false);
	std::cout<<"Time="<<t<<"  Count="<<NCOUNT<<std::endl;
	std::cin>>t;
	return 0;
}
