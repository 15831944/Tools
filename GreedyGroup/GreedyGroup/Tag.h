#pragma once
class CTag
{
private:
	int m_nRate;		// ˢ��Ƶ��
	int m_nStart;		// ��ʼλ��
	int m_nWidth;		// �����ֽ���
	int m_nEnd;			// �յ�λ�� = m_nStart + m_nEnd

public:
	int GetStart() { return m_nStart; }			// ��ʼλ��
	int GetWidth() { return m_nWidth; }			// �����ֽ���
	int GetEnd() { return m_nEnd; }				// �յ�λ�� = m_nStart + m_nEnd
	int GetFrashRate() { return m_nRate; }	// ˢ��Ƶ��

public:
	CTag(int start, int width, int rate) :m_nStart(start), m_nWidth(width), m_nEnd(start + width), m_nRate(rate) { ; }
	~CTag(){}
};

