#pragma once

class CJunior;
class CJuniorInfo
{
private:
	bool m_bLoadData;
	int m_nJuniorCount;
	CJunior *m_Juniors;

public:
	CJuniorInfo();
	~CJuniorInfo();

	void InitData();
};

