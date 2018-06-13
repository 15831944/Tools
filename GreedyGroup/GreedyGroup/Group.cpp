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
}

CGroup::~CGroup()
{
}

// �����ʼλ��
int CGroup::GetStart()
{
	int start = -1;
	for (auto tag : m_ltTag) {
		if (start == -1) start = tag->GetStart();
		else start = min(start, tag->GetStart());
	}
	return start;
}

// ��ô󲿷���Ϣ
void CGroup::GetGroupInfo(int& start, int& end, int& width, int& rate)
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
}

// �Ƿ񳬳��˹涨����
bool CGroup::IsOutsize()
{
	int start, end, width, rate;
	GetGroupInfo(start, end, width, rate);
	return width > m_Mgr->GetRuleSize();
}

// ��ȡ������
int CGroup::GetThroughput()
{
	int start, end, width, rate;
	GetGroupInfo(start, end, width, rate);

	return rate * ( width + 
					m_Mgr->GetPackHead() +
					m_Mgr->GetReqUnit() + 
					m_Mgr->GetHandleUnit());
}

// ��ͼ�̲���ߵĽ�㣬����false���̲���һ��ʧ��
bool CGroup::Try2AddNext(int tAddPrev)
{
	if (!m_pNext)	return false;			// ������û���ˣ�����

	int tSelf = GetThroughput();			// ��������������
	int tNext = m_pNext->GetThroughput();	// �����һ����������

	std::list<std::shared_ptr<CTag>> ltTag = m_ltTag;

	for (auto tag : m_pNext->m_ltTag)		// �̲��������
		m_ltTag.push_back(tag);

	if (IsOutsize()) {						// ��������ˣ������̲�������
		m_ltTag = ltTag;
		return false;
	}

	int tAddNext = GetThroughput();			// ����̲���һ�����������
	if (tAddNext >= tSelf + tNext) {		// ������ţ������̲�������
		m_ltTag = ltTag;
		return false;
	}

	// �����Ǹ�ǰһ���ϲ����ţ����Ǹ���һ���ϲ�����
	if (tAddPrev > 0 && tAddPrev < tAddNext) {	// ��ǰһ���ϲ����ţ������̲�������
		m_ltTag = ltTag;
		return false;
	}

	// ��Nextȥ�ж�������һ���������Ƿ���Һϲ����ţ�����false��ʾ���Һϲ�����
	if (!m_pNext->Try2AddNext(tAddNext)) {	// ���Һϲ����ţ���ô���κϲ���ʵ
		auto next = m_pNext;
		m_pNext = m_pNext->GetNext();
		if (m_pNext)
			m_pNext->SetPrev(this);
		return true;	// �̲��ɹ�
	}

	// ��һ���Ѿ��ϲ��ˣ�������ǰ���̲����������жϸ���һ����������
	m_ltTag = ltTag;
	return Try2AddNext(tAddPrev);
}
