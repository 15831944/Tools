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
	void Step1_Sort();			// 大排序
	void Setp2_Unified();		// 统招
	void Setp3_Target();		// 指标到校
	void Setp4_SchoolAdjust();	// 校内服从
	void Setp5_AllAdjust();		// 指标统调
	void Setp6_SaveData();		// 保存数据

public:
	CDataMgr();
	~CDataMgr();

	void DoWork();
};

