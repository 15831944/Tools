#pragma once
#include "Line.h"

class CGreedyMgr;
class CLineGroup : public CLine
{
public:
	CGreedyMgr *m_Mgr;
	int m_nStartID;			// 起始地址
	int m_nEndID;			// 结尾地址
	CLineGroup *m_pNext;	// 后一个

public:
	void SetNext(CLineGroup *group){m_pNext = group;}
	CLineGroup *GetNext(){return m_pNext;}

	bool Shorter2Prev(int dis2Prev)			// 是否离前一个更近
	{
		if (!m_pNext)	return true;
		return (m_pNext->GetEnd() - GetStart() - dis2Prev) >= 0;
	}

	void AddStartEnd(int fStart, int fEnd)	// 设置前后距离
	{
		if (fStart < GetStart())		SetStart(fStart);
		if (fEnd > GetEnd())			SetEnd(fEnd);
	}

public:
	CLineGroup(CGreedyMgr *mgr, int fStart, int fEnd);
	~CLineGroup(void){;}
	void OnChildChange();
	void Try2AddNext();						// 试图吞并后边的结点
	void Connect2Next();					// 与后边的组合并
};
