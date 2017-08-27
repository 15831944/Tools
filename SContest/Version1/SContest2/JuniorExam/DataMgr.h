#pragma once
#include "JuniorInfo.h"
#include "SeniorInfo.h"
#include "StudentInfo.h"

class CDataMgr
{
private:
	CJuniorInfo m_JuniorInfo;
	CSeniorInfo m_SeniorInfo;
	CStudentInfo m_StudentInfo;
	void Step0_InitData();
	void Step1_Sort();			// ������
	void Setp2_Unified();		// ͳ��
	void Setp3_Target();		// ָ�굽У
	void Setp4_SchoolAdjust();	// У�ڷ���
	void Setp5_AllAdjust();		// ָ��ͳ��
	void Setp6_SaveData();		// ��������

public:
	CDataMgr();
	~CDataMgr();

	void DoWork();
};

