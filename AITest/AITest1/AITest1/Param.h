#pragma once

namespace AI{
	class AI1;
	class Param
	{
	public:
		int m_id;			// 编号
		double m_weight;	// 当前的权重
		double m_value;		// 当前的值
		double m_estiSum;	// 这个值表示代价和
		double m_gradient;	// 梯度值，代价和除以数量

	public:
		double GetEstiVal(){ return m_weight * m_value; }
		void SetEstiResult(double deltaResult, int count)
		{
			// 计算代价和
			m_estiSum += (deltaResult * m_value);
			// 计算梯度
			m_gradient = m_estiSum / count;
			// 重新计算权重 = 原权重-梯度
			m_weight -= m_gradient;
		}

	public:
		Param(int id, double defaultWeight);
		~Param();

		void AddSample(double value);
		void GetGradient(AI1 a1);
		//void SetSample(int idx, double val, double result);
	};
}
