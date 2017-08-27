#include "stdafx.h"
#include "PrintTime.h"

#include "DataMgr.h"
#include <thread>
#include <functional>

CDataMgr::CDataMgr()
{
}


CDataMgr::~CDataMgr()
{
}

void CDataMgr::DoWork()
{
	Step0_InitData();
	Step1_Sort();			// 大排序
	Setp2_Unified();		// 统招
	Setp3_Target();			// 指标到校
	Setp4_SchoolAdjust();	// 校内服从
	Setp5_AllAdjust();		// 指标统调
}

void CDataMgr::Step0_InitData()
{
	PrintTime pt;
	std::thread *tr1 = new std::thread(std::bind(InitData, m_JuniorInfo));
	//m_JuniorInfo.InitData();
	//m_SeniorInfo.InitData();
	//m_StudentInfo.InitData();
	tr1->join();
	pt.MessageBoxTime();
}

// 大排序
void CDataMgr::Step1_Sort()
{
	// 学生成绩大排序
}

// 统招
void CDataMgr::Setp2_Unified()
{
	/*
	遍历所有学生
	将成绩好的学生放进对应的高中
	如果所有高中的统招生都满了
	高中按照最低分排序
	break
	*/
}

// 指标到校
void CDataMgr::Setp3_Target()
{
	/*
	遍历所有学生，不包含已经被招生和非指标生
	找到对应中学，看看报考的指标高中，看看指标是否已经用完了，如果没有则走下面
	如果是第一个
	设置该高中的上限和下限（-35）
	否则，如果成绩够
	看看是否满足下限要求
	否则，如果达不到所有高中的下限要求
	break
	*/
}

// 校内服从
void CDataMgr::Setp4_SchoolAdjust()
{
	/*
	遍历所有学生，不包含已经被招生和非指标生和非服从生
	遍历对应中学的所有剩余指标
	如果某高中还有剩余指标，自己的成绩满足该校下限要求
	成功一位，指标递减
	如果达不到所有高中的下限要求
	break
	*/
}

// 指标统调
void CDataMgr::Setp5_AllAdjust()
{
	/*
	遍历高中，那么还么有招满的
	从学生中，找成绩好的，没被招的，指标生（指标统调有没什么要求，比如学生必须注明“服从”？）
	*/
}
