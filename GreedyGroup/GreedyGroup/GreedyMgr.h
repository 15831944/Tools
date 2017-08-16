#pragma once

class CLine;
class CLineGroup;
class CGreedyMgr
{
private:
	int m_nCount;		// ����������
	int m_nRuleSize;	// ����߶γ���

public:
	std::vector<CLine *> m_vtLine;
	std::list<CLineGroup *> m_ltGroup;
	//std::list<boost::shared_ptr<CLine> > m_ltLine;
	//std::vector<boost::shared_ptr<CLine> > m_vtLine;
	//std::list<boost::shared_ptr<CLineGroup> > m_ltGroup;

public:
	int GetCount(){return m_nCount;}
	int GetRuleSize(){return m_nRuleSize;}
	CLine *GetChild(int id){return m_vtLine[id];}	// ����߶�

public:
	CGreedyMgr(void);
	~CGreedyMgr(void);

	void Init(int count, int ruleSize);				// ��ʼ������
	void Uninit();									// �ͷ���������
	void Start2GreedyGroup();						// ��ʼ̰���ķ����
	void RemoveGroup(CLineGroup *group){m_ltGroup.remove(group);}
};
