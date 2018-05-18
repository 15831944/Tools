#include "stdafx.h"
#include "PidTest.h"
#include "PID.h"

using namespace PID;
CPidTest::CPidTest()
{
}


CPidTest::~CPidTest()
{
}

void CPidTest::StartTest()
{
	CPid pid;
	int count = 100;
	CString str;
	while (count-- > 0)
	{
		double speed = pid.Realize(250.0);
		str.Format(_T("value is %.6f\r\n"), speed);
		TRACE(str);
		Sleep(100);
	}
}
