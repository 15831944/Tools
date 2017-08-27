#pragma once
class CSenior
{
private:
	CString m_strID;
	int m_nUnifiedCount;	// 计划统招生数量
	int m_nTargetCount;		// 计划指标生数量

public:
	CSenior();
	~CSenior();

	void InitData(pugi::xml_node node);

};

