#pragma once

class CLine;
class CLineGroup;
class CGreedyMgr
{
private:
	int m_nCount;		// 基础点数量
	int m_nRuleSize;	// 最大线段长度

public:
	std::vector<CLine *> m_vtLine;
	std::list<CLineGroup *> m_ltGroup;
	//std::list<boost::shared_ptr<CLine> > m_ltLine;
	//std::vector<boost::shared_ptr<CLine> > m_vtLine;
	//std::list<boost::shared_ptr<CLineGroup> > m_ltGroup;

public:
	int GetCount(){return m_nCount;}
	int GetRuleSize(){return m_nRuleSize;}
	CLine *GetChild(int id){return m_vtLine[id];}	// 获得线段

public:
	CGreedyMgr(void);
	~CGreedyMgr(void);

	void Init(int count, int ruleSize);				// 初始化数据
	void Uninit();									// 释放所有数据
	void Start2GreedyGroup();						// 开始贪婪的分组吧
	void RemoveGroup(CLineGroup *group){m_ltGroup.remove(group);}
};
