#include "stdafx.h"
#include "GreedyMgr.h"
#include "Tag.h"
#include "Group.h"


CGroup::CGroup(CGreedyMgr* mgr, std::shared_ptr<CTag> tag)
	: m_Mgr(mgr)
	, m_pPrev(nullptr)
	, m_pNext(nullptr)
{
	m_ltTag.push_back(tag);	// 默认至少包含1个tag
}

CGroup::~CGroup()
{
}

// 获得起始位置
int CGroup::GetStart()
{
	int start = -1;
	for (auto tag : m_ltTag) {
		if (start == -1) start = tag->GetStart();
		else start = min(start, tag->GetStart());
	}
	return start;
}

// 获得大部分信息
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

// 是否超出了规定长度
bool CGroup::IsOutsize()
{
	int start, end, width, rate;
	GetGroupInfo(start, end, width, rate);
	return width > m_Mgr->GetRuleSize();
}

// 获取吞吐量
int CGroup::GetThroughput()
{
	int start, end, width, rate;
	GetGroupInfo(start, end, width, rate);

	return rate * ( width + 
					m_Mgr->GetPackHead() +
					m_Mgr->GetReqUnit() + 
					m_Mgr->GetHandleUnit());
}

// 试图吞并后边的结点，返回false：吞并后一个失败
bool CGroup::Try2AddNext(int tAddPrev)
{
	if (!m_pNext)	return false;			// 如果后边没有了，返回

	int tSelf = GetThroughput();			// 获得自身的吞吐量
	int tNext = m_pNext->GetThroughput();	// 获得下一个的吞吐量

	std::list<std::shared_ptr<CTag>> ltTag = m_ltTag;

	for (auto tag : m_pNext->m_ltTag)		// 吞并后的链表
		m_ltTag.push_back(tag);

	if (IsOutsize()) {						// 如果超界了，撤销吞并，返回
		m_ltTag = ltTag;
		return false;
	}

	int tAddNext = GetThroughput();			// 获得吞并下一个后的吞吐量
	if (tAddNext >= tSelf + tNext) {		// 如果不优，撤销吞并，返回
		m_ltTag = ltTag;
		return false;
	}

	// 看看是跟前一个合并更优，还是跟后一个合并更优
	if (tAddPrev > 0 && tAddPrev < tAddNext) {	// 跟前一个合并更优，撤销吞并，返回
		m_ltTag = ltTag;
		return false;
	}

	// 让Next去判断它的下一个，看看是否跟我合并最优，返回false表示跟我合并最优
	if (!m_pNext->Try2AddNext(tAddNext)) {	// 跟我合并最优，那么本次合并坐实
		auto next = m_pNext;
		m_pNext = m_pNext->GetNext();
		if (m_pNext)
			m_pNext->SetPrev(this);
		return true;	// 吞并成功
	}

	// 后一个已经合并了，撤销当前的吞并，并重新判断跟后一个的吞吐量
	m_ltTag = ltTag;
	return Try2AddNext(tAddPrev);
}
