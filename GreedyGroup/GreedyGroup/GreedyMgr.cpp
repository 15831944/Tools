#include "StdAfx.h"
#include "GreedyMgr.h"
#include "Tag.h"
#include "Group.h"

CGreedyMgr::CGreedyMgr(void)
{
}

CGreedyMgr::~CGreedyMgr(void)
{
}

bool SortTag(std::shared_ptr<CTag> pLhs, std::shared_ptr<CTag> pRhs)
{
	if (pLhs->GetStart() < pRhs->GetStart())	return true;
	return false;
}

void CGreedyMgr::InitBase(int ruleSize, int headWidth)
{
	m_nPackHeadWidth = headWidth;	// 报文头的长度
	m_nReqUnit = 0;					// 请求单位时间
	m_nHandleUnit = 0;				// 处理单位时间
	m_nRuleSize = ruleSize;
	m_ltTag.clear();
	m_ltGroup.clear();
	m_ResultGroup.clear();
}

// 初始化数据
void CGreedyMgr::Init(int count, int ruleSize, int headWidth)
{
	if (count < 0)		return;
	if (ruleSize < 0)	return;
	InitBase(ruleSize, headWidth);

	m_nCount = count;
	int widthAll = max(1000, count * 10);
	std::shared_ptr<CGroup> pGroup;
	std::shared_ptr<CGroup> pGroupPrev;

	std::map<int, int> mpVal;
	//for (int i = 0; i < count; ++i)
	//{
	//	//fStart += 1;
	//	int start = 0, len = 0, rate = 0;
	//	while (true) {
	//		start = rand() % widthAll;
	//		len = rand() % 4 + 1;
	//		rate = ((rand() & 1) == 1) ? 1 : 10;
	//		if (mpVal[start] == 100) continue;
	//		mpVal[start] = 100;
	//		break;
	//	}
	//
	//	std::shared_ptr<CTag> tag = std::shared_ptr<CTag>(new CTag(start, 1, rate));
	//	m_ltTag.push_back(tag);
	//}
	m_ltTag.push_back(std::shared_ptr<CTag>(new CTag(5, 4, 2)));
	m_ltTag.push_back(std::shared_ptr<CTag>(new CTag(25, 4, 20)));
	m_ltTag.push_back(std::shared_ptr<CTag>(new CTag(30, 4, 2)));


	m_ltTag.sort(SortTag);
	for (auto tag : m_ltTag)
	{
		pGroup = std::shared_ptr<CGroup>(new CGroup(this, tag));
		m_ltGroup.push_back(pGroup);

		pGroup = std::shared_ptr<CGroup>(new CGroup(this, tag));
		m_ltGroupFinal.push_back(pGroup);
	}

	for (auto pGroup : m_ltGroup)
	{
		if (!pGroupPrev) {pGroupPrev = pGroup; continue;}

		pGroupPrev->SetNext(pGroup.get());
		pGroup->SetPrev(pGroupPrev.get());
		pGroupPrev = pGroup;
	}
	return;
}

// 开始贪婪的分组吧
int CGreedyMgr::Start2GreedyGroup(int& tOut)
{
	if (m_nCount <= 1){return 1;}
	auto g = m_ltGroup.front().get();
	while (g){
		if (!g->Try2AddNext(-1))
			g = g->GetNext();
	}
	int count = 1;
	tOut = 0;
	m_ResultGroup.clear();
	g = m_ltGroup.front().get();
	m_ResultGroup.push_back(g);
	tOut += g->GetThroughput();
	while (g->GetNext())
	{
		count++;
		g = g->GetNext();
		m_ResultGroup.push_back(g);
		tOut += g->GetThroughput();
	}
	return count;
}

// 寻找最优解
int CGreedyMgr::Start2FinalSolution(int& tOut)
{
	while (!FinalSolution());
	tOut = 0;
	for (auto g : m_ltGroupFinal)
		tOut += g->GetThroughput();
	return (int)m_ltGroupFinal.size();
}

// 寻找最优解，true表示找到
bool CGreedyMgr::FinalSolution()
{
	//std::list<std::shared_ptr<TGroup>> ltTGroup;
	int nThroughput = -1;
	std::shared_ptr<CGroup> gL, gR;

	// 初始化ltTGroup
	for (auto iterL = m_ltGroupFinal.begin(); iterL != m_ltGroupFinal.end(); iterL++)
	{
		int tL = (*iterL)->GetThroughput();

		auto iterR = iterL; iterR++;
		for (; iterR != m_ltGroupFinal.end(); iterR++)
		{
			bool bOutofSize = false;

			int tR = (*iterR)->GetThroughput();
			int tA = (*iterL)->GetThroughputIfMerge((*iterR).get(), bOutofSize);
			if (bOutofSize)		break;		// 后边的不用管了

			if (tA > (tL + tR))	continue;	// 合并没有优化，不需要管着，但还得判断下一个

			if (nThroughput == -1 || nThroughput > tA) {
				nThroughput = tA;
				gL = *iterL;
				gR = *iterR;
			}
		}
	}
	if (nThroughput == -1) return true;

	// Merge two group
	gL->AddGroup(gR.get());
	m_ltGroupFinal.remove(gR);
	return false;
}
