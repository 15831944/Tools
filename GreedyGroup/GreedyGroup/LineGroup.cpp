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

/* ��ͼ�̲���ߵĽ��
*  ����ֵ: ture�̲���ߵĽ��ɹ�,false�̲���ߵĽ��ʧ��
*/
void CLineGroup::Try2AddNext()
{
	//NCOUNT++;
	if (!m_pNext)								return;		// ������û����,����
	int fDistNextMax = m_pNext->GetEnd() - GetStart();		// ���һ���ľ���
	if (fDistNextMax > m_Mgr->GetRuleSize())	return;		// ���������ڹ涨��󳤶�,��ʾ��������,����
	int fDistNextMin = m_pNext->GetEnd() - GetEnd();
	if (m_pNext->Shorter2Prev(fDistNextMin))				// �����ߵĽ�����Լ�����,��ô�ͺϲ���
		Connect2Next();
	else
		m_pNext->Try2AddNext();								// �����ߵĽ�����Լ���Զ,��ô���ú�ߵĽ���Ⱥϲ�

	Try2AddNext();
}

// ���ߵ���ϲ�
void CLineGroup::Connect2Next()
{
	ASSERT(m_pNext);
	if (m_pNext == NULL)						return;
	//m_ltChild.splice(m_ltChild.end(), m_pNext->GetChildList());		// �ȰѺ���������
	m_nEndID = m_pNext->m_nEndID;
	AddStartEnd(m_pNext->GetStart(), m_pNext->GetEnd());			// �ı��Լ��ĳ���

	// ָ����һ������һ��
	CLineGroup *groupOld = m_pNext;
	m_Mgr->RemoveGroup(groupOld);
	m_pNext = m_pNext->GetNext();
	delete groupOld;
}
