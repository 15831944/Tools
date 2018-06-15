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
	m_nPackHeadWidth = headWidth;	// ����ͷ�ĳ���
	m_nReqUnit = 0;					// ����λʱ��
	m_nHandleUnit = 0;				// ����λʱ��
	m_nRuleSize = ruleSize;
	m_ltTag.clear();
	m_ltGroup.clear();
	m_ResultGroup.clear();
}

// ��ʼ������
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
	for (int i = 0; i < count; ++i)
	{
		//fStart += 1;
		int start = 0, len = 0, rate = 0;
		while (true) {
			start = rand() % widthAll;
			len = rand() % 4 + 1;
			rate = ((rand() & 1) == 1) ? 1 : 10;
			if (mpVal[start] == 100) continue;
			mpVal[start] = 100;
			break;
		}
	
		std::shared_ptr<CTag> tag = std::shared_ptr<CTag>(new CTag(start, 1, rate));
		m_ltTag.push_back(tag);
	}
	//m_ltTag.push_back(std::shared_ptr<CTag>(new CTag(5, 4, 2)));
	//m_ltTag.push_back(std::shared_ptr<CTag>(new CTag(25, 4, 20)));
	//m_ltTag.push_back(std::shared_ptr<CTag>(new CTag(30, 4, 2)));

	m_ltTag.sort(SortTag);
	for (auto tag : m_ltTag)
	{
		pGroup = std::shared_ptr<CGroup>(new CGroup(this, tag));
		m_ltGroup.push_back(pGroup);

		pGroup = std::shared_ptr<CGroup>(new CGroup(this, tag));
		m_ltGroupFinal.push_back(pGroup);

		pGroup = std::shared_ptr<CGroup>(new CGroup(this, tag));
		m_ltGroupFinalFast.push_back(pGroup);	// �������Ž�����������ٶ��Ż�
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

// ��ʼ̰���ķ����
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

// Ѱ�����Ž�
int CGreedyMgr::Start2FinalSolution(int& tOut)
{
	while (!FinalSolution());
	tOut = 0;
	for (auto g : m_ltGroupFinal)
		tOut += g->GetThroughput();
	return (int)m_ltGroupFinal.size();
}

// Ѱ�����Ž⣬true��ʾ�ҵ�
bool CGreedyMgr::FinalSolution()
{
	int nThroughput = -1;
	std::shared_ptr<CGroup> gL, gR;

	// ��ʼ��ltTGroup
	for (auto iterL = m_ltGroupFinal.begin(); iterL != m_ltGroupFinal.end(); iterL++)
	{
		int tL = (*iterL)->GetThroughput();

		auto iterR = iterL; iterR++;
		for (; iterR != m_ltGroupFinal.end(); iterR++)
		{
			bool bOutofSize = false;
			int tA = (*iterL)->GetThroughputIfMerge((*iterR).get(), bOutofSize);
			if (bOutofSize)		break;		// ��ߵĲ��ù���

			int tR = (*iterR)->GetThroughput();
			if (tA > (tL + tR))	continue;	// �ϲ�û���Ż�������Ҫ���ţ��������ж���һ��

			if (nThroughput == -1 || nThroughput > tA) {	// �ҵ������Ż���һ��
				nThroughput = tA;
				gL = *iterL;
				gR = *iterR;
			}
		}
	}
	if (nThroughput == -1) return true;

	// Merge two groups
	gL->AddGroup(gR.get());
	m_ltGroupFinal.remove(gR);
	return false;
}

struct SGroup {
	double rA; // ���±� = tA / (tL + tR)
	std::shared_ptr<CGroup> gL;
	std::shared_ptr<CGroup> gR;
	SGroup(double a, std::shared_ptr<CGroup> l, std::shared_ptr<CGroup> r) :rA(a), gL(l), gR(r) { ; }
};

std::list<std::shared_ptr<SGroup>> ltSGroup;
bool CompareSGroup(std::shared_ptr<SGroup> l, std::shared_ptr<SGroup> r) { return l->rA > r->rA; }

// Ѱ��������Ž�
int CGreedyMgr::Start2FinalSolutionFast(int& tOut)
{
	ltSGroup.clear();
	InitGroupConnection();
	while (!FinalSolutionFast());
	tOut = 0;
	for (auto g : m_ltGroupFinalFast)
		tOut += g->GetThroughput();
	return (int)m_ltGroupFinalFast.size();
}


// ȷ��������֮��˴˵�����
void CGreedyMgr::InitGroupConnection()
{
	int nThroughput = -1;
	std::shared_ptr<CGroup> gL, gR;

	// ��ʼ��ltTGroup
	for (auto iterL = m_ltGroupFinalFast.begin(); iterL != m_ltGroupFinalFast.end(); iterL++)
	{
		int tL = (*iterL)->GetThroughput();

		auto iterR = iterL; iterR++;
		for (; iterR != m_ltGroupFinalFast.end(); iterR++)
		{
			if (((*iterR)->GetStart() - (*iterL)->GetStart()) >= GetRuleSize())
				break;

			bool bOutofSize = false;
			int tA = (*iterL)->GetThroughputIfMerge((*iterR).get(), bOutofSize);
			if (bOutofSize)		continue;	// ��ߵ���Ȼ��Ҫ�ܣ���Ϊ���ܴ��ڰ�����ϵ

			int tR = (*iterR)->GetThroughput();
			//if (tA > (tL + tR))	continue;	// �ϲ�û���Ż��������ж���һ��

			ltSGroup.push_back(std::shared_ptr<SGroup>(new SGroup(tA / (tL + tR), *iterL, *iterR)));
		}
	}
}

// Ѱ��������Ž⣬true��ʾ�ҵ���
bool CGreedyMgr::FinalSolutionFast()
{
	double minT = -1.0;
	std::shared_ptr<SGroup> minSG;
	for (auto sg : ltSGroup)
	{
		if (minT == -1.0 || minT > sg->rA) {
			minT = sg->rA;
			minSG = sg;
		}
	}
	if (minT == -1)		return true; // ����������

	// ��ʼ�̲�
	minSG->gL->AddGroup(minSG->gR.get());
	ltSGroup.remove(minSG);
	m_ltGroupFinalFast.remove(minSG->gR);

	std::list<std::shared_ptr<SGroup>> ltDel;
	bool bOutofSize;
	for (auto sg : ltSGroup)
	{
		// ���а���L�ģ�����Ҫ����
		if (sg->gR == minSG->gL || sg->gL == minSG->gL)
		{
			int tA = sg->gL->GetThroughputIfMerge(sg->gR.get(), bOutofSize);
			if (bOutofSize)
				ltDel.push_back(sg);
			else
				sg->rA = tA / (sg->gL->GetThroughput() + sg->gR->GetThroughput());
		}

		// ���а���R�ģ�����Ҫɾ��
		if (sg->gR == minSG->gR || sg->gL == minSG->gR)
			ltDel.push_back(sg);
	}

	// ɾ�����и�ɾ����
	for (auto sg : ltDel)
		ltSGroup.remove(sg);

	return false;
}

