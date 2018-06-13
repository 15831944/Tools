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
	std::list<std::shared_ptr<CGroup>> m_ltGroup;

public:
	int GetCount(){return m_nCount;}
	int GetRuleSize(){return m_nRuleSize;}
	//std::shared_ptr<CTag> GetChild(int id){return m_vtTag[id];}	// ����߶�

	int GetPackHead() { return m_nPackHeadWidth; }	// ��ñ���ͷ����
	int GetReqUnit() { return m_nReqUnit; }			// �������λʱ��
	int GetHandleUnit() { return m_nHandleUnit; }	// ��ô���λʱ��

public:
	CGreedyMgr(void);
	~CGreedyMgr(void);

	void Init(int count, int ruleSize);				// ��ʼ������
	int Start2GreedyGroup();						// ��ʼ̰���ķ����
};
