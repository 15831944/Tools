#pragma once
class CTag;
class CGreedyMgr;
class CGroup
{
private:
	std::list<std::shared_ptr<CTag>> m_ltTag;
	CGreedyMgr* m_Mgr;
	CGroup *m_pNext;	// 下一个组
	CGroup *m_pPrev;	// 前一个组

public:
	CGroup *GetNext() { return m_pNext; }
	CGroup *GetPrev() { return m_pPrev; }
	void SetNext(CGroup *group) { m_pNext = group; }
	void SetPrev(CGroup *group) { m_pPrev = group; }

private:
	bool IsOutsize();	// 是否超出了规定长度
	void GetGroupInfo(int& start, int& end, int& width, int& rate);

public:
	CGroup(CGreedyMgr* mgr, std::shared_ptr<CTag> tag);
	~CGroup();
	int GetStart();						// 获得起始位置
	int GetThroughput();				// 获取吞吐量
	bool Try2AddNext(int tAddPrev);		// 试图吞并后边的结点，返回false：吞并后一个失败
};

