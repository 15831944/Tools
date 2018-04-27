#pragma once
namespace AI1{
	class AIValue
	{
	private:
		double m_ValueCur;
		double m_ValuePrev;
		double m_ValueDetl;

	public:
		AIValue() : AIValue(1.0) {}//m_ValueCur(1.0) , m_ValuePrev(1.0) , m_ValueDetl(0.0) {}
		AIValue(double value) : m_ValueCur(value), m_ValuePrev(value), m_ValueDetl(0.0) {}

		double Value(){ return m_ValueCur; }
		double ValuePrev(){ return m_ValuePrev; }
		double ValueDelta(){ return m_ValueDetl; }

		void operator = (double value)
		{
			m_ValuePrev = m_ValueCur;
			m_ValueCur = value;
			m_ValueDetl = m_ValueCur - m_ValuePrev;
		}
	};
}
