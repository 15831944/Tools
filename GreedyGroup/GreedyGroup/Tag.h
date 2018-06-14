#pragma once
class CTag
{
private:
	int m_nRate;		// 刷新频率
	int m_nStart;		// 起始位置
	int m_nWidth;		// 数据字节数
	int m_nEnd;			// 终点位置 = m_nStart + m_nEnd

public:
	int GetStart() { return m_nStart; }			// 起始位置
	int GetWidth() { return m_nWidth; }			// 数据字节数
	int GetEnd() { return m_nEnd; }				// 终点位置 = m_nStart + m_nEnd
	int GetFrashRate() { return m_nRate; }	// 刷新频率

public:
	CTag(int start, int width, int rate) :m_nStart(start), m_nWidth(width), m_nEnd(start + width), m_nRate(rate) { ; }
	~CTag(){}
};

