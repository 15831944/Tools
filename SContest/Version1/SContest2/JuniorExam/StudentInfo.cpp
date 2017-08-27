#include "stdafx.h"
#include "Gbl.h"

#include "Student.h"
#include "StudentInfo.h"


CStudentInfo::CStudentInfo()
	: m_bLoadData(false)
	, m_Students(nullptr)
	, m_nStudentCount(0)
{
}


CStudentInfo::~CStudentInfo()
{
	if (m_Students != nullptr)
	{
		delete[] m_Students;
		m_Students = nullptr;
		m_nStudentCount = 0;
	}
}

void CStudentInfo::InitData()
{
	pugi::xml_document doc;
	pugi::xml_parse_result result = doc.load_file(CGbl::GetMe().getExePath() + _T("Students.xml"), pugi::parse_default, pugi::encoding_auto);
	if (result)
	{
		m_bLoadData = true;
		pugi::xml_node root = doc.child("Students");
		m_nStudentCount = 0;

		auto children = root.children();
		for (auto it = children.begin(); it != children.end(); it++)
		{
			++m_nStudentCount;
		}
		if (m_nStudentCount > 0)
		{
			m_Students = new CStudent[m_nStudentCount];
		}
		int index = 0;
		for (auto it = children.begin(); it != children.end(); it++)
		{
			m_Students[index++].InitData(*it);
		}
	}
}
