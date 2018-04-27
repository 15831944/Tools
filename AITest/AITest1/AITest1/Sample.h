#pragma once
#include "AI1.h"

namespace AI1{
	class Sample
	{
	private:
		std::vector<double> m_values;

		AIValue m_ResultReal;		// ��ʵֵ
		AIValue m_ResultEsti;		// ��ֵ
		AIValue m_ResultDetl;		// ��ֵ = ��ֵ - ��ʵֵ
		AIValue m_DetlRate;			// ����

	public:
		Sample()
			: m_ResultReal(0.0)
			, m_ResultEsti(0.0)
			, m_ResultDetl(0.0)
			, m_DetlRate(0.0)
		{}

		int ValueSize(){ return (int)m_values.size(); }
		double Value(int i){ return m_values[i]; }
		AIValue& ResultReal(){ return m_ResultReal; }
		AIValue& ResultEsti(){ return m_ResultEsti; }
		AIValue& ResultDetl(){ return m_ResultDetl; }

		void SetValue(std::vector<double>& val){ m_values = val; }
		void SetResultReal(double val) { m_ResultReal = val; }
		void SetResultEsti(double& val)
		{
			m_ResultEsti = val;							// ��ֵ
			m_ResultDetl = val - m_ResultReal.Value();	// ��ֵ = ��ֵ - ��ʵֵ
			m_DetlRate = m_ResultDetl.Value() / m_ResultReal.Value();	// ���� = ��ֵ / ��ʵֵ
		}
	};
}
