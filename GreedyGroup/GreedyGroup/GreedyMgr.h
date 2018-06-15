#pragma once

class CTag;
class CGroup;
class CGreedyMgr
{
private:
	int m_nCount;			// 基础点数量
	int m_nRuleSize;		// 最大线段长度
	int m_nPackHeadWidth;	// 报文头的长度
	int m_nReqUnit;			// 请求单位时间
	int m_nHandleUnit;		// 处理单位时间

public:
	std::list<std::shared_ptr<CTag>> m_ltTag;
	std::list<std::shared_ptr<CGroup>> m_ltGroup;	// 用于贪婪算法的列表
	std::list<CGroup*> m_ResultGroup;				// 用于贪婪算法的返回列表

	std::list<std::shared_ptr<CGroup>> m_ltGroupFinal;		// 用于最优解的链表

	std::list<std::shared_ptr<CGroup>> m_ltGroupFinalFast;	// 用于最优解结果的链表的速度优化

public:
	int GetCount(){return m_nCount;}
	int GetRuleSize(){return m_nRuleSize;}
	//std::shared_ptr<CTag> GetChild(int id){return m_vtTag[id];}	// 获得线段

	int GetPackHead() { return m_nPackHeadWidth; }	// 获得报文头长度
	int GetReqUnit() { return m_nReqUnit; }			// 获得请求单位时间
	int GetHandleUnit() { return m_nHandleUnit; }	// 获得处理单位时间

private:
	void InitBase(int ruleSize, int headWidth);
	bool FinalSolution();								// 寻找最优解，true表示找到
	void InitGroupConnection();							// 确定组与组之间彼此的链接
	bool FinalSolutionFast();							// 寻找最快最优解，true表示找到

public:
	CGreedyMgr(void);
	~CGreedyMgr(void);

	void Init(int count, int ruleSize, int headWidth);	// 初始化数据

	int Start2GreedyGroup(int& tOut);					// 开始贪婪的分组吧

	int Start2FinalSolution(int& tOut);					// 寻找最优解

	int Start2FinalSolutionFast(int& tOut);				// 寻找最快最优解
};
