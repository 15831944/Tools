#pragma once

namespace AI{
	class Sample
	{
	public:
		std::vector<double> m_values;
		double m_realResult;	// 真实的值
		double m_estiResult;	// 估计的值
		double m_detalResult;	// 真实值与估值的差值

	public:
		void SetEstiResult(double& val){ m_estiResult = val; m_detalResult = m_estiResult - m_realResult; }

	public:
		Sample();
		~Sample();
	};
}
