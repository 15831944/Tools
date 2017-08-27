#pragma once
class CStudent;
class CStudentInfo
{
private:
	bool m_bLoadData;
	int m_nStudentCount;
	CStudent *m_Students;

public:
	CStudentInfo();
	~CStudentInfo();

	void InitData();
};

