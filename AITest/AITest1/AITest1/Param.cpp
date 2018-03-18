#include "stdafx.h"
#include "Param.h"
#include "AI1.h"
#include "Sample.h"

using namespace AI;
Param::Param(int id, double defaultWeight)
	: m_id(id)
	, m_weight(defaultWeight)
	, m_value(0.0)
{
}


Param::~Param()
{
}

// 添加当前的值
void Param::AddSample(double value)
{
	m_value = value;
}

// 计算梯度
void Param::GetGradient(AI1 a1)
{
	for (auto sample : a1.m_Samples)
	{
		double res = sample->m_values[m_id];
	}
}

