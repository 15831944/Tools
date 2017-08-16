#pragma once

class CLine
{
private:
	int m_nID;				// 编号
	int m_fStart;			// 起点
	int m_fEnd;				// 终点

public:
	int GetStart(){return m_fStart;}
	int GetEnd(){return m_fEnd;}
	int GetLen(){return m_fEnd - m_fStart;}

	void SetStart(int start){m_fStart = start;}
	void SetEnd(int end){m_fEnd = end;}

	CLine(int nID, int fStart, int fEnd):m_nID(nID)		// 编号
		,m_fStart(fStart)								// 起点
		,m_fEnd(fEnd){;}								// 终点
	~CLine(void){;}
};
