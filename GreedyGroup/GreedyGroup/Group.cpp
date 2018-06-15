#include "stdafx.h"
#include "GreedyMgr.h"
#include "Tag.h"
#include "Group.h"


CGroup::CGroup(CGreedyMgr* mgr, std::shared_ptr<CTag> tag)
	: m_Mgr(mgr)
	, m_pPrev(nullptr)
	, m_pNext(nullptr)
{
	m_ltTag.push_back(tag);	// Ĭ�����ٰ���1��tag
	m_nThroughput = tag->GetFrashRate() * (tag->GetWidth() +
		m_Mgr->GetPackHead() +
		m_Mgr->GetReqUnit() +
		m_Mgr->GetHandleUnit());
}

CGroup::~CGroup()
{
}

// ��ô󲿷���Ϣ
int CGroup::GetGroupInfo(int& start, int& end, int& width, int& rate)
{
	start = end = -1;
	width = rate = 0;
	rate = 0;
	for (auto tag : m_ltTag) {
		if (start == -1)	start = tag->GetStart();
		else				start = min(start, tag->GetStart());

		if (end == -1)		end = tag->GetEnd();
		else				end = max(end, tag->GetEnd());

		rate = max(rate, tag->GetFrashRate());
	}
	width = end - start;

	return rate * (width +
		m_Mgr->GetPackHead() +
		m_Mgr->GetReqUnit() +
		m_Mgr->GetHandleUnit());
}

// �Ƿ񳬳��˹涨����
bool CGroup::IsOutsize(int& thoughput)
{
	int start, end, width, rate;
	thoughput = GetGroupInfo(start, end, width, rate);
	if (width > m_Mgr->GetRuleSize()) return true;

	return false;
}

// ��ȡ������
//int CGroup::GetThroughput()
//{
//	int start, end, width, rate;
//	GetGroupInfo(start, end, width, rate);
//
//	return rate * ( width + 
//					m_Mgr->GetPackHead() +
//					m_Mgr->GetReqUnit() + 
//					m_Mgr->GetHandleUnit());
//}

// ��ó��Ժϲ������������������Ƿ񳬽�ı�־��������ϲ�
int CGroup::GetThroughputIfMerge(CGroup *g, bool& outofSize)
{
	std::list<std::shared_ptr<CTag>> ltTag = m_ltTag;
	for (auto tag : g->m_ltTag)	m_ltTag.push_back(tag);	// �̲��������
	int t = 0;
	outofSize = IsOutsize(t);
	m_ltTag = ltTag;
	return t;
}

// �̲������
void CGroup::AddGroup(CGroup *g)
{
	for (auto tag : g->m_ltTag)	m_ltTag.push_back(tag);	// �̲��������

	int start, end, width, rate;
	m_nThroughput = GetGroupInfo(start, end, width, rate);
}

// ��ͼ�̲���ߵĽ�㣬����false���̲���һ��ʧ��
bool CGroup::Try2AddNext(int tAddPrev)
{
	if (!m_pNext)					return false;		// ������û���ˣ�����

	int tSelf = GetThroughput();						// ��������������
	int tNext = m_pNext->GetThroughput();				// �����һ����������

	bool outofSize = false;
	int tAddNext = GetThroughputIfMerge(m_pNext, outofSize);
	if (outofSize)					return false;
	if (tAddNext >= tSelf + tNext)	return false;		// ������ţ������̲�������

	// �����Ǹ�ǰһ���ϲ����ţ����Ǹ���һ���ϲ�����
	if (tAddPrev > 0 &&
		tAddPrev < tAddNext)		return false;		// ��ǰһ���ϲ����ţ������̲�������

	// ��Nextȥ�ж�������һ���������Ƿ���Һϲ����ţ�����false��ʾ���Һϲ�����
	if (!m_pNext->Try2AddNext(tAddNext)) {	// ���Һϲ����ţ���ô��Ҫ�ϲ���һ����������
		AddGroup(m_pNext);
		auto next = m_pNext;
		m_pNext = m_pNext->GetNext();
		if (m_pNext) m_pNext->SetPrev(this);

		return true;		// �̲��ɹ��������̲���һ��
	}

	// ��һ���Ѿ��ϲ��ˣ������жϸ���һ����������
	return Try2AddNext(tAddPrev);
}

