#pragma once

namespace AI{
	class Sample
	{
	public:
		std::vector<double> m_values;
		double m_realResult;	// 真实的值
		double m_estiResult;	// 估计的值
		double m_detalResult;	// 真实值与估值的差值，损失值
		double m_lostResult;

	public:
		void SetEstiResult(double& val)
		{
			m_estiResult = val;								// 估值
			m_detalResult = m_realResult - m_estiResult;	// 损失值 = 真实值 - 估值
			m_lostResult = m_detalResult * m_detalResult;	// 损失值的平方，用于求方差
		}

	public:
		Sample();
		~Sample();
	};
}
