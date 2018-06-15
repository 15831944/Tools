#pragma once
class CTag;
class CGreedyMgr;
class CGroup
{
private:
	std::list<std::shared_ptr<CTag>> m_ltTag;
	int m_nThroughput;
	CGreedyMgr* m_Mgr;
	CGroup *m_pNext;	// ��һ����
	CGroup *m_pPrev;	// ǰһ����

public:
	std::map<CGroup*, int> m_mpGroupL;	// ��߿��Ը��Һϲ����飬�Լ���Ӧ�ĺϲ�������
	std::map<CGroup*, int> m_mpGroupR;	// �ұ߿��Ը��Һϲ����飬�Լ���Ӧ�ĺϲ�������

public:
	CGroup *GetNext() { return m_pNext; }
	CGroup *GetPrev() { return m_pPrev; }
	void SetNext(CGroup *group) { m_pNext = group; }
	void SetPrev(CGroup *group) { m_pPrev = group; }
	int GetThroughput() { return m_nThroughput; }					// ��ȡ������
	int GetStart() { return m_ltTag.front()->GetStart(); }			// �����ʼλ��

private:
	bool IsOutsize(int& thoughput);									// �Ƿ񳬳��˹涨����
	int GetGroupInfo(int& start, int& end, int& width, int& rate);	// ������ڵ���Ϣ

public:
	CGroup(CGreedyMgr* mgr, std::shared_ptr<CTag> tag);
	~CGroup();
	int GetThroughputIfMerge(CGroup *g, bool& outofSize);			// ��ó��Ժϲ������������������Ƿ񳬽�ı�־��������ϲ�
	bool Try2AddNext(int tAddPrev);		// ��ͼ�̲���ߵĽ�㣬����false���̲���һ��ʧ��
	void AddGroup(CGroup *g);			// �̲������
};

