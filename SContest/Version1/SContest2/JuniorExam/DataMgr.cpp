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
	Step1_Sort();			// ������
	Setp2_Unified();		// ͳ��
	Setp3_Target();			// ָ�굽У
	Setp4_SchoolAdjust();	// У�ڷ���
	Setp5_AllAdjust();		// ָ��ͳ��
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

// ������
void CDataMgr::Step1_Sort()
{
	// ѧ���ɼ�������
}

// ͳ��
void CDataMgr::Setp2_Unified()
{
	/*
	��������ѧ��
	���ɼ��õ�ѧ���Ž���Ӧ�ĸ���
	������и��е�ͳ����������
	���а�����ͷ�����
	break
	*/
}

// ָ�굽У
void CDataMgr::Setp3_Target()
{
	/*
	��������ѧ�����������Ѿ��������ͷ�ָ����
	�ҵ���Ӧ��ѧ������������ָ����У�����ָ���Ƿ��Ѿ������ˣ����û����������
	����ǵ�һ��
	���øø��е����޺����ޣ�-35��
	��������ɼ���
	�����Ƿ���������Ҫ��
	��������ﲻ�����и��е�����Ҫ��
	break
	*/
}

// У�ڷ���
void CDataMgr::Setp4_SchoolAdjust()
{
	/*
	��������ѧ�����������Ѿ��������ͷ�ָ�����ͷǷ�����
	������Ӧ��ѧ������ʣ��ָ��
	���ĳ���л���ʣ��ָ�꣬�Լ��ĳɼ������У����Ҫ��
	�ɹ�һλ��ָ��ݼ�
	����ﲻ�����и��е�����Ҫ��
	break
	*/
}

// ָ��ͳ��
void CDataMgr::Setp5_AllAdjust()
{
	/*
	�������У���ô��ô��������
	��ѧ���У��ҳɼ��õģ�û���еģ�ָ������ָ��ͳ����ûʲôҪ�󣬱���ѧ������ע�������ӡ�����
	*/
}
