#pragma once
#include "AI1.h"
#include "Sample.h"

namespace AI1{
	class Param
	{
	private:
		int m_id;				// 编号
		int m_power;			// 幂数，支持0和正整数，暂时不支持分数和负数

		AIValue m_valueInput;	// 原始输入的值
		AIValue m_valuePower;	// 经过幂运算的值
		AIValue m_weight;		// 权重值
		AIValue m_esti;
		//double m_estiSum;		// 这个值表示代价和
		//double m_gradient;	// 梯度值，代价和除以数量

	public:
		double Weight(){ return m_weight.Value(); }

		Param(int id, int power, double defaultWeight)
			: m_id(id)
			, m_power(power)
			, m_valueInput(1.0)
			, m_valuePower(1.0)
			, m_weight(defaultWeight)
			//, m_estiSum(0.0)	// 这个值表示代价和
			//, m_gradient(0.0)	// 梯度值，代价和除以数量
		{
			if (m_power < 0)	m_power = 0;	// 暂时不支持负数
		}

		// 设置value值，获得原始输入值，计算经过幂指数运算后的值
		void SetValue(double value)
		{
			m_valueInput = value;

			// 幂指数运算
			double vp = 1.0;
			for (int i = 0; i < m_power; i++) vp *= value;
			m_valuePower = vp;
		}

		// 获得单个参数的估算值, w * (X ^ n)
		double GetEstiVal()
		{
			return m_weight.Value() * m_valuePower.Value();
		}

		// 获得导数的系数, w * n, 如果是幂指数是0，这个值会是0
		double GetDWeight()
		{
			return m_weight.Value() * m_power;
		}

		// 获得导数, w * n * (X ^ (n-1)), 
		double GetDValue()
		{
			if (m_power == 0)	return 0.0;	// 如果幂指数是0，导数一定是0
			auto val = 1.0;
			for (int i = 1; i < m_power; i++)
				val *= m_valueInput.Value();
			return GetDWeight() * val;
		}

		void ResetWeight(std::shared_ptr<Sample> sample, int count)
		{
			//m_estiSum += (lostRate * (m_weight - m_prevWeight));
			//auto gradient = m_estiSum / count;
			//m_weight -= gradient;
			//m_prevWeight = m_weight;

			// 计算代价和
			//m_estiSum += (lostRate * m_value);
			//// 计算梯度方差
			////m_gradient = m_estiSum < 0 ?
			////	-sqrt(-m_estiSum) / count :
			////	sqrt(m_estiSum) / count;
			//auto gradient = m_estiSum / count;
			//// 重新计算权重 = 原权重-梯度
			//m_weight -= gradient;
		}

		void GetGradient(AIMgr& a1) {
			for (auto sample : a1.m_Samples)
			{
				double res = sample->Value(m_id);
			}
		}
	};
}
