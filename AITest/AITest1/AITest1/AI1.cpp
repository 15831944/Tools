#include "stdafx.h"
#include "AI1.h"
#include "Param.h"
#include "Sample.h"

using namespace AI;
AI1::AI1()
{
}

AI1::~AI1()
{
}

void AI1::InitParam(std::vector<double> defWeights)
{
	m_weight = 1;
	int count = (int)defWeights.size();
	m_Params.resize(count);
	for (int i = 0; i < count; i++)
		m_Params[i] = std::shared_ptr<Param>(new Param(i, defWeights[i]));
}

void AI1::AddSample(std::shared_ptr<Sample> sample)
{
	ASSERT(sample->m_values.size() == m_Params.size());
	m_Samples.push_back(sample);

	// 给所有参数赋值
	for (int i = 0; i < m_Params.size(); i++)
	{
		m_Params[i]->AddSample(sample->m_values[i]);
	}

	// 根据当前的权重，计算估值
	double estimateVal = 0.0;
	for (int i = 0; i < m_Params.size(); i++)
	{
		estimateVal += m_Params[i]->GetEstiVal();
	}
	sample->SetEstiResult(estimateVal);

	// 计算梯度，和新的权值
	for (int i = 0; i < m_Params.size(); i++)
	{
		m_Params[i]->SetEstiResult(estimateVal, m_Samples.size());
	}
}
