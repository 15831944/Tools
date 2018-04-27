#pragma once
#include "Value.h"

namespace AI1
{
	class Param;
	class Sample;
	class AIMgr
	{
	public:
		std::vector<std::shared_ptr<Param>> m_Params;
		std::list<std::shared_ptr<Sample>> m_Samples;

	public:
		AIMgr();
		~AIMgr();

		// ����ԭʼ�Ĳ�����������Ĭ�ϵ�Ȩֵ����������һ��Ҫ����ʵ���������һ����������ʵ����Ҫͳ��3������������������д��4�������һ��Ϊ�˸������õ�
		void InitParam(std::vector<int> powers, std::vector<double> defWeights);
		// ���������������Ĳ�������Ҳ����ز���������1�������һ���������ã�ÿ�ζ�����Ϊ1.0
		void AddSample(std::shared_ptr<Sample> sample);
		// ��ý��
		AIValue& EstimateResult(std::shared_ptr<Sample> sample);
	};
}
