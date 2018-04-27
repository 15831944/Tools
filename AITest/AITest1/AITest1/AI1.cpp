#include "stdafx.h"
#include "AI1.h"
#include "Param.h"
#include "Sample.h"

using namespace AI1;
AIMgr::AIMgr()
{
}

AIMgr::~AIMgr()
{
}

void AIMgr::InitParam(std::vector<int> powers, std::vector<double> defWeights)
{
	int count = (int)defWeights.size();
	m_Params.resize(count);
	for (int i = 0; i < count; i++)
		m_Params[i] = std::shared_ptr<Param>(new Param(i, powers[i], defWeights[i]));
}

void AIMgr::AddSample(std::shared_ptr<Sample> sample)
{
	ASSERT(sample->ValueSize() == m_Params.size());
	m_Samples.push_back(sample);

	// ���ݵ�ǰ��Ȩ�أ������ֵ
	EstimateResult(sample);

	// �����ݶȣ����µ�Ȩֵ
	CString str;
	for (int i = 0; i < m_Params.size(); i++)
	{
		m_Params[i]->ResetWeight(sample, m_Samples.size());
		
		str.Format(str + _T("%.6f\t"), m_Params[i]->Weight());
	}
	str += _T("\r\n");
	TRACE(str);
}

// ��ý��
AIValue& AIMgr::EstimateResult(std::shared_ptr<Sample> sample)
{
	// �����в�����ֵ
	for (int i = 0; i < m_Params.size(); i++)
	{
		m_Params[i]->SetValue(sample->Value(i));
	}

	// ���ݵ�ǰ��Ȩ�أ������ֵ
	double estimateVal = 0.0;
	for (int i = 0; i < m_Params.size(); i++)
	{
		estimateVal += m_Params[i]->GetEstiVal();
	}
	sample->SetResultEsti(estimateVal);
	return sample->ResultEsti();
}
