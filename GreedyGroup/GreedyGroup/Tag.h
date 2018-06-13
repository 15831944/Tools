#pragma once
class CTag
{
private:
	int m_nStart;		// ��ʼλ��
	int m_nWidth;		// �����ֽ���
	int m_nEnd;			// �յ�λ�� = m_nStart + m_nEnd
	int m_nFrashRate;	// ˢ��Ƶ��

public:
	int GetStart() { return m_nStart; }			// ��ʼλ��
	int GetWidth() { return m_nWidth; }			// �����ֽ���
	int GetEnd() { return m_nEnd; }				// �յ�λ�� = m_nStart + m_nEnd
	int GetFrashRate() { return m_nFrashRate; }	// ˢ��Ƶ��

public:
	CTag(int start, int width, int rate) :m_nStart(start), m_nWidth(width), m_nEnd(start + width), m_nFrashRate(rate) { ; }
	~CTag(){}
};

