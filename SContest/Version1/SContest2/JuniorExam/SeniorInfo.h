#pragma once

class CSenior;
class CSeniorInfo
{
private:
	int m_nSeniorCount;
	CSenior *m_Seniors;

public:
	CSeniorInfo();
	~CSeniorInfo();

	void InitData();
};

