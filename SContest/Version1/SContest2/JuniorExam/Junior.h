#pragma once

class CJunior2Senior
{
private:
	CString m_strID;
	int m_nTargetCount;	// �ڴ˸���ӵ�е�ָ������

public:
	CJunior2Senior();
	void InitData(pugi::xml_node node);
};

class CJunior
{
private:
	CString m_strID;
	int m_nSeniorCount;
	CJunior2Senior *m_arrJ2S;

public:
	CJunior();
	~CJunior();
	void InitData(pugi::xml_node node);
};

