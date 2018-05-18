#include "stdafx.h"
#include "AI2Test.h"
#include "AI2.h"

using namespace LeastSquare;

// y = 2*x + 7 (1,000 cases)
//void LeastSquareTest::TestCase1()
//{
//	auto y = [](double x) -> double {return x * 2.0 + 7.0; };
//	CLeastSquare mgr;
//
//	std::vector<double> vx, vy, result;
//
//	result.resize(3);
//	for (double i = -10.0; i < 10.0; i += 1.0)
//	{
//		vx.push_back(i);
//		vy.push_back(y(i));
//		mgr.CalResult(vx, vy, 2, result);
//		TRACE("样本个数 %d, 拟合方程为：y = %lf + %lfx \n", result[1], result[2]);
//	}
//	//mgr.EMatrix(vx, vy, 2, result);
//	//TRACE("拟合方程为：y = %lf + %lfx \n", result[1], result[2]);
//}
//
//// y = 5*x^2 + 9*x + 22 (1,000 cases)
//void LeastSquareTest::TestCase2()
//{
//	auto y = [](double x) -> double {return 5.0 * x * x + 9.0 * x + 22.0; };
//	CLeastSquare mgr;
//
//	std::vector<double> vx, vy, result;
//
//	result.resize(4);
//	for (double i = -10.0; i < 10.0; i += 1.0)
//	{
//		//AddSample(std::vector<double> {(double)i, 1.0}, y((double)i));
//		vx.push_back(i);
//		vy.push_back(y(i));
//		mgr.CalResult(vx, vy, 3, result);
//		TRACE("样本个数 %d, 拟合方程为：y = %lf + %lfx + %lfx^2 \n", result[1], result[2], result[3]);
//	}
//	//mgr.EMatrix(vx, vy, 3, result);
//	//TRACE("拟合方程为：y = %lf + %lfx + %lfx^2 \n", result[1], result[2], result[3]);
//}
//
//// y = 11*x^3 + 21*x^2 + 74*x + 17 (1,000 cases)
//void LeastSquareTest::TestCase3()
//{
//	auto y = [](double x) -> double {return 11.0 * x * x * x + 21.0 * x * x + 74.0 * x + 17.0; };
//	CLeastSquare mgr;
//
//	std::vector<double> vx, vy, result;
//
//	result.resize(5);
//	for (double i = -10.0; i < 10.0; i += 1.0)
//	{
//		//AddSample(std::vector<double> {(double)i, 1.0}, y((double)i));
//		vx.push_back(i);
//		vy.push_back(y(i));
//		mgr.CalResult(vx, vy, 4, result);
//		TRACE("样本个数 %d, 拟合方程为：y = %lf + %lfx + %lfx^2 + %lfx^3 \n", result[1], result[2], result[3], result[4]);
//	}
//	//mgr.EMatrix(vx, vy, 4, result);
//	//TRACE("拟合方程为：y = %lf + %lfx + %lfx^2 + %lfx^3 \n", result[1], result[2], result[3], result[4]);
//}
//
//// y = 11*x^3 + 74*x + 17 (1,000 cases)
//void LeastSquareTest::TestCase4()
//{
//	auto y = [](double x) -> double {return 11.0 * x * x * x + 74.0 * x + 17.0; };
//	CLeastSquare mgr;
//	auto ecount = 3;
//
//	std::vector<double> vx, vy, result;
//
//	result.resize(ecount + 2);
//	for (double i = -10.0; i < 10.0; i += 1.0)
//	{
//		//AddSample(std::vector<double> {(double)i, 1.0}, y((double)i));
//		vx.push_back(i);
//		vy.push_back(y(i));
//		mgr.CalResult(vx, vy, ecount + 1, result);
//		TRACE("样本个数 %d, 拟合方程为：y = %lf + %lfx + %lfx^2 + %lfx^3 \n", result[1], result[2], result[3], result[4]);
//	}
//	//mgr.EMatrix(vx, vy, 4, result);
//	//TRACE("拟合方程为：y = %lf + %lfx + %lfx^2 + %lfx^3 \n", result[1], result[2], result[3], result[4]);
//}

// y = (n)*x^(n-1) + (n-1)*x^(n-2) + ... + 2*x^1 + 1
void LeastSquareTest::TestCaseN(int n)
{
	// y = 1 - 2x + 3x^2 - 4x^3 + 5x^4 ...
	auto y = [](double x, int n) -> double {
		double s = 1.0;
		for (int i = n; i > 0; i--) {
			double v = (double)(i + 1) * pow(x, i);
			s += ((i & 1) == 0) ? v : -v;
		}
		return s;
	};
	auto ecount = n;

	std::vector<double> vx, vy, result;
	result.resize(ecount + 2);

	int num = 1000;// */n + 2;
	double input = 0.0 - (double)num;
	for (int i = 0; i < num; i++)
	{
		input = (double)(rand()%100);
		vx.push_back(input);
		vy.push_back(y(input, n));
		//mgr.CalEResult(vx, vy, ecount + 1, result);
		//TRACE("样本个数 %d, 拟合方程为：y = %lf + %lfx + %lfx^2 + %lfx^3", result[1], result[2], result[3], result[4]);
		//str.Format(_T("样本个数 %d, 拟合方程为：y = %lf"), vx.size(), result[1]);
		//for (int k = 1; k <= n; k++)
		//{
		//	str.Format(str + _T(" + %lfx^%d"), result[k + 1], k);
		//}
		//str += _T("\r\n");
		//TRACE(str);
		input += 2.0;
	}
	CLeastSquare::CalEResult(vx, vy, ecount + 1, result);

	//CString str;
	//str.Format(_T("样本个数 %d, 拟合方程为：y = %lf"), vx.size(), result[1]);
	//for (int k = 1; k <= n; k++)
	//{
	//	if (result[k + 1] >= 0)
	//		str.Format(str + _T(" + %lfx^%d"), result[k + 1], k);
	//	else
	//		str.Format(str + _T(" - %lfx^%d"), -result[k + 1], k);
	//}
	//return str;
	//str += _T("\r\n");
	//TRACE(str);
	//mgr.EMatrix(vx, vy, 4, result);
	//TRACE("拟合方程为：y = %lf + %lfx + %lfx^2 + %lfx^3 \n", result[1], result[2], result[3], result[4]);
}

void LeastSquareTest::StartTest()
{
	//TestCase1();
	//TestCase2();
	//TestCase3();
	//TestCase4();
	SYSTEMTIME st1, st2;
	CString strResult;
	::GetLocalTime(&st1);
	TestCaseN(3);
	::GetLocalTime(&st2);
	CString str;
	str.Format(strResult + _T("\r\nWith time as below:\r\n%.2d:%.2d.%.3d\r\n%.2d:%.2d.%.3d"),
		st2.wMinute, st2.wSecond, st2.wMilliseconds,
		st1.wMinute, st1.wSecond, st1.wMilliseconds);
	MessageBox(NULL, str, _T("Result"), MB_OK);
}
