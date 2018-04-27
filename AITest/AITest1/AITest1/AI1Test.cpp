#include "stdafx.h"
#include "AI1Test.h"
#include "AI1.h"
#include "Param.h"
#include "Sample.h"

using namespace AI1;
AIMgr _ai;

AI1Test::AI1Test()
{
}


AI1Test::~AI1Test()
{
}

void AddSample(std::vector<double> values, double result)
{
	auto sample = std::shared_ptr<Sample>(new Sample());
	sample->SetValue(values);
	sample->SetResultReal(result);
	_ai.AddSample(sample);
}

// y = 2*x + 7 (1,000 cases)
void TestCase1()
{
	auto y = [](double x) -> double {return x * 2.0 + 7.0; };
	std::vector<int> powers{ 1, 0 };
	std::vector<double> weights{ 8, 20};
	_ai.InitParam(powers, weights);

	for (double i = -1000.0; i < 1000.0; i+=0.1)
	{
		AddSample(std::vector<double> {(double)i, 1.0}, y((double)i));
	}

	auto sample = std::shared_ptr<Sample>(new Sample());

	double test1 = 120.0;
	sample->SetValue(std::vector<double>{test1, 1.0});
	auto val = _ai.EstimateResult(sample).Value();
	auto rate = val / y(test1);
}

// y = 3*x*x + 4*x + 9 (10,000 cases)
void TestCase2()
{}

// y = -4*x*x*x + 3*x*x + 20*x (100,000 cases)
void TestCase3()
{}

void AI1Test::StartTest()
{
	TestCase1();
	TestCase2();
	TestCase3();
}
