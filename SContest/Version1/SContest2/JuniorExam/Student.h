#pragma once
class CStudent
{
private:
	CString m_strID;
	CString m_strJuniorID;
	CString m_strTargetSenior;	// ������ָ�����
	bool m_bTarget;				// �Ƿ���ָ����
	bool m_bAdjust;				// �Ƿ����
	short m_nScore[5];			// �������Դ������ģ���ѧ��Ӣ��ۺ�

	CString m_strUnified[3];	// 3��ͳ��־Ը
	CString m_strTarget[2];		// 2��ָ�굽Уͳ��־Ը

public:
	CStudent();
	~CStudent();

	void InitData(pugi::xml_node node);
};

