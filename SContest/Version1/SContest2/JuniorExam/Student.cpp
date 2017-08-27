#include "stdafx.h"
#include "Student.h"


CStudent::CStudent()
	: m_bTarget(true)
	, m_bAdjust(true)
{
	for (int i = 0; i < 5; ++i) m_nScore[i] = 0;
}


CStudent::~CStudent()
{
}

void CStudent::InitData(pugi::xml_node node)
{
	for (auto it = node.attributes_begin(); it != node.attributes_end(); it++)
	{
		CString name = it->name();
		if (name == _T("ID"))						m_strID = it->value();
		else if (name == _T("JuniorID"))			m_strJuniorID = it->value();
		else if (name == _T("IsTarget"))			m_bTarget = it->value() == "False" ? false : true;
		else if (name == _T("TargetSeniorSchool"))	m_strTargetSenior = it->value();
		else if (name == _T("CanAdjust"))			m_bAdjust = it->value() == "False" ? false : true;
		else if (name == _T("ChineseScore"))		m_nScore[0] = _ttoi(it->value());
		else if (name == _T("MathScore"))			m_nScore[1] = _ttoi(it->value());
		else if (name == _T("EnglishScore"))		m_nScore[2] = _ttoi(it->value());
		else if (name == _T("OtherScore"))			m_nScore[3] = _ttoi(it->value());
	}
	auto seniorNode = node.child(_T("KeySeniorSchools"));
	if (seniorNode)
	{
		for (auto it = seniorNode.attributes_begin(); it != seniorNode.attributes_end(); it++)
		{
			CString name = it->name();
			if (name == _T("FirstChoiceID"))		m_strUnified[0] = it->value();
			else if (name == _T("SecondChoiceID"))	m_strUnified[1] = it->value();
			else if (name == _T("ThirdChoiceID"))	m_strUnified[2] = it->value();
		}
	}
	auto targetNode = node.child(_T("AdjustedTargetSchools"));
	if (targetNode)
	{
		for (auto it = targetNode.attributes_begin(); it != targetNode.attributes_end(); it++)
		{
			CString name = it->name();
			if (name == _T("FirstChoiceID"))		m_strTarget[0] = it->value();
			else if (name == _T("SecondChoiceID"))	m_strTarget[1] = it->value();
		}
	}

	for (int i = 0; i < 4; i++)
	{
		m_nScore[4] += m_nScore[i];
	}
}