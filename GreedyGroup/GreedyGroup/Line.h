#pragma once

class CLine
{
private:
	int m_nID;				// ���
	int m_fStart;			// ���
	int m_fEnd;				// �յ�

public:
	int GetStart(){return m_fStart;}
	int GetEnd(){return m_fEnd;}
	int GetLen(){return m_fEnd - m_fStart;}

	void SetStart(int start){m_fStart = start;}
	void SetEnd(int end){m_fEnd = end;}

	CLine(int nID, int fStart, int fEnd):m_nID(nID)		// ���
		,m_fStart(fStart)								// ���
		,m_fEnd(fEnd){;}								// �յ�
	~CLine(void){;}
};
