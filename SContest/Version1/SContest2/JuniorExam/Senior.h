#pragma once
class CSenior
{
private:
	CString m_strID;
	int m_nUnifiedCount;	// �ƻ�ͳ��������
	int m_nTargetCount;		// �ƻ�ָ��������

public:
	CSenior();
	~CSenior();

	void InitData(pugi::xml_node node);

};

