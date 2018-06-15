#pragma once

class CTag;
class CGroup;
class CGreedyMgr
{
private:
	int m_nCount;			// ����������
	int m_nRuleSize;		// ����߶γ���
	int m_nPackHeadWidth;	// ����ͷ�ĳ���
	int m_nReqUnit;			// ����λʱ��
	int m_nHandleUnit;		// ����λʱ��

public:
	std::list<std::shared_ptr<CTag>> m_ltTag;
	std::list<std::shared_ptr<CGroup>> m_ltGroup;	// ����̰���㷨���б�
	std::list<CGroup*> m_ResultGroup;				// ����̰���㷨�ķ����б�

	std::list<std::shared_ptr<CGroup>> m_ltGroupFinal;		// �������Ž������

	std::list<std::shared_ptr<CGroup>> m_ltGroupFinalFast;	// �������Ž�����������ٶ��Ż�

public:
	int GetCount(){return m_nCount;}
	int GetRuleSize(){return m_nRuleSize;}
	//std::shared_ptr<CTag> GetChild(int id){return m_vtTag[id];}	// ����߶�

	int GetPackHead() { return m_nPackHeadWidth; }	// ��ñ���ͷ����
	int GetReqUnit() { return m_nReqUnit; }			// �������λʱ��
	int GetHandleUnit() { return m_nHandleUnit; }	// ��ô���λʱ��

private:
	void InitBase(int ruleSize, int headWidth);
	bool FinalSolution();								// Ѱ�����Ž⣬true��ʾ�ҵ�
	void InitGroupConnection();							// ȷ��������֮��˴˵�����
	bool FinalSolutionFast();							// Ѱ��������Ž⣬true��ʾ�ҵ�

public:
	CGreedyMgr(void);
	~CGreedyMgr(void);

	void Init(int count, int ruleSize, int headWidth);	// ��ʼ������

	int Start2GreedyGroup(int& tOut);					// ��ʼ̰���ķ����

	int Start2FinalSolution(int& tOut);					// Ѱ�����Ž�

	int Start2FinalSolutionFast(int& tOut);				// Ѱ��������Ž�
};
