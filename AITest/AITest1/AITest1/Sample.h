#pragma once

namespace AI{
	class Sample
	{
	public:
		std::vector<double> m_values;
		double m_realResult;	// ��ʵ��ֵ
		double m_estiResult;	// ���Ƶ�ֵ
		double m_detalResult;	// ��ʵֵ���ֵ�Ĳ�ֵ����ʧֵ
		double m_lostResult;

	public:
		void SetEstiResult(double& val)
		{
			m_estiResult = val;								// ��ֵ
			m_detalResult = m_realResult - m_estiResult;	// ��ʧֵ = ��ʵֵ - ��ֵ
			m_lostResult = m_detalResult * m_detalResult;	// ��ʧֵ��ƽ���������󷽲�
		}

	public:
		Sample();
		~Sample();
	};
}
