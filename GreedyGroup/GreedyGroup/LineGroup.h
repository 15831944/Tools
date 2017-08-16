#pragma once
#include "Line.h"

class CGreedyMgr;
class CLineGroup : public CLine
{
public:
	CGreedyMgr *m_Mgr;
	int m_nStartID;			// ��ʼ��ַ
	int m_nEndID;			// ��β��ַ
	CLineGroup *m_pNext;	// ��һ��

public:
	void SetNext(CLineGroup *group){m_pNext = group;}
	CLineGroup *GetNext(){return m_pNext;}

	bool Shorter2Prev(int dis2Prev)			// �Ƿ���ǰһ������
	{
		if (!m_pNext)	return true;
		return (m_pNext->GetEnd() - GetStart() - dis2Prev) >= 0;
	}

	void AddStartEnd(int fStart, int fEnd)	// ����ǰ�����
	{
		if (fStart < GetStart())		SetStart(fStart);
		if (fEnd > GetEnd())			SetEnd(fEnd);
	}

public:
	CLineGroup(CGreedyMgr *mgr, int fStart, int fEnd);
	~CLineGroup(void){;}
	void OnChildChange();
	void Try2AddNext();						// ��ͼ�̲���ߵĽ��
	void Connect2Next();					// ���ߵ���ϲ�
};
