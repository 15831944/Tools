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

bool SortGroup(std::shared_ptr<CGroup> pLhs, std::shared_ptr<CGroup> pRhs)
{
	if (pLhs->GetStart() < pRhs->GetStart())	return true;
	return false;
}

bool SortTag(std::shared_ptr<CTag> pLhs, std::shared_ptr<CTag> pRhs)
{
	if (pLhs->GetStart() < pRhs->GetStart())	return true;
	return false;
}

//void InitGroup(CLineGroup *pGroup, )

// 初始化数据
void CGreedyMgr::Init(int count, int ruleSize)
{
	m_nPackHeadWidth = 200;	// 报文头的长度
	m_nReqUnit = 0;			// 请求单位时间
	m_nHandleUnit = 0;		// 处理单位时间
	if (count < 0)		return;
	if (ruleSize < 0)	return;
	m_ltTag.clear();
	m_ltGroup.clear();

	m_nCount = count;
	m_nRuleSize = ruleSize;
	std::shared_ptr<CGroup> pGroup;
	std::shared_ptr<CGroup> pGroupPrev;

	//m_vtTag.resize(count);
	//int fStart = 512, fLen = 1;
	std::map<int, int> mpVal;
	for (int i = 0; i < count; ++i)
	{
		//fStart += 1;
		int start = 0, len = 0;
		while (true) {
			start = rand() % 1000;
			len = rand() % 4 + 1;
			if (mpVal[start] == 100) continue;
			mpVal[start] = 100;
			break;
		}

		std::shared_ptr<CTag> tag = std::shared_ptr<CTag>(new CTag(start, len, 10));
		m_ltTag.push_back(tag);
		//pGroup = std::shared_ptr<CGroup>(new CGroup(this, tag));
		//m_ltGroup.push_back(pGroup);
	}
	m_ltTag.sort(SortTag);
	for (auto tag : m_ltTag)
	{
		pGroup = std::shared_ptr<CGroup>(new CGroup(this, tag));
		m_ltGroup.push_back(pGroup);
	}
	//m_ltGroup.sort(SortGroup);
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
int CGreedyMgr::Start2GreedyGroup()
{
	if (m_nCount <= 1){return 1;}
	auto g = m_ltGroup.front().get();
	while (g){
		g->Try2AddNext(-1);
		g = g->GetNext();
	}
	int count = 1;
	g = m_ltGroup.front().get();
	while (g->GetNext())
	{
		count++;
		g = g->GetNext();
	}
	return count;
}

