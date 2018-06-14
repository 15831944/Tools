#pragma once
class CTag;
class CGreedyMgr;
class CGroup
{
private:
	std::list<std::shared_ptr<CTag>> m_ltTag;
	CGreedyMgr* m_Mgr;
	CGroup *m_pNext;	// ��һ����
	CGroup *m_pPrev;	// ǰһ����

public:
	CGroup *GetNext() { return m_pNext; }
	CGroup *GetPrev() { return m_pPrev; }
	void SetNext(CGroup *group) { m_pNext = group; }
	void SetPrev(CGroup *group) { m_pPrev = group; }

private:
	bool IsOutsize();												// �Ƿ񳬳��˹涨����
	void GetGroupInfo(int& start, int& end, int& width, int& rate);	// ������ڵ���Ϣ

public:
	CGroup(CGreedyMgr* mgr, std::shared_ptr<CTag> tag);
	~CGroup();
	int GetThroughput();				// ��ȡ������
	int GetThroughputIfMerge(CGroup *g, bool& outofSize);			// ��ó��Ժϲ������������������Ƿ񳬽�ı�־��������ϲ�
	bool Try2AddNext(int tAddPrev);		// ��ͼ�̲���ߵĽ�㣬����false���̲���һ��ʧ��
	void AddGroup(CGroup *g);			// �̲������
};
