#include "StdAfx.h"
#include "GreedyGroup.h"
#include "LineGroup.h"
#include "GreedyMgr.h"

CLineGroup::CLineGroup(CGreedyMgr *mgr, int fStart, int fEnd)
:CLine(0, fStart, fEnd)
,m_Mgr(mgr)
,m_pNext(NULL)
{
}

void CLineGroup::OnChildChange()
{
	CLine *pStartLine = m_Mgr->GetChild(m_nStartID);
	CLine *pEndLine = m_Mgr->GetChild(m_nEndID);
	SetStart(pStartLine->GetStart());
	SetEnd(pStartLine->GetEnd());
}

/* 试图吞并后边的结点
*  返回值: ture吞并后边的结点成功,false吞并后边的结点失败
*/
void CLineGroup::Try2AddNext()
{
	//NCOUNT++;
	if (!m_pNext)								return;		// 如果后边没有了,返回
	int fDistNextMax = m_pNext->GetEnd() - GetStart();		// 与后一个的距离
	if (fDistNextMax > m_Mgr->GetRuleSize())	return;		// 如果距离大于规定最大长度,表示不能连接,返回
	int fDistNextMin = m_pNext->GetEnd() - GetEnd();
	if (m_pNext->Shorter2Prev(fDistNextMin))				// 如果后边的结点与自己更近,那么就合并它
		Connect2Next();
	else
		m_pNext->Try2AddNext();								// 如果后边的结点与自己较远,那么先让后边的结点先合并

	Try2AddNext();
}

// 与后边的组合并
void CLineGroup::Connect2Next()
{
	ASSERT(m_pNext);
	if (m_pNext == NULL)						return;
	//m_ltChild.splice(m_ltChild.end(), m_pNext->GetChildList());		// 先把孩子整过来
	m_nEndID = m_pNext->m_nEndID;
	AddStartEnd(m_pNext->GetStart(), m_pNext->GetEnd());			// 改变自己的长度

	// 指向下一个的下一个
	CLineGroup *groupOld = m_pNext;
	m_Mgr->RemoveGroup(groupOld);
	m_pNext = m_pNext->GetNext();
	delete groupOld;
}
