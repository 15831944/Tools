// GreedyGroup.cpp : �������̨Ӧ�ó������ڵ㡣
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

//!< �����߾��ȶ�ʱ��,����ͳ�Ƴ���Ч��
bool StartHighTime()
{
	if (!QueryPerformanceFrequency((LARGE_INTEGER *)&__nFrequency))	return false;
	if (!QueryPerformanceCounter((LARGE_INTEGER *)&__nStart))		return false;
	return true;
}

//!< ��ö�ʱ����ֵ,�������Ƿ����¼�ʱ
double GetHighTime(bool bReStart)
{
	if (!QueryPerformanceCounter((LARGE_INTEGER *)&__nEnd))			return -1.0;
	__int64 n = __nEnd - __nStart;
	if (bReStart)	__nStart = __nEnd;
	double t = (double)n / (double)__nFrequency;
	return t;
}

// Ψһ��Ӧ�ó������

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
