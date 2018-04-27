#pragma once
#include "AI1.h"
#include "Sample.h"

namespace AI1{
	class Param
	{
	private:
		int m_id;				// ���
		int m_power;			// ������֧��0������������ʱ��֧�ַ����͸���

		AIValue m_valueInput;	// ԭʼ�����ֵ
		AIValue m_valuePower;	// �����������ֵ
		AIValue m_weight;		// Ȩ��ֵ
		AIValue m_esti;
		//double m_estiSum;		// ���ֵ��ʾ���ۺ�
		//double m_gradient;	// �ݶ�ֵ�����ۺͳ�������

	public:
		double Weight(){ return m_weight.Value(); }

		Param(int id, int power, double defaultWeight)
			: m_id(id)
			, m_power(power)
			, m_valueInput(1.0)
			, m_valuePower(1.0)
			, m_weight(defaultWeight)
			//, m_estiSum(0.0)	// ���ֵ��ʾ���ۺ�
			//, m_gradient(0.0)	// �ݶ�ֵ�����ۺͳ�������
		{
			if (m_power < 0)	m_power = 0;	// ��ʱ��֧�ָ���
		}

		// ����valueֵ�����ԭʼ����ֵ�����㾭����ָ��������ֵ
		void SetValue(double value)
		{
			m_valueInput = value;

			// ��ָ������
			double vp = 1.0;
			for (int i = 0; i < m_power; i++) vp *= value;
			m_valuePower = vp;
		}

		// ��õ��������Ĺ���ֵ, w * (X ^ n)
		double GetEstiVal()
		{
			return m_weight.Value() * m_valuePower.Value();
		}

		// ��õ�����ϵ��, w * n, �������ָ����0�����ֵ����0
		double GetDWeight()
		{
			return m_weight.Value() * m_power;
		}

		// ��õ���, w * n * (X ^ (n-1)), 
		double GetDValue()
		{
			if (m_power == 0)	return 0.0;	// �����ָ����0������һ����0
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

			// ������ۺ�
			//m_estiSum += (lostRate * m_value);
			//// �����ݶȷ���
			////m_gradient = m_estiSum < 0 ?
			////	-sqrt(-m_estiSum) / count :
			////	sqrt(m_estiSum) / count;
			//auto gradient = m_estiSum / count;
			//// ���¼���Ȩ�� = ԭȨ��-�ݶ�
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
