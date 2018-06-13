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
	std::list<std::shared_ptr<CGroup>> m_ltGroup;

public:
	int GetCount(){return m_nCount;}
	int GetRuleSize(){return m_nRuleSize;}
	//std::shared_ptr<CTag> GetChild(int id){return m_vtTag[id];}	// 获得线段

	int GetPackHead() { return m_nPackHeadWidth; }	// 获得报文头长度
	int GetReqUnit() { return m_nReqUnit; }			// 获得请求单位时间
	int GetHandleUnit() { return m_nHandleUnit; }	// 获得处理单位时间

public:
	CGreedyMgr(void);
	~CGreedyMgr(void);

	void Init(int count, int ruleSize);				// 初始化数据
	int Start2GreedyGroup();						// 开始贪婪的分组吧
};
