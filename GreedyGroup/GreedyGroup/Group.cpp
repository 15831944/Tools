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
	m_nThroughput = tag->GetFrashRate() * (tag->GetWidth() +
		m_Mgr->GetPackHead() +
		m_Mgr->GetReqUnit() +
		m_Mgr->GetHandleUnit());
}

CGroup::~CGroup()
{
}

// 获得大部分信息
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

// 是否超出了规定长度
bool CGroup::IsOutsize(int& thoughput)
{
	int start, end, width, rate;
	thoughput = GetGroupInfo(start, end, width, rate);
	if (width > m_Mgr->GetRuleSize()) return true;

	return false;
}

// 获取吞吐量
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

// 获得尝试合并后的吞吐量，并获得是否超界的标志，不是真合并
int CGroup::GetThroughputIfMerge(CGroup *g, bool& outofSize)
{
	std::list<std::shared_ptr<CTag>> ltTag = m_ltTag;
	for (auto tag : g->m_ltTag)	m_ltTag.push_back(tag);	// 吞并后的链表
	int t = 0;
	outofSize = IsOutsize(t);
	m_ltTag = ltTag;
	return t;
}

// 吞并这个组
void CGroup::AddGroup(CGroup *g)
{
	for (auto tag : g->m_ltTag)	m_ltTag.push_back(tag);	// 吞并后的链表

	int start, end, width, rate;
	m_nThroughput = GetGroupInfo(start, end, width, rate);
}

// 试图吞并后边的结点，返回false：吞并后一个失败
bool CGroup::Try2AddNext(int tAddPrev)
{
	if (!m_pNext)					return false;		// 如果后边没有了，返回

	int tSelf = GetThroughput();						// 获得自身的吞吐量
	int tNext = m_pNext->GetThroughput();				// 获得下一个的吞吐量

	bool outofSize = false;
	int tAddNext = GetThroughputIfMerge(m_pNext, outofSize);
	if (outofSize)					return false;
	if (tAddNext >= tSelf + tNext)	return false;		// 如果不优，撤销吞并，返回

	// 看看是跟前一个合并更优，还是跟后一个合并更优
	if (tAddPrev > 0 &&
		tAddPrev < tAddNext)		return false;		// 跟前一个合并更优，撤销吞并，返回

	// 让Next去判断它的下一个，看看是否跟我合并最优，返回false表示跟我合并最优
	if (!m_pNext->Try2AddNext(tAddNext)) {	// 跟我合并最优，那么我要合并下一个的数据啦
		AddGroup(m_pNext);
		auto next = m_pNext;
		m_pNext = m_pNext->GetNext();
		if (m_pNext) m_pNext->SetPrev(this);

		return true;		// 吞并成功，继续吞并下一个
	}

	// 后一个已经合并了，重新判断跟后一个的吞吐量
	return Try2AddNext(tAddPrev);
}

