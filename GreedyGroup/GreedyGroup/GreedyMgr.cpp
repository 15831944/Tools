#include "StdAfx.h"
#include "GreedyMgr.h"
#include "Line.h"
#include "LineGroup.h"

CGreedyMgr::CGreedyMgr(void)
{
}

CGreedyMgr::~CGreedyMgr(void)
{
}

bool SortGroup(CLineGroup *pLhs, CLineGroup *pRhs)
{
	if (pLhs->GetStart() < pRhs->GetStart())	return true;
	return false;
}

// 初始化数据
void CGreedyMgr::Init(int count, int ruleSize)
{
	if (count < 0)		return;
	if (ruleSize < 0)	return;
	m_nCount = count;
	m_nRuleSize = ruleSize;
	CLineGroup *pGroup = NULL, *pGroupPrev = NULL;

	m_vtLine.resize(count);
	int fStart = 512, fLen = 1;
	for (int i = 0; i < count; ++i)
	{
		fStart += 1;
		CLine* pLine = new CLine(i, fStart, fStart + fLen);
		m_vtLine[i] = pLine;
		pGroup = new CLineGroup(this, fStart, pLine->GetEnd());
		pGroup->m_nStartID = pGroup->m_nEndID = i;
		pGroup->SetStart(fStart);
		pGroup->SetEnd(pLine->GetEnd());
		m_ltGroup.push_back(pGroup);
	}
	m_ltGroup.sort(SortGroup);
	foreach (pGroup, m_ltGroup){
		if (pGroupPrev == NULL){pGroupPrev = pGroup; continue;}
		pGroupPrev->SetNext(pGroup);
		pGroupPrev = pGroup;
	}
	return;
}

// 释放所有数据
void CGreedyMgr::Uninit()
{
	foreach (CLineGroup *pGroup, m_ltGroup)		delete pGroup;
	foreach (CLine *pLine, m_vtLine)			delete pLine;
}

// 开始贪婪的分组吧
void CGreedyMgr::Start2GreedyGroup()
{
	if (m_nCount <= 1){return;}
	CLineGroup *lg = m_ltGroup.front();
	while (lg){
		lg->Try2AddNext();
		lg = lg->GetNext();
	}
	return;
}

