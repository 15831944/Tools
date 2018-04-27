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

		// 设置原始的参数数量，和默认的权值，参数数量一定要比真实需求的量多一个，比如现实中需要统计3个输入参数，这里必须写成4个，最后一个为了给常数用的
		void InitParam(std::vector<int> powers, std::vector<double> defWeights);
		// 添加样本，样本里的参数数量也必须必参数数量多1个，最后一个给常数用，每次都必须为1.0
		void AddSample(std::shared_ptr<Sample> sample);
		// 获得结果
		AIValue& EstimateResult(std::shared_ptr<Sample> sample);
	};
}
